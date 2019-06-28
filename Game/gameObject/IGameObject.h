#pragma once
class GameObjectManager;
class IGameObject
{
public:
	IGameObject();
	virtual ~IGameObject();
	virtual bool Start() { return true; }
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Destroy() {};

	//Draw関数が実行される前に呼ばれる描画処理。
	virtual void PreDraw() {};
	//Draw関数が実行された後で呼ばれる描画処理。
	virtual void PostDraw() {};
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
	//オブジェクト
	int GetPriority()
	{
		return m_priority;
	}
	const std::string& GetName()const
	{
		return m_Name;
	}
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
private:
	bool startDed = false;
	int m_priority = 0;
	std::string m_Name = "NULL";     	//インスタンスの名前				//優先度
};