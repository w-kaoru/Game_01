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
	bool Start();
	void Update();
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
	//アニメーションのステートのセッター。
	void SetCombo(Combo state)
	{
		m_combo = state;
	}
	void DamageReceiveIsInit()
	{
		//ダメージを受けたため初期化
		m_combo = Combo::attack_00;
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

