#pragma once
class Hit :public Noncopyable
{
public:
	enum objict_Name {
		enemy,
		player,
		name_num
	};
	Hit();
	~Hit();
	//ÉtÉbÉNä÷êî
	void Create(const CVector3* pos, float radius, std::function<void(float damage)>  object, objict_Name name);
	bool HitTest(CVector3 pos, float radius, float damage);
	objict_Name Getobjict_Name()
	{
		return m_name;
	}
private:
	const CVector3* m_pos = nullptr;
	float m_radius = 0.0f;
	objict_Name m_name = name_num;
	std::function<void(float damage)>  m_fuk;
};
