#include "stdafx.h"
#include "system/system.h"
#include "Title.h"
#include <algorithm>
#include"gameObject/GameObjectManager.h"


///////////////////////////////////////////////////////////////////
//ゲームの更新処理。
//座標の更新とかの処理はここで呼び出すとよいかも？
///////////////////////////////////////////////////////////////////
void UpdateGame()
{
	//ゲームパッドの更新。	
	for (auto& pad : g_pad) {
		pad.Update();
	}
	//物理エンジンの更新。
	g_physics.Update();
	//現在のシーンの更新。
	//g_gameObjM->Update();
}
///////////////////////////////////////////////////////////////////
// ゲームの描画処理。
// 描画と更新は分けてやるほうが良いかも？
///////////////////////////////////////////////////////////////////
void RenderGame()
{
	//描画開始。
	g_graphicsEngine->BegineRender();
	g_gameObjM->Execute();

	//現在のシーンの描画。
	//g_gameObjM->Draw();
	//g_gameObjM->ExecuteDraw();
	//描画終了。
	g_graphicsEngine->EndRender();
}

///////////////////////////////////////////////////////////////////
// ゲームの終了処理。
///////////////////////////////////////////////////////////////////
void TermnateGame()
{

}
///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	//タイトルシーンの作成。

	g_gameObjM->NewGameObject<Title>();

	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		//ゲームの更新。
		UpdateGame();
		//ゲームの描画処理。
		RenderGame();
	}
	//ゲームの終了処理
	TermnateGame();
}