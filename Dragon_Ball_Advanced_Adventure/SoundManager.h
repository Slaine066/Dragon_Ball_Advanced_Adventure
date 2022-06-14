//#pragma once
//
//#include "Enum.h"
//
//class SoundManager
//{
//private:
//	SoundManager();
//	~SoundManager();
//
//	static SoundManager* m_pInstance;
//
//	// ���� ���ҽ� ������ ���� ��ü 
//	map<TCHAR*, FMOD_SOUND*> m_mapSound;
//
//	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
//	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
//
//	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
//	FMOD_SYSTEM* m_pSystem;
//
//	void LoadSoundFile();
//
//public:
//	static SoundManager* Get_Instance()
//	{
//		if (nullptr == m_pInstance)
//			m_pInstance = new SoundManager;
//
//		return m_pInstance;
//	}
//
//	static void Destroy_Instance()
//	{
//		if (m_pInstance)
//		{
//			delete m_pInstance;
//			m_pInstance = nullptr;
//		}
//	}
//
//	void Initialize();
//	void Release();
//
//	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
//	void PlayBGM(TCHAR* pSoundKey, float fVolume);
//	void StopSound(CHANNELID eID);
//	void StopAll();
//	void SetChannelVolume(CHANNELID eID, float fVolume);
//};
//
