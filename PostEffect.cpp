#include "PostEffect.h"

#include <d3dx12.h>

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12RootSignature> PostEffect::rootSignature;
ComPtr<ID3D12PipelineState> PostEffect::pipelineState;

PostEffect::PostEffect(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
	:Sprite(
		100,	//テクスチャ番号
		{0, 0},	//アンカーポイント
		false,	//左右反転フラグ
		false	//上下反転フラグ
	)
{
}

void PostEffect::Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;


	//メンバ変数に書き込む
	texNumber_ = texNumber;
	anchorpoint_ = anchorpoint;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//頂点データ
	VertexPosUv vertices[] = {
		{{0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},//左下
		{{0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},//左上
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},//右下
		{{100.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},//右上
	};

	//指定番号の画像が読み込み済みなら
	//if (spriteCommon->texBuff_[texNumber_])
	if (spriteCommon->GetTexBuff(texNumber))
	{
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(texNumber)->GetDesc();

		//スプライトの大きさを画像の解像度に合わせる
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//頂点バッファ生成
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	//---頂点バッファへのデータ転送---//
	//VertexPosUv* vertMap = nullptr;
	//result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	//memcpy(vertMap, vertices, sizeof(vertices));
	//vertBuff_->Unmap(0, nullptr);

	TransferVertexBuffer();

	//頂点バッファビューの生成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices);
	vbView_.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの生成
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);

	//定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);		//色指定(RGBA)
	constMap->mat = spriteCommon->GetmatProjection();
	constBuff_->Unmap(0, nullptr);
}

PostEffect* PostEffect::Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//メモリ確保
	PostEffect* instance = new PostEffect(texNumber, anchorpoint, isFlipX, isFlipY);
	//インスタンス初期化
	instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void PostEffect::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//非表示フラグがtrueなら
	if (isInvisible_)
	{
		//描画せずに抜ける
		return;
	}

	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	////パイプラインステートの設定
	//commandList->SetPipelineState(pipelineState.Get());

	////ルートシグネチャ設定
	//commandList->SetGraphicsRootSignature(rootSignature.Get());

	////プリミティブ形状を設定
	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点バッファのセット
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	//ルートパラメータ0番に定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//ルートパラメータ1番にシェーダーリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);

	//ポリゴンの描画
	commandList->DrawInstanced(4, 1, 0, 0);
}
