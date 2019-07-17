#pragma once
#include "stdafx.h"
#include "Save.h"
class Save:public IGameObject
{
public:
	int PlayerLv;
	float Playerexp;
	CVector3 position;
	//CVector3 position;
	void NeworLoadGame(int l, float e)
	{
		PlayerLv = l;
		Playerexp = e;
	}
};