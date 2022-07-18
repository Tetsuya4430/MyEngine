#pragma once
#include <string>
#include <vector>

#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

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
	DirectX::XMMATRIX globalTransform /*= DirectX::XMMatrixIdentity()*/;
	//親ノード(ワールド行列を計算するときに使う)
	Node* parent = nullptr;
};

struct TextureData
{
	//テクスチャデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//SRVのGPUハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

private:
	//エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	//std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;



public:	//サブクラス
//頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	struct ConstBufferDataMaterial
	{
		//アルベド
		DirectX::XMFLOAT3 baseColor;
		//金属度
		float metalness;
		//鏡面反射強度
		float specular;
		//粗さ
		float roughness;
		//パディング(16Byte境界)
		float pad[2];
	};


public:	//定数
	//テクスチャの最大数
	static const int MAX_TEXTURES = 4;

	//ベーステクスチャ
	TextureData baseTexture;
	//メタルネステクスチャ
	TextureData metalnessTexture;
	//法線テクスチャ
	TextureData normalTexture;
	//ラフネステクスチャ
	TextureData roughnessTexture;

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

	////テクスチャメタデータ
	//DirectX::TexMetadata metadata = {};

	////スクラッチイメージ
	//DirectX::ScratchImage scratchImg = {};

public:
	//メンバ関数
	void CreateBuffers(ID3D12Device* device);

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルの変形行列取得
	const XMMATRIX& GetModeTransform()
	{
		return meshNode->globalTransform;
	}

	//getter
	const DirectX::XMFLOAT3& GetBaseColor() { return baseColor; }

	float GetMetalness() { return metalness; }
	float GetSpecular() { return specular; }
	float GetRoughness() { return roughness; }

	//setter
	void SetBaseColor(const DirectX::XMFLOAT3& baseColor)
	{
		this->baseColor = baseColor;
	}

	void SetMetalness(float metalness)
	{
		this->metalness = metalness;
	}

	void SetSpecular(float specular)
	{
		this->specular = specular;
	}

	void SetRoughness(float roughness)
	{
		this->roughness = roughness;
	}

	//マテリアルパラメータ転送
	void TransferMaterial();

	void CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex);

private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;

//メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	////テクスチャバッファ
	//ComPtr<ID3D12Resource> texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//アルベド
	DirectX::XMFLOAT3 baseColor = { 1, 1, 1 };

	//金属度
	float metalness = 0.0f;

	//鏡面反射強度
	float specular = 0.5f;

	//粗さ
	float roughness = 0.0f;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffMaterial;
};

