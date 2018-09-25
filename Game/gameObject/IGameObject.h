#pragma once
class GameObjectManager;
class IGameObject
{
public:
	IGameObject();
	virtual ~IGameObject();
	virtual bool Start() { return true; }
	virtual void Update() {}
	virtual void Draw() {}
	friend class CGameObjectManager;
protected:
	//GameObjectManager * m_gojm = nullptr;
};

