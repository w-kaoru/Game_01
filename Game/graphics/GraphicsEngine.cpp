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
													  //描き込み先をバックバッファにする。
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	//バックバッファを灰色で塗りつぶす。
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//フレームバッファのレンダリングターゲットをバックアップしておく。
	m_pd3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	m_pd3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
	ChangeMainRenderTarget();
}
void GraphicsEngine::EndRender()
{
	//バックバッファとフロントバッファを入れ替える。
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
	//スワップチェインを作成するための情報を設定する。
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//スワップチェインのバッファ数。通常は１。
	sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;			//フレームバッファの幅。
	sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;		//フレームバッファの高さ。
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//フレームバッファのフォーマット。R8G8B8A8の32bit。
	sd.BufferDesc.RefreshRate.Numerator = 60;			//モニタのリフレッシュレート。(バックバッファとフロントバッファを入れ替えるタイミングとなる。)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//２にしたら30fpsになる。1でいい。
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//サーフェスまたはリソースを出力レンダー ターゲットとして使用します。
	sd.OutputWindow = hWnd;								//出力先のウィンドウハンドル。
	sd.SampleDesc.Count = 1;							//1でいい。
	sd.SampleDesc.Quality = 0;							//MSAAなし。0でいい。
	sd.Windowed = TRUE;									//ウィンドウモード。TRUEでよい。

														//利用するDirectXの機能セット。DirectX10以上に対応しているGPUを利用可能とする。
														//この配列はD3D11CreateDeviceAndSwapChainの引数として使う。
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	//D3Dデバイスとスワップチェインを作成する。
	D3D11CreateDeviceAndSwapChain(
		NULL,											//NULLでいい。
		D3D_DRIVER_TYPE_HARDWARE,						//D3Dデバイスがアクセスするドライバーの種類。
														//基本的にD3D_DRIVER_TYPE_HARDWAREを指定すればよい。
		NULL,											//NULLでいい。
		0,												//０でいい。
		featureLevels,									//D3Dデバイスのターゲットとなる機能セットを指定する。
														//今回のサンプルはDirectX10以上をサポートするので、
														//それらを含むD3D_FEATURE_LEVELの配列を渡す。
		sizeof(featureLevels) / sizeof(featureLevels[0]),	//機能セットの数。
		D3D11_SDK_VERSION,								//使用するDirectXのバージョン。
														//D3D11_SDK_VERSIONを指定すればよい。
		&sd,											//スワップチェインを作成するための情報。
		&m_pSwapChain,									//作成したスワップチェインのアドレスの格納先。
		&m_pd3dDevice,									//作成したD3Dデバイスのアドレスの格納先。
		&m_featureLevel,								//使用される機能セットの格納先。
		&m_pd3dDeviceContext							//作成したD3Dデバイスコンテキストのアドレスの格納先。
	);

	//書き込み先になるレンダリングターゲットを作成。
	ID3D11Texture2D* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();
	//深度ステンシルビューの作成。
	{
		//深度テクスチャの作成。
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
		//深度ステンシルビューを作成。
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

	//ラスタライザとビューポートを初期化。
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
		//ブレンド設定
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
		//レンダラーを初期化。
		m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			g_graphicsEngine->GetD3DDevice(),			//D3Dデバイス。
			g_graphicsEngine->GetD3DDeviceContext(),	//D3Dデバイスコンテキスト。
			20000										//板ポリの最大数。
		);
		//エフェクトマネージャを初期化。
		m_effekseerManager = Effekseer::Manager::Create(10000);

		// 描画用インスタンスから描画機能を設定
		m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
		m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
		m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
		m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
		m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
		m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());
	}
	//メインレンダリングターゲット作成。
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	//メインレンダリングターゲットに描かれた絵を
	//フレームバッファにコピーするためのスプライトを初期化する。
	m_sprite.Init(
		g_graphicsEngine->GetMainRenderTarget()->GetRenderTargetSRV(),
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
	//シャドウマップをよべー。
	m_shadowMap.Init();
	//ポストエフェクトをよべー。
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
	//レンダリングターゲットの切り替え。
	m_pd3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
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
	//まずはEffeseerの行列型の変数に、カメラ行列とプロジェクション行列をコピー。
	Effekseer::Matrix44 efCameraMat;
	g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
	//カメラ行列とプロジェクション行列を設定。
	m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	m_effekseerRenderer->SetProjectionMatrix(efProjMat);
	//Effekseerを更新。
	m_effekseerManager->Update();
	//エフェクトは不透明オブジェクトを描画した後で描画する。
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
