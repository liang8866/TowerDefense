#include "Ui_LoginAward.h"
#include "PlistManager.h"
#include "UserRecord.h"
USING_NS_CC;
Ui_LoginAward::Ui_LoginAward(void)
{
}

Ui_LoginAward::~Ui_LoginAward(void)
{
}
bool Ui_LoginAward::init()
{
	if (!Widget::init())
	{
		return false;
	}


	bool Ret = false;
	do{
		this->addChild(LayerColor::create(Color4B(0,0,0,100)));
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Login_Awrad/GameUI0.plist","Ui_Login_Awrad/GameUI0.png");
		_widget = GUIReader::getInstance()->widgetFromJsonFile("Ui_Login_Awrad/Ui_Login_Awrad.ExportJson");
		CC_BREAK_IF(!_widget);
		this->addChild(_widget);
		_config = FileUtils::getInstance()->getValueMapFromFile("plist/login.plist");
		CC_BREAK_IF(_config.empty());
		Widget* temp = Helper::seekWidgetByName(_widget,"temp");
		Widget* _item = Helper::seekWidgetByName(temp,"item");
		Widget* items_warp = Helper::seekWidgetByName(_widget,"items_warp");
		int loginDay = UserRecord::getInstance()->getLoginDayNum();

		ImageView* shine1 = static_cast<ImageView*>(Helper::seekWidgetByName(_widget,"Image_27"));
		ImageView* shine2 = static_cast<ImageView*>(shine1->getChildByName("Image_27_0"));
		static_cast<Sprite*>(shine1->getVirtualRenderer())->getTexture()->setAliasTexParameters();
		static_cast<Sprite*>(shine2->getVirtualRenderer())->getTexture()->setAliasTexParameters();

		for (int i=1;i!=5;++i){
			Widget* dorn = Helper::seekWidgetByName(_widget,StringUtils::format("dorn_%d",i).c_str());
			dorn->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.8f,Point(0,5)),MoveBy::create(0.6f,Point(0,-5)),NULL)));
		}
		
		for (int i=1;i<=_config.size();++i)
		{
			Widget* cloner = _item->clone();
			ValueMap day= _config.at(Value(i).asString()).asValueMap();
			std::string info = day.at("info").asString();
			auto label1 = Label::createWithSystemFont(info, "Î¢ÈíÑÅºÚ", 16);
			label1->setColor(Color3B(216,255,203));
			label1->enableOutline(Color4B(92,31,40,255),1);
			label1->setAnchorPoint(Point::ZERO);
			cloner->getChildByName("day_num")->addChild(label1);
			cloner->setPosition(Point(5+(i-1)*(cloner->getSize().width*1.1f),0));
			 if (loginDay == i)
			{
				cloner->getChildByName("border")->getChildByName("bg")->removeFromParentAndCleanup(true);
				cloner->setScale(1.1f);
				cloner->setPositionY(cloner->getPositionY()  - (cloner->getSize().height*0.1f)/2);
				cloner->setPositionX( cloner->getPositionX() - (cloner->getSize().width*0.1f)/2);
			} else {
				auto bg = cloner->getChildByName("border")->getChildByName("bg");
				bg->setVisible(true);
				if (i< loginDay) {
					Widget* obj = temp->getChildByName("isget")->clone();
					cloner->getChildByName("items")->addChild(obj,2);
					obj->setPosition(Point(cloner->getChildByName("items")->getSize().width/2,cloner->getChildByName("items")->getSize().height/2));
				} 
			}
			ValueMap awardForDay = day.at("award").asValueMap();
			ValueMap::iterator itr = awardForDay.begin();
			int k=0;
			while (itr != awardForDay.end())
			{
				Widget* obj = Helper::seekWidgetByName(temp,itr->first.c_str())->clone();
				ui::TextBMFont* num = static_cast<ui::TextBMFont*>(obj->getChildByName("text"));
				num->setText(itr->second.asString());
				auto itemswp  = cloner->getChildByName("items");
				itemswp->addChild(obj);
				obj->setPosition(Point(0,
					(itemswp->getSize().height-obj->getSize().height)-obj->getSize().height*k));
				++itr;
				++k;
			}
			items_warp->addChild(cloner);
		}
		temp->removeFromParentAndCleanup(true);
		Button* add = dynamic_cast<Button*>(Helper::seekWidgetByTag(_widget,11575));
		add->addTouchEventListener(this, toucheventselector(Ui_LoginAward::eventTouchButton));
		clickFlag = false;
		Ret =  true;
	}while(0);

	fitScreen();
	return Ret;
}
void Ui_LoginAward::onEnter()
{
	Widget::onEnter();
}
void Ui_LoginAward::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Login_Awrad/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_Login_Awrad/GameUI0.png");
	Widget::onExit();
}

void Ui_LoginAward::eventTouchButton(Ref* sender, TouchEventType type)
{
	if (clickFlag)
	{
		return;
	}
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		clickFlag = true;
		Widget* btn = dynamic_cast<Widget*>(sender);
		//ÁªÍø
		Connect* handle = Connect::getInstance();
		handle->loadSend("c=award",this, httpresponse_selector(Ui_LoginAward::onHttpRequestCompleted));
	}
}

void Ui_LoginAward::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	//Connect::getInstance()->closeLoadPanel();
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
	{  
		clickFlag = false;
		return ;  
	}  
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		UserRecord::getInstance()->setIsAward(true);
		int loginDay = UserRecord::getInstance()->getLoginDayNum();
		ValueMap day = _config.at(Value(loginDay).asString()).asValueMap();
		ValueMap awardForDay = day.at("award").asValueMap();
		ValueMap::iterator itr = awardForDay.begin();
		while (itr != awardForDay.end())
		{
			if (itr->first == "gold")
			{
				UserRecord::getInstance()->alterGold(itr->second.asInt());
			} else if(itr->first == "hero_soul") {
				UserRecord::getInstance()->alterHeroSoul(itr->second.asInt());
			} else if(itr->first == "wish_egg") {
				//30002
				UserRecord::getInstance()->alterGoodsCount("30002",itr->second.asInt());
			} else if(itr->first == "advance_egg") {
				//30001
				UserRecord::getInstance()->alterGoodsCount("30001",itr->second.asInt());
			}
			++itr;
		}
	} else {
		clickFlag = false;
	}
	this->removeFromParentAndCleanup(true);
}

void Ui_LoginAward::fitScreen()
{
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640); 

	_widget->setPosition(Point(originPt.x, originPt.y));

	Widget *panel_mid = _widget->getChildByName("Panel_mid");
	panel_mid->setScale(_scale);
	panel_mid->setPosition(Point(visibleSize.width/2  - panel_mid->getContentSize().width/2*_scale ,
								 visibleSize.height/2 - panel_mid->getContentSize().height/2*_scale));

	Widget* redbg = panel_mid->getChildByName("red_bg");
	redbg->setScaleX((visibleSize.width/960) * (1/_scale));

	Widget* yellowbg = panel_mid->getChildByName("yellow_bg"); 
	yellowbg->setScaleX((visibleSize.width/960) * (1/_scale));
}