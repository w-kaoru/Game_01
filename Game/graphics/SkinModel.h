#pragma once

#include "Skeleton.h"
#include "SkinModelEffect.h"
const int NUM_DIRECTION_LIG = 4;
/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel :public Noncopyable
{
public:
	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();

	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	���f����`��B
	*@param[in]	viewMatrix		�J�����s��B
	*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
	*@param[in]	projMatrix		�v���W�F�N�V�����s��B
	*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
	*/
	void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix);
	/*!
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
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
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};


	// �}�e���A���ɑ΂��ăN�G�����s���B
	void QueryMaterials(std::function<void(SkinModelEffect*)> func)
	{
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			func(reinterpret_cast<SkinModelEffect*>(material));
		});
	}
	//�V���h�E���V�[�o�[�̃t���O��ݒ肷��B
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
private:
	/*!
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	//�f�B���N�V�������C�g�̏������B
	void InitLight();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);

	//16�̔{���ɐ؂�グ�鏈�����֐����B
	int Raundup16(int n);
private:
	//�f�B���N�V�������C�g�i�ǉ��j
	struct SLight {
		CVector4 direction[NUM_DIRECTION_LIG];				//���C�g�̕����B
		CVector4 color[NUM_DIRECTION_LIG];					//���C�g�̃J���[�B
		CVector4 ambinetLight;								//�A���r�G���g���C�g�B
		CVector3 eyePos;									//���_�̍��W�B
	};
	//���C�g�̍\���́i�ǉ��j
	//struct SLight {
	//	SDirectionLight		directionLight;		//�f�B���N�V�������C�g
	//	CVector3			eyePos;				//���_�̍��W�B
	//	float				specPow;			//���ʔ��˂̍i��B
	//};
	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//todo ���C�g�r���[�s��B
		CMatrix mLightProj;		//todo ���C�g�v���W�F�N�V�����s��B
		int isShadowReciever;	//�V���h�E���V�[�o�[�̃t���O�B
		int isHasNormalMap;		//�@���}�b�v��ێ����Ă���H
		int isHasSpecuraMap;	//�X�y�L�����}�b�v��ێ����Ă���H
		int isHasAoMap;			//AO�}�b�v��ێ����Ă���H
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;		//!<FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;						//!<�萔�o�b�t�@�B
	ID3D11Buffer*		m_lightCb = nullptr;				//���C�g�p�̒萔�o�b�t�@�B�i�ǉ��j
	SLight				m_light;							//���C�g�̍\���́B�i�ǉ��j
	Skeleton			m_skeleton;							//!<�X�P���g���B
	CMatrix				m_worldMatrix;						//!<���[���h�s��B
	DirectX::Model*		m_modelDx;							//!<DirectXTK���񋟂��郂�f���N���X�B
	ID3D11SamplerState* m_samplerState = nullptr;			//!<�T���v���X�e�[�g�B
	bool m_isShadowReciever = false;						//�V���h�E���V�[�o�[�̃t���O�B
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//���}�b�v��SRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;	//�X�y�L�����}�b�v��SRV
	ID3D11ShaderResourceView* m_aoMapSRV = nullptr;			//AO�}�b�v�B
	ID3D11ShaderResourceView* m_cubeMap = nullptr;
	ModelEffect* effect = nullptr;
public:
	/// <summary>
	/// �f�B���N�V�������C�g�̃J���[��ݒ�
	/// </summary>
	/// <param name="ligNo">���C�g�̔ԍ�</param>
	/// <param name="color">���C�g�̃J���[</param>
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
	/// �f�B���N�V�������C�g�̕�����ݒ�B
	/// </summary>
	/// <param name="ligNo">���C�g�̔ԍ�</param>
	/// <param name="dir">���C�g�̕���</param>
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
	/// �����̐ݒ�B
	/// </summary>
	/// <param name="ambinetLight"></param>
	void SetAmbientLight(CVector3 ambinetLight)
	{
		m_light.ambinetLight = ambinetLight;
	}
	/// <summary>
	/// �@���}�b�v�̐ݒ�B
	/// </summary>
	void SetNormalMap(ID3D11ShaderResourceView* srv)
	{
		m_normalMapSRV = srv;
	}
	/// <summary>
	/// �X�y�L�����}�b�v��ݒ�B
	/// </summary>
	/// <param name="srv"></param>
	void SetSpecularMap(ID3D11ShaderResourceView* srv)
	{
		m_specularMapSRV = srv;
	}
	/// <summary>
	/// AO�}�b�v��ݒ�B
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