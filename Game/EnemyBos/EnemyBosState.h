#pragma once
class EnemyBosState
{
public:
	enum MoveState {
		move,
		attack,
		damage,
		num,
		noState
	};

	enum AnimationState {
		AnimIdle,
		AnimMove,
		AnimAttack,
		AnimDamage,
		AnimNum,
		AnimNoState
	};
};

