#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/PlayerControlsRequestBus.h>
#include <AzCore/Component/TickBus.h>

namespace MultiplayerCharacter
{
    // Implements various player controls
    class PlayerControlsComponent
        : public AZ::Component
        , public PlayerControlsRequestBus::Handler
        , public AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT(PlayerControlsComponent,
            "{6621E8AD-B15B-4F47-940D-8ACAE03BDCFF}");

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* ref);

        // what other components does this component require?
        static void GetRequiredServices(
            AZ::ComponentDescriptor::DependencyArrayType& req);

    protected:
        // PlayerControlsRequestBus interface
        void MoveForward(ActionState state) override;
        void MoveBackward(ActionState state) override;
        void StrafeLeft(ActionState state) override;
        void StrafeRight(ActionState state) override;
        void Turn(float amount) override;
        void LookUpOrDown(float amount) override;

        // TickBus interface
        void OnTick(float dt, AZ::ScriptTimePoint) override;

    private:
        bool m_isForward = false;
        bool m_isBackward = false;
        bool m_isStrafingLeft = false;
        bool m_isStrafingRight = false;

        float m_speed = 100.f;
        float m_turnSpeed = 500.f;
        float m_lookUpSpeed = .1f;

        float m_rotY = 0.f;
        float m_rotZ = 0.f;
        void SetRotation();
    };
}