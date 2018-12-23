#pragma once
#include "RenderTarget.h"

class SkinModel;
class ShadowMap
{
public:
	// コンストラクタ。
	ShadowMap();

	// デストラクタ。
	~ShadowMap();

	// ライトビュー行列を取得。
	CMatrix GetLighViewMatrix() const
	{
		return m_lightViewMatrix;
	}

	// ライトプロジェクション行列を取得。
	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
	}

	// 更新(ライトカメラの注視点を指定するバージョン。)
	// ライトビュー行列や、ライトプロジェクション行列を
	// 更新します。UpdateFromLightTargetかUpdateFromLightDirectionのどちらかを
	// 毎フレーム呼び出してください。
	void UpdateFromLightTarget(CVector3 lightCameraPos, CVector3 lightCameraTarget);

	// 更新(ライトカメラの向きを指定するバージョン。
	// ライトビュー行列や、ライトプロジェクション行列を
	// 更新します。UpdateFromLightTargetかUpdateFromLightDirectionのどちらかを
	// 毎フレーム呼び出してください。
	void UpdateFromLightDirection(CVector3 lightCameraPos, CVector3 lightDir);

	// シャドウマップにシャドウキャスターをレンダリング。
	// 毎フレーム呼び出して下さい。
	// シャドウマップに描画されるシャドウキャスターは
	// RegistShadowCaster関数を使用して登録されたキャスターのみです。
	// この関数を呼び出すと、シャドウキャスターのリストはクリアされます。
	void RenderToShadowMap();

	void ShadowMapDraw();
	// シャドウキャスターを登録。
	// この関数を使って、登録されたキャスターが
	// シャドウマップに描画されます。
	// 登録されたキャスターのリストはRenderToShadowMapを呼び出したら
	// クリアされるため、常に影を生成したい場合は、
	// 毎フレームこの関数を呼び出す必要があります。
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}

	// シャドウマップのSRVを取得。
	// <returns>シャドウマップのSRV</returns>
	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//ライトカメラの視点。
	CVector3 m_lightCameraTarget = CVector3::Zero();	//ライトカメラの注視点。
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//ライトビュー行列。
	CMatrix m_lightProjMatrix = CMatrix::Identity();	//ライトプロジェクション行列。
	RenderTarget m_shadowMapRT;						//シャドウマップを描画するレンダリングターゲット。
	std::vector<SkinModel*> m_shadowCasters;	//シャドウキャスターの配列。
};
