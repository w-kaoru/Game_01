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
BattleController* g_battleController;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	//タイトルシーンの作成。
	g_battleController = new BattleController;
	g_gameObjM->NewGO<Title>();

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