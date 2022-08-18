#ifndef STRUCTS_H
#define STRUCTS_H
#include "EnumBitmask.h"

#include <array>
namespace rujin
{
	enum class TransformChanged : uint32_t
	{
		NONE = 0x00,
		POSITION = 0x01,
		ROTATION = 0x02,
		SCALE = 0x04,
	};
	ENABLE_BITMASK_OPERATORS(TransformChanged);


	enum class VSyncMode : int { ON = 1, OFF = 0, ADAPTIVE = -1 };
	enum class Direction {LEFT, UP, RIGHT, DOWN};

	inline Direction GetInverted(Direction dir)
	{
		switch (dir)
		{
			case Direction::LEFT:
				return Direction::RIGHT;
			case Direction::UP:
				return Direction::DOWN;
			case Direction::RIGHT:
				return Direction::LEFT;
			case Direction::DOWN:
				return Direction::UP;
			default:
				return dir;
		}
	}

	inline Direction VectorToDirection(const glm::vec2& dirVec)
	{
		Direction rv;

		if (abs(dirVec.x) > abs(dirVec.y))
		{
			if (dirVec.x > 0.f)
				rv = Direction::RIGHT;
			else
				rv = Direction::LEFT;
		}
		else
		{
			if (dirVec.y > 0.f)
				rv = Direction::UP;
			else
				rv = Direction::DOWN;
		}

		return rv;
	}

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

	template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>, T>>
	struct Rect
	{
		Rect() = default;
		explicit Rect(T left, T bottom, T width, T height)
			: left{ left }
			, bottom{ bottom }
			, width{ width }
			, height{ height }
		{
		}

		explicit Rect(const glm::tvec2<T>& pos, const glm::vec2& size, const glm::vec2& pivot = { 0.5f, 0.5f })
			: Rect(pos.x - size.x * pivot.x, pos.y - size.y * pivot.y, size.x, size.y)
		{}

		struct Vertices
		{
			Vertices() = default;
			Vertices(std::array<glm::tvec2<T>, 4>&& vertices)
				: vertices(std::move(vertices))
			{}

			glm::tvec2<T>& topLeft = vertices[0];
			glm::tvec2<T>& bottomLeft = vertices[1];
			glm::tvec2<T>& bottomRight = vertices[2];
			glm::tvec2<T>& topRight = vertices[3];

			std::array<glm::tvec2<T>, 4> vertices{};
		};

		Vertices GetVertices() const
		{
			return Vertices(
				{
					glm::tvec2<T>{left, bottom + height},
					glm::tvec2<T>{left, bottom},
					glm::tvec2<T>{left + width, bottom},
					glm::tvec2<T>{left + width, bottom + height}
				}
			);
		}

		bool operator==(const Rect& other) const
		{
			return
				abs(left - other.left) <= FLT_EPSILON &&
				abs(bottom - other.bottom) <= FLT_EPSILON &&
				abs(width - other.width) <= FLT_EPSILON &&
				abs(height - other.height) <= FLT_EPSILON;
		}

		T left{};
		T bottom{};
		T width{};
		T height{};
	};

	using Rectf = Rect<float>;
	using Recti = Rect<int>;

	inline bool operator>(const glm::vec2& p1, const glm::vec2& p2) { return p1.x > p2.x && p1.y > p2.y; }
	inline bool operator<(const glm::vec2& p1, const glm::vec2& p2) { return p1.x < p2.x && p1.y < p2.y; }

	using Position = glm::vec2;
	using Rotation = float;
	using Scale = glm::vec2;

	class Collider;
	struct CollisionResult
	{
		bool isColliding = false;
		const Collider* other = nullptr;

		//relative to me, where is the collision happening.
		Direction collisionDirection{Direction::RIGHT};
	};
}

#endif