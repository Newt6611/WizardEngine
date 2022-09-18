#include "Wizard.h"

class Example2D : public Wizard::Layer
{
public:
    Example2D();

    ~Example2D();

    void OnEnter() override;
    void OnUpdate() override;
    void OnEvent(Wizard::Event& e);
    void OnLeave() override;

private:
    void InitFlatRenderPipelineState();
    void InitVertexBuffer();
    void InitIndexBuffer();
    void InitConstBuffer();

private:
    Wizard::OrthographicCamera m_OrthographicCamera;
    
    Wizard::RenderPipelineState m_FlatColorPipeline;
    std::shared_ptr<Wizard::Shader> m_FlatShader;
    std::shared_ptr<Wizard::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Wizard::IndexBuffer> m_IndexBuffer;
    IShaderResourceBinding* m_SRB;

    struct PerFrameConst
    {
        glm::mat4 View;
        glm::mat4 Projection;
    };

    struct PerObjConst
    {
        glm::mat4 World;
    };

    struct PSObjConst
    {
        glm::vec4 Color;
    };

    std::shared_ptr<Wizard::ConstantBuffer<PerFrameConst>> m_PerFrameConst;
    std::shared_ptr<Wizard::ConstantBuffer<PerObjConst>> m_PerObjConst;
    std::shared_ptr<Wizard::ConstantBuffer<PSObjConst>> m_PSObjConst;
};