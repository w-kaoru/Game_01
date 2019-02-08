#include "stdafx.h"
#include "GameObjectManager.h"




GameObjectManager::~GameObjectManager()
{
}

//実行処理
void GameObjectManager::Execute()
{
	g_graphicsEngine->GetMainRenderTarget()->Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	g_graphicsEngine->GetCopyMainRtToFrameBufferSprite()->Init(
		g_graphicsEngine->GetMainRenderTarget()->GetRenderTargetSRV(),
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
	//g_graphicsEngine->GetShadowMap()->Create();
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->StartDed();
		}
	}
	
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj:objList)	{
			obj->UpdateStart();
		}
	}

	// /*
	//描画開始。
	g_graphicsEngine->BegineRender();
	//フレームバッファのレンダリングターゲットをバックアップしておく。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

	//レンダリングターゲットをメインに変更する。
	g_graphicsEngine->ChangeRenderTarget(g_graphicsEngine->GetMainRenderTarget(), &m_frameBufferViewports);
	//メインレンダリングターゲットをクリアする。
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g_graphicsEngine->GetMainRenderTarget()->ClearRenderTarget(clearColor);

	g_graphicsEngine->GetShadowMap()->RenderToShadowMap();



	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->PreRenderStart();
		}
	}

	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->DrawStart();
		}
	}
	g_graphicsEngine->GetPostEffect()->Draw();

	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->PostRenderStart();
		}
	}

	//レンダリングターゲットをフレームバッファに戻す。
	g_graphicsEngine->ChangeRenderTarget(
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//ドロドロ
	//m_copyMainRtToFrameBufferSprite.Draw();

	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
	//描画終了。
	g_graphicsEngine->EndRender();
}