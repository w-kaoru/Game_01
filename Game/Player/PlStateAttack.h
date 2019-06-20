#pragma once
#include "IPlState.h"
class PlStateAttack:public IPlState
{
public:
	enum Combo {
		attack_00,
		attack_01,
		attack_Num
	};
	PlStateAttack(Player *pl, PlStateMachine *psm);
	~PlStateAttack();
	bool Start() override;
	void Update() override;
	/*void SetAttack(int hit)
	{
		m_atkHit = hit;
	}*/
	void SetHit(bool atk)
	{
		m_atk = atk;
	}
	bool GetHit()
	{
		return m_atk;
	}
	void DamageReceiveIsInit(Combo co)
	{
		//ƒ_ƒ[ƒW‚ğó‚¯‚½‚½‚ß‰Šú‰»
		m_combo = co;
		m_hitTimer = 0;
		m_comboTimer = 0;
		m_isAtk = false;
	}
private:
	CVector3 m_attckPos = CVector3::Zero();
	int m_hitTimer = 0;
	bool m_atk = false;
	bool m_isAtk = false;
	int m_atkHit = 0;
	int m_comboTiming = 0;
	int m_comboTimer = 0;
	Combo m_combo = Combo::attack_00;
};

