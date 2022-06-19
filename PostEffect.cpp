#include "PostEffect.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() : Sprite(
	100,	//テクスチャ
	{ 0, 0 },	//座標
	{ 500.0f, 500.0f },	//サイズ
	{ 1, 1, 1, 1 },		//色
	{ 0.0f, 0.0f },		//アンカーポイント
	false,	//左右反転フラグ
	false)	//上下反転フラグ
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//非表示フラグがtrueなら
	if (isInvisible_)
	{
		//描画せずに抜ける
		return;
	}

	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	//頂点バッファのセット
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	//ルートパラメータ0番に定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//ルートパラメータ1番にシェーダーリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);

	//ポリゴンの描画
	commandList->DrawInstanced(4, 1, 0, 0);

	//ワールド行列の更新

	//定数バッファ

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//各種グラフィックスコマンド
}
