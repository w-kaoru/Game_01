#pragma once
class PlayerState
{
public:
	enum AnimationState {
		AnimIdle,		//�ҋ@
		AnimRun,		//����
		AnimAttack_00,	//�U��
		AnimAttack_01,	//�U��
		AnimDamage,		//�_���[�W
		AnimDeath,		//����
		AnimNum,		//AniMove�̐�	
		AnimNostate
	};
	enum MoveState {
		Move,		//����
		Attack,		//�U��
		Damage,		//�_���[�W
		Death,		//����
		Num,		//AniMove�̐�	
		Nostate
	};
};