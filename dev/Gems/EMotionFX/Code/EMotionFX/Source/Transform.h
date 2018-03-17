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

#pragma once

// include required headers
#include "EMotionFXConfig.h"
#include <MCore/Source/MemoryCategoriesCore.h>
#include <MCore/Source/Vector.h>
#include <MCore/Source/Quaternion.h>
#include <MCore/Source/Matrix4.h>


namespace EMotionFX
{
    // forward declarations
    enum EMotionExtractionFlags : uint8;


    class EMFX_API MCORE_ALIGN_PRE(16) Transform
    {
        MCORE_MEMORYOBJECTCATEGORY(Transform, MCore::MCORE_SIMD_ALIGNMENT, EMFX_MEMCATEGORY_TRANSFORM);

    public:
        MCORE_INLINE Transform()
        {
            mPosition = AZ::Vector3::CreateZero();
            mRotation.Identity();
            EMFX_SCALECODE
            (
                mScale.Set(1.0f, 1.0f, 1.0f);
            );
        }

        Transform(const AZ::Vector3 & pos, const MCore::Quaternion & rotation);
        Transform(const AZ::Vector3 & pos, const MCore::Quaternion & rotation, const AZ::Vector3 & scale);
        Transform(const MCore::Matrix & mat);

        void Set(const AZ::Vector3 & pos, const MCore::Quaternion & rotation);
        void Set(const AZ::Vector3 & pos, const MCore::Quaternion & rotation, const AZ::Vector3 & scale);

        void InitFromMatrix(const MCore::Matrix & mat);  // relatively slow as it decomposes the matrix
        MCore::Matrix ToMatrix() const;
        void ToMatrix(MCore::Matrix & outMatrix) const;

        void Identity();
        void Zero();
        void ZeroWithIdentityQuaternion();

        /**
         * Multiply this transform with another transform.
         * @param other The other transformation (right hand side in the multiplication).
         * @return Returns a reference to itself.
         */
        Transform& Multiply(const Transform& other);
        Transform Multiplied(const Transform&other) const;
        Transform& PreMultiply(const Transform& other);
        Transform PreMultiplied(const Transform&other) const;

        /**
         * Multiply this transform with another transform and store the result it in yet another.
         * @param other The other transformation (right hand side in the multiplication).
         * @param outResult The transform that will contain the result of the multiply.
         */
        void Multiply(const Transform&other, Transform * outResult) const;
        void PreMultiply(const Transform&other, Transform * outResult) const;

        /**
         * Inverse the transformation.
         * @return Returns a reference to itself.
         */
        Transform& Inverse();
        Transform Inversed() const;
        void CalcRelativeTo(const Transform&relativeTo, Transform * outTransform) const;
        Transform CalcRelativeTo(const Transform&relativeTo) const;

        /**
         * Inverse the transformation and output the inversed result into a given other transform.
         * @param outResult The transform that will receive the inversed transform.
         */
        void Inverse(Transform * outResult) const;

        /**
         * Mirror this transformation along a plane normal.
         * @param planeNormal The plane normal.
         * @return Returns a reference to itself.
         */
        Transform& Mirror(const AZ::Vector3& planeNormal);
        Transform& MirrorWithFlags(const AZ::Vector3& planeNormal, uint8 flags);

        Transform Mirrored(const AZ::Vector3 & planeNormal) const;

        /**
         * Mirror this transformation along a plane normal.
         * @param planeNormal The plane normal.
         * @param outResult The transform to output the mirrored version into.
         */
        void Mirror(const AZ::Vector3 & planeNormal, Transform * outResult) const;

        void ApplyDelta(const Transform&sourceTransform, const Transform&targetTransform);
        void ApplyDeltaMirrored(const Transform&sourceTransform, const Transform&targetTransform, const AZ::Vector3 & mirrorPlaneNormal, uint8 mirrorFlags = 0);
        void ApplyDeltaWithWeight(const Transform&sourceTransform, const Transform&targetTransform, float weight);

        /**
         * Check if this transform has a non-identity scale or not.
         * @result Returns true when this transform has scaling included, otherwise false is returned.
         */
        bool CheckIfHasScale() const;

        void Log(const char* name = nullptr) const;
        Transform& Normalize();
        Transform Normalized() const;

        Transform& Blend(const Transform& dest, float weight);
        Transform& BlendAdditive(const Transform& dest, const Transform& orgTransform, float weight);
        Transform& Add(const Transform& other, float weight);
        Transform& Add(const Transform& other);
        Transform& Subtract(const Transform& other);

        void ApplyMotionExtractionFlags(EMotionExtractionFlags flags);
        Transform ProjectedToGroundPlane() const;

        static void ApplyMirrorFlags(Transform * inOutTransform, uint8 mirrorFlags);

        // operators
        Transform   operator +  (const Transform& right) const;
        Transform   operator -  (const Transform& right) const;
        Transform   operator *  (const Transform& right) const;
        Transform&  operator += (const Transform& right);
        Transform&  operator -= (const Transform& right);
        Transform&  operator *= (const Transform& right);
        bool        operator == (const Transform& right) const;
        bool        operator != (const Transform& right) const;

    public:
        MCore::Quaternion   mRotation;          /**< The rotation. */
        AZ::Vector3      mPosition;             /**< The position. */
        #ifndef EMFX_SCALE_DISABLED
        AZ::Vector3  mScale;                    /**< The scale. */
        #endif
    }
    MCORE_ALIGN_POST(16);
}   // namespace EMotionFX
