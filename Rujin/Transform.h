#pragma once
namespace rujin
{
	class Transform final
	{
	public:
		explicit Transform(const glm::vec3& pos = glm::vec3{});

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position{};
	};
}
