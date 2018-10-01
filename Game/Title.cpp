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
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonB) == true) {
		//ゲームシーンを作成して、自分は死ぬ。
		g_gameObj = new Game;
		delete this;
	}
}
void Title::Draw()
{
}