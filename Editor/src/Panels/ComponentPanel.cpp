#include "ComponentPanel.h"

ComponentPanel::ComponentPanel()
{

}

ComponentPanel::~ComponentPanel()
{

}

void ComponentPanel::DrawComponent(Scene& scene, Entity& entity)
{
    if (entity == entt::null) return;
    
    ImGui::Begin("Components");
    // AddComponent Button
    const char* components[] = { "SpriteRenderer", "Rigidbody2D", "BoxCollider2D" };
    if (ImGui::Button("AddComponent")) {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent")) {
        DrawAddComponent(scene, entity);
        ImGui::EndPopup();
    }

    ImGui::Separator();

    // Tag
    if (scene.HasComponent<TagComponent>(entity)) {
        auto& tag = scene.GetComponent<TagComponent>(entity);
        char buffer[256];
        memset(buffer, 0, sizeof(tag.Name.c_str()));
        if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
            tag.Name = std::string(buffer);
        }
    }
    ImGui::Separator();

    // Transform
    if (scene.HasComponent<TransformComponent>(entity)) {
        auto& transform = scene.GetComponent<TransformComponent>(entity);
        ImGui::Text("Transform");

		ImGui::DragFloat3("Position", &transform.Position.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", &transform.Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Size", &transform.Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
    }
    ImGui::Separator();

    // SpriteRenderer
    if (scene.HasComponent<SpriteRendererComponent>(entity)) {
        auto& spriteRenderer = scene.GetComponent<SpriteRendererComponent>(entity);
        ImGui::Text("SpriteRenderer");

        ImGui::ColorEdit4("Color", &spriteRenderer.Color.x);
    }
    ImGui::Separator();

    // Rigidbody2D
    if (scene.HasComponent<Rigidbody2DComponent>(entity)) {
        auto& rigidbody = scene.GetComponent<Rigidbody2DComponent>(entity);
        ImGui::Text("Rigidbody2D");

        // Body Type
        int currentType = rigidbody.BodyType;
        const char* type[]{"Static", "Kinematic", "Dynamic"}; // Box2D Order
        if (ImGui::BeginCombo("Type", type[currentType])) {
            for (int i = 0; i < 3; ++i) {
                bool isSelected = (i == currentType);
                if (ImGui::Selectable(type[i], isSelected)) {
                    rigidbody.BodyType = (b2BodyType)i;
                }
            }
            ImGui::EndCombo();
        }

        // Fixed Rotation
        ImGui::Checkbox("FixedRotation", &rigidbody.FixedRotation);
    }
    ImGui::Separator();

    // BoxCollder2D
    if (scene.HasComponent<BoxCollider2DComponent>(entity)) {
        auto& collder = scene.GetComponent<BoxCollider2DComponent>(entity);
        ImGui::Text("BoxCollder2D");

        ImGui::DragFloat2("Size", &collder.Size.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("Density", &collder.Density, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("Friction", &collder.Friction, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("Restitution", &collder.Restitution, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::DragFloat("RestitutionThreshold", &collder.RestitutionThreshold, 0.1f, 0.0f, 0.0f, "%.2f");
    }
    ImGui::Separator();

    ImGui::End();
}

void ComponentPanel::DrawAddComponent(Scene& scene, Entity& entity)
{
    if (ImGui::Selectable("SpriteRenderer")) {
        scene.AddComponent<SpriteRendererComponent>(entity);
    } 
    
    if (ImGui::Selectable("Rigidbody2D")) {
        scene.AddComponent<Rigidbody2DComponent>(entity);    
    }

    if (ImGui::Selectable("BoxCollider2D")) {
        scene.AddComponent<BoxCollider2DComponent>(entity);
    }
}