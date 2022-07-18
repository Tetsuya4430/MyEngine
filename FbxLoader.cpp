﻿#include "FbxLoader.h"
#include <cassert>

/// <summary>
/// 静的メンバ関数の実体
/// </summary>
/// <param name="winApp"></param>
const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

using namespace DirectX;

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

FbxModel* FbxLoader::LoadModelFromFile(const string& modelName)
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

    //モデル生成
    FbxModel* model = new FbxModel();
    model->name = modelName;

    //FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();

    //あらかじめ必要数分のメモリを渡すことで、アドレスがずれるのを予防
    model->nodes.reserve(nodeCount);

    //ルートノードから順に解析してモデルを流し込む
    ParseNodeRecursive(model, fbxScene->GetRootNode());

    //バッファ生成
    model->CreateBuffers(device);

    //FBXシーン解放
    fbxScene->Destroy();

    return model;
}

void FbxLoader::ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent)
{
    //モデルにノードを追加
    model->nodes.emplace_back();
    Node& node = model->nodes.back();

    //ノード名の取得
    node.name = fbxNode->GetName();

    //FBXノードのローカル移動情報
    FbxDouble3 rotation = fbxNode->GeometricRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->GeometricTranslation.Get();

    //形式変換して代入
    node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
    node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
    node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };

    //回転角をDegree(度)かたラジアンに変換
    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);


    //スケール、回転、平行移動の計算
    XMMATRIX matScaling, matRotation, matTranslation;

    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //ローカル変換行列の計算
    node.transForm = XMMatrixIdentity();
    node.transForm *= matScaling;   //ワールド行列にスケーリングを反映
    node.transForm *= matRotation;  //ワールド行列に回転を反映
    node.transForm *= matTranslation;   //ワールド行列に平行移動を反映

    node.globalTransform = node.transForm;

    //グローバル変形行列の計算
    if (parent)
    {
        node.parent = parent;
        //親の変形を計算
        node.globalTransform *= parent->globalTransform;
    }

    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }

    //子ノードに対して再起呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(FbxModel* model, FbxNode* fbxNode)
{
    //ノードのメッシュを取得
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    //頂点座標の読み取り
    ParseMeshVertices(model, fbxMesh);

    //面を構成するデータの読み取り
    ParseMeshFaces(model, fbxMesh);

    //マテリアルの読み取り
    ParseMaterial(model, fbxNode);
}

void FbxLoader::ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;

    //頂点座標データの数
    const int controlPointsCount = fbxMesh->GetControlPointsCount();

    //必要数だけ頂点データ配列を確保
    FbxModel::VertexPosNormalUv vert{};
    model->vertices.resize(controlPointsCount, vert);

    //FBXメッシュの頂点座標配列を取得
    FbxVector4* pCood = fbxMesh->GetControlPoints();

    //FBXメッシュの全頂点座標をモデル内の配列にコピーする
    for (int i = 0; i < controlPointsCount; i++)
    {
        FbxModel::VertexPosNormalUv& vertex = vertices[i];
        //座標のコピー
        vertex.pos.x = (float)pCood[i][0];
        vertex.pos.y = (float)pCood[i][1];
        vertex.pos.z = (float)pCood[i][2];
    }
}

void FbxLoader::ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    //1ファイルに複数メッシュのモデルは非対応
    assert(indices.size() == 0);

    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();

    //UVデータの数
    const int textureUVCount = fbxMesh->GetTextureUVCount();

    //UV名リスト
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    //面ごとの情報読み取り
    for (int i = 0; i < polygonCount; i++)
    {
        //面を構成する頂点の数を取得(3なら三角形ポリゴン)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1頂点ずつ処理
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX頂点配列のいんでっくす
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);


            //頂点法線読み込み
            FbxModel::VertexPosNormalUv& vertex = vertices[index];

            FbxVector4 normal;

            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

            //テクスチャ読み込み
            if (textureUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappedUV;
                
                //0番決め打ちで読み込み
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1];
                }
            }

            //インデックス配列に頂点インデックス追加
            //3頂点目までなら
            if (j < 3)
            {
                //1点追加し、他の2点と三角形を構築する
                indices.push_back(index);
            }

            //4頂点目
            else
            {
                //3点追加し、
                //4角形の0,1,2,3の内 2,3,0で三角形を構築する
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
        }
    }

}

void FbxLoader::ParseMaterial(FbxModel* model, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();

    if (materialCount > 0)
    {
        //先頭のマテリアルを取得
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

        //テクスチャを読み込んだどうかを表すフラグ
        bool textureLoaded = false;

        if (material)
        {
            /*if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {*/
                //マテリアル名(デバッグ用)
                string name = material->GetName();

                //ベースカラー
                const FbxProperty propBaseColor =
                    FbxSurfaceMaterialUtils::GetProperty("baseColor", material);

                if (propBaseColor.IsValid())
                {
                    //FbxDouble3としてプロパティの値を読み取り
                    FbxDouble3 baseColor = propBaseColor.Get<FbxDouble3>();

                    //モデルに読み取った値を書き込む
                    model->baseColor.x = (float)baseColor.Buffer()[0];
                    model->baseColor.y = (float)baseColor.Buffer()[1];
                    model->baseColor.z = (float)baseColor.Buffer()[2];

                    const FbxFileTexture* texture = propBaseColor.GetSrcObject<FbxFileTexture>();

                    if (texture)
                    {
                        const char* filepath = texture->GetFileName();
                        //ファイルパスからファイル名抽出
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        //テクスチャ読み込み
                        LoadTexture(&model->baseTexture, baseDirectory + model->name + "/" + name);
                        model->baseColor = { 0, 0, 0 };
                        textureLoaded = true;
                    }
                }

                //金属度
                const FbxProperty propMetalness =
                    FbxSurfaceMaterialUtils::GetProperty("metalness", material);

                if (propMetalness.IsValid())
                {
                    model->metalness = propMetalness.Get<float>();

                    const FbxFileTexture* texture = propMetalness.GetSrcObject<FbxFileTexture>();

                    if (texture)
                    {
                        const char* filepath = texture->GetFileName();
                        //ファイルパスからファイル名抽出
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        //テクスチャ読み込み
                        LoadTexture(&model->metalnessTexture, baseDirectory + model->name + "/" + name);
                        model->metalness = 0.0f;
                    }
                }

                //隙間
                const FbxProperty propSpecular =
                    FbxSurfaceMaterialUtils::GetProperty("specular", material);

                if (propSpecular.IsValid())
                {
                    model->specular = propSpecular.Get<float>();
                }
                
                //粗さ
                const FbxProperty propSpecularRoughness =
                    FbxSurfaceMaterialUtils::GetProperty("specularRoughness", material);

                if (propSpecularRoughness.IsValid())
                {
                    model->roughness = propSpecularRoughness.Get<float>();

                    const FbxFileTexture* texture = propSpecularRoughness.GetSrcObject<FbxFileTexture>();

                    if (texture)
                    {
                        const char* filepath = texture->GetFileName();
                        //ファイルパスからファイル名抽出
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        //テクスチャ読み込み
                        LoadTexture(&model->roughnessTexture, baseDirectory + model->name + "/" + name);
                        model->roughness = 0.0f;
                    }
                }

                //法線マップテクスチャ
                const FbxProperty propNormalCamera = FbxSurfaceMaterialUtils::GetProperty("normalCamera", material);

                if (propNormalCamera.IsValid())
                {
                    const FbxFileTexture* texture = propNormalCamera.GetSrcObject<FbxFileTexture>();

                    if (texture)
                    {
                        const char* filepath = texture->GetFileName();
                        //ファイルパスからファイル名抽出
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        //テクスチャ読み込み
                        LoadTexture(&model->normalTexture, baseDirectory + model->name + "/" + name);
                    }
                }

            //    FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

            //    //環境光係数
            //    FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
            //    model->ambient.x = (float)ambient.Get()[0];
            //    model->ambient.y = (float)ambient.Get()[1];
            //    model->ambient.z = (float)ambient.Get()[2];

            //    //拡散反射光
            //    FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
            //    model->diffuse.x = (float)diffuse.Get()[0];
            //    model->diffuse.y = (float)diffuse.Get()[1];
            //    model->diffuse.z = (float)diffuse.Get()[2];
            //}

            ////デフューズテクスチャを取り出す
            //const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

            //if (diffuseProperty.IsValid())
            //{
            //    const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();

            //    if (texture)
            //    {
            //        const char* filepath = texture->GetFileName();

            //        //ファイルパスからファイル名を抽出
            //        string path_str(filepath);
            //        string name = ExtractFileName(path_str);

            //        //テクスチャ読み込み
            //        LoadTexture(model, baseDirectory + model->name + "/" + name);

            //        textureLoaded = true;
            //    }
            //}
        }

        //テクスチャが進まない場合は白テクスチャを貼る
        if (!textureLoaded)
        {
            LoadTexture(&model->baseTexture, baseDirectory + defaultTextureFileName);
        }
    }
}

void FbxLoader::LoadTexture(TextureData* texdata, const std::string& fullpath)
{
    HRESULT result = S_FALSE;

    //WICテクスチャのロード
    TexMetadata& metadata = texdata->metadata;
    ScratchImage& scratchImg = texdata->scratchImg;

    //ユニコード文字列に変換
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));

    result = LoadFromWICFile(
        wfilepath, WIC_FLAGS_NONE,
        &metadata, scratchImg
    );

    if (FAILED(result))
    {
        assert(0);
    }
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;

    //区切り文字'\\'が出てくる1番最後の部分を検索
    pos1 = path.rfind('\\');

    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    //区切り文字'/'が出てくる1番最後の部分を検索
    pos1 = path.rfind('/');

    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    return path;
}
