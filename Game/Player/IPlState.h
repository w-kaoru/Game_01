#pragma once

class Player;
class PlStateMachine;
class IPlState
{
public:
	IPlState(Player *pl,PlStateMachine *psm);
	~IPlState();
	virtual bool Start() = 0;
	virtual void Update() = 0;
protected:
	PlStateMachine * m_plstma = nullptr;
	Player * m_player = nullptr;
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
};

