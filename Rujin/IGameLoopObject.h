#pragma once

class IGameLoopObject
{
public:
	explicit IGameLoopObject() = default;
	IGameLoopObject(const IGameLoopObject& other) = default;
	IGameLoopObject(IGameLoopObject&& other) = default;
	IGameLoopObject& operator=(const IGameLoopObject& other) = default;
	IGameLoopObject& operator=(IGameLoopObject&& other) = default;
	virtual ~IGameLoopObject() = 0;

	virtual void Start() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void OnGui(SDL_Window* /*pWindow*/) {};
	virtual void Render() const {};
	virtual void Destroy() {};

	void SetEnabled(const bool enabled);
	void Enable();
	void Disable();
	bool IsEnabled() const;

	/*virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();*/

protected:
	bool m_IsEnabled = true;
};