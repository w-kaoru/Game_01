#pragma once
#include "stdafx.h"
#include "Save.h"
class Save :public IGameObject
{
public:
	int PlayerLv; //プレイヤーのレベル保存用。
	float Playerexp; //プレイヤーの経験値保存用。
	int SaveStage;
	//CVector3 position;
	/*!
		* @brief	初期化。
		*@param[in]	Lv		レベルの初期化。
		*@param[in]	EXP		経験値の初期化。
		*@param[in]	Stage	ステージの初期化。
	*/
	void NeworLoadGame(int Lv, float EXP, int Stage = 0)
	{
		PlayerLv = Lv;
		Playerexp = EXP;
		SaveStage = Stage;
	}
};