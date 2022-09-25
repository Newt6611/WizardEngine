#pragma once
#include <entt/entt.hpp>

namespace Wizard {
    using Entity = entt::entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void OnUpdate(float ts);

        Entity CreateEntity()
        {
            return m_Registry.create();
        }

        template<typename Component, typename... Args>
        void AddComponent(Entity& e, Args &&...args)
        {
            m_Registry.emplace<Component>(e, std::forward<Args>(args)...);
        }

        template<typename T>
        void RemoveComponent(Entity& e)
        {
            m_Registry.remove<T>(e);
        }

    private:
        entt::registry m_Registry;
    };
}