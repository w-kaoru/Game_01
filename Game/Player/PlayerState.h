#pragma once
class PlayerState
{
public:
	enum AnimationState {
		AnimIdle,		//�ҋ@
		AnimRun,		//����
		AnimAttack,		//�U��
		AnimDamage,		//�_���[�W
		AnimNum,		//AniMove�̐�	
		AnimNostate
	};
	enum MoveState {
		Move,		//
		Attack,		//�U��
		Damage,		//�_���[�W
		Num,		//AniMove�̐�	
		Nostate
	};
};