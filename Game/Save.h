#pragma once
#include "stdafx.h"
#include "Save.h"
class Save :public IGameObject
{
public:
	int PlayerLv; //�v���C���[�̃��x���ۑ��p�B
	float Playerexp; //�v���C���[�̌o���l�ۑ��p�B
	int SaveStage;
	//CVector3 position;
	/*!
		* @brief	�������B
		*@param[in]	Lv		���x���̏������B
		*@param[in]	EXP		�o���l�̏������B
		*@param[in]	Stage	�X�e�[�W�̏������B
	*/
	void NeworLoadGame(int Lv, float EXP, int Stage = 0)
	{
		PlayerLv = Lv;
		Playerexp = EXP;
		SaveStage = Stage;
	}
};