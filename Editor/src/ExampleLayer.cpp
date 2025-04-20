#include "ExampleLayer.h"

#include <glm/glm.hpp>
#include <Math/MathFormatters.h>
#include <imgui.h>

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate() 
{
	if (Idra::Input::IsKeyPressed(IDRA_KEY_TAB))
		IDRA_TRACE("Tab key is pressed! (Poll!)"); // #DEBUG
}

/*
* If built as DLL, ImGui context is not shared across DLL boundaries.
* Static Library builds are fine.
*/
void ExampleLayer::OnImGuiRender()
{
	// @TODO ASSERTS are not defined properly currently
	IDRA_ASSERT(ImGui::GetCurrentContext(), "No ImGui context available!");
	ImGui::Begin("Example Layer");
	ImGui::Text("Hello from Example Layer!");
	ImGui::End();
}

void ExampleLayer::OnAttach()
{
	IDRA_INFO("Example Layer Attached"); // #DEBUG

	glm::vec3 test = { 1.0f, 2.0f, 3.0f };
	IDRA_WARN("Test: {0}", test); // #DEBUG

	glm::vec2 testVec2 = { 1.0f, 2.0f };
	IDRA_WARN("TestVec2: {0}", testVec2); // #DEBUG

	glm::vec4 testVec4 = { 1.0f, 2.0f, 3.0f, 4.0f };
	IDRA_WARN("TestVec4: {0}", testVec4); // #DEBUG

	glm::mat3 testMat3 = { 1.0f, 2.0f, 3.0f,
						  4.0f, 5.0f, 6.0f,
						  7.0f, 8.0f, 9.0f };
	IDRA_WARN("TestMat3: {0}", testMat3); // #DEBUG

	glm::mat4 testMat4 = { 1.0f, 2.0f, 3.0f, 4.0f,
						  5.0f, 6.0f, 7.0f, 8.0f,
						  9.0f, 10.0f, 11.0f, 12.0f,
						  13.0f, 14.0f, 15.0f, 16.0f };
	IDRA_WARN("TestMat: {0}", testMat4); // #DEBUG

}

void ExampleLayer::OnDetach()
{
	IDRA_INFO("Example Layer Detached"); // #DEBUG
}

void ExampleLayer::OnEvent(Idra::Event& e) 
{
	if (e.GetEventType() != Idra::EventType::MouseMoved)
		IDRA_TRACE("Example Layer: {0}", e); // #DEBUG
}