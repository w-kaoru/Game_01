#pragma once
class GameObjectManager;
class IGameObject
{
public:
	IGameObject();
	virtual ~IGameObject();
	virtual bool Start() { return true; }
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Destroy() {};
protected:
	GameObjectManager * m_gojm = nullptr;
};