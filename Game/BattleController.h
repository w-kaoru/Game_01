#pragma once
class Player;
class Enemy;
class BattleController :public IGameObject
{
public:
	BattleController();
	~BattleController();
	bool Start();
	void Attack();
	void Damage();
	void Update();
	float SetHP(float hp)
	{
		m_hp = hp;
	}
	float SetATK(float atk)
	{
		m_atk = atk;
	}
	float SetDEF(float def)
	{
		m_def = def;
	}
	float GetDamage()
	{
		return m_damage;
	}
private:
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	float m_hp = 0.0f;				//‘Ì—Í
	float m_atk = 0.0f;			//UŒ‚—Í
	float m_def = 0.0f;			//–hŒä—Í
	float m_damage = 0.0f;		//ƒ_ƒ[ƒW
};

