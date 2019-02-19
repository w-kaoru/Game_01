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

	m_titelSprite.Init(L"Assets/sprite/kitune.dds",1.0f,1.0f);
	return false;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonB) == true) {
		//ゲームシーンを作成して、自分は死ぬ。
		g_gameObjM->NewGameObject<Game>();
		g_gameObjM->DeleteGameObject(this);
	}
	m_titelSprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}
void Title::Draw()
{
	m_titelSprite.Draw();
}