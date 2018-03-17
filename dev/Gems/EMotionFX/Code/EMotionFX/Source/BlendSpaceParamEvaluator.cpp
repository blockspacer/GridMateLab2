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

#include "BlendSpaceParamEvaluator.h"
#include "MotionInstance.h"
#include "ActorInstance.h"
#include "Node.h"


namespace EMotionFX
{
    MCORE_INLINE bool GetMotionActorAndNode(const MotionInstance& motionInstance,
        Motion*& motion, Actor*& actor, Node*& node)
    {
        motion = motionInstance.GetMotion();
        AZ_Assert(motion, "Invalid motion pointer in MotionInstance");
        if (!motion)
        {
            return false;
        }
        ActorInstance* actorInstance = motionInstance.GetActorInstance();
        AZ_Assert(actorInstance, "Invalid actorInstance");
        if (!actorInstance)
        {
            return false;
        }
        actor = actorInstance->GetActor();
        AZ_Assert(actor, "Invalid actor");
        if (!actor)
        {
            return false;
        }
        node = actor->GetMotionExtractionNode();
        if (!node)
        {
            node = actor->FindBestMotionExtractionNode();
            if (node)
            {
                actor->SetMotionExtractionNode(node);
            }
        }
        AZ_Assert(node, "Motion extraction node not found");
        if (!node)
        {
            return false;
        }
        return true;
    }

    float BlendSpaceParamEvaluatorNone::ComputeParamValue(const MotionInstance& motionInstance)
    {
        MCORE_UNUSED(motionInstance);
        return 0;
    }

    const char* BlendSpaceParamEvaluatorNone::GetName() const
    {
        return "Select an evaluator";
    }

    float BlendSpaceMoveSpeedParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const bool retargeting = motionInstance.GetRetargetingEnabled();
        const float sampleRate = motion->GetMotionFPS();
        float sampleTimeStep;
        uint32 numSamples;
        MCore::CalcSampleRateInfo(sampleRate, duration, &sampleTimeStep, &numSamples);

        Transform transform;
        motion->CalcNodeTransform(&motionInstance, &transform, actor, node, 0, retargeting);
        AZ::Vector3 position(transform.mPosition);
        float distance = 0.0f;
        float time = sampleTimeStep;
        for (uint32 i = 1; i < numSamples; ++i, time += sampleTimeStep)
        {
            motion->CalcNodeTransform(&motionInstance, &transform, actor, node, time, retargeting);
            distance += (transform.mPosition - position).GetLength();
            position = transform.mPosition;
        }

        return distance / duration;
    }

    const char* BlendSpaceMoveSpeedParamEvaluator::GetName() const
    {
        return "Move speed";
    }

    float BlendSpaceTurnSpeedParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const bool retargeting = motionInstance.GetRetargetingEnabled();
        const float sampleRate = motion->GetMotionFPS();
        float sampleTimeStep;
        uint32 numSamples;
        MCore::CalcSampleRateInfo(sampleRate, duration, &sampleTimeStep, &numSamples);

        Transform transform;
        motion->CalcNodeTransform(&motionInstance, &transform, actor, node, 0, retargeting);
        MCore::Quaternion rotation(transform.mRotation);
        float totalAngle = 0.0f;
        float time = sampleTimeStep;
        for (uint32 i = 1; i < numSamples; ++i, time += sampleTimeStep)
        {
            motion->CalcNodeTransform(&motionInstance, &transform, actor, node, time, retargeting);
            MCore::Quaternion deltaRotation = transform.mRotation * rotation.Conjugated();
            const float angle = -deltaRotation.GetEulerZ();// negating because we prefer the convention of clockwise being +ve
            totalAngle += angle;
            rotation = transform.mRotation;
        }

        return totalAngle / duration;
    }

    const char* BlendSpaceTurnSpeedParamEvaluator::GetName() const
    {
        return "Turn speed";
    }

    float BlendSpaceTravelDirectionParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const bool retargeting = motionInstance.GetRetargetingEnabled();

        Transform startTransform;
        motion->CalcNodeTransform(&motionInstance, &startTransform, actor, node, 0, retargeting);
        Transform endTransform;
        motion->CalcNodeTransform(&motionInstance, &endTransform, actor, node, duration, retargeting);

        AZ::Vector3 diffVec(endTransform.mPosition - startTransform.mPosition);
        return ::atan2f(diffVec.GetX(), diffVec.GetY());
    }

    const char* BlendSpaceTravelDirectionParamEvaluator::GetName() const
    {
        return "Travel direction";
    }

    float BlendSpaceTravelSlopeParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const bool retargeting = motionInstance.GetRetargetingEnabled();
        const float sampleRate = motion->GetMotionFPS();
        float sampleTimeStep;
        uint32 numSamples;
        MCore::CalcSampleRateInfo(sampleRate, duration, &sampleTimeStep, &numSamples);

        Transform transform;
        motion->CalcNodeTransform(&motionInstance, &transform, actor, node, 0, retargeting);
        AZ::Vector3 position(transform.mPosition);
        float slopeSum = 0.0f;
        float time = sampleTimeStep;
        uint32 count = 0; // number of samples added to slopeSum
        for (uint32 i = 1; i < numSamples; ++i, time += sampleTimeStep)
        {
            motion->CalcNodeTransform(&motionInstance, &transform, actor, node, time, retargeting);
            AZ::Vector3 diffVec(transform.mPosition - position);
            float horizontalDistance = AZ::Vector2(diffVec.GetX(), diffVec.GetY()).GetLength();
            if (horizontalDistance > 0)
            {
                slopeSum += atan2f(diffVec.GetZ(), horizontalDistance);
                position = transform.mPosition;
                count++;
            }
        }

        return (count > 0) ? (slopeSum / count) : 0.0f;
    }

    const char* BlendSpaceTravelSlopeParamEvaluator::GetName() const
    {
        return "Travel slope";
    }

    float BlendSpaceTurnAngleParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const bool retargeting = motionInstance.GetRetargetingEnabled();
        const float sampleRate = motion->GetMotionFPS();
        float sampleTimeStep;
        uint32 numSamples;
        MCore::CalcSampleRateInfo(sampleRate, duration, &sampleTimeStep, &numSamples);
        AZ_Assert(numSamples > 1, "There should be at least two samples");

        Transform transform;
        motion->CalcNodeTransform(&motionInstance, &transform, actor, node, 0, retargeting);
        MCore::Quaternion rotation(transform.mRotation);
        float totalTurnAngle = 0.0f;
        float time = sampleTimeStep;
        for (uint32 i = 1; i < numSamples; ++i, time += sampleTimeStep)
        {
            motion->CalcNodeTransform(&motionInstance, &transform, actor, node, time, retargeting);
            MCore::Quaternion deltaRotation = transform.mRotation * rotation.Conjugated();
            const float angle = -deltaRotation.GetEulerZ();// negating because we prefer the convention of clockwise being +ve
            totalTurnAngle += angle;
            rotation = transform.mRotation;
        }

        return totalTurnAngle;
    }

    const char* BlendSpaceTurnAngleParamEvaluator::GetName() const
    {
        return "Turn angle";
    }

    float BlendSpaceTravelDistanceParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const bool retargeting = motionInstance.GetRetargetingEnabled();

        Transform startTransform;
        motion->CalcNodeTransform(&motionInstance, &startTransform, actor, node, 0, retargeting);
        Transform endTransform;
        motion->CalcNodeTransform(&motionInstance, &endTransform, actor, node, duration, retargeting);

        return (endTransform.mPosition - startTransform.mPosition).GetLength();
    }

    const char* BlendSpaceTravelDistanceParamEvaluator::GetName() const
    {
        return "Travel distance";
    }

    float BlendSpaceLeftRightVelocityParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const AZ::Vector3 xAxis(1.0f, 0, 0);

        const bool retargeting = motionInstance.GetRetargetingEnabled();
        const float sampleRate = motion->GetMotionFPS();
        float sampleTimeStep;
        uint32 numSamples;
        MCore::CalcSampleRateInfo(sampleRate, duration, &sampleTimeStep, &numSamples);

        Transform transform;
        motion->CalcNodeTransform(&motionInstance, &transform, actor, node, 0, retargeting);
        AZ::Vector3 position(transform.mPosition);
        float distance = 0.0f;
        float time = sampleTimeStep;
        for (uint32 i = 1; i < numSamples; ++i, time += sampleTimeStep)
        {
            motion->CalcNodeTransform(&motionInstance, &transform, actor, node, time, retargeting);
            AZ::Vector3 moveVec(transform.mPosition - position);
            distance += moveVec.Dot(xAxis);
            position = transform.mPosition;
        }

        return distance / duration;
    }

    const char* BlendSpaceLeftRightVelocityParamEvaluator::GetName() const
    {
        return "Left-right velocity";
    }

    float BlendSpaceFrontBackVelocityParamEvaluator::ComputeParamValue(const MotionInstance& motionInstance)
    {
        Motion* motion;
        Actor* actor;
        Node* node;
        if (!GetMotionActorAndNode(motionInstance, motion, actor, node))
        {
            return 0.0f;
        }

        const float duration = motion->GetMaxTime();
        if (duration <= 0)
        {
            return 0.0f;
        }

        const AZ::Vector3 yAxis(0, 1.0f, 0);

        const bool retargeting = motionInstance.GetRetargetingEnabled();
        const float sampleRate = motion->GetMotionFPS();
        float sampleTimeStep;
        uint32 numSamples;
        MCore::CalcSampleRateInfo(sampleRate, duration, &sampleTimeStep, &numSamples);

        Transform transform;
        motion->CalcNodeTransform(&motionInstance, &transform, actor, node, 0, retargeting);
        AZ::Vector3 position(transform.mPosition);
        float distance = 0.0f;
        float time = sampleTimeStep;
        for (uint32 i = 1; i < numSamples; ++i, time += sampleTimeStep)
        {
            motion->CalcNodeTransform(&motionInstance, &transform, actor, node, time, retargeting);
            AZ::Vector3 moveVec(transform.mPosition - position);
            distance += moveVec.Dot(yAxis);
            position = transform.mPosition;
        }

        return distance / duration;
    }

    const char* BlendSpaceFrontBackVelocityParamEvaluator::GetName() const
    {
        return "Front-back velocity";
    }
} // namespace EMotionFX
