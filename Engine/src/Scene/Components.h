#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Renderer/Texture.h"
#include "ScriptingEntity.h"

#include <functional>
#include <memory>
#include <box2d/box2d.h>

namespace Wizard {
    
    struct TagComponent
    {
        std::string Name;
        TagComponent() = default;
        TagComponent(const std::string& name) : Name(name) { };
    };

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

    struct SpriteAnimationComponent
    {
        std::vector<std::shared_ptr<Texture2D>> Sprites;
        int CurrentSpriteIndex = 0;
        float Speed = 0.5f;
        float TimeBtw = 0.0f;

        SpriteAnimationComponent() = default;
    };

    struct Rigidbody2DComponent
    {
        b2Body* Body;
        b2BodyType BodyType = b2BodyType::b2_staticBody;
        bool FixedRotation = false;

        Rigidbody2DComponent() = default;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
        float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

        BoxCollider2DComponent() = default;
    };

    struct ScriptingComponent
    {
        ScriptingEntity* Instance = nullptr;
        bool IsCreated = false;
        ScriptingComponent(ScriptingEntity* entity) 
            : Instance(entity) { }
    };
}
