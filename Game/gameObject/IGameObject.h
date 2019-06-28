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

	//Draw�֐������s�����O�ɌĂ΂��`�揈���B
	virtual void PreDraw() {};
	//Draw�֐������s���ꂽ��ŌĂ΂��`�揈���B
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
	//�I�u�W�F�N�g
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
	std::string m_Name = "NULL";     	//�C���X�^���X�̖��O				//�D��x
};