#include "StoreyData.h"
#include "bossObjects.h"
#include "WindowManager.h"
#include "Ui_HeroAltar.h"
#include "SceneData.h"
#include "Ui_MainBack.h"
#include "headstoneObjects.h"
#include "Event1Object.h"
#include "audioPlayer.h"

StoreyData::StoreyData(void):
strawberryCnt(nullptr),
mintCnt(nullptr),
lemonCnt(nullptr)
{
}
StoreyData::~StoreyData(void)
{
}

StoreyData::StoreyData(size_t _index)
{
	if (!__init())
		return;
	auto _lastStoreyIndex = SceneData::getInstance()->getLastStoreyIndex();
	int randomType1 = CCRANDOM_0_1()*3;
	mStoreyType = randomType1;
	// ����㼶֮�䲻ͬ��������ʾ
	auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_lastStoreyIndex);
	if (_curStoreyData)
	{
		while (true)
		{
			randomType1 = CCRANDOM_0_1()*3;
			mStoreyType = randomType1;
			if (mStoreyType != _curStoreyData->mStoreyType)
			{
				break;
			}
		}			
	}

	const char* jsonFile = "BackCell_1/BackCell_1.ExportJson";
	if (mStoreyType == 0){
		jsonFile = "BackCell_1/BackCell_1.ExportJson";
		//���ű�������
		
	}
	else if (mStoreyType == 1){
		jsonFile = "BackCell_2/BackCell_2.ExportJson";
		//���ű�������
		
	}
	else if (mStoreyType == 2){

		jsonFile = "BackCell_3/BackCell_3.ExportJson";
		//���ű�������
		
	}
	Widget* _newItemCell = dynamic_cast<Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonFile));
	if (!_newItemCell)
		return;

	mIndex = _index;
	mStoreyIndex = _index;
	mItem = _newItemCell;
	updateCell();
}

StoreyData::StoreyData(size_t _index, Widget* _item)
{
	if (!__init())
		return;

	mIndex = _index;
	mStoreyIndex = _index;
	mItem = _item;
	updateCell();
}
StoreyData::StoreyData(size_t _uiIndex, size_t _storeyIndex)
{
	if (!__init())
		return;

	mIndex			= _uiIndex;
	mStoreyIndex	= _storeyIndex;
	mStoreyType		= 0;
	const char* jsonFile = "BackCell_1/BackCell_1.ExportJson";
	if (mStoreyIndex == SCENE_STOREY_TOP)
	{
		mStoreyType		= 0;
		jsonFile = "BackCell_1/BackCell_1.ExportJson";
	}
	else if (mStoreyIndex == SCENE_STOREY_HOME)
	{
		mStoreyType		= 3;
		jsonFile = "BackCell_Home/BackCell_Home.ExportJson";
		//this->schedule(schedule_selector(StoreyData::updatePropsNum), 1.0);
		
	}
	else{
		auto _lastStoreyIndex = SceneData::getInstance()->getLastStoreyIndex();
		int randomType1 = CCRANDOM_0_1()*3;
		mStoreyType = randomType1;
		// ����㼶֮�䲻ͬ��������ʾ
		auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_lastStoreyIndex);
		if (_curStoreyData)
		{
			while (true)
			{
				randomType1 = CCRANDOM_0_1()*3;
				mStoreyType = randomType1;
				if (mStoreyType != _curStoreyData->mStoreyType)
				{
					break;
				}
			}			
		}
		if (mStoreyType == 0)
			jsonFile = "BackCell_1/BackCell_1.ExportJson";
		else if (mStoreyType == 1)
			jsonFile = "BackCell_2/BackCell_2.ExportJson";
		else if (mStoreyType == 2)
			jsonFile = "BackCell_3/BackCell_3.ExportJson";
	}
	Widget* _newItemCell = dynamic_cast<Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonFile));
	if (!_newItemCell)
		return;
	mItem = _newItemCell;

	do{
		ImageView *leftDoor = static_cast<ImageView*>(_newItemCell->getChildByName("Image_CellLeft"));
		CC_BREAK_IF(!leftDoor);
		Sprite* _left = static_cast<Sprite*>(leftDoor->getVirtualRenderer());
		_left->getTexture()->setAliasTexParameters();

		ImageView *rightDoor = static_cast<ImageView*>(_newItemCell->getChildByName("Image_CellRight"));
		CC_BREAK_IF(!rightDoor);
		Sprite* _right = static_cast<Sprite*>(rightDoor->getVirtualRenderer());
		_right->getTexture()->setAliasTexParameters();
	}while(0);

	//���߰�ť
	if (mStoreyIndex == SCENE_STOREY_HOME){
		strawberryBtn = static_cast<Button*>(_newItemCell->getChildByName("Button_caomei"));
		mintBtn       = static_cast<Button*>(_newItemCell->getChildByName("Button_bohe"));
		lemonBtn      = static_cast<Button*>(_newItemCell->getChildByName("Button_ningmeng"));

		strawberryBtn->addTouchEventListener(this,toucheventselector(StoreyData::propBtnsTouchEvent));
		mintBtn      ->addTouchEventListener(this,toucheventselector(StoreyData::propBtnsTouchEvent));
		lemonBtn     ->addTouchEventListener(this,toucheventselector(StoreyData::propBtnsTouchEvent));

		strawberryCnt = static_cast<TextBMFont*>(Helper::seekWidgetByName(mItem, "prop_cnt_caomei"));
		mintCnt       = static_cast<TextBMFont*>(Helper::seekWidgetByName(mItem, "prop_cnt_bohe"));
		lemonCnt      = static_cast<TextBMFont*>(Helper::seekWidgetByName(mItem, "prop_cnt_ningmeng"));

		strawberryCnt->setText(StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10001")));
		mintCnt      ->setText(StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10002")));
		lemonCnt     ->setText(StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10003")));

		if (UserRecord::getInstance()->getGoodsCountByID("10001")<=0){
			
			Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_caomei"));
			strawberryCnt->setVisible(false);
			propAdd->setVisible(true);

		}
		if (UserRecord::getInstance()->getGoodsCountByID("10002")<=0){
			
			Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_bohe"));
			mintCnt->setVisible(false);
			propAdd->setVisible(true);
		}
		if (UserRecord::getInstance()->getGoodsCountByID("10003")<=0){
			

			Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_ningmeng"));
			lemonCnt->setVisible(false);
			propAdd->setVisible(true);
		}
	}

	updateCell();
}
bool StoreyData::__init()
{
	mIndex = 0;
	mStoreyIndex = 0;
	mItem = nullptr;
	mObjMaxCount = 5;
	mObjectData.clear();

	mButtonHomeHeroAltar = nullptr;
	mButtonCreateHero = nullptr;
	mButtonMoveLeft = nullptr;
	mButtonMoveRight = nullptr;
	mButtonAddStorey = nullptr;
	mImageAnim = nullptr;
	mArmature1 = nullptr;
	mArmature2 = nullptr;
	mImageGanTanHao = nullptr;
	mImageGuanTanHaoCount = 0;
	mIsYuJing = false;
	mIsProAddStorey = false;
	mImageLineQZ = nullptr;
	mImageForbid = nullptr;

	return true;
}
void StoreyData::updateCell()
{
	if (!mItem)
		return;

	// ���ò�������ʾ
	Text* textCtrl = static_cast<Text*>(mItem->getChildByName("Lable_Num"));
	if (textCtrl)
		textCtrl->setText(StringUtils::format("%d", mStoreyIndex));

	// ���ص�ƽ��
	Widget* _thisItemHorizonY = mItem->getChildByName("HorizonY");
	if (_thisItemHorizonY)
		_thisItemHorizonY->setVisible(false);
	
	// ���ز���Ա����
	Widget* _thisItemRectCtrl = mItem->getChildByName("Image_Rect");
	if (_thisItemRectCtrl)
		_thisItemRectCtrl->setVisible(false);

	// ���ز��϶�����
	for (int i = 1; i <= 5; i++){
		char _curName[32];
		sprintf(_curName, "Image_DragView%d", i);  
		Widget* _thisDragView = mItem->getChildByName(_curName);
		if (_thisDragView)
			_thisDragView->setVisible(false);
	}
	// ���ز㴴������
	for (int i = 1; i <= 5; i++){
		char _curName[32];
		sprintf(_curName, "Image_CreateView%d", i);  
		Widget* _thisCreateView = mItem->getChildByName(_curName);
		if (_thisCreateView)
			_thisCreateView->setVisible(false);
	}
	if (!mButtonHomeHeroAltar)
	{
		mButtonHomeHeroAltar = static_cast<Button*>(Helper::seekWidgetByName(mItem, "Button_HeroAltar"));
		if (mButtonHomeHeroAltar)
		{
			mButtonHomeHeroAltar->addTouchEventListener(this, toucheventselector(StoreyData::onButtonEvent));
		}
	}
	if (!mButtonCreateHero)
	{
		mButtonCreateHero = static_cast<Button*>(Helper::seekWidgetByName(mItem, "Button_HeroBoss"));
		 
		if (mButtonCreateHero)
		{
			mButtonCreateHero->addTouchEventListener(this, toucheventselector(StoreyData::onButtonEvent));
		}
	}
	if (!mButtonMoveLeft)
	{
		mButtonMoveLeft = static_cast<Button*>(Helper::seekWidgetByName(mItem, "Button_MoveLeft"));
		if (mButtonMoveLeft)
		{
			mButtonMoveLeft->addTouchEventListener(this, toucheventselector(StoreyData::onButtonEvent_Move));
			mButtonMoveLeft->setVisible(false);
			mButtonMoveLeft->setTouchEnabled(false);
		}
	}
	if (!mButtonMoveRight)
	{
		mButtonMoveRight = static_cast<Button*>(Helper::seekWidgetByName(mItem, "Button_MoveRight"));
		if (mButtonMoveRight)
		{
			mButtonMoveRight->addTouchEventListener(this, toucheventselector(StoreyData::onButtonEvent_Move));
			mButtonMoveRight->setVisible(false);
			mButtonMoveRight->setTouchEnabled(false);
		}
	}
	if (!mButtonAddStorey)
	{
		mButtonAddStorey = static_cast<Button*>(Helper::seekWidgetByName(mItem, "Button_AddStorey"));
		if (mButtonAddStorey)
		{
			mButtonAddStorey->addTouchEventListener(this, toucheventselector(StoreyData::onButtonEvent));
		}
	}
	if (!mImageAnim)
	{
		mImageAnim = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_Anim"));
		if (mImageAnim)
		{
			mImageAnim->setVisible(false);
		}
	}
	if (!mImageGanTanHao)
	{
		mImageGanTanHao = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_GanTanHao"));
		if (mImageGanTanHao)
		{
			mImageGanTanHao->setVisible(false);
		}
	}
	ImageView* imagePro = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_NoAdd"));
	if (imagePro)
	{
		imagePro->setVisible(false);
	}
	if (!mImageLineQZ)
	{
		mImageLineQZ = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_QZ"));
		if (mImageLineQZ)
		{
			mImageLineQZ->setVisible(false);
		}
	}
	if (!mImageForbid)
	{
		mImageForbid = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_Forbid"));
		if (mImageForbid)
		{
			mImageForbid->setVisible(false);
		}
	}
}

Widget* StoreyData::getHorizon()
{
	if (!mItem)
		return nullptr;
	return Helper::seekWidgetByName(mItem, "HorizonY");
}

Widget* StoreyData::getLeftDoor()
{
	if (!mItem)
		return nullptr;
	return Helper::seekWidgetByName(mItem, "Image_CellLeft");
}

Widget* StoreyData::getRightDoor()
{
	if (!mItem)
		return nullptr;
	return Helper::seekWidgetByName(mItem, "Image_CellRight");
}
Widget* StoreyData::getMovementWidget()
{
	if (!mItem)
		return nullptr;
	return Helper::seekWidgetByName(mItem, "Image_Rect");
}

bool StoreyData::findObject(GameObjects* _obj)
{
	if (!_obj)
		return false;
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter) == _obj)
		{
			return true;
		}
		++iter;
	}
	return false;
}
void StoreyData::addObject(GameObjects* _obj)
{
	if (!_obj)
		return;
	mObjectData.push_back(_obj);
}

void StoreyData::removeObject(GameObjects* _obj)
{
	if (!_obj)
		return;
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter) == _obj)
		{
			mObjectData.erase(iter);
			break;
		}
		++iter;
	}
}
void StoreyData::clearObject()
{
	mObjectData.clear();
}

// ȡboss
GameObjects* StoreyData::getHero()
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_BOSS)
		{
			return *iter;
		}
		++iter;
	}
	return nullptr;
}

bool StoreyData::isCanMouseClick()
{
	if (getHero())
	{
		return true;
	}
	return false;
}


// ȡboss
GameObjects* StoreyData::getDoor()
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_DOOR)
		{
			return *iter;
		}
		++iter;
	}
	return nullptr;
}
void StoreyData::removeDoorObject()
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_DOOR)
		{
			mItem->removeChild(*iter);
			if (mItem->getChildByTag(SCENE_STOREY_DOOR_INDEX))
			{
				mItem->removeChildByTag(SCENE_STOREY_DOOR_INDEX);
			}
			//mItem->removeNode(*iter);
			//if (mItem->getNodeByTag(SCENE_STOREY_DOOR_INDEX))
			//	mItem->removeNodeByTag(SCENE_STOREY_DOOR_INDEX);
			mObjectData.erase(iter);
			break;
		}
		++iter;
	}
}

bool StoreyData::isClearMonster()
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_ENEMY)
		{
			return false;
		}
		++iter;
	}

	return true;
}

int  StoreyData::getObjCount()
{
	return mObjectData.size();
}

Vector<GameObjects*> StoreyData::findObjByType(int _objtype , int _objSelfType)
{
	Vector<GameObjects*> vec;
	vec.clear();
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	for (;iter!= mObjectData.end();++iter){
		if ((*iter)->getobjectType() == _objtype && (*iter)->getSelfType() == _objSelfType)
		{
			vec.pushBack(*iter);
		}
	}
	return vec;
}

void StoreyData::onButtonEvent(Object* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		Button* _curButton = static_cast<Button*>(sender);
		if (mButtonHomeHeroAltar && _curButton->getName() == mButtonHomeHeroAltar->getName())
		{
			// ��Ӣ�ۼ�̳����
			WindowManager::getInstance()->createHeroAltar();
		}else if (mButtonCreateHero && _curButton->getName() == mButtonCreateHero->getName())
		{
			// �򿪴���Ӣ�۽���
			if (UserRecord::getInstance()->getHP()<=0)//��Ϸʧ���˲��ܴ�Ӣ�۽���
			{
				return;
			}
			if (UserRecord::getInstance()->getRound()== UserRecord::getInstance()->getMaxRound() && SceneData::getInstance()->isClearAllMonster()){//��Ϸʧ����Ҳ���ܴ�
				return;
			}

			WindowManager::getInstance()->createHeroSelect(this->mStoreyIndex);

			//����������ѧ
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}
		}else if (mButtonAddStorey && _curButton->getName() == mButtonAddStorey->getName())
		{
			Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
			if (mainBack)
			{
				mainBack->setAddStoreyNum(1);
				mainBack->doAddStorey(true);

				//����������ѧ
				if (UserRecord::getInstance()->getIsFreshMan()){
					UserRecord::getInstance()->setFreshManMutex(false);
				}
			}
		}
		break;
	}
}
void StoreyData::onButtonEvent_Move(Object* sender, TouchEventType type)
{
	Button* _curButton = static_cast<Button*>(sender);
	auto _hero = getHero();
	if (!_hero)
	{
		return;
	}
	auto _curHero = static_cast<bossObjects*>(_hero);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		if (mButtonMoveLeft && _curButton->getName() == mButtonMoveLeft->getName())
		{
			_curHero->beginPressBossRun(D_LEFT);
		}else if (mButtonMoveRight && _curButton->getName() == mButtonMoveRight->getName())
		{
			_curHero->beginPressBossRun(D_RIGHT);
		}

		break;
	case TOUCH_EVENT_ENDED:
	case TOUCH_EVENT_CANCELED:
		_curHero->endPressBossRun();
		break;
	}
}
void StoreyData::setIsCanMove(bool _isBool)
{
	mButtonMoveLeft->setVisible(_isBool);
	mButtonMoveLeft->setTouchEnabled(_isBool);
	mButtonMoveRight->setVisible(_isBool);
	mButtonMoveRight->setTouchEnabled(_isBool);
	if (_isBool)
	{
		mButtonMoveLeft->setTouchPriority(-20);
		mButtonMoveRight->setTouchPriority(-20);
	}else
	{
		mButtonMoveLeft->setTouchPriority(20);
		mButtonMoveRight->setTouchPriority(20);
	}
}

// ����1��ײ����ʯ
void StoreyData::createArmature1()
{	
	if (!mArmature1)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("kaicheng_posui_010.png","kaicheng_posui_010.plist","kaicheng_posui_01.ExportJson"); 
		mArmature1 = cocostudio::Armature::create("kaicheng_posui_01");   
		if (mArmature1)
		{
			mArmature1->setPosition(mItem->getSize().width/2, mItem->getSize().height/2);
			mArmature1->setTag(20);
			mItem->addChild(mArmature1, 21);
			mArmature1->setPositionX(mImageAnim->getPositionX() - 60);	
			mArmature1->setPositionY(mImageAnim->getPositionY() - mImageAnim->getSize().height/2 + 10);
		}
	}
}
void StoreyData::beginArmature1()
{	
	if (!mArmature1)
	{
		createArmature1();
	}
	if (mArmature1)
	{
		audioPlayer::getInstance()->playEffect("S62.mp3");	// ������Ч
		mArmature1->getAnimation()->play("Animation1", -1, 0);
		//mArmature1->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(StoreyData::onAnimationFrameEvent1));

		auto delay=DelayTime::create(0.5f);
		auto ddlay = Sequence::create(
			delay,
			CallFuncN::create(CC_CALLBACK_1(StoreyData::doArmature2, this)),
			NULL);
		mArmature2->runAction(ddlay);
	}
}
void StoreyData::closeArmature1()
{	
	if (mArmature1)
	{
		mArmature1->getAnimation()->stop();
		mArmature1->removeAllChildrenWithCleanup(true);
		mArmature1 = nullptr;
		
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("kaicheng_posui_010.plist");
		TextureCache::getInstance()->removeTextureForKey("kaicheng_posui_010.png");
		ArmatureDataManager::getInstance()->removeArmatureFileInfo("kaicheng_posui_01.ExportJson");
	}
}
void StoreyData::onAnimationFrameEvent1(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		// ���Ŷ���2
		//beginArmature2();
	}
}

// ����2��������ʯ
void StoreyData::doArmature2(Ref* sender)
{	
	beginArmature2();
}
void StoreyData::createArmature2()
{	
	if (!mArmature2)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("kaicheng0.png","kaicheng0.plist","kaicheng.ExportJson"); 
		mArmature2 = cocostudio::Armature::create("kaicheng");   
		if (mArmature2)
		{
			mArmature2->setPosition(mItem->getSize().width/2, mItem->getSize().height/2);
			mArmature2->setTag(20);
			mItem->addChild(mArmature2, 20);
			mArmature2->setPositionX(mImageAnim->getPositionX() - 10);	
			mArmature2->setPositionY(mImageAnim->getPositionY() - mImageAnim->getSize().height/2 + 10);
		}
	}
}
void StoreyData::beginArmature2()
{	
	if(!mArmature2)
	{
		createArmature2();
	}
	if (mArmature2)
	{
		mArmature2->getAnimation()->play("kaicheng", -1, 0);
		mArmature2->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(StoreyData::onAnimationFrameEvent2));
	}
}
void StoreyData::closeArmature2()
{	
	if (mArmature2)
	{
		mArmature2->getAnimation()->stop();
		mArmature2->removeAllChildrenWithCleanup(true);
		mArmature2 = nullptr;

		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("kaicheng0.plist");
		TextureCache::getInstance()->removeTextureForKey("kaicheng0.png");
		ArmatureDataManager::getInstance()->removeArmatureFileInfo("kaicheng.ExportJson");
	}
}
void StoreyData::onAnimationFrameEvent2(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		closeArmature1();
		closeArmature2();
		Ui_MainBack* mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
		if (mainBack)
		{
			mainBack->onAddStoreyAinmFinish();
		}
	}
}

// ��̾�Ŷ���
void StoreyData::beginGamTanHaoAction()
{	
	if (!mImageGanTanHao)
		return;
	mIsYuJing = true;
	mImageGanTanHao->setVisible(true);
	auto _run1 = RotateTo::create(0.2f, 15);
	auto _run2 = RotateTo::create(0.2f, 0);

	
	auto _actions = Sequence::create(
		_run1,
		_run2,
		CallFuncN::create(CC_CALLBACK_1(StoreyData::onGanTanHaoCallBack, this)),
		NULL);
	mImageGanTanHao->runAction(_actions);

	Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
	if (mainBack)
	{
		mainBack->updateYuJing();
	}
}

// ��̾�Ŷ���
void StoreyData::onGanTanHaoCallBack(Ref* sender)
{	
	mImageGuanTanHaoCount = mImageGuanTanHaoCount + 1;
	if (mImageGuanTanHaoCount < 7)
	{
		beginGamTanHaoAction();
	}else
	{
		Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));
		if (mainBack)
		{
			mainBack->updateYuJing();
		}
		mImageGanTanHao->setVisible(false);
		mImageGuanTanHaoCount = 0;
		mIsYuJing = false;
		return;
	}
}
// ��̾��״̬
bool StoreyData::isYuJing()
{	
	return mIsYuJing;
}


void StoreyData::propBtnsTouchEvent(Ref* pSender,TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	Button *btn = static_cast<Button*>(pSender);
	int itemID = 0;
	audioPlayer::getInstance()->playEffect("S61.mp3");
	if (strcmp(btn->getName(),"Button_caomei")==0){
		if (UserRecord::getInstance()->getGoodsCountByID("10001")<1){

			WindowManager::getInstance()->createShopFrame();
		}
		else
		{
			audioPlayer::getInstance()->playEffect("S61.mp3");	// ��ݮ��ʹ����Ч
			UserRecord::getInstance()->setGoodsCount("10001",StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10001")-1));
			if (UserRecord::getInstance()->getGoodsCountByID("10001")<1){//���������֮��С��0�Ļ���ʾ�Ӻ�
				Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_caomei"));
				strawberryCnt->setVisible(false);
				propAdd->setVisible(true);
			}

			btn->setGrey();
			btn->runAction(Sequence::create(DelayTime::create(30.0f),
				CallFuncN::create(CC_CALLBACK_1(StoreyData::resumeStrawberryBtnTouch,this)),
				NULL
				));

			Widget *warp = this->mItem->getChildByName("sweet_warp");
			if (warp){
				AltarObject *altar = static_cast<AltarObject*>(warp->getChildByTag(AlTAR_MY_TAG));
				altar->propsAddProduct(30.0f);
			}

			//�ǹ���ʾЧ��
			Point pos = btn->getPosition();
			Point endPos = Point(warp->getPositionX()+warp->getSize().width/2,warp->getPositionY()+warp->getSize().height/2);
			Sprite* icon = Sprite::createWithSpriteFrameName("image/UI_zjm_24.png");
			icon->setPosition(pos);
			mItem->addChild(icon,10);

			icon->runAction(Sequence::create(MoveTo::create(0.3f,endPos),CallFunc::create([=](){
				icon->removeFromParentAndCleanup(true);
			}),NULL));


			itemID =10001;
		}

	
	}else if (strcmp(btn->getName(),"Button_bohe")==0){
		if (UserRecord::getInstance()->getGoodsCountByID("10002")<1)
		{
			WindowManager::getInstance()->createShopFrame();
		}else{
			UserRecord::getInstance()->setGoodsCount("10002",StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10002")-1));
			if (UserRecord::getInstance()->getGoodsCountByID("10002")<1){//���������֮��С��0�Ļ���ʾ�Ӻ�
				Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_bohe"));
				mintCnt->setVisible(false);
				propAdd->setVisible(true);
			}
			Point pos = btn->getWorldPosition();
/*			CCLOG("pos.x=%f,pos.y=%f",pos.x,pos.y);*/

			Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
			if (mainLayer)
				mainLayer->setDynamicSweetNum(3000,pos);

			itemID =10002;
		}

	
	}else if (strcmp(btn->getName(),"Button_ningmeng")==0){
		if (UserRecord::getInstance()->getGoodsCountByID("10003")<1){
			WindowManager::getInstance()->createShopFrame();
		}else{
			UserRecord::getInstance()->setGoodsCount("10003",StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10003")-1));
			if (UserRecord::getInstance()->getGoodsCountByID("10003")<1){//���������֮��С��0�Ļ���ʾ�Ӻ�
				Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_ningmeng"));
				lemonCnt->setVisible(false);
				propAdd->setVisible(true);
			}
			
			UserRecord::getInstance()->setIsObjsNoCD(true);
			btn->setGrey();
			btn->runAction(Sequence::create(DelayTime::create(10.0f),
				CallFuncN::create(CC_CALLBACK_1(StoreyData::resumeLemonBtnTouch,this)),
				NULL
				));

			Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
			if (mainLayer){
				mainLayer->enterObjsNoCDstatus();

				//��λ��CD��ʾЧ��
				Point pos = btn->getPosition();
				Point endPos = mItem->convertToNodeSpace(mainLayer->currPanel->getPosition());
				Sprite* icon = Sprite::createWithSpriteFrameName("image/UI_zjm_24.png");
				icon->setPosition(pos);
				mItem->addChild(icon,10);

				icon->runAction(Sequence::create(MoveTo::create(0.3f,endPos),CallFunc::create([=](){
					icon->removeFromParentAndCleanup(true);
				}),NULL));

			}
			itemID =10003;
		}
	}
	if (itemID != 0)
	{
		//֪ͨ������������
		auto postDatas = __String::createWithFormat("c=props&iid=%d",itemID)->getCString();
		Connect::getInstance()->send(postDatas,this,httpresponse_selector(StoreyData::onHttpRequestCompleted));
	}
}

void StoreyData::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		//����
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		//����ɹ�


	}  else {
		//����ʧ��
		/*NotificationCenter::getInstance()->postNotification(PAUSEKEY, (Ref*)1);
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		auto Text = PlistManager::getInstance()->getPlistMap("Text");
		__String* TextStr = static_cast<__String*>(Text->objectForKey("1001"));
		prompt->initWithRetry(TextStr->getCString());
		Director::getInstance()->getRunningScene()->addChild(prompt,300);*/
	}
	
}
void StoreyData::resumeStrawberryBtnTouch(Ref* pSender)
{
	Button *btn = static_cast<Button*>(pSender);
	btn->removeGrey();
// 	if (UserRecord::getInstance()->getGoodsCountByID("10001")>0) 
// 		btn->removeGrey();
}

void StoreyData::resumeLemonBtnTouch(Ref* pSender)
{

	Button *btn = static_cast<Button*>(pSender);
	//if (UserRecord::getInstance()->getGoodsCountByID("10003")>0) 
		btn->removeGrey();
	UserRecord::getInstance()->setIsObjsNoCD(false);

	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	if (mainLayer)
		mainLayer->leaveObjsNoCDstatus();
}

bool StoreyData::isFinishBatch(int _batch)
{
	if (_batch < 0)
		return false;

	int count = 0;
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if (*iter != nullptr)
		{
			if ((*iter)->getobjectType() == TYPE_ENEMY)
			{
				int _curBatch = (*iter)->getBatch();
				if (_curBatch >= 0 && _curBatch == _batch)
				{
					count = count + 1;
				}
			}
		}
		++iter;
	}

	if (count > 0)
	{
		return false;
	}
	return true;
}

// �����¼�103���ƿ��㶯��
void StoreyData::doEvent103Action()
{
	ImageView* imagePro = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_NoAdd"));
	if (imagePro)
	{
		imagePro->setVisible(true);
		imagePro->setOpacity(0);
		imagePro->setScale(5.0f);
		auto _defaultPos = imagePro->getPosition();
		auto action1 = FadeIn::create(0.3f);
		auto action2 = ScaleTo::create(0.3f, 0.5f, 0.5f);
		auto actions = Sequence::create(action1, action2, CallFuncN::create(CC_CALLBACK_1(StoreyData::doEvent103ActionCallBack, this)),NULL);
		imagePro->runAction(actions);
	}
}
// 103���㶯���������
void StoreyData::doEvent103ActionCallBack(Ref* _sender)
{
	setIsProAddStorey(true);
}
// �����Ƿ����ƿ���
void StoreyData::setIsProAddStorey(bool _isPro)
{
	mIsProAddStorey = _isPro;
	ImageView* imagePro = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_NoAdd"));
	if (imagePro)
	{
		if (_isPro)
		{
			imagePro->setVisible(true);
			setWidgetGray(mButtonAddStorey, true);
			mButtonAddStorey->setTouchEnabled(false);
		}else
		{
			imagePro->setVisible(false);
			setWidgetGray(mButtonAddStorey, false);
			mButtonAddStorey->setTouchEnabled(true);
		}
	}
}
bool StoreyData::getIsProAddStorey()
{
	return mIsProAddStorey;
}


void StoreyData::setWidgetGray(Widget* _widget, bool _isGray)
{
	if (_widget)
	{
		if (_isGray)
		{
			_widget->setGrey();
		}else
		{
			_widget->removeGrey();
		}
	}
}


void StoreyData::updatePropsNum(float dt){//����home ����ĵ��ߵ���Ŀ����ֹ�������ʾ����������

	
	strawberryCnt->setText(StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10001")));
	mintCnt      ->setText(StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10002")));
	lemonCnt     ->setText(StringUtils::format("%d",UserRecord::getInstance()->getGoodsCountByID("10003")));

	if (UserRecord::getInstance()->getGoodsCountByID("10001")<=0){
	
		Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_caomei"));
		strawberryCnt->setVisible(false);
		propAdd->setVisible(true);

	}else{
		Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_caomei"));
		strawberryCnt->setVisible(true);
		propAdd->setVisible(false);
	}


	if (UserRecord::getInstance()->getGoodsCountByID("10002")<=0){
	
		Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_bohe"));
		mintCnt->setVisible(false);
		propAdd->setVisible(true);
	}else
	{
		Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_bohe"));
		mintCnt->setVisible(true);
		propAdd->setVisible(false);
	}




	if (UserRecord::getInstance()->getGoodsCountByID("10003")<=0){
		
		Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_ningmeng"));
		lemonCnt->setVisible(false);
		propAdd->setVisible(true);
	}else
	{
		Widget*propAdd=static_cast<Text*>(Helper::seekWidgetByName(mItem, "prop_add_ningmeng"));
		lemonCnt->setVisible(true);
		propAdd->setVisible(false);
	}



}
// ���ò������ڵ�������ʾ�ر�
void StoreyData::setLineQZVisible(bool _isBool)
{
	if (mImageLineQZ)
	{
		mImageLineQZ->setVisible(_isBool);
	}
}
// ����Home ����������ʾ
void StoreyData::updateAddStoreyCostView(int _num)
{
	if (_num <= 0 || _num > 9999)
		return;
	ImageView* _imageQian = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_Qian"));
	ImageView* _imageBai = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_Bai"));
	ImageView* _imageShi = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_Shi"));
	ImageView* _imageGe = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_Ge"));
	if (!_imageQian || !_imageBai || !_imageShi || !_imageGe)
		return;

	int a = _num;
	int _ge, _shi, _bai, _qian;
	_ge		= a%10;
	_shi	= ((a-_ge)/10)%10;
	_bai	= ((a-(_shi*10)-_ge)/100)%10;
	_qian	= ((a-(_bai*100)-(_shi*10)-_ge)/1000)%10;

	_imageQian->setVisible(false);
	_imageBai->setVisible(false);
	_imageShi->setVisible(false);
	_imageGe->setVisible(false);
	if (_qian <= 0 && _bai <= 0 && _shi <= 0 && _ge <= 0)
	{
		_imageGe->setVisible(true);
		updateImageCtrlNumber(_imageGe, 0);
	}else if (_qian <= 0 && _bai <= 0 && _shi <= 0)
	{
		_imageGe->setVisible(true);
		updateImageCtrlNumber(_imageGe, _num);
	}else if (_qian <= 0 && _bai <= 0)
	{
		_imageGe->setVisible(true);
		_imageShi->setVisible(true);
		updateImageCtrlNumber(_imageGe, _ge);
		updateImageCtrlNumber(_imageShi, _shi);
	}else if (_qian <= 0)
	{
		_imageGe->setVisible(true);
		_imageShi->setVisible(true);
		_imageBai->setVisible(true);
		updateImageCtrlNumber(_imageGe, _ge);
		updateImageCtrlNumber(_imageShi, _shi);
		updateImageCtrlNumber(_imageBai, _bai);
	}else
	{
		_imageGe->setVisible(true);
		_imageShi->setVisible(true);
		_imageBai->setVisible(true);
		_imageQian->setVisible(true);
		updateImageCtrlNumber(_imageGe, _ge);
		updateImageCtrlNumber(_imageShi, _shi);
		updateImageCtrlNumber(_imageBai, _bai);
		updateImageCtrlNumber(_imageQian, _qian);
	}
}
// ����Home ����������ʾ
void StoreyData::updateImageCtrlNumber(ImageView* _imageCtrl, int _num)
{
	if (_imageCtrl)
	{
		if (_num >= 0 && _num <= 9)
		{
			auto _valueString = StringUtils::format("image/home_%d.png", _num);
			_imageCtrl->loadTexture(_valueString);
		}
	}
}
// ���ú�ɫ��ֹ���Ƿ���ʾ
void StoreyData::setForbidVisible(bool _isVisible)
{
	if (mImageForbid)
	{
		mImageForbid->setVisible(_isVisible);
	}
	ImageView* _imageNoAddTag = static_cast<ImageView*>(Helper::seekWidgetByName(mItem, "Image_NoAddTag"));
	if (_imageNoAddTag)
	{
		if (_isVisible)
		{
			if (!_imageNoAddTag->isVisible())
			{
				audioPlayer::getInstance()->playEffect("S14.mp3");
			}
		}
		_imageNoAddTag->setVisible(_isVisible);
	}
}
// ��������Ĺ��(������)��ͣ������ͣ
void StoreyData::isPauseHeadstone(bool _isBool)
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_HEADSTONE)
		{
			headstoneObjects* _curObj = static_cast<headstoneObjects*>(*iter);
			if (_curObj)
			{
				_curObj->isPauseHeadstone(_isBool);	
			}
		}
		++iter;
	}
}
// ȡboss
GameObjects* StoreyData::getHeadstone()
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_HEADSTONE)
		{
			return *iter;
		}
		++iter;
	}
	return nullptr;
}

// ��������event1��ͣ������ͣ
void StoreyData::setPause_AllEvent1(bool _isBool)
{
	std::list<GameObjects*>::iterator iter  = mObjectData.begin();
	while (iter != mObjectData.end())
	{
		if ((*iter)->getobjectType() == TYPE_EVENT1)
		{
			event1Object* _curObj = static_cast<event1Object*>(*iter);
			if (_curObj)
			{
				_curObj->setWarningPause(_isBool);	
			}
		}
		++iter;
	}
}

// �������в��̾���¼�
void StoreyData::setPause_GanTanHao(bool _isBool)
{
	if (mImageGanTanHao)
	{
		if (_isBool)
		{
			mImageGanTanHao->pause();
			mImageGanTanHao->pauseSchedulerAndActions();
		}else
		{
			mImageGanTanHao->resume();
			mImageGanTanHao->resumeSchedulerAndActions();
		}
	}
}