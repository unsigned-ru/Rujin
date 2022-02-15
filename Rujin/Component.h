#pragma once
#include "MonoBehaviour.h"

class Component : public MonoBehaviour
{
public:
	explicit Component(bool startEnabled = true);
	~Component() override = default;
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void Render() const override;
};