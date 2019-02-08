#include "stdafx.h"
#include "GameObjectManager.h"




GameObjectManager::~GameObjectManager()
{
}

//���s����
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
	//�`��J�n�B
	g_graphicsEngine->BegineRender();
	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

	//�����_�����O�^�[�Q�b�g�����C���ɕύX����B
	g_graphicsEngine->ChangeRenderTarget(g_graphicsEngine->GetMainRenderTarget(), &m_frameBufferViewports);
	//���C�������_�����O�^�[�Q�b�g���N���A����B
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

	//�����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂��B
	g_graphicsEngine->ChangeRenderTarget(
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//�h���h��
	//m_copyMainRtToFrameBufferSprite.Draw();

	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
	//�`��I���B
	g_graphicsEngine->EndRender();
}