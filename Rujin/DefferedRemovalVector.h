#ifndef DEFFERED_REMOVAL_VECTOR_H
#define DEFFERED_REMOVAL_VECTOR_H


#include <functional>
#include <stack>
#include <memory>
#include <utility>

template<typename ElementType, typename PointerType>
class DeferredVector
{
public:
	explicit DeferredVector() = default;
	explicit DeferredVector(const std::vector<ElementType>& vec)
		: m_Elements(vec)
	{}

	std::vector<ElementType>& GetVector() { return m_Elements; }
	const std::vector<ElementType>& GetVector() const { return m_Elements; }
	const std::vector<ElementType>& GetElementsToAdd() const { return m_ElementsToAdd; }

	std::vector<PointerType>&  GetElementsToRemove() { return m_ElementsToRemove; }
	const std::vector<PointerType>&  GetElementsToRemove() const { return m_ElementsToRemove; }

	void ProcessAdditions()
	{
		/*Pending additions*/
		if constexpr (!std::is_pointer_v<ElementType>)
		{
			if(std::is_same_v<std::decay_t<ElementType>, std::unique_ptr<typename std::decay_t<ElementType>::element_type>>)
			{
				for (auto& element : m_ElementsToAdd)
				{
					m_Elements.push_back(std::move(element));
					m_OnAddedCallback(m_Elements.at(m_Elements.size() -1));
				}
			}
		}
		else
		{
			for (auto& element : m_ElementsToAdd)
			{
				m_Elements.push_back(element);
				m_OnAddedCallback(element);
			}
		}

		m_ElementsToAdd.clear();
	}

	void ProcessRemovals()
	{
		/*Pending removal*/
		//remove...
		for (PointerType pElementToRemove : m_ElementsToRemove)
		{
			m_OnRemoveCallback(pElementToRemove);

			if constexpr (std::is_pointer<ElementType>())
			{
				m_Elements.erase(std::find_if(m_Elements.begin(), m_Elements.end(), [pElementToRemove](ElementType element) { return element == pElementToRemove; }));
			}
			else
			{
				m_Elements.erase(std::find_if(m_Elements.begin(), m_Elements.end(), [pElementToRemove](ElementType& element) { return element.get() == pElementToRemove; }));
			}
		}

		//clear
		m_ElementsToRemove.clear();
	}


	void PendRemove(ElementType pElementToRemove) requires std::is_pointer_v<ElementType>
	{
		if (!pElementToRemove) return;

		m_ElementsToRemove.emplace_back(pElementToRemove);
	}

	void PendRemove(PointerType elementToRemove) requires !std::is_pointer_v<ElementType>
	{
		m_ElementsToRemove.emplace_back(elementToRemove);
	}

	void PendPushBack(ElementType elementToAdd)
	{
		if constexpr (!std::is_pointer_v<ElementType>)
		{
			if constexpr (std::is_same_v<std::decay_t<ElementType>, std::unique_ptr<typename std::decay_t<ElementType>::element_type>>)
				m_ElementsToAdd.emplace_back(std::move(elementToAdd));
		}
		else
		{
			m_ElementsToAdd.push_back(elementToAdd);
		}
	}

	void SetElementAddedCallback(const std::function<void(std::conditional_t<std::is_pointer_v<ElementType>, ElementType, ElementType&>)>& callback)
	{
		m_OnAddedCallback = callback;
	}

	void SetElementRemovedCallback(const std::function<void(PointerType)>& callback)
	{
		m_OnRemoveCallback = callback;
	}

private:
	std::vector<PointerType> m_ElementsToRemove{};
	std::vector<ElementType> m_ElementsToAdd{};
	std::vector<ElementType> m_Elements{};

	std::function<void(std::conditional_t<std::is_pointer_v<ElementType>, ElementType, ElementType&>)> m_OnAddedCallback = [](std::conditional_t<std::is_pointer_v<ElementType>, ElementType, ElementType&>) {};
	std::function<void(PointerType)> m_OnRemoveCallback = [](PointerType ) {};
};


#endif // Include Guard: DEFFERED_REMOVAL_VECTOR_H
