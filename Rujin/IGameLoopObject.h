#ifndef IGAMELOOPOBJECT_H
#define IGAMELOOPOBJECT_H


class IGameLoopObject
{
public:
	explicit IGameLoopObject() = default;
	IGameLoopObject(const IGameLoopObject& other) = default;
	IGameLoopObject(IGameLoopObject&& other) = default;
	IGameLoopObject& operator=(const IGameLoopObject& other) = default;
	IGameLoopObject& operator=(IGameLoopObject&& other) = default;
	virtual ~IGameLoopObject() = default;

	virtual void Start() {};
	virtual void LateStart() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void OnGui(SDL_Window* /*pWindow*/) {};
	virtual void Draw() const {};


	virtual void Destroy() {};
};


#endif // Include Guard: IGAMELOOPOBJECT_H
