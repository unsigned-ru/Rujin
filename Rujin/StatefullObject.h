#ifndef STATEFULL_OBJECT_H
#define STATEFULL_OBJECT_H


class StatefullObject
{
public:
	StatefullObject() = default;
	virtual ~StatefullObject() = default;

	StatefullObject(const StatefullObject&) = default;
	StatefullObject(StatefullObject&&) noexcept = default;

	StatefullObject& operator=(StatefullObject&& other) noexcept;
	StatefullObject& operator=(const StatefullObject& other);

	void SetEnabled(const bool enabled);
	void Enable();
	void Disable();
	bool IsEnabled() const;

protected:
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnDestroy() {};

	bool m_IsEnabled = true;
};


#endif // Include Guard: STATEFULL_OBJECT_H
