#pragma once

#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Fbx3d.h"
#include "Input.h"
#include "ObjectManager.h"

#include <memory>

/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GameScene : public BaseScene
{
public:
	// �R���X�g�N���^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(/*DirectXCommon* dxCommon*/) override;

	//�I������
	void Finalize() override;

	//�X�V
	void Update() override;

	//�`��
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

	std::shared_ptr<Object3d> object3d_1;
	std::shared_ptr<Object3d> object3d_2;
	std::shared_ptr<Object3d> object3d_3;
	std::unique_ptr<ObjectManager> objectManager_1;
	std::unique_ptr<ObjectManager> objectManager_2;
	std::unique_ptr<ObjectManager> objectManager_3;

	float MoveVec = 1.0f;
};

