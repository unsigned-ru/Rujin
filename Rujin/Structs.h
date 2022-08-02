﻿#ifndef STRUCTS_H
#define STRUCTS_H
#include <array>

namespace rujin
{
	enum class VSyncMode : int { ON = 1, OFF = 0, ADAPTIVE = -1 };

	struct WindowContext
	{
		std::string windowTitle{};
		glm::ivec2 windowSize{};
		SDL_Window* pWindow{};
		glm::vec4 glClearColor{};
		VSyncMode vSyncMode = VSyncMode::ON;
	};

	struct Margins
	{
		float left;
		float right;
		float top;
		float bottom;
	};

	struct Rectf
	{
		Rectf() = default;
		explicit Rectf(float left, float bottom, float width, float height)
			: left{ left }
			, bottom{ bottom }
			, width{ width }
			, height{ height }
		{
		}

		struct Vertices
		{
			Vertices(const std::array<glm::vec2, 4>&& vertices)
				: vertices(std::move(vertices))
			{}

			glm::vec2& topLeft = vertices[0];
			glm::vec2& bottomLeft = vertices[1];
			glm::vec2& bottomRight = vertices[2];
			glm::vec2& topRight = vertices[3];

			std::array<glm::vec2, 4> vertices;
		};

		Vertices GetVertices() const
		{
			return Vertices(
				{
					glm::vec2{left, bottom + height},
					glm::vec2{left, bottom},
					glm::vec2{left + width, bottom},
					glm::vec2{left + width, bottom + height}
				}
			);
		}

		float left{};
		float bottom{};
		float width{};
		float height{};
	};

	using Position = glm::vec2;
	using Rotation = float;
	using Scale = glm::vec2;

	struct Transform
	{
		Position pos{};
		Rotation rot{};
		Scale scale{ 1.f, 1.f };


		Transform operator+(const Transform& other) const
		{
			Transform rv = *this;
			rv.pos += other.pos;
			rv.rot += other.rot;
			rv.scale *= other.scale;
			return rv;
		}
	};


	class Collider;
	struct CollisionResult
	{
		bool isColliding = false;
		const Collider* other = nullptr;
	};
}

#endif