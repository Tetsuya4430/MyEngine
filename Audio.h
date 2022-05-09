#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <cstdint>
#pragma comment(lib, "xaudio2.lib")

#include <wrl.h>
#include <map>
#include <string>

// �I�[�f�B�I audio
class Audio
{
private: // �G�C���A�X alias
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // �T�u�N���X Subclass
	// �`�����N�w�b�_ Chunk header
	struct ChunkHeader
	{
		char	id[4]; // �`�����N����ID ID for each chunk
		int32_t		size;  // �`�����N�T�C�Y Chunk size
	};

	// RIFF�w�b�_�`�����N RIFF header chunk
	struct RiffHeader
	{
		ChunkHeader	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMT�`�����N FMT chunk
	struct FormatChunk
	{
		ChunkHeader		chunk; // "fmt "
		WAVEFORMATEX	fmt;   // �g�`�t�H�[�}�b�g Waveform format
	};

	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;

		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;

		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
	};


public: // �����o�֐� Member function

	static Audio* GetInstance();

	// ������ Initialization
	void Initialize(const std::string& directoryPath = "Resources/");

	//�������
	void Finalize();

	//�����ǂݍ���
	void LoadWave(const std::string& filename);

	//�T�E���h�f�[�^�̉��
	void UnLoad(SoundData* soundData);

	// �T�E���h�t�@�C���̍Đ� playing sound files
	void PlayWave(const std::string& filename);

private: // �����o�ϐ� Member variables
	ComPtr<IXAudio2> xAudio2;

	//�T�E���h�f�[�^�̘A�z�z��
	std::map<std::string, SoundData> soundDatas;

	//�T�E���h�i�[�f�B���N�g��
	std::string directoryPath_;
};

