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

#include <AzCore/Component/Component.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

#include "InputDeviceGestures.h"

#include "InputChannelGestureClickOrTap.h"
#include "InputChannelGestureDrag.h"
#include "InputChannelGestureHold.h"
#include "InputChannelGesturePinch.h"
#include "InputChannelGestureRotate.h"
#include "InputChannelGestureSwipe.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Gestures
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    class GesturesSystemComponent : public AZ::Component
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        // AZ::Component Setup
        AZ_COMPONENT(GesturesSystemComponent, "{18F55947-9ED4-483D-A3AB-86B848350AF5}");

        ////////////////////////////////////////////////////////////////////////////////////////////
        // AZ::ComponentDescriptor Services
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AZ::ComponentDescriptor::Reflect
        static void Reflect(AZ::ReflectContext* context);

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! Constructor
        GesturesSystemComponent();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Disable copying (protected to workaround a VS2013 bug in std::is_copy_constructible)
        // https://connect.microsoft.com/VisualStudio/feedback/details/800328/std-is-copy-constructible-is-broken
    protected:
        AZ_DISABLE_COPY_MOVE(GesturesSystemComponent);
    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! Destructor
        ~GesturesSystemComponent() override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AZ::Component::Init
        void Init() override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AZ::Component::Activate
        void Activate() override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AZ::Component::Deactivate
        void Deactivate() override;

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ///@{
        //! The config used to create a default gesture input channel
        InputChannelGestureClickOrTap::TypeAndConfig m_doublePressConfig;
        InputChannelGestureDrag::TypeAndConfig       m_dragConfig;
        InputChannelGestureHold::TypeAndConfig       m_holdConfig;
        InputChannelGesturePinch::TypeAndConfig      m_pinchConfig;
        InputChannelGestureRotate::TypeAndConfig     m_rotateConfig;
        InputChannelGestureSwipe::TypeAndConfig      m_swipeConfig;
        ///@}

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! A map of custom gesture name/config pairs that will be used to create additional gesture
        //! input channels, in addition to the default gestures that are provided "out of the box".
        InputDeviceGestures::ConfigsByNameMap m_customGestureConfigsByName;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! Unique pointer to the gestures device
        AZStd::unique_ptr<InputDeviceGestures> m_gesturesDevice;
    };
} // namespace Gestures
