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
	friend class GameObjectManager;
	void StartDed(){
		if (startDed == false) {
			Start();
			startDed = true;
		}
	}
private:
	bool startDed = false;
};