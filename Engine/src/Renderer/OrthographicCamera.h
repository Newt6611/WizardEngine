#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Wizard {
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera();


        void SetPosition(const glm::vec3& position) { m_Position = position; ReCalculate(); }
        void SetRotation(float r) { m_Rotation = r; }

        inline const glm::mat4& GetViewMatrix() { return m_View; ReCalculate(); }
        inline const glm::mat4& GetProjectionMatrix() { return m_Projection; }
        inline const glm::mat4& GetViewProjectionMatrix() { return m_Projection * m_View; }
        inline const glm::vec3& GetPosition() { return m_Position; }
        inline float GetRotation() { return m_Rotation; }

    private:
        void ReCalculate();

    private:
        glm::vec3 m_Position;
        float m_Rotation = 0.f;
        glm::mat4 m_View = glm::mat4(1.0);
        glm::mat4 m_Projection = glm::mat4(1.0);

    };
}