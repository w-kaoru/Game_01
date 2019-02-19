#pragma once
#include "../Render\ShadowMap.h"
#include "../Render/PostEffect.h"
#include "../Render/Sprite.h"
// レンダリングモード。
enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_Normal,			//通常レンダリング。
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
	/*void SetShadowMap(ShadowMap shadowMap) {
		m_shadowMap = shadowMap;
	}*/
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

	void ShadowDraw();
	void PostEffectDraw();
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

	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。
};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン