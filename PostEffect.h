#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    /// <summary>
    /// 描画コマンドリストの発行
    /// </summary>
    /// <param name="cmdList"></param>
    void Draw(ID3D12GraphicsCommandList* cmdList);
};

