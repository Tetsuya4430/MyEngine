#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Fbx3d.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public BaseScene
{
public:
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

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;
	Camera* camera = nullptr;

	float MoveVec = 0.1f;
};

