#include "Fbx3d.h"

//�ÓI�����o�ϐ��̎���
ID3D12Device* Fbx3d::device = nullptr;
Camera* Fbx3d::camera = nullptr;

void Fbx3d::Initialize()
{
	HRESULT result;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
}
