#pragma once
class PlayerState
{
public:
	enum AniMove {
		idle,		//�ҋ@
		run,		//����
		jump,		//�W�����v
		attack,		//�U��
		num,		//AniMove�̐�	
		nostate
	};
};