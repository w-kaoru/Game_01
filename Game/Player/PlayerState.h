#pragma once
class PlayerState
{
public:
	enum AnimationState {
		AnimIdle,		//待機
		AnimRun,		//走る
		AnimAttack,		//攻撃
		AnimDamage,		//ダメージ
		AnimNum,		//AniMoveの数	
		AnimNostate
	};
	enum MoveState {
		Move,		//
		Attack,		//攻撃
		Damage,		//ダメージ
		Num,		//AniMoveの数	
		Nostate
	};
};