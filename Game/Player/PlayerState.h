#pragma once
class PlayerState
{
public:
	enum AnimState {
		idle,		//待機
		run,		//走る
		jump,		//ジャンプ
		attack,		//攻撃
		num,		//AniMoveの数	
		nostate
	};
	/*enum MoveState {
		num,		//AniMoveの数	
		nostate
	};*/
};