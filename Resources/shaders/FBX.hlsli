cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	//ビュープロジェクション行列
	matrix world;		//ワールド座標
	float3 cameraPos;	//カメラ座標(ワールド座標)
};

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
	float3 normal : NORMAL;		//法線
	float2 uv : TEXCOORD;		//uv値
};