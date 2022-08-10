#ifndef STRUCTS_H
#define STRUCTS_H
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
	inline TransformChanged operator|(TransformChanged& lhs, TransformChanged rhs)
	{
		return static_cast<TransformChanged>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
	inline TransformChanged& operator|=(TransformChanged& lhs, TransformChanged rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	enum class VSyncMode : int { ON = 1, OFF = 0, ADAPTIVE = -1 };
	enum class Direction {LEFT, UP, RIGHT, DOWN};

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
	};
}

#endif