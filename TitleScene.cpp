#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
//#include "DebugText.h"

void TitleScene::Initialize(/*DirectXCommon* dxCommon*/)
{
	//////�X�v���C�g���ʃe�N�X�`���ǂݍ���
	//SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");
	Sprite::LoadTexture(3, L"Resources/Image/background.png");

	////	�X�v���C�g�̐���
	sprite = Sprite::Create(3, { 0, 0 });

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

	object1->PlayAnimation();

	//�J���������_���Z�b�g
	camera->SetTarget({ 0, 20, 0 });
}

void TitleScene::Finalize()
{
	//�X�v���C�g���
	delete sprite;

	
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	
	//�X�v���C�g�̍X�V
	//sprite->Update();

	//FBX�I�u�W�F�N�g�̍X�V
	object1->Update();

	//Esc�L�[�ŃE�B���h�E�����
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESC�L�[�ŃE�B���h�E�����
	{
		//endRequest_ = true;
		return;
	}
}

void TitleScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();

	//FBX�I�u�W�F�N�g�̕`��
	object1->Draw(cmdList);

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

	//////�X�v���C�g�̋��ʃR�}���h
	//SpriteCommon::GetInstance()->PreDraw();

	Sprite::PreDraw(cmdList);

	//�X�v���C�g�`��
	//sprite->Draw();

	Sprite::PostDraw();

}
