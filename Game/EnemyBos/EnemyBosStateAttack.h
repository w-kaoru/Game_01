#pragma once
#include"IEnemyBosState.h"

class EnemyBosStateAttack : public IEnemyBosState
{
public:
	EnemyBosStateAttack(EnemyBos* enbos, EnemyBosStateMachine* estm);
	~EnemyBosStateAttack();
	bool Start() override;
	void Update() override;
	void SetAttack(int hit, int start)
	{
		m_atkHit = hit;
		m_atkAnimStart = start;
	}
	void SetTiming(int timing)
	{
		m_AttackTiming = timing;
	}
	bool GetAtkFlag()
	{
		return m_attackFlag;
	}
	void SetAtkFlag(bool flag)
	{
		m_attackFlag = flag;
	}
	void SetNo(int no) {
		m_no = no;
	}
private:
	int m_AttackTiming = 0;
	int m_atkAnimStart = 20;
	int m_atkHit = 35;
	bool m_attackFlag = false;
	bool m_isAtk = false;
	int m_no = 0;
	Effekseer::Handle m_playEffectHandle = -1;
	CVector3 m_hitPos;
};

