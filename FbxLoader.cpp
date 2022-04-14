#include "FbxLoader.h"
#include <cassert>

/// <summary>
/// 静的メンバ関数の実体
/// </summary>
/// <param name="winApp"></param>
const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
    //再初期化をチェック
    assert(fbxManager == nullptr);

    //引数からメンバ変数に代入
    this->device = device;

    //FBXマネージャーの生成
    fbxManager = FbxManager::Create();

    //FBXマネージャーの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);

    //FBXインポーターの生成
    fbxImpoter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
    //各種インスタンスの破棄
    fbxImpoter->Destroy();
    fbxManager->Destroy();
}

void FbxLoader::LoadModelFromFile(const string& modelName)
{
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";

    //拡張子.fbxを付加
    const string fileName = modelName + ".fbx";

    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;

    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImpoter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    //シーンの生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImpoter->Import(fbxScene);
}
