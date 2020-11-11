#ifndef __UI_MAINLAYER_H__
#define __UI_MAINLAYER_H__ 


#include "cocos2d.h"
#include "Window.h"
#include "GameConfig.h"
#include "bossObjects.h"


class ItemInfo
{
public:
	ItemInfo();
	~ItemInfo();
public:
	string unit_id;      
	float totalCDtime;
	float leftCDtime;
	int   price;
	int   objType;
	int   selfType;
};

class Ui_MainLayer : public Window
{

public:
	Ui_MainLayer();
	~Ui_MainLayer();

	static Ui_MainLayer * create(const char *fileName);
	virtual bool initialise(const char* fileName);

	void initData();

	void eventTouchUiButton(Ref* sender, TouchEventType type);      //������ͣ���̵갴ť����¼�
	void eventTouchMagicButton(Ref* sender, TouchEventType type);   //����ħ����ť����¼�  
	void eventTouchItemObjButton(Ref* sender, TouchEventType type); //������Ŀ�ڶ������¼�

	void hideMagics();                                 //����ħ��
	void showMagics();                                 //��ʾħ��

	void updateItemObjs();                             //������Ŀ����

	void setRoundNum(int currRound , int totalRound); //���ûغ���
	void setLifeNum(int life);                        //��������ֵ 
	void setSweetNum(int addsweet , int limit);            //�����ǹ���
	void setDynamicSweetNum(int sweet , Point pos);               //�����ǹ�����+����̬Ч��
	void setSoulNum(int soul);                        //����Ӣ��֮������
	void setMagicNum(int index ,int num);             //����ħ������
	void setItemObjPrice(Widget *obj ,int price);     //���ò˵���Ŀ�ڶ���۸�
	void updateAllObjsPrice();                        //����������Ŀ�����ж���۸�
	void createObjCost(Ref *obj, int _indexId = 0);  //������λ�ɹ�����Ҫ�۳���Ӧ���ǹ�

	void updateObjsIcon();                            //������Ŀ�ϵ�Icon
	void FindObjToNoCd(int oType,int sType);
	void hideProgress(Ref *pSender);
	void showProgress(Ref *pSender, int _indexId = 0);
	void progressActionCallFunc(Ref *pSender,const int key);

	void showHeroInfo(int selfType,int Level,bossObjects *obj);
	void hideHeroInfo();
	void setHeroHp(int hp,int maxHp);
	void eventTouchHeroBtn(Ref* sender, TouchEventType type); //����Ӣ��ͷ�����¼�
	void bossCdPrgCallfunc(Ref *sender);//��ȴ���ܽ������Ļص�

	void onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	//�ű����ʱ�����ʾ
	Sprite *waring;
	void SetAtkDoorWarning();
    void WarningCallback(Ref *f);

	int getSweetNum(){                         //��ȡ�ǹ��� 
		return m_sweet;
	};                  
	int getMaxSweet(){                         //��ȡ�ǹ�������� 
		return m_totalSweet;
	};

	int getCurrRound(){                        //��ȡ��ǰ����
		return m_round;
	};
	int getMaxRound(){                         //��ȡ��󲨴�
		return m_totalRound;                    
	}

	int getLifeNum(){                          //��ȡ����ֵHP
		return m_life;     
	}

	int getSoulNum(){                          //��ȡӢ�����
		return m_soul;         
	}

	void enterObjsNoCDstatus();               //���������CD״̬
	void leaveObjsNoCDstatus();               //�뿪�����CD״̬
	Animate* noCDeffect();                     //�����CD��Ч

	void update(float dt);

	ItemInfo& getItemInfoByID(int id); 
	void fitScreen();
	
	void changePrice(Ref *obj);                        //��λ��������۸��� 

	void myPauseAllAction(Ref *obj); //��ͣ
	void myResumeAllAction(Ref *obj);//ȡ����ͣ

	// ħ���ͷż���
	EventListenerTouchOneByOne* _magicListener;
	void createMagicListenerTouch();
	void removeMagicListenerTouch();
	bool onMagicTouchBegan(Touch* _touch, Event* _unusedEvent);  
	void onMagicTouchMove(Touch* _touch, Event* _unusedEvent);
	void onMagicTouchEnd(Touch* _touch, Event* _unusedEvent);

	void updateYuJing(bool isVisible = false, bool isUp = false);

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void setItemSelect(int _index, bool _isHideAll = false);	// ����ѡ��

	void eventTouch_Debug(Ref* sender, TouchEventType type);
public:
	std::map<int,ItemInfo> m_itemInfo;  //�洢�˵����������Ϣ
	int noCDprice;
	

	static const int nextRoundArrivedTime = 10;

	int _disTime;
	//�ݶ���ʱ
	int _lastKey;
	Map<int,ProgressTimer*> loadBars;


	void setPauseBtnGray();//����ͣ��ť�ҵ�
	void removePauseBtnGray();//��ͣ��ť�ָ�
public:
	Widget*     currPanel;
	Armature*   _armature;
	Button*	    m_PauseBtn;
	Button*     m_ShopBtn;
	//20001����    20002 ����  20003 ������   20004 ��˹��
	Button*     HeroBtn;
	Button*     m_AddMagicBtn;
	Button*     m_MagicBtn_1;//20001����  
	Button*     m_MagicBtn_2;//20002 ���� 
	Button*     m_MagicBtn_3;//20003 ������
	Button*     m_MagicBtn_4;//20004 ��˹��
	Widget*     magic_bk; 
	ImageView*  _soulIcon;

	ProgressTimer *skillPrg;//������ȴ����������
	bossObjects *targetBoss;//Ӣ�۶���

	int         m_sweet;
	int         m_totalSweet;
	int         m_round;
	int         m_totalRound;
	int         m_life;
	int         m_soul;

public://ħ�����ܴ���
    

	int magicIndex;// ���ͷŵڼ�������'
	void reduceOneMagic(std::string id,Button *btn);
	void refreshMagic(std::string id,Button *btn = nullptr);
	
	void magic1SkillAction(Point pos,int floor);//20001���� 
	void magic2SkillAction(Point pos,int floor);//20002 ����
	void magic3SkillAction(Point pos,int floor);// 20003 ������
	void magic4SkillAction(Point pos,int floor);//20004 ��˹��

	//20001����    20002 ����  20003 ������   20004 ��˹��

	void magicCdAction(Button* btn,float cdtime);
	void magicCdCallback(Ref *b);

	float m_magicCdTime_1;//20001 ������ȴʱ��
	float m_magicCdTime_2;//20002 ������ȴʱ��
	float m_magicCdTime_3;//20003 ������ȴʱ��
	float m_magicCdTime_4;//20004 ��˹����ȴʱ��
	void cdBtnProgess(Button*btn,int isPause);

	bool  skillCDstatus;

private:
	Point clickPos;
	int   clickStoreyIndex;
};


#endif // !__UI_MAINLAYER_H__
