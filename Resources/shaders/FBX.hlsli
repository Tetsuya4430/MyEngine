cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	//ビュープロジェクション行列
	matrix world;		//ワールド座標
	float3 cameraPos;	//カメラ座標(ワールド座標)
};

cbuffer cbuff1 : register(b1)
{
	//アルベド
	float3 baseColor;
	//金属度
	float metalness;
	//鏡面反射強度
	float specular;
	//粗さ
	float roughness;
}

//頂点バッファの入力
struct VSInput
{
	float4 pos : POSITION;	//位置
	float3 normal : NORMAL;	//頂点座標
	float2 uv : TEXCOORD;	//テクスチャー座標
};

//頂点シェーダからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点構造体
	float3 worldpos :POS;	//ワールド座標
	float3 normal : NORMAL;		//法線
	float2 uv : TEXCOORD;		//uv値
};

//平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

//点光源の数
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
		float3 lightpos;
		float3 lightcolor;
		float3 lightatten;
		uint active;
};

//スポットライトの数
static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float3 lightfactoranglecos;
	uint active;
};

//定数バッファ(ライト情報)
cbuffer cbuff2 : register(b2)
{
	float3 ambientLightColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
}