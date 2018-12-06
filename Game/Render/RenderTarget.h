#pragma once
class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	// レンダリングターゲットの解放。
	void Release();

	// レンダリングターゲットの作成。
	void Create(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

	// レンダリングターゲットとデプスステンシルのクリア。
	void ClearRenderTarget(float* clearColor);

	// レンダリングターゲットビューを取得。
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_renderTargetView;
	}

	// デプスステンシルビューを取得。
	ID3D11DepthStencilView* GetDepthStensilView()
	{
		return m_depthStencilView;
	}
	// レンダリングターゲットのSRVを取得。
	ID3D11ShaderResourceView* GetRenderTargetSRV()
	{
		return m_renderTargetSRV;
	}
	// ビューポートの取得。
	D3D11_VIEWPORT* GetViewport()
	{
		return &m_viewport;
	}
private:
	ID3D11Texture2D *			m_renderTargetTex = nullptr;		//レンダリングターゲットとなるテクスチャ。
	ID3D11RenderTargetView*		m_renderTargetView = nullptr;		//レンダーターゲットビュー。
	ID3D11ShaderResourceView*	m_renderTargetSRV = nullptr;		//レンダリングターゲットのSRV
	ID3D11Texture2D*			m_depthStencilTex = nullptr;		//デプスステンシルとなるテクスチャ。
	ID3D11DepthStencilView*		m_depthStencilView = nullptr;		//デプスステンシルビュー。
	D3D11_VIEWPORT				m_viewport;							//ビューポート。
};

