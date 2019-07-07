#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	switch (m_renderMode) {
	case enRenderMode_Normal:{
		deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
		//todo シェーダーリソースビューを一気に設定する。
		ID3D11ShaderResourceView* srvArray[] = {
			m_albedoTex,							//アルベドテクスチャ。
			g_graphicsEngine->GetShadowMap()->GetShadowMapSRV()//シャドウマップ。
		};
		deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &srvArray[0]);
		deviceContext->PSSetShaderResources(1, 1, &srvArray[1]);
		}break;
	case enRenderMode_CreateShadowMap:
		//シャドウマップ生成。
		deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	case enRenderMode_CubeMap:
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psCubeMap.GetBody(), NULL, 0);
		deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
	}
}