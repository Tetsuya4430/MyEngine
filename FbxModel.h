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
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//���[�J���ϊ��s��
	DirectX::XMMATRIX transForm;
	//�O���[�o���ϊ��s��
	DirectX::XMMATRIX globalTransform /*= DirectX::XMMatrixIdentity()*/;
	//�e�m�[�h(���[���h�s����v�Z����Ƃ��Ɏg��)
	Node* parent = nullptr;
};

struct TextureData
{
	//�e�N�X�`���f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//SRV��GPU�n���h��
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class FbxModel
{
public:
	//�t�����h�N���X
	friend class FbxLoader;

private:
	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	//std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;



public:	//�T�u�N���X
//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	struct ConstBufferDataMaterial
	{
		//�A���x�h
		DirectX::XMFLOAT3 baseColor;
		//�����x
		float metalness;
		//���ʔ��ˋ��x
		float specular;
		//�e��
		float roughness;
		//�p�f�B���O(16Byte���E)
		float pad[2];
	};


public:	//�萔
	//�e�N�X�`���̍ő吔
	static const int MAX_TEXTURES = 4;

	//�x�[�X�e�N�X�`��
	TextureData baseTexture;
	//���^���l�X�e�N�X�`��
	TextureData metalnessTexture;
	//�@���e�N�X�`��
	TextureData normalTexture;
	//���t�l�X�e�N�X�`��
	TextureData roughnessTexture;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;

	//���_�C���f�b�N�X
	std::vector<unsigned short> indices;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };

	//�f�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };

	////�e�N�X�`�����^�f�[�^
	//DirectX::TexMetadata metadata = {};

	////�X�N���b�`�C���[�W
	//DirectX::ScratchImage scratchImg = {};

public:
	//�����o�֐�
	void CreateBuffers(ID3D12Device* device);

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���̕ό`�s��擾
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

	//�}�e���A���p�����[�^�]��
	void TransferMaterial();

	void CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex);

private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;

//�����o�ϐ�
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	////�e�N�X�`���o�b�t�@
	//ComPtr<ID3D12Resource> texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//�A���x�h
	DirectX::XMFLOAT3 baseColor = { 1, 1, 1 };

	//�����x
	float metalness = 0.0f;

	//���ʔ��ˋ��x
	float specular = 0.5f;

	//�e��
	float roughness = 0.0f;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffMaterial;
};

