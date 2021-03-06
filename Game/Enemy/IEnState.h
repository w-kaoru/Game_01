#pragma once
class Enemy;
class EnStateMachine;
class IEnState
{
public:
	IEnState(Enemy* en, EnStateMachine* esm);
	~IEnState();
	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Destroy() {};
protected:
	EnStateMachine* m_stMa = nullptr;
	Enemy* m_enemy = nullptr;
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
};

