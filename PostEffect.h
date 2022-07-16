#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
private:
    // Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect();

    /// <summary>
    /// ������
    /// </summary>
    void Initialize();


    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList"></param>
    void Draw(ID3D12GraphicsCommandList* cmdList);


        /// <summary>
        /// �V�[���`�揈��
        /// </summary>
        /// <param name="cmdList"></param>
        void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

        /// <summary>
        /// �V�[���`�揈��
        /// </summary>
        /// <param name="cmdList"></param>
        void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

        /// <summary>
        /// �p�C�v���C������
        /// </summary>
        void CreateGraphicsPipelineState();

public:
    static const float clearColor[4];

private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;

    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;

    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;

    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //�O���t�B�N�X�p�C�v���C��
    ComPtr<ID3D12PipelineState> pipelineState;

    //���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature;
}; 

