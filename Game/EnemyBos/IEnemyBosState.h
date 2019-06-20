#pragma once
class EnemyBos;
class EnemyBosStateMachine;
class IEnemyBosState
{
public:
	IEnemyBosState(EnemyBos* en, EnemyBosStateMachine* esm);
	~IEnemyBosState();
	virtual bool Start() = 0;
	virtual void Update() = 0;
protected:
	EnemyBosStateMachine* m_stMa = nullptr;
	EnemyBos* m_enemy = nullptr;
	CVector3 m_moveSpeed = CVector3::Zero();			//ˆÚ“®‘¬“x
	CQuaternion m_rotation = CQuaternion::Identity();	//‰ñ“]B
};

