#pragma once
class BattleHit:public Noncopyable
{
public:
	enum objict_Name {
		enemy,
		player,
		name_num
	};
	BattleHit();
	~BattleHit();
	//ÉtÉbÉNä÷êî
	void Create(const CVector3* pos, float radius, std::function<void(float damage)>  object, objict_Name name);
	bool Hit(CVector3 pos, float damage);
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
