#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"


static float baseColor[3];
static float metalness;
static float specular;
static float roughness;

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
	////�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");

	//	�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0, 0 }, false, false);

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
		//FbxLoader::GetInstance()->LoadModelFromFile("cube");
		model1 = FbxLoader::GetInstance()->LoadModelFromFile("SpherePBRMaps");

		//3d�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
		object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);

		//�J���������_���Z�b�g
		camera->SetTarget({ 0, 0, 0 });

		// ���C�g����
		lightGroup = LightGroup::Create();
		// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
		Fbx3d::SetLightGroup(lightGroup);

		lightGroup->SetDirLightActive(0, true);
		lightGroup->SetDirLightActive(1, true);
		lightGroup->SetDirLightActive(2, true);
	/*	lightGroup->SetPointLightActive(0, true);
		pointLightPos[0] = 0.5f;
		pointLightPos[1] = 1.0f;
		pointLightPos[2] = 0.0f;*/
		lightGroup->SetPointLightActive(0, false);
		lightGroup->SetPointLightActive(1, false);
		lightGroup->SetPointLightActive(2, false);
		lightGroup->SetCircleShadowActive(0, true);

		//�}�e���A���p�����[�^�[�̏����l���擾
		baseColor[0] = model1->GetBaseColor().x;
		baseColor[1] = model1->GetBaseColor().y;
		baseColor[2] = model1->GetBaseColor().z;
		metalness = model1->GetMetalness();
		specular = model1->GetSpecular();
		roughness = model1->GetRoughness();

	//�����ǂݍ��݂ƍĐ�
	Audio::GetInstance()->LoadWave("Alarm01.wav");
	/*Audio::GetInstance()->PlayWave("Alarm01.wav");*/
}

void GamePlayScene::Finalize()
{
	//�X�v���C�g���
	delete sprite;

	//���f���̉��
	delete model_1;
	delete model_2;
}

void GamePlayScene::Update()
{
	//---�f�o�b�O�e�L�X�g�֌W---//
	//X���W�AY���W���w�肵�ĕ\��
	DebugText::GetInstance()->Print("Debug Text", 0, 0);

	//X���W�AY���W�A�k�ڂ��w�肵�ĕ\��
	DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);


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


	//3D�I�u�W�F�N�g�̍X�V
	objectManager_1->Update();
	objectManager_2->Update();
	objectManager_3->Update();

	//���C�g�X�V
	/*lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));*/

	//lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	//lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	//lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	//lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	//lightGroup->SetSpotLightFactorAngle(0, XMFLOAT2(spotLightFactorAngle));

	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(fighterPos[0], fighterPos[1], fighterPos[2]));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	//���C�g�O���[�v���Z�b�g
	Fbx3d::SetLightGroup(lightGroup);

	//�}�e���A���p�����[�^�����f���ɔ��f
	model1->SetBaseColor(XMFLOAT3(baseColor));
	model1->SetMetalness(metalness);
	model1->SetSpecular(specular);
	model1->SetRoughness(roughness);
	model1->TransferMaterial();

	//FBX�I�u�W�F�N�g�̍X�V
	object1->Update();
	
	//�X�v���C�g�̍X�V
	sprite->Update();

	//�J�����̍X�V
	camera->Update();

	//Esc�L�[�ŃE�B���h�E�����
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESC�L�[�ŃE�B���h�E�����
	{
		//endRequest_ = true;
		return;
	}
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	////�X�v���C�g�̋��ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();

	////�X�v���C�g�`��
	sprite->Draw();

	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();

	////3D�I�u�W�F�N�g�̕`��
	/*objectManager_1->Draw();
	objectManager_2->Draw();
	objectManager_3->Draw();*/

	//FBX�I�u�W�F�N�g�̕`��
	object1->Draw(cmdList);

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();


	////�X�v���C�g�̋��ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();


	//Imgui�`��
	ImGui::Begin("Material");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 130));
	ImGui::ColorEdit3("baseColor", baseColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat("metalness", &metalness, 0, 1);
	ImGui::SliderFloat("specular", &specular, 0, 1);
	ImGui::SliderFloat("roughness", &roughness, 0, 1);
	ImGui::End();


	//////�X�v���C�g�`��
	//sprite->Draw();
}


