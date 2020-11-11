#ifndef __AUDIOPLAYER_H__
#define __AUDIOPLAYER_H__
#include "cocos2d.h"
#include "GameConfig.h"
#include "UserRecord.h"
USING_NS_CC;

class audioPlayer : public Ref
{
	
public:
	audioPlayer();
	~audioPlayer();

	static audioPlayer* getInstance();
	void destroyInstance();

	//static audioPlayer* create();
	virtual bool init();

public:
	void pauseAllMusicEffect();			//暂停所有的音乐和音效
	void resumeAllMusicEffect();		//恢复所有的音乐和音效
	void stopAllMusicEffect();			//停止所有的音效和音乐

	void  playBackgroundMusic(const char* pszFilePath, bool bLoop = false);	//播放音乐
	void  stopBackgroundMusic();	//停止音乐
	void  pauseBackgroundMusic();	//暂停音乐
	void  resumeBackgroundMusic();	//恢复音乐

	 unsigned int playEffect(const char* pszFilePath, bool bLoop = false, float _volume = 1.0f);	//播放音效
	 void pauseEffect(unsigned int nSoundId);	//暂停音效
	 void resumeEffect(unsigned int nSoundId);	//恢复音效
	 void pauseAllEffects();					//暂停所有音效
	 void resumeAllEffects();					//恢复所有音效
	 void stopEffect(unsigned int nSoundId);	//停止音效
	 void stopAllEffects();						//停止所有音效
};

#endif //
