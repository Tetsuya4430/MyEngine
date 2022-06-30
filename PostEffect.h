#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    // DirectX::���ȗ�
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

    /// <summary>
    /// ������
    /// </summary>
    /// <param name="texNumber"></param>
    /// <param name="anchorpoint"></param>
    /// <param name="isFlipX"></param>
    /// <param name="isFlipY"></param>
    void Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);


    /// <summary>
    /// �X�v���C�g����
    /// </summary>
    /// <param name="texNumber"></param>
    /// <param name="anchorpoint"></param>
    /// <param name="isFlipX"></param>
    /// <param name="isFlipY"></param>
    /// <returns></returns>
    static PostEffect* Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

    /// <summary>
    /// �`��
    /// </summary>
    void Draw();

    public:
    // ���[�g�V�O�l�`��
    static ComPtr<ID3D12RootSignature> rootSignature;
    // �p�C�v���C���X�e�[�g�I�u�W�F�N�g
    static ComPtr<ID3D12PipelineState> pipelineState;

    //�e�N�X�`���ԍ�
    UINT texNumber_ = 0;

    //�A���J�[�|�C���g
    DirectX::XMFLOAT2 anchorpoint_ = { 0.5f, 0.5f };

    //���E���]
    bool isFlipX_ = false;

    //�㉺���]
    bool isFlipY_ = false;
};

