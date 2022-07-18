#pragma once

#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Fbx3d.h"
#include "Input.h"
#include "ObjectManager.h"
#include "LightGroup.h"

#include <memory>

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// コンストクラタ
	GamePlayScene();

	// デストラクタ
	~GamePlayScene();

	//初期化
	void Initialize(/*DirectXCommon* dxCommon*/) override;

	//終了処理
	void Finalize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;


private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite* sprite = nullptr;
	Model* model_1 = nullptr;
	Model* model_2 = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	Camera* camera = nullptr;
	//ライト
	LightGroup* lightGroup = nullptr;

	std::shared_ptr<Object3d> object3d_1;
	std::shared_ptr<Object3d> object3d_2;
	std::shared_ptr<Object3d> object3d_3;
	std::unique_ptr<ObjectManager> objectManager_1;
	std::unique_ptr<ObjectManager> objectManager_2;
	std::unique_ptr<ObjectManager> objectManager_3;

	float pointLightPos[3] = { 0, 0, 0 };
	float pointLightColor[3] = { 1, 1, 1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 5, 0 };
	float spotLightColor[3] = { 1, 1, 1 };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightFactorAngle[2] = { 10.0f, 30.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.5f, 0.5f };

	float fighterPos[3] = { 1, 0.0f, 0 };

};

