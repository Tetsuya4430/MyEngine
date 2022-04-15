#pragma once
#include <string>
#include <vector>

#include <DirectXMath.h>

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//���[�J���ϊ��s��
	DirectX::XMMATRIX transForm;
	//�O���[�o���ϊ��s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h(���[���h�s����v�Z����Ƃ��Ɏg��)
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//�t�����h�N���X
	friend class FbxLoader;

private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
};
