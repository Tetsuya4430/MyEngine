#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
private:
    // Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();


    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList"></param>
    void Draw(ID3D12GraphicsCommandList* cmdList);


        /// <summary>
        /// シーン描画処理
        /// </summary>
        /// <param name="cmdList"></param>
        void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

        /// <summary>
        /// シーン描画処理
        /// </summary>
        /// <param name="cmdList"></param>
        void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

        /// <summary>
        /// パイプライン生成
        /// </summary>
        void CreateGraphicsPipelineState();

public:
    static const float clearColor[4];

private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;

    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;

    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;

    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //グラフィクスパイプライン
    ComPtr<ID3D12PipelineState> pipelineState;

    //ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;
}; 

