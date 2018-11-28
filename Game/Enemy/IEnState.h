#pragma once
class Enemy;
class EnStateMachine;
class IEnState:public IGameObject
{
public:
	IEnState(Enemy* en, EnStateMachine* esm);
	~IEnState();
	virtual bool Start() = 0;
	virtual void Update() = 0;
protected:
	EnStateMachine *m_esm = nullptr;
	Enemy* m_enemy = nullptr;
	CVector3 m_moveSpeed = CVector3::Zero();			//ˆÚ“®‘¬“x
	CQuaternion m_rotation = CQuaternion::Identity();	//‰ñ“]B
};

