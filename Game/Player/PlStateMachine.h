#pragma once
#include "PlStateIdle.h"
#include "PlStateRun.h"
#include "PlStateAttack.h"
#include "PlayerState.h"


class Player;
class PlStateMachine
{
public:
	PlStateMachine(Player *pl);
	~PlStateMachine();
	bool Start();
	void Update();
	void Change(PlayerState::AnimState state);
private:
	PlStateIdle m_plIdle;
	PlStateRun m_plRun;
	PlStateAttack m_plAttack;
	IPlState *m_ips = nullptr;
	PlayerState::AnimState m_state = PlayerState::nostate;
};