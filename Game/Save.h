#pragma once
#include "stdafx.h"
#include "Save.h"
class Save:public IGameObject
{
public:
	int PlayerLv;
	float Playerexp;
	CVector3 position;
	int SaveStage;
	//CVector3 position;
	void NeworLoadGame(int l, float e, int s = 0)
	{
		PlayerLv = l;
		Playerexp = e;
		SaveStage = s;
	}
};