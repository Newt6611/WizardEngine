#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Wizard {
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera();

        void SetProjection(float left, float right, float bottom, float top);
        void SetPosition(const glm::vec3& position) { m_Position = position; ReCalculate(); }
        void SetRotation(float r) { m_Rotation = r; ReCalculate(); }

        inline const glm::mat4& GetViewMatrix() const { return m_View;}
        inline const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
        inline const glm::mat4& GetViewProjectionMatrix() const { return m_Projection * m_View; }
        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline float GetRotation() { return m_Rotation; }

    private:
        void ReCalculate();

    private:
        glm::vec3 m_Position = glm::vec3(0, 0, 0);
        float m_Rotation = 0.f;
        glm::mat4 m_View = glm::mat4(1.0);
        glm::mat4 m_Projection = glm::mat4(1.0);

    };
}