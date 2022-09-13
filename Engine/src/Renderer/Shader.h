#pragma once
#include "wzpch.h"

namespace Wizard {
    class Shader
    {
    public:
        static std::unique_ptr<Shader> Create();
        Shader() {}
        virtual ~Shader() {}

        static std::vector<char> ReadByteCode(const std::string& fileName);
    };
}