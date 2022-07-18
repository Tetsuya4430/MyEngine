#include "FBX.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> baseTex : register(t0);
Texture2D<float4> metalnessTex : register(t1);
Texture2D<float4> normalTex : register(t2);
Texture2D<float4> roughnessTex : register(t3);

//�A���x�h
static float3 s_baseColor;
//�����x
static float s_metalness;
//�e��
static float s_roughness;

//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

//��
static const float PI = 3.141592654f;

//���˓_�̖@���x�N�g��
static float3 N;

//UE4��Smith���f��
float GeometricSmith(float cosine)
{
	float k = (s_roughness + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

//�f�B�Y�j�[�̃t���l���v�Z
float3 DisneyFresnel(float LdotH)
{
	//F��(�t���l��)
	//�P�x
	float luminance = 0.3f * s_baseColor.r + 0.6f * s_baseColor.g + 0.1f * s_baseColor.b;
	//�F����
	float3 tintColor = s_baseColor / luminance;
	//������̋��ʔ��˂��v�Z
	float3 nonMetalColor = specular * 0.08f * tintColor;
	//metalness�ɂ��F�␳ �����̏ꍇ�̓x�[�X�J���[
	float3 specularColor = lerp(nonMetalColor, s_baseColor, s_metalness);
	//NdotH�̊�����SchlickFresnel���
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}


float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}


float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	//D��(���z)
	//float Ds = 1.0f;
	float Ds = DistributionGGX(s_roughness * s_roughness, NdotH);

	//F��(�t���l��)
	float3 Fs = DisneyFresnel(LdotH);

	//G��
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	//m��(����)
	float m = 4.0f * NdotL * NdotV;

	//�������ċ��ʔ��˂̐F��h��
	return Ds * Fs * Gs / m;
}

//Schlick�ɂ��ߎ�
//f0��f90�̒l��(1 - cosine)��5���leap����
//f0 : ���������ɓ��˂����Ƃ��̔��˗�
//f90 : �������s�ɓ��˂����Ƃ��̔��˗�
//cosine : 2�x�N�g���̂Ȃ��p�̃R�T�C��
float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}



//�o�������˕��z�֐�
float3 BRDF(float3 L, float3 V)
{
	//�@���ƃ��C�g�����̓���
	float NdotL = dot(N, L);
	//�@���ƃJ���������̓���
	float NdotV = dot(N, L);
	//�ǂ��炩��90�x�ȏ�ł���ΐ^������Ԃ�
	if (NdotL < 0 || NdotV < 0)
	{
		return float3(0, 0, 0);
	}

	//�g�U���˗�
	float diffuseReflectance = 1.0f / PI;

	//���ˊp���̓J�����x�N�g���Ɩ@����90�x�ȏゾ������return
	//���C�g�����ƃJ���������̒���(�n�[�t�x�N�g��)
	float3 H = normalize(L + V);
	//�@���ƃn�[�t�x�N�g���̓���
	float NdotH = dot(N, H);
	//���C�g�ƃn�[�t�x�N�g���̓���
	float LdotH = dot(L, H);

	float energyBias = 0.5f * s_roughness;

	//���ˊp��90�x�̏ꍇ�̊g�U���˗�
	float Fd90 = energyBias + 2 * LdotH * LdotH * s_roughness;

	//�����Ă������̊g�U���˗�
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);

	//�o�Ă����Ƃ��̊g�U���˗�
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);

	float energyFactor = lerp(1.0f, 1.0f / 1.51f, s_roughness);

	//�����Ă����܂ł̊g�U���˗�
	float Fd = FL * FV * energyFactor;

	//�g�U���ˍ�
	float3 diffuseColor = diffuseReflectance * Fd * s_baseColor * (1 - s_metalness);
	
	//���ʔ��ˍ�
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	return diffuseColor + specularColor;
}

//�G���g���[�|�C���g
float4 main(VSOutput input) : SV_TARGET
{
	////�e�N�X�`���}�b�s���O
	//float4 texcolor = tex.Sample(smp, input.uv);

	////Lambert����
	//float3 light = normalize(float3 (1, -1, 1));	//�E���������̃��C�g
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadercolor = float4(brightness, brightness, brightness, 1.0f);

	//�ʂ̏���static�ϐ��ɑ�����A�֐�����Q�Ƃł���悤�ɂ���
	N = input.normal;
	//�p�����[�^���e�N�X�`����RGB����擾
	s_baseColor = baseColor * baseTex.Sample(smp, input.uv).rgb;
	s_metalness = metalness + metalness + metalnessTex.Sample(smp, input.uv).r;
	s_roughness = roughness + roughnessTex.Sample(smp, input.uv).r;
	//�ŏI�I��RGB
	float3 finalRGB = float3(0, 0, 0);
	//���_���王�_�ւ̃x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active)
		{
			continue;
		}
		//BRDF�̌��ʂƃ��C�g�F������
		finalRGB += BRDF(dirLights[i].lightv, eyedir) * dirLights[i].lightcolor;
	}

	return float4(finalRGB, 1);
}
