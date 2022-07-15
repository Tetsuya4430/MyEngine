#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
//#include "DebugText.h"

void TitleScene::Initialize(/*DirectXCommon* dxCommon*/)
{
	//////スプライト共通テクスチャ読み込み
	//SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");
	Sprite::LoadTexture(3, L"Resources/Image/background.png");

	////	スプライトの生成
	sprite = Sprite::Create(3, { 0, 0 });

	//デバイスをセット
	Fbx3d::SetDevice(dxCommon->GetDev());
	//カメラセット
	Fbx3d::SetCamera(camera);
	//グラフィックスパイプラインを生成
	Fbx3d::CreateGraphicsPipeline();

	//モデルを指定してFBXファイルを読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3dオブジェクト生成とモデルのセット
	object1 = new Fbx3d;
	object1->Initialize();
	object1->SetModel(model1);

	object1->PlayAnimation();

	//カメラ注視点をセット
	camera->SetTarget({ 0, 20, 0 });
}

void TitleScene::Finalize()
{
	//スプライト解放
	delete sprite;

	
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	
	//スプライトの更新
	//sprite->Update();

	//FBXオブジェクトの更新
	object1->Update();

	//Escキーでウィンドウを閉じる
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESCキーでウィンドウを閉じる
	{
		//endRequest_ = true;
		return;
	}
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//3Dオブジェクトの描画前処理
	Object3d::PreDraw();

	//FBXオブジェクトの描画
	object1->Draw(cmdList);

	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();

	//////スプライトの共通コマンド
	//SpriteCommon::GetInstance()->PreDraw();

	Sprite::PreDraw(cmdList);

	//スプライト描画
	//sprite->Draw();

	Sprite::PostDraw();

}
