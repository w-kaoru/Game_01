#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{
	//MessageBox(nullptr, "�ŏ��̉�ʂ̓^�C�g���V�[���IA�{�^���������ƃQ�[�����n�܂��I", "�ʒm", MB_OK);
}


Title::~Title()
{
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonB) == true) {
		//�Q�[���V�[�����쐬���āA�����͎��ʁB
		g_gameObj = new Game;
		delete this;
	}
}
void Title::Draw()
{
}