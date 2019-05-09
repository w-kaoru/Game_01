#include "stdafx.h"
#include "GraphicsEngine.h"


GraphicsEngine::GraphicsEngine()
{
}


GraphicsEngine::~GraphicsEngine()
{
	Release();
}

void GraphicsEngine::BegineRender()
{
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
													  //�`�����ݐ���o�b�N�o�b�t�@�ɂ���B
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	//�o�b�N�o�b�t�@���D�F�œh��Ԃ��B
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	m_pd3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	m_pd3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
	ChangeMainRenderTarget();
}
void GraphicsEngine::EndRender()
{
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	m_pSwapChain->Present(2, 0);
}
void GraphicsEngine::Release()
{
#ifdef _DEBUG
	if (m_userAnnoation) {
		m_userAnnoation->Release();
	}
#endif
	if (m_rasterizerState != NULL) {
		m_rasterizerState->Release();
		m_rasterizerState = NULL;
	}
	if (m_depthStencil != NULL) {
		m_depthStencil->Release();
		m_depthStencil = NULL;
	}
	if (m_depthStencilView != NULL) {
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
	if (m_backBuffer != NULL) {
		m_backBuffer->Release();
		m_backBuffer = NULL;
	}
	if (m_pSwapChain != NULL) {
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pd3dDeviceContext != NULL) {
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = NULL;
	}
	if (m_pd3dDevice != NULL) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
	if (m_effekseerManager != NULL)
	{
		m_effekseerManager->Release();
		//m_shadowMap = NULL;
	}
	if (m_effekseerRenderer != NULL)
	{
		m_effekseerRenderer->Destroy();
		m_effekseerRenderer = NULL;
	}
}
void GraphicsEngine::Init(HWND hWnd)
{
	//�X���b�v�`�F�C�����쐬���邽�߂̏���ݒ肷��B
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//�X���b�v�`�F�C���̃o�b�t�@���B�ʏ�͂P�B
	sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;			//�t���[���o�b�t�@�̕��B
	sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;		//�t���[���o�b�t�@�̍����B
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�t���[���o�b�t�@�̃t�H�[�}�b�g�BR8G8B8A8��32bit�B
	sd.BufferDesc.RefreshRate.Numerator = 60;			//���j�^�̃��t���b�V�����[�g�B(�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���^�C�~���O�ƂȂ�B)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//�Q�ɂ�����30fps�ɂȂ�B1�ł����B
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�T�[�t�F�X�܂��̓��\�[�X���o�̓����_�[ �^�[�Q�b�g�Ƃ��Ďg�p���܂��B
	sd.OutputWindow = hWnd;								//�o�͐�̃E�B���h�E�n���h���B
	sd.SampleDesc.Count = 1;							//1�ł����B
	sd.SampleDesc.Quality = 0;							//MSAA�Ȃ��B0�ł����B
	sd.Windowed = TRUE;									//�E�B���h�E���[�h�BTRUE�ł悢�B

														//���p����DirectX�̋@�\�Z�b�g�BDirectX10�ȏ�ɑΉ����Ă���GPU�𗘗p�\�Ƃ���B
														//���̔z���D3D11CreateDeviceAndSwapChain�̈����Ƃ��Ďg���B
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	//D3D�f�o�C�X�ƃX���b�v�`�F�C�����쐬����B
	D3D11CreateDeviceAndSwapChain(
		NULL,											//NULL�ł����B
		D3D_DRIVER_TYPE_HARDWARE,						//D3D�f�o�C�X���A�N�Z�X����h���C�o�[�̎�ށB
														//��{�I��D3D_DRIVER_TYPE_HARDWARE���w�肷��΂悢�B
		NULL,											//NULL�ł����B
		0,												//�O�ł����B
		featureLevels,									//D3D�f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g���w�肷��B
														//����̃T���v����DirectX10�ȏ���T�|�[�g����̂ŁA
														//�������܂�D3D_FEATURE_LEVEL�̔z���n���B
		sizeof(featureLevels) / sizeof(featureLevels[0]),	//�@�\�Z�b�g�̐��B
		D3D11_SDK_VERSION,								//�g�p����DirectX�̃o�[�W�����B
														//D3D11_SDK_VERSION���w�肷��΂悢�B
		&sd,											//�X���b�v�`�F�C�����쐬���邽�߂̏��B
		&m_pSwapChain,									//�쐬�����X���b�v�`�F�C���̃A�h���X�̊i�[��B
		&m_pd3dDevice,									//�쐬����D3D�f�o�C�X�̃A�h���X�̊i�[��B
		&m_featureLevel,								//�g�p�����@�\�Z�b�g�̊i�[��B
		&m_pd3dDeviceContext							//�쐬����D3D�f�o�C�X�R���e�L�X�g�̃A�h���X�̊i�[��B
	);

	//�������ݐ�ɂȂ郌���_�����O�^�[�Q�b�g���쐬�B
	ID3D11Texture2D* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();
	//�[�x�X�e���V���r���[�̍쐬�B
	{
		//�[�x�e�N�X�`���̍쐬�B
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = (UINT)FRAME_BUFFER_W;
		texDesc.Height = (UINT)FRAME_BUFFER_H;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_depthStencil);
		//�[�x�X�e���V���r���[���쐬�B
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = texDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		m_pd3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
	}
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//���X�^���C�U�ƃr���[�|�[�g���������B
	m_pd3dDevice->CreateRasterizerState(&desc, &m_rasterizerState);

	D3D11_VIEWPORT viewport;
	viewport.Width = FRAME_BUFFER_W;
	viewport.Height = FRAME_BUFFER_H;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_pd3dDeviceContext->RSSetViewports(1, &viewport);
	m_pd3dDeviceContext->RSSetState(m_rasterizerState);
	{
		//�u�����h�ݒ�
		D3D11_BLEND_DESC BLEND_DETE; 
		ID3D11BlendState* BlendState;
		BLEND_DETE.AlphaToCoverageEnable = false;
		BLEND_DETE.IndependentBlendEnable = false;
		BLEND_DETE.RenderTarget[0].BlendEnable = true;
		BLEND_DETE.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BLEND_DETE.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BLEND_DETE.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_pd3dDevice->CreateBlendState(&BLEND_DETE, &BlendState);
		m_pd3dDeviceContext->OMSetBlendState(BlendState, nullptr, 0xFFFFFFFF);
		MemoryBarrier();
	}
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthWriteMask = false ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		desc.StencilEnable = false;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		desc.BackFace = desc.FrontFace;

		ID3D11DepthStencilState* depthStencilState;
		m_pd3dDevice->CreateDepthStencilState(&desc, &depthStencilState);
		m_pd3dDeviceContext->OMSetDepthStencilState(depthStencilState, 0);
		MemoryBarrier();
	}
	{
		//�����_���[���������B
		m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			g_graphicsEngine->GetD3DDevice(),			//D3D�f�o�C�X�B
			g_graphicsEngine->GetD3DDeviceContext(),	//D3D�f�o�C�X�R���e�L�X�g�B
			20000										//�|���̍ő吔�B
		);
		//�G�t�F�N�g�}�l�[�W�����������B
		m_effekseerManager = Effekseer::Manager::Create(10000);

		// �`��p�C���X�^���X����`��@�\��ݒ�
		m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
		m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
		m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
		m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
		m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
		m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());
	}
	//���C�������_�����O�^�[�Q�b�g�쐬�B
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G��
	//�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������B
	m_sprite.Init(
		g_graphicsEngine->GetMainRenderTarget()->GetRenderTargetSRV(),
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
	//�V���h�E�}�b�v����ׁ[�B
	m_shadowMap.Init();
	//�|�X�g�G�t�F�N�g����ׁ[�B
	m_postEffect.Init();
	
#ifdef _DEBUG
	m_pd3dDeviceContext->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)&m_userAnnoation);
#endif
}

void GraphicsEngine::ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}
void GraphicsEngine::ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//�����_�����O�^�[�Q�b�g�̐؂�ւ��B
	m_pd3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//�r���[�|�[�g���w�肳��Ă�����A�r���[�|�[�g���ύX����B
		m_pd3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GraphicsEngine::ShadowDraw()
{
	m_shadowMap.RenderToShadowMap();
	ChangeMainRenderTarget();
	float clearColor[] = { 0.10f, 0.10f, 0.10f, 1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
}

void GraphicsEngine::PostEffectDraw()
{
	m_postEffect.Draw();
	ChangeRenderTarget(
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//g_camera2D.Update();
	m_sprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	// /*
	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
	// */
}
void GraphicsEngine::EffectDraw()
{
	//�܂���Effeseer�̍s��^�̕ϐ��ɁA�J�����s��ƃv���W�F�N�V�����s����R�s�[�B
	Effekseer::Matrix44 efCameraMat;
	g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
	//�J�����s��ƃv���W�F�N�V�����s���ݒ�B
	m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	m_effekseerRenderer->SetProjectionMatrix(efProjMat);
	//Effekseer���X�V�B
	m_effekseerManager->Update();
	//�G�t�F�N�g�͕s�����I�u�W�F�N�g��`�悵����ŕ`�悷��B
	m_effekseerRenderer->BeginRendering();
	m_effekseerManager->Draw();
	m_effekseerRenderer->EndRendering();
}

void GraphicsEngine::ChangeMainRenderTarget()
{
	ChangeRenderTarget(
		&m_mainRenderTarget,
		m_mainRenderTarget.GetViewport()
	);
}
