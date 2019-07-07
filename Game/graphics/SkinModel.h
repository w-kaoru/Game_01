#pragma once

#include "Skeleton.h"
#include "SkinModelEffect.h"
const int NUM_DIRECTION_LIG = 4;
/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	スキンモデルクラス。
*/
class SkinModel :public Noncopyable
{
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();

	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	モデルを描画。
	*@param[in]	viewMatrix		カメラ行列。
	*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
	*@param[in]	projMatrix		プロジェクション行列。
	*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
	*/
	void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix);
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};


	// マテリアルに対してクエリを行う。
	void QueryMaterials(std::function<void(SkinModelEffect*)> func)
	{
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			func(reinterpret_cast<SkinModelEffect*>(material));
		});
	}
	//シャドウレシーバーのフラグを設定する。
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	//ディレクションライトの初期化。
	void InitLight();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);

	//16の倍数に切り上げる処理を関数化。
	int Raundup16(int n);
private:
	//ディレクションライト（追加）
	struct SLight {
		CVector4 direction[NUM_DIRECTION_LIG];				//ライトの方向。
		CVector4 color[NUM_DIRECTION_LIG];					//ライトのカラー。
		CVector4 ambinetLight;								//アンビエントライト。
		CVector3 eyePos;									//視点の座標。
	};
	//ライトの構造体（追加）
	//struct SLight {
	//	SDirectionLight		directionLight;		//ディレクションライト
	//	CVector3			eyePos;				//視点の座標。
	//	float				specPow;			//鏡面反射の絞り。
	//};
	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//todo ライトビュー行列。
		CMatrix mLightProj;		//todo ライトプロジェクション行列。
		int isShadowReciever;	//シャドウレシーバーのフラグ。
		int isHasNormalMap;		//法線マップを保持している？
		int isHasSpecuraMap;	//スペキュラマップを保持している？
		int isHasAoMap;			//AOマップを保持している？
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;		//!<FBXの上方向。
	ID3D11Buffer*		m_cb = nullptr;						//!<定数バッファ。
	ID3D11Buffer*		m_lightCb = nullptr;				//ライト用の定数バッファ。（追加）
	SLight				m_light;							//ライトの構造体。（追加）
	Skeleton			m_skeleton;							//!<スケルトン。
	CMatrix				m_worldMatrix;						//!<ワールド行列。
	DirectX::Model*		m_modelDx;							//!<DirectXTKが提供するモデルクラス。
	ID3D11SamplerState* m_samplerState = nullptr;			//!<サンプラステート。
	bool m_isShadowReciever = false;						//シャドウレシーバーのフラグ。
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//線マップのSRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;	//スペキュラマップのSRV
	ID3D11ShaderResourceView* m_aoMapSRV = nullptr;			//AOマップ。
	ID3D11ShaderResourceView* m_cubeMap = nullptr;
	ModelEffect* effect = nullptr;
public:
	/// <summary>
	/// ディレクションライトのカラーを設定
	/// </summary>
	/// <param name="ligNo">ライトの番号</param>
	/// <param name="color">ライトのカラー</param>
	void SetDirectionLightColor(int ligNo, CVector4 color)
	{
		if (ligNo < NUM_DIRECTION_LIG) {
			m_light.color[ligNo] = color;
		}
		else {
			std::abort();
		}
	}
	/// <summary>
	/// ディレクションライトの方向を設定。
	/// </summary>
	/// <param name="ligNo">ライトの番号</param>
	/// <param name="dir">ライトの方向</param>
	void SetDirectionLightDirection(int ligNo, CVector4 dir)
	{
		if (ligNo < NUM_DIRECTION_LIG) {
			m_light.direction[ligNo] = dir;
		}
		else {
			std::abort();
		}
	}
	/// <summary>
	/// 環境光の設定。
	/// </summary>
	/// <param name="ambinetLight"></param>
	void SetAmbientLight(CVector3 ambinetLight)
	{
		m_light.ambinetLight = ambinetLight;
	}
	/// <summary>
	/// 法線マップの設定。
	/// </summary>
	void SetNormalMap(ID3D11ShaderResourceView* srv)
	{
		m_normalMapSRV = srv;
	}
	/// <summary>
	/// スペキュラマップを設定。
	/// </summary>
	/// <param name="srv"></param>
	void SetSpecularMap(ID3D11ShaderResourceView* srv)
	{
		m_specularMapSRV = srv;
	}
	/// <summary>
	/// AOマップを設定。
	/// </summary>
	/// <param name="srv"></param>
	void SetAOMap(ID3D11ShaderResourceView* srv)
	{
		m_aoMapSRV = srv;
	}
	void SetCubeMap(ID3D11ShaderResourceView* srv)
	{
		m_cubeMap = srv;
	}
};