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
	bool       lock;		// 道具锁定
	int        price;
	bool       selected;
	string     name;

	int        unLockPrice;
	int		   objectType;
	int		   selfType;
	bool	   close;		// 道具禁止
	int        isComing;	// 敬请期待
	string     desc;		// 文字介绍
};

class _ReadyItemCellData
{
public:
	string		id;			// 道具id
	int			type;		// 道具类型
	bool		isComing;	// 是否敬请期待
	bool		isDesc;		// 道具是否显示Desc说明状态
	string		desc;
	string		name;		
	Widget*		itemCell;	// 道具cell格子
	int			price;
	int			unLockPrice;
	bool		lock;		// 道具锁定
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
	// 事件104 限制道具使用
	__Dictionary* doEvent104();

	void selectDesc(string unit_id);						// 选中进入说明状态
	void selectDescAction(Widget* _widget, bool _isBig);	// 说明状态的放大缩小动画
	bool isDesc(string unit_id);							// 是否是选中说明状态
	void createAnimArrows();								// 创建动态箭头
	void selectDescActionCallBack_Big(Ref* sender);

	void onButtonClick_UnLock(Ref *pSender , TouchEventType type);
	int getDescSelectIndex(string _id);	
	void updateSceneNumber();	// 更新关卡数字

	void createLineAction(Widget* _widget, float _speedTime = 0.5f, int _disY = 5);
public:
	map<int,vector<ReadyItemInfo> > _itemInfo;
	int      seatCount;
	int      occupySeatIndex;
	map<string,int> _vecObj; //int is seatindex
	
	static const int addSeatCost = 1000;

	Sprite* mArrows;	// 动态箭头
	vector<_ReadyItemCellData> mCurReadyItems;	// 当前页面格子数据
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

	Widget   *currWidget;  //当前底部携带栏模板

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
