#include "PostEffect.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() : Sprite(
	100,	//�e�N�X�`��
	{ 0, 0 },	//���W
	{ 500.0f, 500.0f },	//�T�C�Y
	{ 1, 1, 1, 1 },		//�F
	{ 0.0f, 0.0f },		//�A���J�[�|�C���g
	false,	//���E���]�t���O
	false)	//�㉺���]�t���O
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//��\���t���O��true�Ȃ�
	if (isInvisible_)
	{
		//�`�悹���ɔ�����
		return;
	}

	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	//���_�o�b�t�@�̃Z�b�g
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//���[�g�p�����[�^1�ԂɃV�F�[�_�[���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);

	//�|���S���̕`��
	commandList->DrawInstanced(4, 1, 0, 0);

	//���[���h�s��̍X�V

	//�萔�o�b�t�@

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e��O���t�B�b�N�X�R�}���h
}
