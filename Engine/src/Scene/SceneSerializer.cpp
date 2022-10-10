#include "wzpch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include "Scene.h"
#include "Core/Log.h"

#include <fstream>
#include <entt/entt.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Wizard {

    static const char* b2BodyTypeToString(b2BodyType type)
    {
        switch (type)
        {
        case b2BodyType::b2_staticBody:
            return "StaticBody";
        case b2BodyType::b2_kinematicBody:
            return "KinematicBody";
        case b2BodyType::b2_dynamicBody:
            return "DynamicBody";
        default:
            return "Unknow Type";
        }
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& val)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << val.x << val.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& val)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << val.x << val.y << val.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& val)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << val.x << val.y << val.z << val.w << YAML::EndSeq;
        return out;
    }

    void SceneSerializer::Serialize(const std::string& filepath, Scene& scene)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::BeginSeq;

        scene.m_Registry.each([&](auto entity) {
            out << YAML::BeginMap; // Entity
		    out << YAML::Key << "Entity" << YAML::Value << (int)entity;    
            
            // Tag
            if (scene.HasComponent<TagComponent>(entity)) {
                TagComponent& t = scene.GetComponent<TagComponent>(entity);
                out << YAML::Key << "Tag";
                out << YAML::BeginMap;
                out << YAML::Key << "Name" << YAML::Value << t.Name.c_str();
                out << YAML::EndMap;
            }

            // Transform
            if (scene.HasComponent<TransformComponent>(entity)) {
                TransformComponent& t = scene.GetComponent<TransformComponent>(entity);
                out << YAML::Key << "Transform";
                out << YAML::BeginMap;
                out << YAML::Key << "Position" << YAML::Value << t.Position; 
                out << YAML::Key << "Rotation" << YAML::Value << t.Rotation; 
                out << YAML::Key << "Scale" << YAML::Value << t.Scale; 
                out << YAML::EndMap;
            }

            // SpriteRenderer
            if (scene.HasComponent<SpriteRendererComponent>(entity)) {
                SpriteRendererComponent& s = scene.GetComponent<SpriteRendererComponent>(entity);
                out << YAML::Key << "SpriteRenderer"; 
                out << YAML::BeginMap;
                out << YAML::Key << "Color" << YAML::Value << s.Color; 
                out << YAML::EndMap;
            }

            // Rigidbody2D
            if (scene.HasComponent<Rigidbody2DComponent>(entity)) {
                Rigidbody2DComponent& r = scene.GetComponent<Rigidbody2DComponent>(entity);
                out << YAML::Key << "Rigidbody2D"; 
                out << YAML::BeginMap;
                out << YAML::Key << "BodyType" << YAML::Value << b2BodyTypeToString(r.BodyType);
                out << YAML::Key << "FixedRotation" << YAML::Value << (r.FixedRotation == true ? 1 : 0); 
                out << YAML::EndMap;
            }

            // BoxCollider2D
            if (scene.HasComponent<BoxCollider2DComponent>(entity)) {
                BoxCollider2DComponent& b = scene.GetComponent<BoxCollider2DComponent>(entity);
                out << YAML::Key << "BoxCollider2D"; 
                out << YAML::BeginMap;
                out << YAML::Key << "Offset" << YAML::Value << b.Offset;
                out << YAML::Key << "Size" << YAML::Value << b.Size; 
                out << YAML::Key << "Density" << YAML::Value << b.Density; 
                out << YAML::Key << "Friction" << YAML::Value << b.Friction; 
                out << YAML::Key << "Restitution" << YAML::Value << b.Restitution; 
                out << YAML::Key << "RestitutionThreshold" << YAML::Value << b.RestitutionThreshold; 
                out << YAML::EndMap;
            }

            out << YAML::EndMap;
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
        fout.close();
    }

    void SceneSerializer::Deserialize(const std::string& filepath, Scene& scene)
    {
        YAML::Node data;
		try {
			data = YAML::LoadFile(filepath);
		} catch (YAML::ParserException e) {
			WZ_ENGINE_ERROR("Failed When Opening .wizard file. {} {}", filepath, e.what());
			return;
		}

        if (!data["Scene"]) {
            WZ_ENGINE_ERROR("Failed When Opening Scene {}", filepath);
            return;
        }

        auto entities = data["Entities"];
        if (entities) {
            for (auto entity : entities) {
                Entity newEntity = scene.CreateEntity();

                // Tag 
                {
                    std::string tagName = "";
                    YAML::Node tag = entity["Tag"];
                    if (tag) {
                        tagName = tag["Name"].as<std::string>();
                        scene.AddComponent<TagComponent>(newEntity, tagName);
                    }
                }

                // Transform
                {
                    YAML::Node transform = entity["Transform"];
                    if (transform) {
                        auto& t = scene.AddComponent<TransformComponent>(newEntity);
                        t.Position = transform["Position"].as<glm::vec3>();
                        t.Rotation = transform["Rotation"].as<glm::vec3>();
                        t.Scale = transform["Scale"].as<glm::vec3>();
                    }
                }
                
                // SpriteRenderer // TODO: Load Texture
                {
                    YAML::Node spriteRenderer = entity["SpriteRenderer"];
                    if (spriteRenderer) {
                        auto& s = scene.AddComponent<SpriteRendererComponent>(newEntity);
                        s.Color = spriteRenderer["Color"].as<glm::vec4>();
                    }
                }

                // Rigidbody2D
                {
                    YAML::Node rigidbody2d = entity["Rigidbody2D"];
                    if (rigidbody2d) {
                        auto& r = scene.AddComponent<Rigidbody2DComponent>(newEntity);
                        std::string bodytype = rigidbody2d["BodyType"].as<std::string>();
                        if (bodytype == "StaticBody")
                            r.BodyType = b2BodyType::b2_staticBody;
                        else if (bodytype == "KinematicBody")
                            r.BodyType = b2BodyType::b2_kinematicBody;
                        else if (bodytype == "DynamicBody")
                            r.BodyType = b2BodyType::b2_dynamicBody;
                        r.FixedRotation = rigidbody2d["FixedRotation"].as<double>();
                    }
                }

                // BoxCollider2D
                {
                    YAML::Node boxcollider2d = entity["BoxCollider2D"];
                    if (boxcollider2d) {
                        auto& b = scene.AddComponent<BoxCollider2DComponent>(newEntity);
                        b.Offset = boxcollider2d["Offset"].as<glm::vec2>();
                        b.Size = boxcollider2d["Size"].as<glm::vec2>();
                        b.Density = boxcollider2d["Density"].as<double>();
                        b.Friction = boxcollider2d["Friction"].as<double>();
                        b.Restitution = boxcollider2d["Restitution"].as<double>();
                        b.RestitutionThreshold = boxcollider2d["RestitutionThreshold"].as<double>();
                    }
                }
            }
        }


    }
}