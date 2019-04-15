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
	m_titelSprite.Init(L"Assets/sprite/TitelBack.dds",1280.0f,720.0f);
	m_titelSprite1.Init(L"Assets/sprite/Titel.dds", 1280.0f, 720.0f);
	return false;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA) == true) {
		//�Q�[���V�[�����쐬���āA�����͎��ʁB
		g_gameObjM->NewGameObject<Game>();
		g_gameObjM->DeleteGameObject(this);
	}
	m_titelSprite1.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	m_titelSprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}
void Title::PostDraw()
{
	m_titelSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	m_titelSprite1.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}