#pragma once
class PlayerState
{
public:
	enum AniMove {
		idle,		//待機
		run,		//走る
		jump,		//ジャンプ
		attack,		//攻撃
		num,		//AniMoveの数	
		nostate
	};
};