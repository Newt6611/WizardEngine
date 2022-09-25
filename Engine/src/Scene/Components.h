#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include "Renderer/Texture.h"

namespace Wizard {
    
    struct TransformComponent
    {
        glm::vec3 Position = { 0.f, 0.f, 0.f };
        glm::vec3 Rotation = { 0.f, 0.f, 0.f };
        glm::vec3 Scale = { 1.f , 1.f, 1.f };

        TransformComponent() = default;
        TransformComponent(const glm::vec3& position) 
            : Position(position) { }
        TransformComponent(const glm::vec2& position) 
            : Position(glm::vec3(position.x, position.y, 0)) { }

        glm::mat4 GetTransform() const {
            glm::mat4 r = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position) * 
                r * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent
    {
        std::shared_ptr<Texture2D> Texture = nullptr;
        glm::vec4 Color = {1.f, 1.f, 1.f, 1.f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const glm::vec4& color) 
            : Color(color) { }
        SpriteRendererComponent(std::shared_ptr<Texture2D> texture)
            : Texture(texture) { }
        SpriteRendererComponent(std::shared_ptr<Texture2D> texture, const glm::vec4& color)
            : Texture(texture), Color(color) { }
    };
}
