#include "RujinPCH.h"
#include "TextureRenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "TransformComponent.h"


rujin::TextureRenderComponent::TextureRenderComponent(const std::weak_ptr<GameObject> gameObject)
	: Component(gameObject)
{
}

void rujin::TextureRenderComponent::Start()
{
	Component::Start();
}

void rujin::TextureRenderComponent::Update()
{
	Component::Update();
}

void rujin::TextureRenderComponent::Destroy()
{
	Component::Destroy();
}

void rujin::TextureRenderComponent::FixedUpdate()
{
	Component::FixedUpdate();
}

void rujin::TextureRenderComponent::Render() const
{
	Component::Render();

	if (!m_Texture.expired()) //todo: m_Texture seems to be expired while it shouldn't
	{
		const auto transform = GetGameObject().lock()->GetComponent<TransformComponent>().lock();
		const auto texture{ m_Texture.lock() };

		Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
	}
}

void rujin::TextureRenderComponent::SetTexture(const std::weak_ptr<Texture2D> texture)
{
	m_Texture = texture;
}
