#pragma once
class PlayerState
{
public:
	enum AnimationState {
		AnimIdle,		//Ò@
		AnimRun,		//é
		AnimAttack_00,	//U
		AnimAttack_01,	//U
		AnimAttack_02,	//U
		AnimDamage,		//_[W
		AnimDeath,		//Ê
		AnimNum,		//AniMoveÌ	
		AnimNostate
	};
	enum MoveState {
		Move,		//®«
		Attack,		//U
		Damage,		//_[W
		Death,		//Ê
		Num,		//AniMoveÌ	
		Nostate
	};
};