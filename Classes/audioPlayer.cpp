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
//暂停所有的音乐和音效
void audioPlayer::pauseAllMusicEffect()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
//恢复所有的音乐和音效
void audioPlayer::resumeAllMusicEffect()
{
	UserRecord *userData= UserRecord::getInstance();
	//说明是锁着，不播放音乐啊
	if (userData->getmusicLock()==YES)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		
	}else{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	//说明是锁着，不播放音效啊
	if (userData->geteffectLock()==YES)
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}else{
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	}
}
//停止所有的音效和音乐
void audioPlayer::stopAllMusicEffect()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

//----------------以下是音乐部分----------------
//播放音乐
void audioPlayer:: playBackgroundMusic(const char* pszFilePath, bool bLoop )
{
	UserRecord *userData= UserRecord::getInstance();
	
	if (userData->getmusicLock()==NO)
	{
		
		SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath,bLoop);
	}
}
//停止音乐
void audioPlayer::stopBackgroundMusic()
{
	
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}
//暂停音乐
void audioPlayer::pauseBackgroundMusic()
{
	
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); 
}
//恢复音乐
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

//----------------以下是音效部分----------------
//播放音效
unsigned int audioPlayer::playEffect(const char* pszFilePath, bool bLoop, float _volume)
{
	UserRecord* userData = UserRecord::getInstance();
	if (userData->geteffectLock() == YES)
		return -1;

	unsigned int _id = 0;
	// 先设置一次音量
	SimpleAudioEngine::getInstance()->setEffectsVolume(_volume);
	// 再执行播放
	_id = SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop);
	return _id;
}
//暂停音效
void audioPlayer::pauseEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->pauseEffect(nSoundId);
}
//恢复音效
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

//恢复所有音效
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
//停止音效
void audioPlayer::stopEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}
//停止所有音效
void audioPlayer::stopAllEffects()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}