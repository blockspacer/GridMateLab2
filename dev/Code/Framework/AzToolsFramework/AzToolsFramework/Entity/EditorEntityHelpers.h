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
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <AzToolsFramework/ToolsComponents/EditorDisabledCompositionBus.h>
#include <AzToolsFramework/ToolsComponents/EditorPendingCompositionBus.h>
#include <AzToolsFramework/API/EntityCompositionRequestBus.h>
#include <AzCore/Component/ComponentApplication.h>

namespace AzToolsFramework
{
    // Entity helpers (Uses EBus calls, so safe)
    AZ::Entity* GetEntityById(const AZ::EntityId& entityId);

    inline AZ::Entity* GetEntity(const AZ::EntityId& entityId)
    {
        return GetEntityById(entityId);
    }

    inline AZ::Entity* GetEntity(AZ::Entity* entity)
    {
        return entity;
    }

    inline AZ::EntityId GetEntityId(const AZ::Entity* entity)
    {
        return entity->GetId();
    }

    inline AZ::EntityId GetEntityId(const AZ::EntityId& entityId)
    {
        return entityId;
    }

    template <typename... ComponentTypes>
    struct AddComponents
    {
        template <typename... EntityType>
        static EntityCompositionRequests::AddComponentsOutcome ToEntities(EntityType... entities)
        {
            EntityCompositionRequests::AddComponentsOutcome outcome = AZ::Failure(AZStd::string(""));
            EntityCompositionRequestBus::BroadcastResult(outcome, &EntityCompositionRequests::AddComponentsToEntities, EntityIdList{ GetEntityId(entities)... }, AZ::ComponentTypeList{ azrtti_typeid<ComponentTypes>()... });
            return outcome;
        }
    };

    template <typename ComponentType>
    struct FindComponent
    {
        template <typename EntityType>
        static ComponentType* OnEntity(EntityType entityParam)
        {
            auto entity = GetEntity(entityParam);
            if (!entity)
            {
                return nullptr;
            }
            return entity->FindComponent<ComponentType>();
        }
    };

    template <typename... ComponentType>
    EntityCompositionRequests::RemoveComponentsOutcome RemoveComponents(ComponentType... components)
    {
        EntityCompositionRequests::RemoveComponentsOutcome outcome = AZ::Failure(AZStd::string(""));
        EntityCompositionRequestBus::BroadcastResult(outcome, &EntityCompositionRequests::RemoveComponents, AZ::Entity::ComponentArrayType{ components... });
        return outcome;
    }

    template <typename... ComponentType>
    void EnableComponents(ComponentType... components)
    {
        EntityCompositionRequestBus::Broadcast(&EntityCompositionRequests::EnableComponents, AZ::Entity::ComponentArrayType{ components... });
    }

    template <typename... ComponentType>
    void DisableComponents(ComponentType... components)
    {
        EntityCompositionRequestBus::Broadcast(&EntityCompositionRequests::DisableComponents, AZ::Entity::ComponentArrayType{ components... });
    }

    void GetAllComponentsForEntity(const AZ::Entity* entity, AZ::Entity::ComponentArrayType& componentsOnEntity);
    void GetAllComponentsForEntity(const AZ::EntityId& entityId, AZ::Entity::ComponentArrayType& componentsOnEntity);

    // Component helpers (Uses EBus calls, so safe)
    AZ::Uuid GetComponentTypeId(const AZ::Component* component);
    const AZ::SerializeContext::ClassData* GetComponentClassData(const AZ::Component* component);
    const AZ::SerializeContext::ClassData* GetComponentClassDataForType(const AZ::Uuid& componentTypeId);
    AZStd::string GetFriendlyComponentName(const AZ::Component* component);
    const char* GetFriendlyComponentDescription(const AZ::Component* component);
    AZ::ComponentDescriptor* GetComponentDescriptor(const AZ::Component* component);
    Components::EditorComponentDescriptor* GetEditorComponentDescriptor(const AZ::Component* component);
    Components::EditorComponentBase* GetEditorComponent(AZ::Component* component);

    AZ::EntityId GetEntityIdForSortInfo(const AZ::EntityId parentId);

    void AddEntityIdToSortInfo(const AZ::EntityId parentId, const AZ::EntityId childId, bool forceAddToBack = false);

    void RecoverEntitySortInfo(const AZ::EntityId parentId, const AZ::EntityId childId, AZ::u64 sortIndex);

    void RemoveEntityIdFromSortInfo(const AZ::EntityId parentId, const AZ::EntityId childId);

    void SetEntityChildOrder(const AZ::EntityId parentId, const EntityIdList& children);

    EntityIdList GetEntityChildOrder(const AZ::EntityId parentId);

    void GetEntityLocationInHierarchy(const AZ::EntityId& entityId, std::list<AZ::u64>& location);

    void SortEntitiesByLocationInHierarchy(EntityIdList& entityIds);

    bool EntityHasComponentOfType(const AZ::EntityId& entityId, AZ::Uuid componentType);
    bool IsComponentWithServiceRegistered(const AZ::Crc32& serviceId);

}; // namespace AzToolsFramework