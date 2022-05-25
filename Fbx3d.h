#pragma once
#include "Model.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>


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

public:	//サブクラス
	//定数バッファ用データ構造体(座標変換行列)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標(ワールド座標)
	};

public:	//静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { Fbx3d::device = device; }
	static void SetCamera(Camera* camera) { Fbx3d::camera = camera; }

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

public:	//メンバ関数
	//初期化
	void Initialize();

protected://メンバ変数
	ComPtr<ID3D12Resource> constBuffTransform;
};

