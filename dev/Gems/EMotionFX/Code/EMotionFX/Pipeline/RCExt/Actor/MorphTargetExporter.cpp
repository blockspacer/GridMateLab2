/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
//
#include <Cry_Geo.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Math/Transform.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/algorithm.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>
#include <SceneAPI/SceneCore/Utilities/SceneGraphSelector.h>
#include <SceneAPI/SceneCore/DataTypes/DataTypeUtilities.h>
#include <SceneAPI/SceneCore/Containers/Scene.h>
#include <SceneAPI/SceneCore/Containers/SceneGraph.h>
#include <SceneAPI/SceneCore/Containers/Utilities/Filters.h>
#include <SceneAPI/SceneCore/Containers/Utilities/SceneGraphUtilities.h>
#include <SceneAPI/SceneCore/Containers/Views/SceneGraphDownwardsIterator.h>
#include <SceneAPI/SceneCore/DataTypes/GraphData/IBlendShapeData.h>
#include <SceneAPI/SceneCore/DataTypes/GraphData/ITransform.h>
#include <SceneAPI/SceneCore/DataTypes/Rules/IBlendShapeRule.h>



#include <SceneAPIExt/Groups/IActorGroup.h>
#include <RCExt/ExportContexts.h>
#include <RCExt/Actor/MorphTargetExporter.h>
#include <RCExt/CoordinateSystemConverter.h>
#include <EMotionFX/Source/Actor.h>
#include <EMotionFX/Source/Node.h>
#include <EMotionFX/Source/Mesh.h>
#include <EMotionFX/Source/MeshBuilder.h>
#include <EMotionFX/Source/MorphTargetStandard.h>
#include <EMotionFX/Source/MorphSetup.h>
#include <Integration/System/SystemCommon.h>

namespace EMotionFX
{
    namespace Pipeline
    {
        namespace SceneEvents = AZ::SceneAPI::Events;
        namespace SceneUtil = AZ::SceneAPI::Utilities;

        MorphTargetExporter::MorphTargetExporter()
        {
            BindToCall(&MorphTargetExporter::ProcessMorphTargets);
        }

        void MorphTargetExporter::Reflect(AZ::ReflectContext* context)
        {
            AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
            if (serializeContext)
            {
                serializeContext->Class<MorphTargetExporter, AZ::SceneAPI::SceneCore::ExportingComponent>()->Version(1);
            }
        }

        AZ::SceneAPI::Events::ProcessingResult MorphTargetExporter::ProcessMorphTargets(ActorMorphBuilderContext& context)
        {
            if (context.m_phase != AZ::RC::Phase::Filling)
            {
                return AZ::SceneAPI::Events::ProcessingResult::Ignored;
            }

            if (!context.m_group.RTTI_IsTypeOf(EMotionFX::Pipeline::Group::IActorGroup::TYPEINFO_Uuid()))
            {
                return AZ::SceneAPI::Events::ProcessingResult::Ignored;
            }

            AZStd::shared_ptr<const AZ::SceneAPI::DataTypes::IBlendShapeRule> morphTargetRule = context.m_group.GetRuleContainerConst().FindFirstByType<AZ::SceneAPI::DataTypes::IBlendShapeRule>();
            if (!morphTargetRule)
            {
                return AZ::SceneAPI::Events::ProcessingResult::Ignored;
            }

            const AZ::SceneAPI::Containers::SceneGraph& graph = context.m_scene.GetGraph();

            //clone the actor
            auto baseActorImage = EMotionFX::Integration::EMotionFXPtr<EMotionFX::Actor>::MakeFromNew(context.m_actor->Clone());
            baseActorImage->RemoveAllNodeMeshes();

            AZStd::unordered_set<AZStd::string> visitedBlendShapeNames;
            AZStd::string morphTargetName;
            AZStd::string morphNodeName;
            AZStd::string morphParentNodeName;

            AZ::Transform globalTransform;

            //outer loop isolates unique morph target names in the selection list
            const size_t selectionNodeCount = morphTargetRule->GetSceneNodeSelectionList().GetSelectedNodeCount();
            for (size_t index = 0; index < selectionNodeCount; ++index)
            {
                AZ::SceneAPI::Containers::SceneGraph::NodeIndex nodeIndex = graph.Find(morphTargetRule->GetSceneNodeSelectionList().GetSelectedNode(index));
                morphTargetName = context.m_scene.GetGraph().GetNodeName(nodeIndex).GetName();
                if (AZStd::find(visitedBlendShapeNames.begin(), visitedBlendShapeNames.end(), morphTargetName) != visitedBlendShapeNames.end())
                {
                    continue;
                }
                visitedBlendShapeNames.insert(morphTargetName);
                EMotionFX::Actor* morphTargetActor = baseActorImage->Clone();
                if (!morphTargetActor)
                {
                    return SceneEvents::ProcessingResult::Failure;
                }
                morphTargetActor->SetName(morphTargetName.c_str());

                //for the current found morph target name - find all the morph targets in the selection list that match that name
                //so that they can be processed together
                for (size_t morphIndex = index; morphIndex < selectionNodeCount; ++morphIndex)
                {
                    AZ::SceneAPI::Containers::SceneGraph::NodeIndex nodeIndex = graph.Find(morphTargetRule->GetSceneNodeSelectionList().GetSelectedNode(morphIndex));
                    morphNodeName = context.m_scene.GetGraph().GetNodeName(nodeIndex).GetName();
                    if (morphNodeName == morphTargetName)
                    {
                        const AZStd::shared_ptr<const AZ::SceneAPI::DataTypes::IBlendShapeData>& morphTargetData =
                            azrtti_cast<const AZ::SceneAPI::DataTypes::IBlendShapeData*>(graph.GetNodeContent(nodeIndex));
                        AZ_Assert(morphTargetData, "Node is expected to be a Morph Target/BlendShape, but isn't.");
                        if (morphTargetData)
                        {
                            //add the current morph data to the morphActor
                            AZ::SceneAPI::Containers::SceneGraph::NodeIndex morphMeshParentIndex = graph.GetNodeParent(nodeIndex);
                            morphParentNodeName = context.m_scene.GetGraph().GetNodeName(morphMeshParentIndex).GetName();
                            EMotionFX::Node* emfxNode = morphTargetActor->GetSkeleton()->FindNodeByName(morphParentNodeName.c_str());

                            globalTransform = SceneUtil::BuildWorldTransform(graph, nodeIndex);
                            // Inverse transpose for normal
                            AZ::Transform globalTransformN = globalTransform.GetInverseFull().GetTranspose();

                            BuildMorphTargetMesh(morphTargetActor, emfxNode, morphTargetData, globalTransform, globalTransformN, context.m_coordinateSystemConverter, context.m_useMeshOptimization);
                        }
                    }
                }
                //add the morph target actor to the main actor
                EMotionFX::MorphTargetStandard* morphTarget = EMotionFX::MorphTargetStandard::Create(false, true, context.m_actor, morphTargetActor, morphTargetName.c_str());
                //assume LOD 0
                EMotionFX::MorphSetup* morphSetup = context.m_actor->GetMorphSetup(0);
                if (!morphSetup)
                {
                    morphSetup = EMotionFX::MorphSetup::Create();
                    context.m_actor->SetMorphSetup(0, morphSetup);
                }

                if (morphSetup)
                {
                    morphSetup->AddMorphTarget(morphTarget);
                }
            }

            return SceneEvents::ProcessingResult::Success;
        }

        void MorphTargetExporter::BuildMorphTargetMesh(EMotionFX::Actor* actor, EMotionFX::Node* emfxNode,
            const AZStd::shared_ptr<const AZ::SceneAPI::DataTypes::IBlendShapeData>& morphTargetData, const AZ::Transform& globalTransform, const AZ::Transform& globalTransformN, CoordinateSystemConverter& coordinateSystemConverter, bool optimizeTriangleList)
        {
            //// Get the number of triangles (faces)
            const AZ::u32 numFaces = morphTargetData->GetFaceCount();

            //// Get the number of orgVerts (control point)
            const AZ::u32 controlPointCount = morphTargetData->GetUsedControlPointCount();
            const AZ::u32 normalCount = morphTargetData->GetVertexCount();
            EMotionFX::MeshBuilder* meshBuilder = EMotionFX::MeshBuilder::Create(emfxNode->GetNodeIndex(), controlPointCount, false);

            //// Original vertex numbers
            EMotionFX::MeshBuilderVertexAttributeLayerUInt32* orgVtxLayer = EMotionFX::MeshBuilderVertexAttributeLayerUInt32::Create(controlPointCount, EMotionFX::Mesh::ATTRIB_ORGVTXNUMBERS, false, false);
            meshBuilder->AddLayer(orgVtxLayer);

            //// The positions layer
            EMotionFX::MeshBuilderVertexAttributeLayerVector3* posLayer = EMotionFX::MeshBuilderVertexAttributeLayerVector3::Create(controlPointCount, EMotionFX::Mesh::ATTRIB_POSITIONS, false, true);
            meshBuilder->AddLayer(posLayer);

            //// The normals layer
            EMotionFX::MeshBuilderVertexAttributeLayerVector3* normalsLayer = EMotionFX::MeshBuilderVertexAttributeLayerVector3::Create(normalCount, EMotionFX::Mesh::ATTRIB_NORMALS, false, true);
            meshBuilder->AddLayer(normalsLayer);

            //// Data for each vertex
            AZ::Vector3 pos;
            AZ::Vector3 normal;

            for (AZ::u32 faceIndex = 0; faceIndex < numFaces; ++faceIndex)
            {
                // Start the triangle
                const AZ::u32 materialID = 0;
                meshBuilder->BeginPolygon(materialID);

                // Add all triangle points (We are not supporting non-triangle face)
                for (AZ::u32 vertexIndexInsidePoly = 0; vertexIndexInsidePoly < 3; ++vertexIndexInsidePoly)
                {
                    const uint32 vertexIndex = morphTargetData->GetFaceVertexIndex(faceIndex, vertexIndexInsidePoly);

                    const AZ::u32 controlPointIndex = morphTargetData->GetControlPointIndex(vertexIndex);

                    int orgVertexNumber = morphTargetData->GetUsedPointIndexForControlPoint(controlPointIndex);
                    AZ_Assert(orgVertexNumber >= 0, "Invalid vertex number");
                    orgVtxLayer->SetCurrentVertexValue(&orgVertexNumber);

                    pos = morphTargetData->GetPosition(vertexIndex);
                    pos = globalTransform * pos;
                    pos = coordinateSystemConverter.ConvertVector3(pos);
                    normal = morphTargetData->GetNormal(vertexIndex);
                    normal = globalTransformN * normal;
                    normal = coordinateSystemConverter.ConvertVector3(normal);

                    posLayer->SetCurrentVertexValue(&pos);
                    normal.Normalize();
                    normalsLayer->SetCurrentVertexValue(&normal);

                    meshBuilder->AddPolygonVertex(orgVertexNumber);
                }
                meshBuilder->EndPolygon();
            }

            // Cache optimize the index buffer list
            if (optimizeTriangleList)
            {
                meshBuilder->OptimizeTriangleList();
            }

            // Link the mesh to the node
            EMotionFX::Mesh* emfxMesh = meshBuilder->ConvertToEMotionFXMesh();
            actor->SetMesh(0, emfxNode->GetNodeIndex(), emfxMesh);

            meshBuilder->Destroy();
        }
    } // namespace Pipeline

} // namespace EMotionFX

