#pragma once
class EnemyBosState
{
public:
	enum MoveState {
		move,
		attack,
		damage,
		death,
		num,
		noState
	};

	enum AnimationState {
		AnimIdle,
		AnimMove,
		AnimAttack,
		AnimAttackPunch,
		AnimAttackStrong,
		AnimDamage,
		AnimDeath,
		AnimNum,
		AnimNoState
	};
};

