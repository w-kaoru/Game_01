/*!
 * @brief	���f���V�F�[�_�[�B
 */

 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
Texture2D<float4> g_albedoTexture : register(t0);	//�A���x�h�e�N�X�`���B
Texture2D<float4> g_shadowMap : register(t1);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_normalMap : register(t2);		//	�@���}�b�v�B
Texture2D<float4> g_specularMap : register(t3);		//�X�y�L�����}�b�v�B
Texture2D<float4> g_aoMap : register(t4);			//AO�}�b�v�B

//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler g_sampler : register(s0);


/// <summary>
/// ���f���V�F�[�_�[�p�̒萔�o�b�t�@�B
/// </summary>
cbuffer ModelFxConstantBuffer : register(b0) {
	float4x4 mWorld			: packoffset(c0);
	float4x4 mView			: packoffset(c4);
	float4x4 mProj			: packoffset(c8);
	float4x4 mLightView		: packoffset(c12);	//���C�g�r���[�s��B
	float4x4 mLightProj		: packoffset(c16);	//���C�g�v���W�F�N�V�����s��B
	int isShadowReciever : packoffset(c20.x);	//�V���h�E���V�[�o�[�t���O�B
	int isHasNormalMap : packoffset(c20.y);	//�@���}�b�v����H
	int isHasSpecularMap : packoffset(c20.z);	//�X�y�L�����}�b�v����H
	int isHasAoMap : packoffset(c20.w);	//AO�}�b�v����H
};
static const int NUM_DIRECTION_LIG = 4;

/// <summary>
/// ���C�g�p�̒萔�o�b�t�@�B
/// </summary>
cbuffer LightCb : register(b1) {
	float4 dligDirection[NUM_DIRECTION_LIG];
	float4 dligColor[NUM_DIRECTION_LIG];
	float4 ambientLight;		//�A���r�G���g���C�g�B
	float3 eyePos;				//�J�����̎��_�B
};

/// <summary>
/// �V���h�E�}�b�v�p�̒萔�o�b�t�@�B
/// </summary>
cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//���C�g�r���[�v���W�F�N�V�����s��B
}

/// <summary>
/// �X�L���Ȃ����f���̒��_�\���́B
/// </summary>
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;		//���_���W�B
	float3 Normal   : NORMAL;			//�@���B
	float2 TexCoord	: TEXCOORD0;		//UV���W�B
	float3 tangent	: TANGENT;			//�ڃx�N�g���B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/// <summary>
/// �s�N�Z���V�F�[�_�[�̓��́B
/// </summary>
struct PSInput {
	float4 Position 	: SV_POSITION;	//���W�B
	float3 Normal		: NORMAL;		//�@���B
	float2 TexCoord		: TEXCOORD0;	//UV���W�B
	float4 posInLVP		: TEXCOORD1;	//���C�g�r���[�v���W�F�N�V������Ԃł̍��W�B
	float3 tangent		: TEXCOORD2;	//�ڃx�N�g���B
	float3 posInWorld	: TEXCOORD3;	//���[���h���W�B
	float3 cameraPos	: TEXCOORD4;
};
/// <summary>
/// �V���h�E�}�b�v�p�̃s�N�Z���V�F�[�_�ւ̓��͍\���́B
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//���W�B
};

/// <summary>
/// �X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput VSMain(VSInputNmTxVcTangent In)
{
	PSInput psInput = (PSInput)0;
	//���[�J�����W�n���烏�[���h���W�n�ɕϊ�����B
	float4 worldPos = mul(mWorld, In.Position);
	//���ʔ��˂̂��߂Ƀ��[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
	psInput.posInWorld = worldPos.xyz;
	//���[���h���W�n����J�������W�n�ɕϊ�����B
	psInput.Position = mul(mView, worldPos);
	psInput.cameraPos = psInput.Position.xyz ;
	//�J�������W�n����X�N���[�����W�n�ɕϊ�����B
	psInput.Position = mul(mProj, psInput.Position);

	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		psInput.posInLVP = mul(mLightView, worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}

	//UV���W�͂��̂܂܃s�N�Z���V�F�[�_�[�ɓn���B
	psInput.TexCoord = In.TexCoord;
	//�@�������[���h�s��ŕϊ�����B
	//�@����float3��3�v�f�̃x�N�g���Ȃ̂ŁA��]�Ɗg��̕ϊ��������s����B
	psInput.Normal = mul(mWorld, In.Normal);
	//�g�听���������Ă��邩������Ȃ��̂ŁA���K�����Ƃ��B
	psInput.Normal = normalize(psInput.Normal);
	//�ڃx�N�g�����񂷁B
	psInput.tangent = mul(mWorld, In.tangent);
	psInput.tangent = normalize(psInput.tangent);


	return psInput;
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin(VSInputNmTxWeights In)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;

	float w = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
		//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
		//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
		skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
		w += In.Weights[i];
	}
	//�Ō�̃{�[�����v�Z����B
	skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
	//mul�͏�Z���߁B
	//���[�J�����W�n���烏�[���h���W�n�ɕϊ�����B
	float4 worldPos = mul(skinning, In.Position);
	//���ʔ��˂̂��߂Ƀ��[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
	psInput.posInWorld = worldPos.xyz;
	psInput.Normal = normalize(mul(skinning, In.Normal));
	psInput.tangent = normalize(mul(skinning, In.Tangent));

	worldPos = mul(mView, worldPos);
	worldPos = mul(mProj, worldPos);
	psInput.Position = worldPos;

	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		psInput.posInLVP = mul(mLightView, worldPos);
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
/// <summary>
/// ���C���o�[�g�g�U���˂��v�Z����B
/// </summary>
float3 CalcDiffuseLight(float3 normal)
{
	float3 lig = 0.0f;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		//�����o�[�g�g�U���ˁB
		lig += max(0.0f, dot(normal * -1.0f, dligDirection[i])) * dligColor[i];
	}
	return lig;
}
/// <summary>
/// �X�y�L�������C�g���v�Z����B
/// </summary>
float3 CalcSpecularLight(float3 normal, float3 worldPos, float2 uv)
{
	float3 lig = 0.0f;
	if (isHasSpecularMap) {
		float3 specLig = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {

			//���C�g�𓖂Ă�ʂ��王�_�ɐL�т�x�N�g��toEyeDir�����߂�B
			//���_�̍��W�͒萔�o�b�t�@�œn����Ă���BLightCb���Q�Ƃ���悤�ɁB
			float3 toEyeDir = normalize(eyePos - worldPos);
			//���߂�toEyeDir�̔��˃x�N�g�������߂�B
			float3 reflectEyeDir = -toEyeDir + 2 * dot(normal, toEyeDir) * normal;
			//���߂����˃x�N�g���ƃf�B���N�V�������C�g�̕����Ƃ̓��ς�����āA�X�y�L�����̋������v�Z����B
			float t = max(0.0f, dot(-dligDirection[i], reflectEyeDir));
			//pow�֐����g���āA�X�y�L�������i��B�i��̋����͒萔�o�b�t�@�œn����Ă���B
			//LightCb���Q�Ƃ���悤�ɁB
			float3 specColor = 1.0f;

			//�X�y�L�����}�b�v������B
			specColor = g_specularMap.Sample(g_sampler, uv).r;

			specLig = pow(t, 2.0f) * specColor * dligColor[i] * 7.0f;

			//�X�y�L�������˂����܂�����Alig�ɉ��Z����B
			//���ʔ��˂𔽎ˌ��ɉ��Z����B
			lig += specLig;
		}
	}
	return lig;
}

/// <summary>
/// �f�v�X�V���h�E�}�b�v�@���g���āA�e���v�Z����B�B
/// </summary>
//������inout������ƎQ�Ɠn���ɂȂ�B
void CalcShadow(inout float3 lig, float4 posInLvp)
{
	if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
		//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
		float2 shadowMapUV = posInLvp.xy / posInLvp.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP��Ԃł̐[�x�l���v�Z�B
			float zInLVP = posInLvp.z / posInLvp.w;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
			float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//�e�������Ă���̂ŁA�����キ����
				lig *= 0.5f;
			}
		}
	}
}
/// <summary>
/// �@�����v�Z����B
/// </summary>
float3 CalcNormal(float3 normal, float3 tangent, float2 uv)
{
	float3 worldSpaceNormal;
	if (isHasNormalMap == 1) {
		//�@���}�b�v������B
		//�@���Ɛڃx�N�g���̊O�ς��v�Z���āA�]�x�N�g�����v�Z����B
		float3 biNormal = cross(normal, tangent);
		float3 tangentSpaceNormal = g_normalMap.Sample(g_sampler, uv);
		//0.0�`1.0�͈̔͂ɂȂ��Ă���^���W�F���g�X�y�[�X�@����
		//-1.0�`1.0�͈̔͂ɕϊ�����B
		tangentSpaceNormal = (tangentSpaceNormal * 2.0f) - 1.0f;
		//�@�����^���W�F���g�X�y�[�X����A���[���h�X�y�[�X�ɕϊ�����B
		worldSpaceNormal = tangent * tangentSpaceNormal.x + biNormal * tangentSpaceNormal.y + normal * tangentSpaceNormal.z;
	}
	else {
		//�Ȃ��B
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
/// �V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
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
//�V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
//�X�L������
PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;

	float w = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
		//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
		//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
		skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
		w += In.Weights[i];
	}
	//�Ō�̃{�[�����v�Z����B
	skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
	//mul�͏�Z���߁B
	pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z / In.Position.w;
}

/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain(PSInput In) : SV_Target0
{
	//albedo�e�N�X�`������J���[���t�F�b�`����B
	float4 albedoColor = g_albedoTexture.Sample(g_sampler, In.TexCoord);

	//�@�����v�Z����B
	float3 normal = CalcNormal(In.Normal, In.tangent, In.TexCoord);

	float3 lig = 0.0f;

	//�f�B�t���[�Y���C�g�����Z�B
	lig += CalcDiffuseLight(normal);

	//�X�y�L�������C�g�����Z�B
	lig += CalcSpecularLight(normal, In.posInWorld, In.TexCoord);

	//�A���r�G���g���C�g�����Z�B
	lig += CalcAmbientLight(albedoColor, In.TexCoord);

	//�f�v�X�V���h�E�}�b�v���g���ĉe�𗎂Ƃ��B�B
	CalcShadow(lig, In.posInLVP);
	/*
	//Fog
	//�[�x�l���v�Z�B
	float z = In.Position.z / In.Position.w;
	//�t�H�O�̐F�̐ݒ�
	float3 FogCoor = { 0.2f,0.2f,0.2f };
	float3 Fog;
	//���`���
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