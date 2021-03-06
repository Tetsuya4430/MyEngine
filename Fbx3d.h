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
protected:	//エイリアス
	//MIcrosoft::WRLを省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//定数
	//ボーン最大数
	static const int MAX_BONES = 32;

public:	//サブクラス
	//定数バッファ用データ構造体(座標変換行列)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標(ワールド座標)
	};

	//定数バッファ用構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { Fbx3d::device = device; }
	static void SetCamera(Camera* camera) { Fbx3d::camera = camera; }

	//グラフィックスパイプラインの生成
	static void CreateGraphicsPipeline();

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public:	//メンバ関数
	//初期化
	void Initialize();

	//毎フレーム処理
	void Update();

	//モデルのセット
	void SetModel(FbxModel* fbxmodel) { this->fbxmodel = fbxmodel; }

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//アニメーション再生
	void PlayAnimation();


	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

protected://メンバ変数
	ComPtr<ID3D12Resource> constBuffTransform;
	ComPtr<ID3D12Resource> constBuffSkin;

	//ローカルスケール
	XMFLOAT3 scale = { 1, 1, 1 };
	//X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	//ローカル座標
	XMFLOAT3 position = { 0, 0, 0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	FbxModel* fbxmodel = nullptr;

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};

