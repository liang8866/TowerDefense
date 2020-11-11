
#ifndef __UI_UPGRADES_H__
#define __UI_UPGRADES_H__

#include "GameConfig.h"
#include "levelScene.h"
#include "network/HttpClient.h"

USING_NS_CC;

enum TechTreeItem{
	TechEconomyItemIdx,
	TechTowerItemIdx,
	TechTrapItemIdx,
	TechSoliderItemIdx,
	TechHeroItemIdx,
	TechMagicItemIdx,
};

class TechItemInfo
{
public:

	int			needMoney;
	string		des;
	bool		isSelect;
	float		val;
	bool		isGradeUp;  //�ܲ�������
	Widget*		itemBack;	
};

class UIUpgrades:public Widget
{
public:
	
	UIUpgrades(void);
   ~UIUpgrades(void);

	virtual bool init() override;

	CREATE_FUNC(UIUpgrades);

	vector<TechItemInfo> mSelectItemInfo;

	int selectedIndex;
	
public:
	void BookMarkEventCallFunc(Ref *pSender , TouchEventType type);//�������ǩҳ�л��Ļص�
	void closeEventCallFunc(Ref *pSender , TouchEventType type);//�رհ�ť�Ļص�
    void ShopEventCallFunc(Ref *pSender , TouchEventType type);//תȥ�̵�Ļص�
	void resetEventCallFunc(Ref *pSender , TouchEventType type);//����İ�ť�ص�
	void upgradeEventCallFunc(Ref *pSender , TouchEventType type);//�����İ�ť�ص�
	//model�ϵİ�ť�ص�
	void modeleventCallFunc(Ref *pSender , TouchEventType type);
	void refreshMarkBook(int index);

	void updateSelectedFramePos(int tag);

	void updateSelectItemIcon();
	void updateSelectItemInfo();

	void confirmResetCallBack();

	void setValue(string &key , int &itemCtn , Widget* &curr_model);

	CC_SYNTHESIZE(TechTreeItem,_selectedItem,SelectedItem);

	virtual void onEnter() override;
	virtual void onExit() override;
	void listenerCallFunc(Ref *obj);
	void fitScreen();
	int currentTag;

	//�Ƿ���������
	bool _connecting;
	//��ǰ����������
	std::string upType;
	//��ǰ������item
	std::string upItem;
	//��ǰ�����ĵȼ�
	int upLevel;

	//�����츳����ص�
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	
	//�����츳����ص�
	void onHttpRequestCompletedReset(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	
	//��������
	void doRequest();
	void showError(int errType);
	void beginAnim_UpSkillLevel();	// �ɹ���Ч
	void endAnim_UpSkillLevel(Ref *sender);
	Animate* createAnimation(const char*FrameName,int max);
public:
	Widget *colorBg; 
	Widget *mainWidget;
	Button *Btn_tech,*Btn_magic,*Btn_war,*Btn_hero,*Btn_tower,*Btn_trap;
	Button *closeBtn;//�رհ�ť
	Button *resetBtn;//���谴ť
	
	//�������ѵ�
	Button *costBtn;//������ť
	Text *costText;
	
	//youMoneyBg �����
	Button *shopBtn;//תȥ�̵�İ�ť
	TextBMFont *MoneyText;

	//describebg�����
	Widget*descWidget;
	TextBMFont *descText;
	TextBMFont *descPrice; //�۸�

	Widget *model5;//5��ģ��,25����ť
	Widget *model4;//4��ģ��,20����ť
	Widget *model3;//3��ģ��,15����ť

	ImageView* selectFrame; //ѡ�п�

	Point FixedPos;//�̶���λ�õ�
	Point OutPos;//�ƶ��������λ��

	string resetPrompt; //ϴ����ʾ����

	Sprite*	mSkillLevelSprite;	// �ɹ���Ч
};
#endif