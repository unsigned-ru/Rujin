
#include <functional>
#include <stack>

template<typename ElementType>
class DeferredRemovalVector
{
public:
	explicit DeferredRemovalVector() = default;
	explicit DeferredRemovalVector(const std::vector<ElementType>& vec)
		: m_Elements(vec)
	{}

	std::vector<ElementType>& GetVector() { return m_Elements; }
	const std::vector<ElementType>& GetVector() const { return m_Elements; }

	std::vector<typename std::vector<ElementType>::iterator>&  GetElementsToRemove() { return m_ElementsToRemove; }
	const std::vector<typename std::vector<ElementType>::iterator>&  GetElementsToRemove() const { return m_ElementsToRemove; }

	bool IsIterating() { return m_Iterating.empty();  }
	
	void SetIterating(const bool isIterating)
	{
		if (isIterating)
		{
			m_Iterating.push(true);
		}
		else if (!m_Iterating.empty())
		{
			m_Iterating.pop();

			if (m_Iterating.empty())
				RemovePending();
		}
	}

	void RemovePending()
	{
		for (auto it : m_ElementsToRemove)
			m_Elements.erase(it);

		m_ElementsToRemove.clear();
	}

	void RemoveIf(const std::function<bool(const ElementType&)>& fn)
	{
		auto it = std::find_if(m_Elements.begin(), m_Elements.end(), fn);

		if (IsIterating())
			//pend removal.
			m_ElementsToRemove.emplace_back(it);
		else
			//remove instantly.
			m_Elements.erase(it);
	}


#pragma region Pointer functions.

	void Remove(ElementType pElementToRemove) requires std::is_pointer_v<ElementType>
	{
		if (!pElementToRemove) return;

		if (IsIterating())
			//pend removal.
			m_ElementsToRemove.emplace_back(std::find(m_Elements.begin(), m_Elements.end(), pElementToRemove));
		else
			//remove instantly.
			m_Elements.erase(std::find(m_Elements.begin(), m_Elements.end(), pElementToRemove));
	}

	//const for-each
	template<typename UnaryPredicate>
	requires std::is_pointer_v<ElementType> && std::is_invocable_v<UnaryPredicate, ElementType>
	void Iterate(UnaryPredicate&& fn) const
	{
		for (const ElementType element : m_Elements)
			fn(element);
	}

	//for-each
	template<typename UnaryPredicate>
	requires std::is_pointer_v<ElementType> && std::is_invocable_v<UnaryPredicate, ElementType>
	void Iterate(UnaryPredicate&& fn)
	{
		SetIterating(true);
		for (ElementType element : m_Elements)
			fn(element);
		SetIterating(false);
	}
#pragma endregion

#pragma region reference functions.
	void Remove(const ElementType& pElementToRemove) requires !std::is_pointer_v<ElementType>
	{
		if (IsIterating())
			//pend removal.
			m_ElementsToRemove.emplace_back(std::find(m_Elements.begin(), m_Elements.end(), pElementToRemove));
		else
			//remove instantly.
			m_Elements.erase(std::find(m_Elements.begin(), m_Elements.end(), pElementToRemove));
	}

	//const foreach
	template<typename UnaryPredicate>
	requires !std::is_pointer_v<ElementType> && std::is_invocable_v<UnaryPredicate, ElementType>
	void Iterate(UnaryPredicate&& fn) const
	{
		for (const ElementType& element : m_Elements)
			fn(element);
	}

	//foreach
	template<typename UnaryPredicate>
	requires !std::is_pointer_v<ElementType> && std::is_invocable_v<UnaryPredicate, ElementType>
	void Iterate(UnaryPredicate&& fn)
	{
		SetIterating(true);
		for (ElementType& element : m_Elements)
			fn(element);
		SetIterating(false);
	}

#pragma endregion

private:
	std::vector<ElementType> m_Elements{};
	std::vector<typename std::vector<ElementType>::iterator> m_ElementsToRemove{};

	std::stack<bool> m_Iterating{};
};
