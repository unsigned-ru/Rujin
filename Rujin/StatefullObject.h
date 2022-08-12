#ifndef STATEFULL_OBJECT_H
#define STATEFULL_OBJECT_H


class StatefullObject
{
public:
	StatefullObject() = default;
	virtual ~StatefullObject() = default;

	StatefullObject(const StatefullObject&) = delete;
	StatefullObject(StatefullObject&&) noexcept = delete;

	StatefullObject& operator=(StatefullObject&& other) noexcept = delete;
	StatefullObject& operator=(const StatefullObject& other) = delete;

	void SetEnabled(const bool enabled);
	void Enable();
	void Disable();
	bool IsEnabled() const;

protected:
	virtual void OnEnable() {};
	virtual void OnDisable() {};

	bool m_IsEnabled = true;
	bool m_PendingDestroy = false;
};


#endif // Include Guard: STATEFULL_OBJECT_H
