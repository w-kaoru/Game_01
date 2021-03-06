#pragma once
#include "ShadowMap.h"
#include "PostEffect.h"
#include "Sprite.h"
// レンダリングモード。
enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_Normal,			//通常レンダリング。
	enRenderMode_CubeMap,
	enRenderMode_Num,				//レンダリングモードの数。
};
/*!
 *@brief	グラフィックスエンジン。
 */
class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/*!
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
	 */
	void EndRender();
	// エフェクトのマネージャークラスの取得
	// エフェクトのマネージャークラス（Effekseer::Manager*）
	Effekseer::Manager* GetEffekseerManager()
	{
		return m_effekseerManager;
	}
	void EffectDraw();

	ShadowMap* GetShadowMap() {
		return &m_shadowMap;
	}
	/// <summary>
	/// レンダリングターゲットの切り替え。
	/// </summary>
	/// <param name="renderTarget">レンダリングターゲット</param>
	/// <param name="viewport">ビューポート</param>
	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	/// <summary>
	/// メインレンダリングターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget* GetMainRenderTarget()
	{
		return &m_mainRenderTarget;
	}
	Sprite* GetSprite()
	{
		return &m_sprite;
	}
	/*void SetPostEffect(PostEffect pe)
	{
		m_postEffect = pe;
	}*/
	PostEffect* GetPostEffect()
	{
		return &m_postEffect;
	}

	// ラスタライザステートのゲット
	// ラスタライザステート
	ID3D11RasterizerState* GetRasterizerState()
	{
		return m_rasterizerState;
	}
	// ゲームでデフォルトのフォントを取得
	// フォントデータ
	DirectX::SpriteFont* GetSpriteFont()
	{
		return m_SpriteFont;
	}
	// フォント表示に必要なクラスを取得
	// フォント表示に必要なクラス
	DirectX::SpriteBatch* GetSpriteBatch()
	{
		return m_SpriteBatch;
	}
	void BeginGPUEvent(const wchar_t* eventName)
	{
#ifdef _DEBUG
		if (m_userAnnoation != nullptr) {
			m_userAnnoation->BeginEvent(eventName);
		}
#endif
	}
	void EndGPUEvent()
	{
#ifdef _DEBUG
		if (m_userAnnoation != nullptr) {
			m_userAnnoation->EndEvent();
		}
#endif
	}
	void ShadowDraw();
	void PostEffectDraw();
	void ChangeMainRenderTarget();
private:
	D3D_FEATURE_LEVEL		m_featureLevel;					//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;			//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;			//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;		//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;			//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;		//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;			//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;		//デプスステンシルビュー。
	RenderTarget			m_mainRenderTarget;				//メインレンダリングターゲット。
	Sprite					m_sprite;//メインレンダリングターゲットに描かれた絵をフレームバッファにコピーするためのスプライト。
	ShadowMap				m_shadowMap;					//シャドウマップ。
	PostEffect				m_postEffect;					//ポストエフェクト。
	ID3DUserDefinedAnnotation*		m_userAnnoation = nullptr;
	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。

	DirectX::SpriteFont*    m_SpriteFont = NULL;		//ゲームのデフォルトのフォントデータ
	DirectX::SpriteBatch*   m_SpriteBatch = NULL;		//ゲームのデフォルトのフォント表示用データ

	Effekseer::Manager*	m_effekseerManager = nullptr;	//エフェクトマネージャークラス
	EffekseerRenderer::Renderer*	m_effekseerRenderer = nullptr;  //エフェクトレンダラークラス

};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン