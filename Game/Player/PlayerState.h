#pragma once
class PlayerState
{
public:
	enum AnimationState {
		AnimIdle,		//待機
		AnimRun,		//走る
		AnimAttack_00,	//攻撃
		AnimAttack_01,	//攻撃
		AnimDamage,		//ダメージ
		AnimDeath,		//死ぬ
		AnimNum,		//AniMoveの数	
		AnimNostate
	};
	enum MoveState {
		Move,		//動き
		Attack,		//攻撃
		Damage,		//ダメージ
		Death,		//死ぬ
		Num,		//AniMoveの数	
		Nostate
	};
};