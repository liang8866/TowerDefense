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
	//����ʾ�����淢������
	void send(std::string data,Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector);
	//��ʾ�����淢������
	void loadSend(std::string data,Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void reLoginCallBack(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void reSend(int tag);
	void login(Ref* pTarget, cocos2d::network::SEL_HttpResponse pSelector);
	std::string encryptData(std::string);

	 /**
     * ���µ�¼���ɹ�������ִ����һ����������
     *
     * @param tag   
     */
	void reLogin(int tag);
	void logicUpdate(float dt); //��ʱ��
	void doLoop();
	void cancelConnect();
	Ui_Loading* _widget;
	void createLoadPanel();
	void closeLoadPanel();
	void setAutoConnect(bool enabled);
	CC_SYNTHESIZE(bool,_loadEnabled,LoadEnabled); //load���濪��
	CC_SYNTHESIZE(bool,_pauseGame,PauseGame); //����ʱ�Ƿ���ͣ��Ϸ
private:
	cocos2d::Map<int,RequestMap*> _requests;
	bool _autoConnect; //�Զ����ӿ���
	int _reLoginTag;
	bool reLoginFlag;
	 int _autoTag; 
	 int _connectCount;
	 int _chronon;//��ʱ��λ
	 int _deltaTime; //update������ʱ
	 int _lastTime; //���һ�γɹ�����ʱ��
};
#endif


