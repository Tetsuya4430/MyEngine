#pragma once
#include <string>
#include <vector>

#include <DirectXMath.h>
#include <DirectXTex.h>

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//ローカル変換行列
	DirectX::XMMATRIX transForm;
	//グローバル変換行列
	DirectX::XMMATRIX globalTransform;
	//親ノード(ワールド行列を計算するときに使う)
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

public:	//サブクラス
//頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	//メッシュを持つノード
	Node* meshNode = nullptr;

	//頂点データ配列
	std::vector<VertexPosNormalUv> vertices;

	//頂点インデックス
	std::vector<unsigned short> indices;

	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };

	//デフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };

	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};

	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};


private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
};

