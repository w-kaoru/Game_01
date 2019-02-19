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
bool Title::Start()
{
	//2D���������B

	m_titelSprite.Init(L"Assets/sprite/kitune.dds",1.0f,1.0f);
	return false;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonB) == true) {
		//�Q�[���V�[�����쐬���āA�����͎��ʁB
		g_gameObjM->NewGameObject<Game>();
		g_gameObjM->DeleteGameObject(this);
	}
	m_titelSprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}
void Title::Draw()
{
	m_titelSprite.Draw();
}