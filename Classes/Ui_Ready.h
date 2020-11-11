#ifndef __UI_READY_H__
#define __UI_READY_H__
#include "GameConfig.h"
#include "Ui_Prompt.h"
#include "UserRecord.h"
#include "Ui_Shop.h"
#include "Connect.h"

using namespace std;
enum ReadyItem
{
	ReadySodierItem=1,
	ReadyTowerItem,
	ReadyTrapItem
};

enum ReadyItemZorder
{
	ReadyTrapZorder = 7,
	ReadyTowerZorder,
	ReadySoldierZorder,
	ReadyHighLightZorder=12,
};

class ReadyItemInfo
{
public:
	ReadyItemInfo();
	~ReadyItemInfo();

public:
	string     unit_id;
	bool       lock;		// ��������
	int        price;
	bool       selected;
	string     name;

	int        unLockPrice;
	int		   objectType;
	int		   selfType;
	bool	   close;		// ���߽�ֹ
	int        isComing;	// �����ڴ�
	string     desc;		// ���ֽ���
};

class _ReadyItemCellData
{
public:
	string		id;			// ����id
	int			type;		// ��������
	bool		isComing;	// �Ƿ����ڴ�
	bool		isDesc;		// �����Ƿ���ʾDesc˵��״̬
	string		desc;
	string		name;		
	Widget*		itemCell;	// ����cell����
	int			price;
	int			unLockPrice;
	bool		lock;		// ��������
};

class Ui_Ready:public Widget
{
public:
	Ui_Ready();
	~Ui_Ready();

	virtual bool init();
	CREATE_FUNC(Ui_Ready);

	virtual void onEnter() override;
	virtual void onExit() override;
	void fitScreen();

	void selectIndex(ReadyItem idx);
	void initData();
	void initObjs();

	void itemBtnClickedCallFuc(Ref *pSender , TouchEventType type);
	void initItemZOrder();
	void updateItemInfo(int idx);

	void close(Ref *pSender , TouchEventType type);

	void selectedUnitEventCallFunc(Ref *pSender , TouchEventType type);
	void selectedNullEventCallFunc(Ref *pSender , TouchEventType type);
	void lockItemClickCallFunc(Ref *pSender , TouchEventType type);
	void confirmUnLockItem();

	void shopBtnCallFunc(Ref *pSender , TouchEventType type);
	void goldNoEnoughCallfunc();
	void GameStartCallFunc(Ref *pSender , TouchEventType type);

	void setSeatCount(int num); //(5-7)

	void addSeatCountCallFunc(Ref *pSender , TouchEventType type);

	void buyPromptCallFunc();

	void objBtnsEventCallFuc(Ref *pSender , TouchEventType type);

	void insertObjsIcon();

	void RemoveObjsIcon(int idx);

	void ObjsMoveCallFunc(Ref *pSender);

	void changeSeatIdx(string key);

	void hideSelectFrame(string unit_id);
	void showSelectFrame(string unit_id);

	void fadeInAction(Node *pSender,float delay);

	CC_SYNTHESIZE(ReadyItem,_selectedItem,SelectedItem);

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void onHttpRequestAddSeat(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	// �¼�104 ���Ƶ���ʹ��
	__Dictionary* doEvent104();

	void selectDesc(string unit_id);						// ѡ�н���˵��״̬
	void selectDescAction(Widget* _widget, bool _isBig);	// ˵��״̬�ķŴ���С����
	bool isDesc(string unit_id);							// �Ƿ���ѡ��˵��״̬
	void createAnimArrows();								// ������̬��ͷ
	void selectDescActionCallBack_Big(Ref* sender);

	void onButtonClick_UnLock(Ref *pSender , TouchEventType type);
	int getDescSelectIndex(string _id);	
	void updateSceneNumber();	// ���¹ؿ�����

	void createLineAction(Widget* _widget, float _speedTime = 0.5f, int _disY = 5);
public:
	map<int,vector<ReadyItemInfo> > _itemInfo;
	int      seatCount;
	int      occupySeatIndex;
	map<string,int> _vecObj; //int is seatindex
	
	static const int addSeatCost = 1000;

	Sprite* mArrows;	// ��̬��ͷ
	vector<_ReadyItemCellData> mCurReadyItems;	// ��ǰҳ���������
	string mCurSelectId;
private:
	Widget   *m_widget;
	ListView *listView;
	Text     *goldNum;

	Button   *soldierBtn;
	Button   *towerBtn;
	Button   *trapBtn;
	Button   *beginGameBtn;
	Button   *addItemBtn; 

	Widget   *currWidget;  //��ǰ�ײ�Я����ģ��

	int      clickedItemTag;

	float	bottomPosY;
	TextBMFont*		mBitmapLabelName;
	TextBMFont*		mBitmapLabelDesc;
	TextBMFont*		mBitmapLabelUnLockCost;

	int		mDefaultItemCount;
	string	mIsComingText;
	float	mDefaultItemScale;

	Layout* panel_test;

	Button*	mButtonUnLock;
};



#endif // !__UI_SHOP_H__
