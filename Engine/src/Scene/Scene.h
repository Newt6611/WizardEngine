#pragma once
#include <entt/entt.hpp>


class b2World;
namespace Wizard {
    using Entity = entt::entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void OnUpdate(float ts);

        void OnSimulatePhysicsStart();

        Entity CreateEntity()
        {
            return m_Registry.create();
        }

        template<typename T, typename... Args>
        T& AddComponent(Entity& e, Args &&...args)
        {
            T& c = m_Registry.emplace<T>(e, std::forward<Args>(args)...);
            return c;
        }

        template<typename T>
        T& GetComponent(Entity& e) 
        {
            return m_Registry.get<T>(e);
        }

        template<typename T>
        void RemoveComponent(Entity& e)
        {
            m_Registry.remove<T>(e);
        }

        template<typename T>
        bool HasComponent(Entity& e)
        {
            return m_Registry.try_get<T>(e) != nullptr;
        }

    private:
        entt::registry m_Registry;
        b2World* m_PhysicsWorld = nullptr;
        bool simulatePhysics = false;

    friend class SceneSerializer;
    };
}