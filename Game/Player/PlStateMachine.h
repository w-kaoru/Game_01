#pragma once
#include "PlStateIdle.h"
#include "PlStateRun.h"
#include "PlayerState.h"


class Player;
class PlStateMachine
{
public:
	PlStateMachine(Player *pl);
	~PlStateMachine();
	bool Start();
	void Update();
	void Change(PlayerState::AniMove state);
private:
	PlStateIdle m_plIdle;
	PlStateRun m_plRun;
	IPlState *m_ips = nullptr;
	PlayerState::AniMove m_state = PlayerState::nostate;
};