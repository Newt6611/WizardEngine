#include "wzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Core/Log.h"

namespace Wizard {

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {

    }

    void Scene::OnUpdate(float ts)
    {

        // Draw Sprites
        {
            auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
            for(auto entity: group) {
                auto& transform = group.get<TransformComponent>(entity);
                auto& spriteRenderer = group.get<SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.GetTransform(), spriteRenderer.Color, spriteRenderer.Texture);
            }
        }
    }
}