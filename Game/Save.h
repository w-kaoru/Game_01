#pragma once
#include "stdafx.h"
#include "Save.h"
class Save:public IGameObject
{
public:
	int Lv;
	float exp;
	CVector3 position;
	//CVector3 position;
	void NeworLoadGame(int l, float e)
	{
		Lv = l;
		exp = e;
	}
	int GetloopcCount()
	{
		return m_loopcCount;
	}
	void SetLoopcCount(int l)
	{
		m_loopcCount = l;
	}
	void LoopcCount()
	{
		m_loopcCount++;
	}
private:
	int m_loopcCount = 0; 
};