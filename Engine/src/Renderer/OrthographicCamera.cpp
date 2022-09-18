#include "wzpch.h"
#include "OrthographicCamera.h"

namespace Wizard {
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_Projection(glm::orthoLH(left, right, bottom, top, -1.f, 1.f))
    {

    }

    void OrthographicCamera::ReCalculate()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0), m_Position) * 
            glm::rotate(glm::mat4(1.0), m_Rotation, glm::vec3(0, 0, 1)); 

        m_View = glm::inverse(transform);
    }
}