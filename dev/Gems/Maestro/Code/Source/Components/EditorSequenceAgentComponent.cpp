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
#include "Maestro_precompiled.h"
#include "EditorSequenceAgentComponent.h"
#include "SequenceAgentComponent.h"

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzFramework/API/ApplicationAPI.h>
#include <AzToolsFramework/ToolsComponents/GenericComponentWrapper.h>
#include <AzCore/Component/Entity.h>
#include <AzToolsFramework/API/EntityCompositionRequestBus.h>
#include <Maestro/Types/AnimParamType.h>

namespace Maestro
{
    void EditorSequenceAgentComponent::Reflect(AZ::ReflectContext* context)
    {
        AZ::EntityComponentIdPair::Reflect(context);

        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);

        if (serializeContext)
        {
            serializeContext->Class<EditorSequenceAgentComponent, EditorComponentBase>()
                ->Field("SequenceComponentEntityIds", &EditorSequenceAgentComponent::m_sequenceEntityIds)
                ->Version(3);

            AZ::EditContext* editContext = serializeContext->GetEditContext();

            if (editContext)
            {
                editContext->Class<EditorSequenceAgentComponent>(
                    "SequenceAgent", "Maps Director Component Animations to Behavior Properties on this Entity")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Cinematics")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/SequenceAgent.png")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Components/Viewport/SequenceAgent.png")
                    ->Attribute(AZ::Edit::Attributes::AddableByUser, false)     // SequenceAgents are only added by TrackView
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    const AZ::Uuid& EditorSequenceAgentComponent::GetComponentTypeUuid(const AZ::Component& component) const
    {
        return AzToolsFramework::GetUnderlyingComponentType(component);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::Activate()
    {
        // cache pointers and animatable addresses for animation
        //
        CacheAllVirtualPropertiesFromBehaviorContext(GetEntity());

        ConnectAllSequences();

        EditorComponentBase::Activate();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::Deactivate()
    {
        // invalidate all cached pointers and addresss
        m_addressToBehaviorVirtualPropertiesMap.clear();

        DisconnectAllSequences();

        EditorComponentBase::Deactivate();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::ConnectSequence(const AZ::EntityId& sequenceEntityId)
    {
        // check that we aren't already connected to this SequenceComponent - add it if we aren't
        if (m_sequenceEntityIds.find(sequenceEntityId) == m_sequenceEntityIds.end())
        {
            m_sequenceEntityIds.insert(sequenceEntityId);
            // connect to EBus between the given SequenceComponent and me
            Maestro::SequenceAgentEventBusId busId(sequenceEntityId, GetEntityId());
            EditorSequenceAgentComponentRequestBus::MultiHandler::BusConnect(busId);
            SequenceAgentComponentRequestBus::MultiHandler::BusConnect(busId);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::DisconnectSequence()
    {   
        const Maestro::SequenceAgentEventBusId* busIdToDisconnect = SequenceAgentComponentRequestBus::GetCurrentBusId();

        if (!busIdToDisconnect)
        {
            return;
        }

        AZ::EntityId sequenceEntityId = busIdToDisconnect->first;

        // we only process DisconnectSequence events sent over an ID'ed bus - otherwise we don't know which SequenceComponent to disconnect
        auto findIter = m_sequenceEntityIds.find(sequenceEntityId);
        AZ_Assert(findIter != m_sequenceEntityIds.end(), "A sequence not connected to SequenceAgentComponent on %s is requesting a disconnection", GetEntity()->GetName());

        m_sequenceEntityIds.erase(sequenceEntityId);

        // Disconnect from the bus between the SequenceComponent and me
        EditorSequenceAgentComponentRequestBus::MultiHandler::BusDisconnect(*busIdToDisconnect);
        SequenceAgentComponentRequestBus::MultiHandler::BusDisconnect(*busIdToDisconnect);

        if (m_sequenceEntityIds.size())
        {
            return;
        }

        // remove this SequenceAgent from this entity if no sequenceComponents are connected to it
        AzToolsFramework::EntityCompositionRequestBus::Broadcast(&AzToolsFramework::EntityCompositionRequests::RemoveComponents, AZ::Entity::ComponentArrayType{this});        
        
        // CAUTION!
        // THIS CLASS INSTANCE IS NOW DEAD DUE TO DELETION BY THE ENTITY DURING RemoveComponents!
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::ConnectAllSequences()
    {
        // Connect all buses
        for (auto iter = m_sequenceEntityIds.begin(); iter != m_sequenceEntityIds.end(); iter++)
        {
            Maestro::SequenceAgentEventBusId busIdToConnect(*iter, GetEntityId());
            EditorSequenceAgentComponentRequestBus::MultiHandler::BusConnect(busIdToConnect);
            SequenceAgentComponentRequestBus::MultiHandler::BusConnect(busIdToConnect);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::DisconnectAllSequences()
    {
        // disconnect all buses
        for (auto iter = m_sequenceEntityIds.begin(); iter != m_sequenceEntityIds.end(); iter++)
        {
            Maestro::SequenceAgentEventBusId busIdToDisconnect(*iter, GetEntityId());
            EditorSequenceAgentComponentRequestBus::MultiHandler::BusDisconnect(busIdToDisconnect);
            SequenceAgentComponentRequestBus::MultiHandler::BusDisconnect(busIdToDisconnect);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        SequenceAgentComponent *sequenceAgentComponent = gameEntity->CreateComponent<SequenceAgentComponent>();
        if (sequenceAgentComponent)
        {
            // TODO: when we have code which only allows animation of properties that are common between the SequenceAgent and EditorSequenceAgent,
            // transfer the mappings to the game behaviors here
            sequenceAgentComponent->m_sequenceEntityIds = m_sequenceEntityIds;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::GetAllAnimatableProperties(IAnimNode::AnimParamInfos& properties, AZ::ComponentId componentId)
    {       
        // add all properties found during Activate() that match the given componentId
        for (auto propertyIter = m_addressToBehaviorVirtualPropertiesMap.begin(); propertyIter != m_addressToBehaviorVirtualPropertiesMap.end(); propertyIter++)
        {
            if (propertyIter->first.GetComponentId() == componentId)
            {
                AZ::BehaviorEBus::VirtualProperty* virtualProperty = propertyIter->second;
                // all behavior properties as string params with the virtual property name as the string
                IAnimNode::SParamInfo paramInfo;

                // by default set up paramType as an AnimParamType::ByString with the name as the Virtual Property name
                paramInfo.paramType = propertyIter->first.GetVirtualPropertyName().c_str();

                // check for paramType specialization attributes on the getter method of the virtual property. if found, reset
                // to the eAnimParamType enum - this leaves the paramType name unchanged but changes the type.
                for (int i = virtualProperty->m_getter->m_attributes.size(); --i >= 0;)
                {
                    if (virtualProperty->m_getter->m_attributes[i].first == AZ::Crc32("Position"))
                    {
                        paramInfo.paramType = AnimParamType::Position;
                        break;
                    }
                    else if (virtualProperty->m_getter->m_attributes[i].first == AZ::Crc32("Rotation"))
                    {
                        paramInfo.paramType = AnimParamType::Rotation;
                        break;
                    }
                    else if (virtualProperty->m_getter->m_attributes[i].first == AZ::Crc32("Scale"))
                    {
                        paramInfo.paramType = AnimParamType::Scale;
                        break;
                    }
                }

                properties.push_back(paramInfo);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::GetAnimatableComponents(AZStd::vector<AZ::ComponentId>& animatableComponentIds)
    {
        AZStd::set<AZ::ComponentId> appendedComponentIds;

        // go through all known properties found during Activate() - insert all unique components found
        for (auto address = m_addressToBehaviorVirtualPropertiesMap.begin(); address != m_addressToBehaviorVirtualPropertiesMap.end(); address++)
        {
            // only append component if it's not already been appended
            auto findIter = appendedComponentIds.find(address->first.GetComponentId());
            if (findIter == appendedComponentIds.end())
            {
                animatableComponentIds.push_back(address->first.GetComponentId());   
                appendedComponentIds.insert(address->first.GetComponentId());
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    AZ::Uuid EditorSequenceAgentComponent::GetAnimatedAddressTypeId(const AnimatablePropertyAddress& animatableAddress)
    {
        return GetVirtualPropertyTypeId(animatableAddress);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::GetAnimatedPropertyValue(AnimatedValue& returnValue, const AnimatablePropertyAddress& animatableAddress)
    {
        SequenceAgent::GetAnimatedPropertyValue(returnValue, GetEntityId(), animatableAddress);
    }

    bool EditorSequenceAgentComponent::SetAnimatedPropertyValue(const AnimatablePropertyAddress& animatableAddress, const AnimatedValue& value)
    {
        return SequenceAgent::SetAnimatedPropertyValue(GetEntityId(), animatableAddress, value);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorSequenceAgentComponent::GetAssetDuration(AnimatedValue& returnValue, AZ::ComponentId componentId, const AZ::Data::AssetId& assetId)
    {
        SequenceAgent::GetAssetDuration(returnValue, componentId, assetId);
    }

} // namespace Maestro