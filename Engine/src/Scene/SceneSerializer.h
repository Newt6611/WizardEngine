#pragma once

namespace Wizard {
    class Scene;
    class SceneSerializer
    {
    public:
        static void Serialize(const std::string& filepath, Scene& scene);
        static void Deserialize(const std::string& filepath, Scene& scene);
    };
}