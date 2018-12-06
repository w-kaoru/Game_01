#include "stdafx.h"
#include "Game.h"
#include "Player\Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"

//グローバルなアクセスポイントをグローバル変数として提供する。
Game* g_game = nullptr;

Game::Game()
{
	//m_player = g_gameObjM->NewGameObject<Player>();
	g_game = this;
	//レベルを初期化。
	//m_level.Init(L"Assets/level/stage_02.tkl",&m_shadowMap, nullptr);
	m_level.Init(L"Assets/level/stage_02.tkl",&m_shadowMap, [&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGameObject<Player>();
			m_player->SetPosition(objData.position);
			m_player->SetShadow(&m_shadowMap);
			return true;
		}
		else if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemy->SetEnemySelect(1);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->GetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			m_enemy->SetShadow(&m_shadowMap);
			return true;
		}
		else if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemy->SetEnemySelect(2);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->GetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			m_enemy->SetShadow(&m_shadowMap);
			return true;
		}
		return false;
	});
	m_gameCamera=g_gameObjM->NewGameObject<GameCamera>(4);
	m_gameCamera->SetPlayer(m_player);
}


Game::~Game()
{
	g_gameObjM->DeleteGameObject(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGameObject(enemy);
	}
	g_gameObjM->DeleteGameObject(m_gameCamera);
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonB)) {
		g_gameObjM->DeleteGameObject(this);
		//タイトルシーンの作成。
		g_gameObjM->NewGameObject<Title>();
	}
	//シャドウマップを更新。
	m_shadowMap.UpdateFromLightTarget(
		{ 1000.0f, 1000.0f, 1000.0f },
		{ 0.0f, 0.0f, 0.0f }
	);
}

void Game::Draw()
{
	//描画開始。
	g_graphicsEngine->BegineRender();
	///////////////////////////////////////////////
	//シャドウマップにレンダリング
	///////////////////////////////////////////////
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//現在のレンダリングターゲットをバックアップしておく。
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&oldRenderTargetView,
		&oldDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);

	//シャドウマップにレンダリング
	m_shadowMap.RenderToShadowMap();

	//元に戻す。
	d3dDeviceContext->OMSetRenderTargets(
		1,
		&oldRenderTargetView,
		oldDepthStencilView
	);
	d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	//レンダリングターゲットとデプスステンシルの参照カウンタを下す。
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();

	///////////////////////////////////////////////
	//ここから通常レンダリング。
	///////////////////////////////////////////////
	m_level.Draw();
	m_player->Draw();
	for (auto& enemy : m_enemyList) {
		enemy->Draw();
	}
	//描画終了。
	g_graphicsEngine->EndRender();
}

void Game::Destroy()
{
}
