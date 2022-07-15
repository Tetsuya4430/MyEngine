#include "GameScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
//#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"
#include "PostEffect.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete(object1);
	delete(model1);
	//delete(postEffect);
}

void GameScene::Initialize()
{ 
	////�X�v���C�g���ʃe�N�X�`���ǂݍ���
	//SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");
	//SpriteCommon::GetInstance()->SpriteCommonLoadTexture(100, L"Resources/Image/Rena.jpg");


	Sprite::LoadTexture(1, L"Resources/Image/GamePlay.png");
	//Sprite::LoadTexture(100, L"Resources/Image/background.png");

	//	�X�v���C�g�̐���
	title = Sprite::Create(1, { 0, 0 });

	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();


	//�|�X�g�G�t�F�N�g�̐���
	postEffect = PostEffect::Create(1, { 0, 0 });

	//OBJ���烂�f���f�[�^��ǂݍ���
	model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("triangle_mat");

	//�I�u�W�F�N�g�}�l�[�W���[����
	objectManager_1 = std::make_unique<ObjectManager>();
	objectManager_2 = std::make_unique<ObjectManager>();
	objectManager_3 = std::make_unique<ObjectManager>();

	//�I�u�W�F�N�g�}�l�[�W���[����->3D�I�u�W�F�N�g����->�I�u�W�F�N�g�̎Q�ƃJ�E���g�𑝉�
	std::shared_ptr<Object3d> object_1 = Object3d::Create(model_1, camera);
	std::weak_ptr<Object3d> ObjectWp_1 = objectManager_1->AddObject(object_1);
	object3d_1 = ObjectWp_1.lock();

	std::shared_ptr<Object3d> object_2 = Object3d::Create(model_2, camera);
	std::weak_ptr<Object3d> ObjectWp_2 = objectManager_2->AddObject(object_2);
	object3d_2 = ObjectWp_2.lock();

	std::shared_ptr<Object3d> object_3 = Object3d::Create(model_2, camera);
	std::weak_ptr<Object3d> ObjectWp_3 = objectManager_3->AddObject(object_3);
	object3d_3 = ObjectWp_3.lock();

	//3D�I�u�W�F�N�g�̈ʒu�Ɗg�嗦���w��
	
		object3d_1->SetScale({ 20.0f, 20.0f, 20.0f });

		object3d_2->SetPosition({ -15, 0, -15 });
		object3d_2->SetScale({ 21.0f, 21.0f, 21.0f });
	
		object3d_3->SetPosition({ +15, 0, +15 });
		object3d_3->SetScale({ 21.0f, 21.0f, 21.0f });
	
	
		//�f�o�C�X���Z�b�g
		Fbx3d::SetDevice(dxCommon->GetDev());
		//�J�����Z�b�g
		Fbx3d::SetCamera(camera);
		//�O���t�B�b�N�X�p�C�v���C���𐶐�
		Fbx3d::CreateGraphicsPipeline();

		//���f�����w�肵��FBX�t�@�C����ǂݍ���
		model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

		//3d�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
		object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);



		//�J���������_���Z�b�g
		camera->SetTarget({ 0, 20, 0 });

	//�����ǂݍ��݂ƍĐ�
	Audio::GetInstance()->LoadWave("Alarm01.wav");
	/*Audio::GetInstance()->PlayWave("Alarm01.wav");*/


	object1->PlayAnimation();
}

void GameScene::Finalize()
{
	//�X�v���C�g���
	delete sprite;

	//���f���̉��
	delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	////---�f�o�b�O�e�L�X�g�֌W---//
	////X���W�AY���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Debug Text", 0, 0);

	////X���W�AY���W�A�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);


	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if (Input::GetInstance()->TriggerKey(DIK_A))
	{
		Audio::GetInstance()->PlayWave("Alarm01.wav");
	}

	//���W����
	if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
	{
			DirectX::XMFLOAT3 pos = object3d_1->GetPosition();

			//pos.x += 1.0f;

			object3d_1->SetPosition(pos);

			if (Input::GetInstance()->PushKey(DIK_DOWN))
			{
				camera->CameraMoveVector({ -MoveVec, 0, 0 });
			}

			if (Input::GetInstance()->PushKey(DIK_UP))
			{
				camera->CameraMoveVector({ +MoveVec, 0, 0 });
			}

			if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				camera->CameraMoveVector({ 0, +MoveVec, 0 });
			}

			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				camera->CameraMoveVector({ 0, -MoveVec, 0 });
			}
	}


	//3D�I�u�W�F�N�g�̍X�V
	objectManager_1->Update();
	objectManager_2->Update();
	objectManager_3->Update();

	//FBX�I�u�W�F�N�g�̍X�V
	object1->Update();
	
	//�X�v���C�g�̍X�V
	//sprite->Update();


	//�J�����̍X�V
	camera->Update();


	//Esc�L�[�ŃE�B���h�E�����
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESC�L�[�ŃE�B���h�E�����
	{
		//endRequest_ = true;
		return;
	}

}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	////�����_�[�^�[�Q�b�g
	//postEffect->PreDrawScene(cmdList);

	//postEffect->PostDrawScene(cmdList);

	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();

	////3D�I�u�W�F�N�g�̕`��
	objectManager_1->Draw();
	objectManager_2->Draw();
	objectManager_3->Draw();

	//FBX�I�u�W�F�N�g�̕`��
	object1->Draw(cmdList);

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();



	Sprite::PreDraw(cmdList);

	//�|�X�g�G�t�F�N�g�`��
	postEffect->Draw();

	Sprite::PostDraw();




}

