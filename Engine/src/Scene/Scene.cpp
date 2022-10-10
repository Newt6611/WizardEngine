#include "wzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Core/Log.h"
#include "Renderer/RenderTool.h"
#include "Renderer/Renderer.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Wizard {

    Scene::Scene()
    {
        
    }

    Scene::~Scene()
    {
        if (m_PhysicsWorld)
            delete m_PhysicsWorld;

        auto view = m_Registry.view<ScriptingComponent>();
        for (auto entity : view) {
            auto& scripting = m_Registry.get<ScriptingComponent>(entity);
            if (scripting.Instance)
                delete scripting.Instance;
        }
    }

    void Scene::OnSimulatePhysicsStart()
    {
        if (m_PhysicsWorld) 
            return;
        simulatePhysics = true;
        m_PhysicsWorld = new b2World( { 0, -9.8f } );

        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view) {
            auto& transform = m_Registry.get<TransformComponent>(e);
            auto& rigidbody = m_Registry.get<Rigidbody2DComponent>(e);

            b2BodyDef bodyDef;
			bodyDef.type = rigidbody.BodyType;
			bodyDef.position.Set(transform.Position.x, transform.Position.y);
			bodyDef.angle = transform.Rotation.z;

            b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rigidbody.FixedRotation);
            rigidbody.Body = body;

            if (HasComponent<BoxCollider2DComponent>(e)) {
				auto& collider = m_Registry.get<BoxCollider2DComponent>(e);

				b2PolygonShape boxShape;
				boxShape.SetAsBox(collider.Size.x * transform.Scale.x, collider.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = collider.Density;
				fixtureDef.friction = collider.Friction;
				fixtureDef.restitution = collider.Restitution;
				fixtureDef.restitutionThreshold = collider.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
        }
    }

    void Scene::OnUpdate(float ts)
    {
        // Physics
        if (simulatePhysics) {
            {
                const int32_t velocityIterations = 6;
                const int32_t positionIterations = 2;
                m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
                
                auto view = m_Registry.view<Rigidbody2DComponent>();
                for (auto e : view) {
                    auto& transform = m_Registry.get<TransformComponent>(e);
                    auto& rb2d = m_Registry.get<Rigidbody2DComponent>(e);

                    const auto& position = rb2d.Body->GetPosition();
                    transform.Position.x = position.x;
                    transform.Position.y = position.y;
                    transform.Rotation.z = rb2d.Body->GetAngle();
                }
            }
        }

        // Debug Physics Collder
        // {
        //     auto view = m_Registry.view<BoxCollider2DComponent>();
        //     for (auto e : view) {
        //         auto& transform = m_Registry.get<TransformComponent>(e);
        //         auto& collider = m_Registry.get<BoxCollider2DComponent>(e);
        //         Renderer2D::DrawQuad(transform.Position, 
        //             glm::vec2{collider.Size.x * transform.Scale.x * 2, collider.Size.y * transform.Scale.y * 2}, 
        //                 glm::vec4{0.f, 1.f, 0.f, 0.2f}, glm::degrees(transform.Rotation.z));
        //     }
        // }

        // Scripting
        {
            auto view = m_Registry.view<ScriptingComponent>();
            for (auto entity : view) {
                auto& scripting = m_Registry.get<ScriptingComponent>(entity);
                if (!scripting.IsCreated) {
                    scripting.Instance->OnCreate();
                    scripting.IsCreated = true;
                }
                
                scripting.Instance->OnUpdate(ts);
            }
        }

        // Render
        // -- Draw Sprites
        {
            auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
            for (auto entity: group) {
                auto& transform = group.get<TransformComponent>(entity);
                auto& spriteRenderer = group.get<SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.GetTransform(), spriteRenderer.Color, spriteRenderer.Texture, (int)entity);
            }
        }

        // -- SpriteAnimation
        {
            auto view = m_Registry.view<SpriteAnimationComponent>();
            for (auto entity : view) {
                auto& transform = m_Registry.get<TransformComponent>(entity);
                auto& spriteAnimation = m_Registry.get<SpriteAnimationComponent>(entity);

                spriteAnimation.TimeBtw += ts;
                if (spriteAnimation.TimeBtw >= spriteAnimation.Speed) {
                    spriteAnimation.TimeBtw = 0;
                    spriteAnimation.CurrentSpriteIndex = (spriteAnimation.CurrentSpriteIndex + 1) % spriteAnimation.Sprites.size(); 
                }
                Renderer2D::DrawQuad(transform.GetTransform(), glm::vec4{1, 1, 1, 1}, spriteAnimation.Sprites[spriteAnimation.CurrentSpriteIndex], (int)entity);
            }
        }
    }
}