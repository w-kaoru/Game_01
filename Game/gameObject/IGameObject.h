#pragma once
class GameObjectManager;
class IGameObject
{
public:
	IGameObject();
	virtual ~IGameObject();
	virtual bool Start() { return true; }
	virtual void Update() = 0;
	virtual void Draw() {};
	virtual void Destroy() {};
	friend class GameObjectManager;
	void StartDed(){
		if (startDed == false) {
			Start();
			startDed = true;
		}
	}
	void DrawStart() {
		if (startDed == true) {
			Draw();
		}
	}
	void UpdateStart() {
		if (startDed == true) {
			Update();
		}
	}
private:
	bool startDed = false;
};