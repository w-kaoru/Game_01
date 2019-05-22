#pragma once
class PlayerState
{
public:
	enum AnimState {
		idle,		//待機
		run,		//走る
		attack,		//攻撃
		damage,		//ダメージ
		num,		//AniMoveの数	
		nostate
	};
};