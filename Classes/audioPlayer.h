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
	void pauseAllMusicEffect();			//��ͣ���е����ֺ���Ч
	void resumeAllMusicEffect();		//�ָ����е����ֺ���Ч
	void stopAllMusicEffect();			//ֹͣ���е���Ч������

	void  playBackgroundMusic(const char* pszFilePath, bool bLoop = false);	//��������
	void  stopBackgroundMusic();	//ֹͣ����
	void  pauseBackgroundMusic();	//��ͣ����
	void  resumeBackgroundMusic();	//�ָ�����

	 unsigned int playEffect(const char* pszFilePath, bool bLoop = false, float _volume = 1.0f);	//������Ч
	 void pauseEffect(unsigned int nSoundId);	//��ͣ��Ч
	 void resumeEffect(unsigned int nSoundId);	//�ָ���Ч
	 void pauseAllEffects();					//��ͣ������Ч
	 void resumeAllEffects();					//�ָ�������Ч
	 void stopEffect(unsigned int nSoundId);	//ֹͣ��Ч
	 void stopAllEffects();						//ֹͣ������Ч
};

#endif //
