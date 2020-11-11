#ifndef __MAGIC_OBJECTS_H__  
#define __MAGIC_OBJECTS_H__  

#include "cocos2d.h"
#include "GameObjects.h"
#include "GameConfig.h"
#include "SceneData.h"
#include "StoreyData.h"
#include "audioPlayer.h"

 USING_NS_CC;



 class magicObjects : public GameObjects 
{
	
public:
	magicObjects(void);
   ~magicObjects(void);

public:
   
    static magicObjects* create(const std::string& filename,const std::string&name,int otype);
    virtual bool magicObjectsInit(const std::string& filename,const std::string&name,int otype); //�Զ���ĳ�ʼ��
	
	void CreateArmature(const std::string& filename,const std::string&name );//������������������:�ļ�Ŀ¼ ����2:�����Ĺ�������
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //֡�¼��ص�
	void magicupdate(float dt);   //����ʱ��


	



	void Run();
	void Fight_Begin();
	void Fight_Mid();
	void Fight_End();
	void Load();
	


	//cocostudio::Armature *dragsmokeArm;
	void dragObjectSmoke();//����ȥ��ʱ�������
public:
	void beginWithCruising(float dt);//��ʼѲ����
	bool isCruising;
	bool firstTimeFlag;
	float left_offset,right_offset;
	
 
 GameObjects * FindObject(int addr);
public:
	CC_SYNTHESIZE(int,_otherType,otherType);//����������������
	CC_SYNTHESIZE(int,isDel,isDel);
	CC_SYNTHESIZE(int,CountNum,CountNum);//������Ŀ
	CC_SYNTHESIZE(int,maxNum,maxNum);//������Ŀ
	void playTransmitIn();//���Ŵ���ħ�����ܵĽ�
	void playTransmitOut();//���Ŵ���ħ�����ܵĳ�
	void delayKeepHere(float t);//���ö���������ɾ��
	void delayKeepHereCallback(Ref *f);//�ص�
	
	void chuansongmenEffect();
	void chuansongmenEffectcallback();

	CC_SYNTHESIZE(int,itsCallObjectAddr,itsCallObjectAddr);//�������Ҫ���Զ��֧ԮӢ�۵�С���飬����С�����ס������Ӣ��
	Point itsCallObjectPos;

public://���ݣ�Ч��
	float keepTime;
	void setProperty();//��������

	Point fatherPos;
	void setFatherPos(Point pos){ fatherPos=pos;};
	int  effectId;
};
#endif  



