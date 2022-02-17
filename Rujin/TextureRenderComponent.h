#pragma once
#include "Component.h"
namespace rujin
{
	class Texture2D;

	class TextureRenderComponent final : public Component
	{
	public:
		explicit TextureRenderComponent(const std::weak_ptr<GameObject> gameObject);

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() const override;
		void Destroy() override;

		void SetTexture(std::weak_ptr<Texture2D> texture);
		

	private:
		std::weak_ptr<Texture2D> m_Texture{};
	};
}

