#include "TronPCH.h"
#include "TronTestComponent.h"

#include "GameObject.h"
#include "imgui.h"
#include "Rujin.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

void TronTestComponent::Start()
{

}

void TronTestComponent::Update()
{
	TransformComponent* pTransform = GameObject()->GetTransform();
	if(m_ShouldRotate)
	{
		pTransform->SetLocalRotation(pTransform->GetLocalRotation() + m_RotationSpeed * Rujin::Get()->GetDeltaTime());
	}

	if (m_ShouldMove)
	{
		const float period = 2 * std::numbers::pi_v<float> / m_Frequency;

		//calculate sin-wave x-Value
		float xPos = m_Distance * sin(Rujin::Get()->GetTotalTime() * period);

		pTransform->SetLocalPosition({ xPos, pTransform->GetLocalPosition().y });
	}

	//update parent pos.
	const Position& parentPos = GameObject()->GetParent()->GetTransform()->GetPosition();
	m_ParentPos[0] = parentPos.x;
	m_ParentPos[1] = parentPos.y;
}

void TronTestComponent::OnGui(SDL_Window* /*sdlWindow*/)
{
	ImGui::Begin("TronTest", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::LabelText("Rotation", "");
		ImGui::Checkbox("Should Rotate", &m_ShouldRotate);
		ImGui::InputFloat("Speed", &m_RotationSpeed, 0.5f, 1.f);

		ImGui::LabelText("Movement", "");
		ImGui::Checkbox("Should Move", &m_ShouldMove);
		ImGui::InputFloat("Frequency", &m_Frequency, 0.5f, 1.f);
		ImGui::InputFloat("Distance", &m_Distance, 5.f, 50.f);

		ImGui::LabelText("Parent", "");
		if (ImGui::InputFloat("XPos", &m_ParentPos[0], 5.f, 50.f) || ImGui::InputFloat("YPos", &m_ParentPos[1], 5.f, 50.f))
		{
			GameObject()->GetParent()->GetTransform()->SetPosition({m_ParentPos[0], m_ParentPos[1]});
		}

		if (ImGui::Button("Reset"))
		{
			m_ShouldRotate = false;
			GameObject()->GetTransform()->SetLocalRotation(0);

			m_ShouldMove = false;
		}
	}
	ImGui::End();
}
