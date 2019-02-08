/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	
Texture2D<float4> g_shadowMap : register(t1);		//todo シャドウマップ。
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ライトビュー行列を追加。
	float4x4 mLightView;	//ライトビュー行列。
	float4x4 mLightProj;	//ライトプロジェクション行列。
	int isShadowReciever;	//シャドウレシーバーフラグ。
};
static const int NumDirection = 4;

//ディレクションライト。
struct SDirectionLight {
	float4 direction[NumDirection];
	float4 color[NumDirection];
};
//ライト用の定数バッファ
cbuffer LightCb:register(b1) {
	SDirectionLight		directionLight;		//ディレクションライト。
	//float4		      	ambinetLight;		//環境光
	float3				eyePos;				//カメラの視点。
	float				specPow;			//スペキュラライトの絞り。
};
/// シャドウマップ用の定数バッファ。
//cbuffer ShadowMapCb : register(b1) {
//	float4x4 lightViewProjMatrix;	//ライトビュープロジェクション行列。
//}
/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float4 worldPos		: TEXCOORD1;	//ワールド座標。(追加)
	float4 posInLVP		: TEXCOORD2;	//ライトビュープロジェクション空間での座標。(追加)
};
/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
// シャドウマップ用のピクセルシェーダへの入力構造体。
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	//鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。(追加)
	psInput.worldPos = pos;

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;

	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		///////////////////////////////////////////////
		psInput.posInLVP = mul(mLightView, psInput.worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}

	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin(VSInputNmTxWeights In)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;

	float w = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		//boneMatrixにボーン行列が設定されていて、
		//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
		//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
		skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
		w += In.Weights[i];
	}
	//最後のボーンを計算する。
	skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
	//mulは乗算命令。
	pos = mul(skinning, In.Position);
	//鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。(追加)
	psInput.worldPos = pos;
	psInput.Normal = normalize(mul(skinning, In.Normal));
	psInput.Tangent = normalize(mul(skinning, In.Tangent));

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;

	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		psInput.posInLVP = mul(mLightView, psInput.worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	//albedoテクスチャからカラーをフェッチする。
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	//ディレクションライトの拡散反射光を計算する。
	float4 lig = 0.0f;
	for (int i = 0; i < NumDirection; i++) {
		lig += max(0.5f, dot(In.Normal * -1.0f, directionLight.direction[i])) * directionLight.color[i];
		//ライトを当てるピクセルのワールド座標から視点に伸びるベクトルtoEyeDirを求める。
		// 視点の座標は定数バッファで渡されている。LightCbを参照するように。
		 /*
		{
			//スペキュラライトの処理
			float3 toEyeDir = In.worldPos - eyePos;
			toEyeDir = normalize(toEyeDir);
			//求めたtoEyeDirの反射ベクトルを求める。
			float3 rVec = reflect(toEyeDir, In.Normal);
			//求めた反射ベクトルとディレクションライトの方向との内積を取って、スペキュラの強さを計算する。
			float t = max(0.0f, dot(-directionLight.direction[i], rVec));
			//pow関数を使って、スペキュラを絞る。絞りの強さは定数バッファで渡されている。
			float4 specLig = pow(t, specPow) * directionLight.color[i];
			//スペキュラ反射が求まったら、ligに加算する。
			//鏡面反射を反射光に加算する。
			lig.xyz += specLig * 1.0f;
		}
		// */
		if (isShadowReciever == 1) {	//シャドウレシーバー。
			//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
			float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//シャドウマップの範囲内かどうかを判定する。
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {

				///LVP空間での深度値を計算。
				float zInLVP = In.posInLVP.z / In.posInLVP.w;
				//シャドウマップに書き込まれている深度値を取得。
				float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

				if (zInLVP > zInShadowMap + 0.01f) {
					//影が落ちているので、光を弱くする
					lig *= 0.5f;
				}
			}
		}
	}

	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
 //シャドウマップ生成用の頂点シェーダー。
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
//シャドウマップ生成用の頂点シェーダー。
//スキンあり
PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;

	float w = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		//boneMatrixにボーン行列が設定されていて、
		//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
		//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
		skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
		w += In.Weights[i];
	}
	//最後のボーンを計算する。
	skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
	//mulは乗算命令。
	pos = mul(skinning, In.Position);

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
// ピクセルシェーダーのエントリ関数。
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}

