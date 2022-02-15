#pragma once

class MonoBehaviour
{
public:
	explicit MonoBehaviour() = default;
	MonoBehaviour(const MonoBehaviour& other) = default;
	MonoBehaviour(MonoBehaviour&& other) = default;
	MonoBehaviour& operator=(const MonoBehaviour& other) = default;
	MonoBehaviour& operator=(MonoBehaviour&& other) = default;
	virtual ~MonoBehaviour() = default;

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Render() const = 0;

	virtual void Destroy();

	void SetEnabled(const bool enabled);
	void Enable();
	void Disable();
	bool IsEnabled() const;

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();

protected:
	bool m_IsEnabled = true;
};