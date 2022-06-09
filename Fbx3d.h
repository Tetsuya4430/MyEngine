#pragma once
#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include "DirectXCommon.h"


class Fbx3d
{
protected:	//�G�C���A�X
	//MIcrosoft::WRL���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�萔
	//�{�[���ő吔
	static const int MAX_BONES = 32;

public:	//�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
	};

	//�萔�o�b�t�@�p�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//�ÓI�����o�֐�
	//setter
	static void SetDevice(ID3D12Device* device) { Fbx3d::device = device; }
	static void SetCamera(Camera* camera) { Fbx3d::camera = camera; }

	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;

	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public:	//�����o�֐�
	//������
	void Initialize();

	//���t���[������
	void Update();

	//���f���̃Z�b�g
	void SetModel(FbxModel* fbxmodel) { this->fbxmodel = fbxmodel; }

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�A�j���[�V�����Đ�
	void PlayAnimation();

protected://�����o�ϐ�
	ComPtr<ID3D12Resource> constBuffTransform;
	ComPtr<ID3D12Resource> constBuffSkin;

	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1, 1, 1 };
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���[�J�����W
	XMFLOAT3 position = { 0, 0, 0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	FbxModel* fbxmodel = nullptr;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
};

