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

#include <AzCore/std/containers/array.h>
#include "AnimGraphTransitionFixture.h"
#include <EMotionFX/Source/Actor.h>
#include <EMotionFX/Source/ActorInstance.h>
#include <EMotionFX/Source/AnimGraph.h>
#include <EMotionFX/Source/AnimGraphInstance.h>
#include <EMotionFX/Source/AnimGraphMotionCondition.h>
#include <EMotionFX/Source/AnimGraphMotionNode.h>
#include <EMotionFX/Source/AnimGraphStateMachine.h>
#include <EMotionFX/Source/AnimGraphStateTransition.h>
#include <EMotionFX/Source/MotionInstance.h>
#include <EMotionFX/Source/MotionSet.h>
#include <EMotionFX/Source/Node.h>
#include <EMotionFX/Source/SkeletalMotion.h>

namespace EMotionFX
{
    void AnimGraphTransitionFixture::SetUp()
    {
        SystemComponentFixture::SetUp();

        // This test sets up an anim graph with 2 motions, each of which is 1
        // second long. There is a transition from the first to the second that
        // triggers when the first is complete, and takes 0.5 seconds to
        // transition, and an identical one going the other way. During the
        // transition, the weights of the motion states should add up to 1.
        mActor = Actor::Create("testActor");
        Node* rootNode = Node::Create("rootNode", mActor->GetSkeleton());
        mActor->AddNode(rootNode);

        mAnimGraph = AnimGraph::Create("testAnimGraph");

        mStateMachine = AnimGraphStateMachine::Create(mAnimGraph, "rootStateMachine");
        mAnimGraph->SetRootStateMachine(mStateMachine);

        mMotionNode0 = AnimGraphMotionNode::Create(mAnimGraph);
        mMotionNode1 = AnimGraphMotionNode::Create(mAnimGraph);

        AnimGraphStateTransition* transition0 = AnimGraphStateTransition::Create(mAnimGraph);
        transition0->SetSourceNode(mMotionNode0);
        transition0->SetTargetNode(mMotionNode1);
        AnimGraphMotionCondition* condition0 = AnimGraphMotionCondition::Create(mAnimGraph);
        transition0->AddCondition(condition0);

        AnimGraphStateTransition* transition1 = AnimGraphStateTransition::Create(mAnimGraph);
        transition1->SetSourceNode(mMotionNode1);
        transition1->SetTargetNode(mMotionNode0);
        AnimGraphMotionCondition* condition1 = AnimGraphMotionCondition::Create(mAnimGraph);
        transition1->AddCondition(condition1);

        mStateMachine->AddChildNode(mMotionNode0);
        mStateMachine->AddChildNode(mMotionNode1);
        mStateMachine->SetEntryState(mMotionNode0);
        mStateMachine->AddTransition(transition0);
        mStateMachine->AddTransition(transition1);

        mMotionSet = MotionSet::Create("testMotionSet");
        for (int i = 0; i < 2; ++i)
        {
            // The motion set keeps track of motions by their name. Each motion
            // within the motion set must have a unique name.
            AZStd::string motionId = AZStd::string::format("testSkeletalMotion%i", i);
            SkeletalMotion* motion = SkeletalMotion::Create(motionId.c_str());
            motion->SetMaxTime(1.0f);
            MotionSet::MotionEntry* motionEntry = MotionSet::MotionEntry::Create(motion->GetName(), motion->GetName(), motion);
            mMotionSet->AddMotionEntry(motionEntry);

            AnimGraphMotionNode* motionNode = i == 0 ? mMotionNode0 : mMotionNode1;
            motionNode->SetName(motionId.c_str());
            MCore::AttributeArray* motionArrayAttr = motionNode->GetAttributeArray(AnimGraphMotionNode::ATTRIB_MOTION);
            MCore::AttributeString* motionAttr = static_cast<MCore::AttributeString*>(motionArrayAttr->AddAttribute());
            motionAttr->SetValue(motionId.c_str());
            motionNode->OnUpdateAttributes();
        }

        condition0->GetAttributeString(AnimGraphMotionCondition::ATTRIB_MOTIONNODE)->SetValue("testSkeletalMotion0");
        condition0->GetAttributeFloat(AnimGraphMotionCondition::ATTRIB_FUNCTION)->SetValue(AnimGraphMotionCondition::FUNCTION_HASENDED);
        condition0->OnUpdateAttributes();
        transition0->GetAttributeFloat(AnimGraphStateTransition::ATTRIB_BLENDTIME)->SetValue(0.5f);
        transition0->OnUpdateAttributes();

        condition1->GetAttributeString(AnimGraphMotionCondition::ATTRIB_MOTIONNODE)->SetValue("testSkeletalMotion1");
        condition1->GetAttributeFloat(AnimGraphMotionCondition::ATTRIB_FUNCTION)->SetValue(AnimGraphMotionCondition::FUNCTION_HASENDED);
        condition1->OnUpdateAttributes();
        transition1->GetAttributeFloat(AnimGraphStateTransition::ATTRIB_BLENDTIME)->SetValue(0.5f);
        transition1->OnUpdateAttributes();

        mActorInstance = ActorInstance::Create(mActor);

        mAnimGraphInstance = AnimGraphInstance::Create(mAnimGraph, mActorInstance, mMotionSet);

        mActorInstance->SetAnimGraphInstance(mAnimGraphInstance);

        mAnimGraphInstance->RecursivePrepareNodes();
        mAnimGraphInstance->Init();
        mAnimGraphInstance->OnUpdateUniqueData();
    }

    void AnimGraphTransitionFixture::TearDown()
    {
        const AZStd::array<MCore::MemoryObject*, 4> objectsToDestroy = {{mActorInstance, mMotionSet, mAnimGraph, mActor}};
        for (MCore::MemoryObject* object : objectsToDestroy) 
        {
            if (object)
            {
                object->Destroy();
            }
        }

        SystemComponentFixture::TearDown();
    }
}
