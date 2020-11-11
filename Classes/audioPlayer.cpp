#include "audioPlayer.h"

static audioPlayer* mAudioPlayer = nullptr;

audioPlayer::audioPlayer(){

}
audioPlayer::~audioPlayer(){
	SimpleAudioEngine::getInstance()->end();
}
audioPlayer* audioPlayer::getInstance()
{
	if (!mAudioPlayer)
	{
		mAudioPlayer = new audioPlayer();
		mAudioPlayer->init();
		mAudioPlayer->autorelease();
	}
	return mAudioPlayer;
}
void audioPlayer::destroyInstance()
{
	CC_SAFE_DELETE(mAudioPlayer);
}

bool audioPlayer::init()
{
	return true;
}
//��ͣ���е����ֺ���Ч
void audioPlayer::pauseAllMusicEffect()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
//�ָ����е����ֺ���Ч
void audioPlayer::resumeAllMusicEffect()
{
	UserRecord *userData= UserRecord::getInstance();
	//˵�������ţ����������ְ�
	if (userData->getmusicLock()==YES)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		
	}else{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	//˵�������ţ���������Ч��
	if (userData->geteffectLock()==YES)
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}else{
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	}
}
//ֹͣ���е���Ч������
void audioPlayer::stopAllMusicEffect()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

//----------------���������ֲ���----------------
//��������
void audioPlayer:: playBackgroundMusic(const char* pszFilePath, bool bLoop )
{
	UserRecord *userData= UserRecord::getInstance();
	
	if (userData->getmusicLock()==NO)
	{
		
		SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath,bLoop);
	}
}
//ֹͣ����
void audioPlayer::stopBackgroundMusic()
{
	
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}
//��ͣ����
void audioPlayer::pauseBackgroundMusic()
{
	
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); 
}
//�ָ�����
void audioPlayer::resumeBackgroundMusic()
{
	UserRecord *userData= UserRecord::getInstance();
	if (userData->getmusicLock()==NO)
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}else{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
}

//----------------��������Ч����----------------
//������Ч
unsigned int audioPlayer::playEffect(const char* pszFilePath, bool bLoop, float _volume)
{
	UserRecord* userData = UserRecord::getInstance();
	if (userData->geteffectLock() == YES)
		return -1;

	unsigned int _id = 0;
	// ������һ������
	SimpleAudioEngine::getInstance()->setEffectsVolume(_volume);
	// ��ִ�в���
	_id = SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop);
	return _id;
}
//��ͣ��Ч
void audioPlayer::pauseEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->pauseEffect(nSoundId);
}
//�ָ���Ч
void audioPlayer::resumeEffect(unsigned int nSoundId)
{
	UserRecord *userData= UserRecord::getInstance();
	if (userData->geteffectLock()==NO)
	{
		SimpleAudioEngine::getInstance()->resumeEffect(nSoundId);
	}else{
		SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
	}
}
void audioPlayer::pauseAllEffects()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}

//�ָ�������Ч
void audioPlayer::resumeAllEffects()
{
	UserRecord *userData= UserRecord::getInstance();
	if (userData->geteffectLock()==NO)
	{
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	}else{
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}
}
//ֹͣ��Ч
void audioPlayer::stopEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}
//ֹͣ������Ч
void audioPlayer::stopAllEffects()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}