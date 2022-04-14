//クラス化したヘッダファイルのインクルード
#include "MyGame.h"

#include "fbxsdk.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT result;

	FbxManager* fbxManager = FbxManager::Create();

	FrameWork* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}