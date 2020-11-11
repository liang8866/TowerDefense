#ifndef __NPC_OBJECTS_H__  
#define __NPC_OBJECTS_H__  
#include "cocos2d.h"
#include "GameObjects.h"
USING_NS_CC;

//CCStandardTouchDelegate 
 class npcObjects : public GameObjects
{
public:
	npcObjects(void);
   ~npcObjects(void);
public:
	//---------
	static npcObjects* create();
	 
	virtual bool npcObjectsInit(); //�Զ���ĳ�ʼ��

	string getFilePathByClothes();
	
	CC_SYNTHESIZE(int,ttag,TTag);//����

	bool isArmature;
	EventListenerTouchOneByOne *listener;
	//����������ʵ�ֺͻ�ȡ
	
	void CreateArmature(const std::string& filename,const std::string&name );//������������������:�ļ�Ŀ¼ ����2:�����Ĺ�������
	//Armature *_armature;
	//Armature* getArmature();//��ȡ ��������
	 
	//���������¼�����
	void CreateListenerTouch_Armature();//���ڹ�����

	//ȥ����������
	void RemoveListenerTouch();
	void ActionDone();
	int _maxX;
	int _minX;
};

#endif  



