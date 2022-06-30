#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
private: // エイリアス
// Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    // DirectX::を省略
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="texNumber"></param>
    /// <param name="anchorpoint"></param>
    /// <param name="isFlipX"></param>
    /// <param name="isFlipY"></param>
    void Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);


    /// <summary>
    /// スプライト生成
    /// </summary>
    /// <param name="texNumber"></param>
    /// <param name="anchorpoint"></param>
    /// <param name="isFlipX"></param>
    /// <param name="isFlipY"></param>
    /// <returns></returns>
    static PostEffect* Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    public:
    // ルートシグネチャ
    static ComPtr<ID3D12RootSignature> rootSignature;
    // パイプラインステートオブジェクト
    static ComPtr<ID3D12PipelineState> pipelineState;

    //テクスチャ番号
    UINT texNumber_ = 0;

    //アンカーポイント
    DirectX::XMFLOAT2 anchorpoint_ = { 0.5f, 0.5f };

    //左右反転
    bool isFlipX_ = false;

    //上下反転
    bool isFlipY_ = false;
};

