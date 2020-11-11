#ifndef __CONNECT_H__
#define __CONNECT_H__
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "Ui_Loading.h"
class RequestMap : public cocos2d::Ref
{
public:
	std::string queryStr;
	Ref* pTarget;
	cocos2d::network::SEL_HttpResponse pSelector;
};
class Connect : public cocos2d::Ref
{
public:
	Connect();
	~Connect();
	static Connect* getInstance();
	void init();
	static void destroyInstance();
	//不显示加载面发送数据
	void send(std::string data,Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector);
	//显示加载面发送数据
	void loadSend(std::string data,Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void reLoginCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void reSend(int tag);
	void login(Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector);
	std::string encryptData(std::string);

	 /**
     * 重新登录。成功后重新执行上一次连接任务
     *
     * @param tag   
     */
	void reLogin(int tag);
	void logicUpdate(float dt); //定时器
	void doLoop();
	void cancelConnect();
	Ui_Loading* _widget;
	void createLoadPanel();
	void closeLoadPanel();
	void setAutoConnect(bool enabled);
	CC_SYNTHESIZE(bool,_loadEnabled,LoadEnabled); //load界面开关
	CC_SYNTHESIZE(bool,_pauseGame,PauseGame); //连接时是否暂停游戏
private:
	cocos2d::Map<int,RequestMap*> _requests;
	bool _autoConnect; //自动连接开关
	int _reLoginTag;
	bool reLoginFlag;
	 int _autoTag; 
	 int _connectCount;
	 int _chronon;//定时单位
	 int _deltaTime; //update自增计时
	 int _lastTime; //最后一次成功连接时间
};
#endif


