#pragma once
class PlayerState
{
public:
	enum AnimState {
		idle,		//�ҋ@
		run,		//����
		jump,		//�W�����v
		attack,		//�U��
		num,		//AniMove�̐�	
		nostate
	};
	/*enum MoveState {
		num,		//AniMove�̐�	
		nostate
	};*/
};