#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <cstdint>
#pragma comment(lib, "xaudio2.lib")

#include <wrl.h>
#include <map>
#include <string>

// オーディオ
class Audio
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // サブクラス
	// チャンクヘッダ
	struct ChunkHeader
	{
		char	id[4]; // チャンク毎のID
		int32_t		size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク FMT chunk
	struct FormatChunk
	{
		ChunkHeader		chunk; // "fmt "
		WAVEFORMATEX	fmt;   // 波形フォーマット
	};

	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;

		//バッファの先頭アドレス
		BYTE* pBuffer;

		//バッファのサイズ
		unsigned int bufferSize;
	};


public: // メンバ関数
	static Audio* GetInstance();

	// 初期化
	void Initialize(const std::string& directoryPath = "Resources/Music/");

	//解放処理
	void Finalize();

	//音声読み込み
	void LoadWave(const std::string& filename);

	//サウンドデータの解放
	void UnLoad(SoundData* soundData);

	// サウンドファイルの再生
	void PlayWave(const std::string& filename);

private: // メンバ変数
	ComPtr<IXAudio2> xAudio2;

	//サウンドデータの連想配列
	std::map<std::string, SoundData> soundDatas;

	//サウンド格納ディレクトリ
	std::string directoryPath_;
};

