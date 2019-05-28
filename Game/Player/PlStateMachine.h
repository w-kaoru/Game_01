#pragma once
#include "PlStateMove.h"
#include "PlStateAttack.h"
#include "PlStateDamage.h"
#include "PlStateDeath.h"
#include "PlayerState.h"


class Player;
class PlStateMachine
{
public:
	PlStateMachine(Player *pl);
	~PlStateMachine();
	bool Start();
	void Update();
	void Change(PlayerState::MoveState state);
	PlStateAttack* StateAttack()
	{
		return &m_plAttack;
	}
	PlStateDamage* StateDamage()
	{
		return &m_plDamage;
	}
	PlStateDeath* StateDeath()
	{
		return &m_plDeath;
	}
private:
	PlStateMove m_plMove;
	PlStateAttack m_plAttack;
	PlStateDamage m_plDamage;
	PlStateDeath m_plDeath;
	IPlState *m_ips = nullptr;
	PlayerState::MoveState m_state = PlayerState::MoveState::Nostate;
};