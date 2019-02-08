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

	//Draw関数が実行される前に呼ばれる描画処理。
	virtual void PreRender() {};
	//Draw関数が実行された後で呼ばれる描画処理。
	virtual void PostRender() {};
	friend class GameObjectManager;

	void StartDed(){
		if (startDed == false) {
			Start();
			startDed = true;
		}
	}
	void UpdateStart() {
		if (startDed == true) {
			Update();
		}
	}
	void PreRenderStart() {
		if (startDed == true) {
			PreRender();
		}
	}
	void PostRenderStart() {
		if (startDed == true) {
			PostRender();
		}
	}
	void DrawStart() {
		if (startDed == true) {
			Draw();
		}
	}
private:
	bool startDed = false;
};