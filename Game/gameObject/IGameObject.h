#pragma once
class GameObjectManager;
typedef unsigned char	GameObjectPrio;
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
extern IGameObject* g_gameObj;
