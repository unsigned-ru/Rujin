#pragma once
#include <thread>

#include "Singleton.h"

enum class DataGenStatus
{
	Uninitialized,
	InProgress,
	Finished
};

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D
{
public:
	Transform transform;
	int ID;
};

class GameObject3DAlt
{
public:
	Transform* transform;
	int ID;
};

namespace rujin
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;
		const SDL_Color& GetBackgroundColor() const;
		void SetBackgroundColor(const SDL_Color& color);

	private:

		void ImGuiEx1();
		void ImGuiEx2();

		int m_IntSamples{10};
		DataGenStatus m_IntGenStatus{ DataGenStatus::Uninitialized };
		std::vector<float> m_IntDataX{};
		std::vector<float> m_IntDataY{};

		int m_GoSamples{ 10 };
		DataGenStatus m_GoGenStatus{ DataGenStatus::Uninitialized };
		std::vector<float> m_GoDataX{};
		std::vector<float> m_GoDataY{};

		int m_GoAltSamples{ 10 };
		DataGenStatus m_GoAltGenStatus{ DataGenStatus::Uninitialized };
		std::vector<float> m_GoAltDataX{};
		std::vector<float> m_GoAltDataY{};
	};
}

