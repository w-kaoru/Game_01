/*!
 * @brief	モデルシェーダー。
 */

 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
Texture2D<float4> g_albedoTexture : register(t0);	//アルベドテクスチャ。
Texture2D<float4> g_shadowMap : register(t1);		//todo シャドウマップ。
Texture2D<float4> g_normalMap : register(t2);		//	法線マップ。
Texture2D<float4> g_specularMap : register(t3);		//スペキュラマップ。
Texture2D<float4> g_aoMap : register(t4);			//AOマップ。

//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler g_sampler : register(s0);


/// <summary>
/// モデルシェーダー用の定数バッファ。
/// </summary>
cbuffer ModelFxConstantBuffer : register(b0) {
	float4x4 mWorld			: packoffset(c0);
	float4x4 mView			: packoffset(c4);
	float4x4 mProj			: packoffset(c8);
	float4x4 mLightView		: packoffset(c12);	//ライトビュー行列。
	float4x4 mLightProj		: packoffset(c16);	//ライトプロジェクション行列。
	int isShadowReciever : packoffset(c20.x);	//シャドウレシーバーフラグ。
	int isHasNormalMap : packoffset(c20.y);	//法線マップある？
	int isHasSpecularMap : packoffset(c20.z);	//スペキュラマップある？
	int isHasAoMap : packoffset(c20.w);	//AOマップある？
};
static const int NUM_DIRECTION_LIG = 4;

/// <summary>
/// ライト用の定数バッファ。
/// </summary>
cbuffer LightCb : register(b1) {
	float4 dligDirection[NUM_DIRECTION_LIG];
	float4 dligColor[NUM_DIRECTION_LIG];
	float4 ambientLight;		//アンビエントライト。
	float3 eyePos;				//カメラの視点。
};

/// <summary>
/// シャドウマップ用の定数バッファ。
/// </summary>
cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//ライトビュープロジェクション行列。
}

/// <summary>
/// スキンなしモデルの頂点構造体。
/// </summary>
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;		//頂点座標。
	float3 Normal   : NORMAL;			//法線。
	float2 TexCoord	: TEXCOORD0;		//UV座標。
	float3 tangent	: TANGENT;			//接ベクトル。
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

/// <summary>
/// ピクセルシェーダーの入力。
/// </summary>
struct PSInput {
	float4 Position 	: SV_POSITION;	//座標。
	float3 Normal		: NORMAL;		//法線。
	float2 TexCoord		: TEXCOORD0;	//UV座標。
	float4 posInLVP		: TEXCOORD1;	//ライトビュープロジェクション空間での座標。
	float3 tangent		: TEXCOORD2;	//接ベクトル。
	float3 posInWorld	: TEXCOORD3;	//ワールド座標。
	float3 cameraPos	: TEXCOORD4;
};
/// <summary>
/// シャドウマップ用のピクセルシェーダへの入力構造体。
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};

/// <summary>
/// スキンなしモデル用の頂点シェーダー。
/// </summary>
PSInput VSMain(VSInputNmTxVcTangent In)
{
	PSInput psInput = (PSInput)0;
	//ローカル座標系からワールド座標系に変換する。
	float4 worldPos = mul(mWorld, In.Position);
	//鏡面反射のためにワールド座標をピクセルシェーダーに渡す。
	psInput.posInWorld = worldPos.xyz;
	//ワールド座標系からカメラ座標系に変換する。
	psInput.Position = mul(mView, worldPos);
	psInput.cameraPos = psInput.Position.xyz ;
	//カメラ座標系からスクリーン座標系に変換する。
	psInput.Position = mul(mProj, psInput.Position);

	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		psInput.posInLVP = mul(mLightView, worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}

	//UV座標はそのままピクセルシェーダーに渡す。
	psInput.TexCoord = In.TexCoord;
	//法線をワールド行列で変換する。
	//法線がfloat3の3要素のベクトルなので、回転と拡大の変換だけが行われる。
	psInput.Normal = mul(mWorld, In.Normal);
	//拡大成分が入っているかもしれないので、正規化しとこ。
	psInput.Normal = normalize(psInput.Normal);
	//接ベクトルも回す。
	psInput.tangent = mul(mWorld, In.tangent);
	psInput.tangent = normalize(psInput.tangent);


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
	//ローカル座標系からワールド座標系に変換する。
	float4 worldPos = mul(skinning, In.Position);
	//鏡面反射のためにワールド座標をピクセルシェーダーに渡す。
	psInput.posInWorld = worldPos.xyz;
	psInput.Normal = normalize(mul(skinning, In.Normal));
	psInput.tangent = normalize(mul(skinning, In.Tangent));

	worldPos = mul(mView, worldPos);
	worldPos = mul(mProj, worldPos);
	psInput.Position = worldPos;

	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		psInput.posInLVP = mul(mLightView, worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
/// <summary>
/// ラインバート拡散反射を計算する。
/// </summary>
float3 CalcDiffuseLight(float3 normal)
{
	float3 lig = 0.0f;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		//ランバート拡散反射。
		lig += max(0.0f, dot(normal * -1.0f, dligDirection[i])) * dligColor[i];
	}
	return lig;
}
/// <summary>
/// スペキュラライトを計算する。
/// </summary>
float3 CalcSpecularLight(float3 normal, float3 worldPos, float2 uv)
{
	float3 lig = 0.0f;
	if (isHasSpecularMap) {
		float3 specLig = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {

			//ライトを当てる面から視点に伸びるベクトルtoEyeDirを求める。
			//視点の座標は定数バッファで渡されている。LightCbを参照するように。
			float3 toEyeDir = normalize(eyePos - worldPos);
			//求めたtoEyeDirの反射ベクトルを求める。
			float3 reflectEyeDir = -toEyeDir + 2 * dot(normal, toEyeDir) * normal;
			//求めた反射ベクトルとディレクションライトの方向との内積を取って、スペキュラの強さを計算する。
			float t = max(0.0f, dot(-dligDirection[i], reflectEyeDir));
			//pow関数を使って、スペキュラを絞る。絞りの強さは定数バッファで渡されている。
			//LightCbを参照するように。
			float3 specColor = 1.0f;

			//スペキュラマップがある。
			specColor = g_specularMap.Sample(g_sampler, uv).r;

			specLig = pow(t, 2.0f) * specColor * dligColor[i] * 7.0f;

			//スペキュラ反射が求まったら、ligに加算する。
			//鏡面反射を反射光に加算する。
			lig += specLig;
		}
	}
	return lig;
}

/// <summary>
/// デプスシャドウマップ法を使って、影を計算する。。
/// </summary>
//引数にinoutをつけると参照渡しになる。
void CalcShadow(inout float3 lig, float4 posInLvp)
{
	if (isShadowReciever == 1) {	//シャドウレシーバー。
		//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
		float2 shadowMapUV = posInLvp.xy / posInLvp.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//シャドウマップの範囲内かどうかを判定する。
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP空間での深度値を計算。
			float zInLVP = posInLvp.z / posInLvp.w;
			//シャドウマップに書き込まれている深度値を取得。
			float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//影が落ちているので、光を弱くする
				lig *= 0.5f;
			}
		}
	}
}
/// <summary>
/// 法線を計算する。
/// </summary>
float3 CalcNormal(float3 normal, float3 tangent, float2 uv)
{
	float3 worldSpaceNormal;
	if (isHasNormalMap == 1) {
		//法線マップがある。
		//法線と接ベクトルの外積を計算して、従ベクトルを計算する。
		float3 biNormal = cross(normal, tangent);
		float3 tangentSpaceNormal = g_normalMap.Sample(g_sampler, uv);
		//0.0～1.0の範囲になっているタンジェントスペース法線を
		//-1.0～1.0の範囲に変換する。
		tangentSpaceNormal = (tangentSpaceNormal * 2.0f) - 1.0f;
		//法線をタンジェントスペースから、ワールドスペースに変換する。
		worldSpaceNormal = tangent * tangentSpaceNormal.x + biNormal * tangentSpaceNormal.y + normal * tangentSpaceNormal.z;
	}
	else {
		//ない。
		worldSpaceNormal = normal;
	}
	return worldSpaceNormal;
}
float3 CalcAmbientLight(float4 albedoColor, float2 uv)
{
	float3 ao = 1.0f;
	if (isHasAoMap) {
		ao = g_aoMap.Sample(g_sampler, uv).xyz;
	}
	return albedoColor.xyz * ambientLight.xyz * ao;
}

/// <summary>
/// シャドウマップ生成用の頂点シェーダー。
/// </summary>
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
/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}

/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain(PSInput In) : SV_Target0
{
	//albedoテクスチャからカラーをフェッチする。
	float4 albedoColor = g_albedoTexture.Sample(g_sampler, In.TexCoord);

	//法線を計算する。
	float3 normal = CalcNormal(In.Normal, In.tangent, In.TexCoord);

	float3 lig = 0.0f;

	//ディフューズライトを加算。
	lig += CalcDiffuseLight(normal);

	//スペキュラライトを加算。
	lig += CalcSpecularLight(normal, In.posInWorld, In.TexCoord);

	//アンビエントライトを加算。
	lig += CalcAmbientLight(albedoColor, In.TexCoord);

	//デプスシャドウマップを使って影を落とす。。
	CalcShadow(lig, In.posInLVP);
	/*
	//Fog
	//深度値を計算。
	float z = In.Position.z / In.Position.w;
	//フォグの色の設定
	float3 FogCoor = { 0.2f,0.2f,0.2f };
	float3 Fog;
	//線形補間
	//*
	Fog.x = albedoColor.x + (FogCoor.x - albedoColor.x) * z;
	Fog.y = albedoColor.y + (FogCoor.y - albedoColor.y) * z;
	Fog.z = albedoColor.z + (FogCoor.z - albedoColor.z) * z;
	albedoColor.xyz = Fog;
	// */
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}