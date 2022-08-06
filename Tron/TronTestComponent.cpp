#include "TronPCH.h"
#include "TronTestComponent.h"

#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "imgui.h"
#include "InputEvents.h"
#include "InputService.h"
#include "RenderService.h"
#include "Rujin.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"


TronTestComponent::TronTestComponent(PlayerIndex playerIndex)
	: m_PlayerIndex(playerIndex)
{
	InputService& input = ServiceLocator::GetService<InputService>();
	input.AddInputAction(m_PlayerIndex, static_cast<uint32_t>(InputAction::LeftClick), { ButtonState::Down, VK_LBUTTON });
	input.AddInputAction(m_PlayerIndex, static_cast<uint32_t>(InputAction::RightClick), { ButtonState::Down, VK_RBUTTON });
}

void TronTestComponent::Start()
{

}

void TronTestComponent::Update()
{
	const InputService& input = ServiceLocator::GetService<InputService>();


	if(input.IsInputActionTriggered(m_PlayerIndex, static_cast<uint32_t>(InputAction::LeftClick)))
	{
		//set starting point
		m_RayStart = input.GetMousePosition();
	}

	if (input.IsInputActionTriggered(m_PlayerIndex, static_cast<uint32_t>(InputAction::RightClick)))
	{
		//set end point
		m_RayEnd = input.GetMousePosition();
	}

	m_RaycastHit = GameObject()->GetScene()->GetCollisionQuadTree()->Raycast(m_RayStart, m_RayEnd);
}

void TronTestComponent::Draw() const
{
	RenderService& renderer = ServiceLocator::GetService<RenderService>();

	if (m_RaycastHit) 
		renderer.SetColor({ 0.f, 1.f, 0.f, 1.f });
	else
		renderer.SetColor({ 1.f, 0.f, 0.f, 1.f });

	renderer.DrawLine(m_RayStart, m_RayEnd, 3.f);
}

void TronTestComponent::OnGui(SDL_Window* /*sdlWindow*/)
{
	
}
