#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{
	//MessageBox(nullptr, "最初の画面はタイトルシーン！Aボタンを押すとゲームが始まるよ！", "通知", MB_OK);
}


Title::~Title()
{
}
bool Title::Start()
{
	//2Dを初期化。
	m_titelBackSprite.Init(L"Assets/sprite/TitelBack.dds",1280.0f,720.0f);
	m_titelSprite.Init(L"Assets/sprite/Titel.dds", 1280.0f, 720.0f);

	//BGM
	m_bgm.Init(L"Assets/sound/bgm_title.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.2f);
	return false;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA) == true) {
		//ゲームシーンを作成して、自分は死ぬ。
		g_gameObjM->NewGO<Game>(0,"Game");
		g_gameObjM->DeleteGO(this);
	}
	m_titelSprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	m_titelBackSprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}
void Title::PostDraw()
{
	m_titelBackSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	m_titelSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}