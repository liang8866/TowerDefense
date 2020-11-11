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
    virtual bool magicObjectsInit(const std::string& filename,const std::string&name,int otype); //自定义的初始化
	
	void CreateArmature(const std::string& filename,const std::string&name );//创建骨骼动画，参数:文件目录 参数2:创建的骨骼动画
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //帧事件回调
	void magicupdate(float dt);   //主定时器


	



	void Run();
	void Fight_Begin();
	void Fight_Mid();
	void Fight_End();
	void Load();
	


	//cocostudio::Armature *dragsmokeArm;
	void dragObjectSmoke();//放上去的时候的烟雾
public:
	void beginWithCruising(float dt);//开始巡逻了
	bool isCruising;
	bool firstTimeFlag;
	float left_offset,right_offset;
	
 
 GameObjects * FindObject(int addr);
public:
	CC_SYNTHESIZE(int,_otherType,otherType);//可能用于其他属性
	CC_SYNTHESIZE(int,isDel,isDel);
	CC_SYNTHESIZE(int,CountNum,CountNum);//计算数目
	CC_SYNTHESIZE(int,maxNum,maxNum);//最大的数目
	void playTransmitIn();//播放传送魔法技能的进
	void playTransmitOut();//播放传送魔法技能的出
	void delayKeepHere(float t);//设置多少秒后调用删除
	void delayKeepHereCallback(Ref *f);//回调
	
	void chuansongmenEffect();
	void chuansongmenEffectcallback();

	CC_SYNTHESIZE(int,itsCallObjectAddr,itsCallObjectAddr);//这个是主要针对远程支援英雄的小精灵，方便小精灵记住放它的英雄
	Point itsCallObjectPos;

public://数据，效果
	float keepTime;
	void setProperty();//设置属性

	Point fatherPos;
	void setFatherPos(Point pos){ fatherPos=pos;};
	int  effectId;
};
#endif  



