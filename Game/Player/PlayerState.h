#pragma once
class PlayerState
{
public:
	enum AnimState {
		idle,		//�ҋ@
		run,		//����
		attack,		//�U��
		damage,		//�_���[�W
		num,		//AniMove�̐�	
		nostate
	};
};