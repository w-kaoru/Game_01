#pragma once
#include "stdafx.h"
#include "PlayerSave.h"
class PlayerSave:public IGameObject
{
public:
	int Lv;
	float exp;
	//CVector3 position;
	void NeworLoadGame(int l, float e)
	{
		Lv = l;
		exp = e;
	}

};