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

// include the required headers
#include <AzCore/Math/Vector2.h>
#include "StandardHeaders.h"
#include "Attribute.h"
#include "Vector.h"
#include "StringConversions.h"

namespace MCore
{
    /**
     * The Vector2 attribute class.
     * This attribute represents one Vector2.
     */
    class MCORE_API AttributeVector2
        : public Attribute
    {
        friend class AttributeFactory;
    public:
        enum
        {
            TYPE_ID = 0x00000005
        };

        static AttributeVector2* Create();
        static AttributeVector2* Create(const AZ::Vector2& value);
        static AttributeVector2* Create(float x, float y);

        MCORE_INLINE uint8* GetRawDataPointer()                     { return reinterpret_cast<uint8*>(&mValue); }
        MCORE_INLINE uint32 GetRawDataSize() const                  { return sizeof(AZ::Vector2); }

        // adjust values
        MCORE_INLINE const AZ::Vector2& GetValue() const                { return mValue; }
        MCORE_INLINE void SetValue(const AZ::Vector2& value)            { mValue = value; }

        // overloaded from the attribute base class
        Attribute* Clone() const override                           { return AttributeVector2::Create(mValue); }
        Attribute* CreateInstance(void* destMemory) override        { return new(destMemory) AttributeVector2(); }
        const char* GetTypeString() const override                  { return "AttributeVector2"; }
        bool InitFrom(const Attribute* other) override
        {
            if (other->GetType() != TYPE_ID)
            {
                return false;
            }
            mValue = static_cast<const AttributeVector2*>(other)->GetValue();
            return true;
        }
        bool InitFromString(const AZStd::string& valueString) override
        {
            return AzFramework::StringFunc::LooksLikeVector2(valueString.c_str(), &mValue);
        }
        bool ConvertToString(AZStd::string& outString) const override      { AZStd::to_string(outString, mValue); return true; }
        uint32 GetClassSize() const override                        { return sizeof(AttributeVector2); }
        uint32 GetDefaultInterfaceType() const override             { return ATTRIBUTE_INTERFACETYPE_VECTOR2; }

    private:
        AZ::Vector2     mValue;     /**< The Vector2 value. */

        AttributeVector2()
            : Attribute(TYPE_ID)                    { mValue.Set(0.0f, 0.0f); }
        AttributeVector2(const AZ::Vector2& value)
            : Attribute(TYPE_ID)
            , mValue(value)     { }
        ~AttributeVector2() { }

        uint32 GetDataSize() const override                         { return sizeof(AZ::Vector2); }

        // read from a stream
        bool ReadData(MCore::Stream* stream, MCore::Endian::EEndianType streamEndianType, uint8 version) override
        {
            MCORE_UNUSED(version);

            // read the value
            AZ::Vector2 streamValue;
            if (stream->Read(&streamValue, sizeof(AZ::Vector2)) == 0)
            {
                return false;
            }

            // convert endian
            Endian::ConvertVector2(&streamValue, streamEndianType);
            mValue = streamValue;

            return true;
        }

    };
}   // namespace MCore
