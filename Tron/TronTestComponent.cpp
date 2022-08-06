#include "TronPCH.h"
#include "TronTestComponent.h"

#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "imgui.h"
#include "InputService.h"
#include "RenderService.h"
#include "Rujin.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

void TronTestComponent::Start()
{

}

void TronTestComponent::Update()
{
	const InputService& input = ServiceLocator::GetService<InputService>();

	m_RaycastHit = GameObject()->GetScene()->GetCollisionQuadTree()->Raycast({ 0.f, 0.f }, { input.GetMousePosition().x, input.GetMousePosition().y });
}

void TronTestComponent::Draw() const
{
	const InputService& input = ServiceLocator::GetService<InputService>();
	RenderService& renderer = ServiceLocator::GetService<RenderService>();

	if (m_RaycastHit) 
		renderer.SetColor({ 0.f, 1.f, 0.f, 1.f });
	else
		renderer.SetColor({ 1.f, 0.f, 0.f, 1.f });

	renderer.DrawLine({ 0.f, 0.f }, { input.GetMousePosition().x, input.GetMousePosition().y }, 3.f);
}

void TronTestComponent::OnGui(SDL_Window* /*sdlWindow*/)
{
	
}
