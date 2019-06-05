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
	virtual void PreDraw() {};
	//Draw関数が実行された後で呼ばれる描画処理。
	virtual void PostDraw() {};
	virtual void PostPostDraw() {};
	friend class GameObjectManager;

	void StartWrapper(){
		if (startDed == false) {
			Start();
			startDed = true;
		}
	}
	void UpdateWrapper() {
		if (startDed == true) {
			Update();
		}
	}
	void DrawWrapper() {
		if (startDed == true) {
			Draw();
		}
	}
	void PreDrawWrapper() {
		if (startDed == true) {
			PreDraw();
		}
	}
	void PostDrawWrapper() {
		if (startDed == true) {
			PostDraw();
		}
	}
	void PostPostDrawWrapper() {
		if (startDed == true) {
			PostPostDraw();
		}
	}
	//オブジェクト
	int GetPriority()
	{
		return m_priority;
	}
private:
	bool startDed = false;
	int m_priority = 0;					//優先度
};