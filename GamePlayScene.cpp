#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"


GamePlayScene::GamePlayScene()
{
}

GamePlayScene::~GamePlayScene()
{
	delete(object1);
	delete(model1);
}

void GamePlayScene::Initialize()
{ 
	////スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");

	//	スプライトの生成
	sprite = Sprite::Create(1, { 0, 0 }, false, false);

	//OBJからモデルデータを読み込む
	model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("triangle_mat");

	//オブジェクトマネージャー生成
	objectManager_1 = std::make_unique<ObjectManager>();
	objectManager_2 = std::make_unique<ObjectManager>();
	objectManager_3 = std::make_unique<ObjectManager>();

	//オブジェクトマネージャー生成->3Dオブジェクト生成->オブジェクトの参照カウントを増加
	std::shared_ptr<Object3d> object_1 = Object3d::Create(model_1, camera);
	std::weak_ptr<Object3d> ObjectWp_1 = objectManager_1->AddObject(object_1);
	object3d_1 = ObjectWp_1.lock();

	std::shared_ptr<Object3d> object_2 = Object3d::Create(model_2, camera);
	std::weak_ptr<Object3d> ObjectWp_2 = objectManager_2->AddObject(object_2);
	object3d_2 = ObjectWp_2.lock();

	std::shared_ptr<Object3d> object_3 = Object3d::Create(model_2, camera);
	std::weak_ptr<Object3d> ObjectWp_3 = objectManager_3->AddObject(object_3);
	object3d_3 = ObjectWp_3.lock();

	//3Dオブジェクトの位置と拡大率を指定
	
		object3d_1->SetScale({ 20.0f, 20.0f, 20.0f });

		object3d_2->SetPosition({ -15, 0, -15 });
		object3d_2->SetScale({ 21.0f, 21.0f, 21.0f });
	
		object3d_3->SetPosition({ +15, 0, +15 });
		object3d_3->SetScale({ 21.0f, 21.0f, 21.0f });
	
	
		//デバイスをセット
		Fbx3d::SetDevice(dxCommon->GetDev());
		//カメラセット
		Fbx3d::SetCamera(camera);
		//グラフィックスパイプラインを生成
		Fbx3d::CreateGraphicsPipeline();

		//モデルを指定してFBXファイルを読み込み
		//FbxLoader::GetInstance()->LoadModelFromFile("cube");
		model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

		//3dオブジェクト生成とモデルのセット
		object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);



		//カメラ注視点をセット
		camera->SetTarget({ 0, 20, 0 });

	//音声読み込みと再生
	Audio::GetInstance()->LoadWave("Alarm01.wav");
	/*Audio::GetInstance()->PlayWave("Alarm01.wav");*/
}

void GamePlayScene::Finalize()
{
	//スプライト解放
	delete sprite;

	//モデルの解放
	delete model_1;
	delete model_2;
}

void GamePlayScene::Update()
{
	//---デバッグテキスト関係---//
	//X座標、Y座標を指定して表示
	DebugText::GetInstance()->Print("Debug Text", 0, 0);

	//X座標、Y座標、縮尺を指定して表示
	DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if (Input::GetInstance()->TriggerKey(DIK_A))
	{
		Audio::GetInstance()->PlayWave("Alarm01.wav");
	}

	//座標操作
	if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
	{
			DirectX::XMFLOAT3 pos = object3d_1->GetPosition();

			//pos.x += 1.0f;

			object3d_1->SetPosition(pos);

			if (Input::GetInstance()->PushKey(DIK_DOWN))
			{
				camera->CameraMoveVector({ 0, 0, -1.0f });
			}

			if (Input::GetInstance()->PushKey(DIK_UP))
			{
				camera->CameraMoveVector({ 0, 0, +1.0f });
			}

			if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				camera->CameraMoveVector({ 0, +1.0f, 0 });
			}
	}


	//3Dオブジェクトの更新
	objectManager_1->Update();
	objectManager_2->Update();
	objectManager_3->Update();

	//FBXオブジェクトの更新
	object1->Update();
	
	//スプライトの更新
	sprite->Update();

	//カメラの更新
	camera->Update();


	//Escキーでウィンドウを閉じる
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESCキーでウィンドウを閉じる
	{
		//endRequest_ = true;
		return;
	}
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();

	////スプライト描画
	sprite->Draw();

	//3Dオブジェクトの描画前処理
	Object3d::PreDraw();

	////3Dオブジェクトの描画
	objectManager_1->Draw();
	objectManager_2->Draw();
	objectManager_3->Draw();

	//FBXオブジェクトの描画
	object1->Draw(cmdList);

	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();


	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();

	//////スプライト描画
	//sprite->Draw();
}


