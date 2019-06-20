#pragma once
class EnemyState
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
		AnimDamage,
		AnimNum,
		AnimNoState
	};
};

