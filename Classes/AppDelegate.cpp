#include "AppDelegate.h"
#include "startScene.h"
#include "audioPlayer.h"
#include "AndroidStartup.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
	auto eglView = director->getOpenGLView();
	if (!eglView)
	{
		eglView = GLView::create("TowerGame");
		director->setOpenGLView(eglView);
		
	}

	
	// add path...........................
	FileUtils::getInstance()->addSearchPath("image");
	FileUtils::getInstance()->addSearchPath("layout");
	FileUtils::getInstance()->addSearchPath("guge");
	FileUtils::getInstance()->addSearchPath("fonts");
	FileUtils::getInstance()->addSearchPath("boss");
	FileUtils::getInstance()->addSearchPath("war");
	FileUtils::getInstance()->addSearchPath("newEnemy");
	FileUtils::getInstance()->addSearchPath("tower");
	FileUtils::getInstance()->addSearchPath("guangquan");
	FileUtils::getInstance()->addSearchPath("plist");
	FileUtils::getInstance()->addSearchPath("npc");
    FileUtils::getInstance()->addSearchPath("trap");
    FileUtils::getInstance()->addSearchPath("magicskill");
    FileUtils::getInstance()->addSearchPath("startScene");
	FileUtils::getInstance()->addSearchPath("audioMusic");
	FileUtils::getInstance()->addSearchPath("audioEffect");
	FileUtils::getInstance()->addSearchPath("roleIcon");
	
	//eglView->setDesignResolutionSize(960 ,640 ,kResolutionNoBorder);
	//eglView->setFrameSize(854,480);
	//eglView->setFrameSize(1024,768);
	//eglView->setFrameSize(1136,640);

    //director->setDisplayStats(true);

	
	//auto size of screen
	Size frameSize = director->getOpenGLView()->getVisibleSize();
	Size lsSize = Size(960, 640);

	float scaleX = (float) frameSize.width / (float)lsSize.width;
	float scaleY = (float) frameSize.height / (float)lsSize.height;
	float scale = 1.0f;
	float bvar = 1.0f;
	// 固定宽度
	if (scaleX > scaleY)
	{
		scale = 1.0f;
		bvar = scaleY / scaleX;
	}
	else
	{
		scale = scaleY / scaleX;
		bvar = scaleY / scaleX;
	}
	
	eglView->setDesignResolutionSize(lsSize.width * scale, lsSize.height * bvar, ResolutionPolicy::NO_BORDER);
	

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


// kTargetWindows:   window系统
// kTargetLinux:  linux系统
// kTargetMacOS:  mac os系统
// kTargetAndroid:   Android系统
// kTargetIphone: iOS系统-iphone&itouch
// kTargetIpad:      iOS系统-Ipad
// kTargetBlackBerry:   黑莓系统

	Scene *scene;

	TargetPlatform target = Application::getInstance()->getTargetPlatform();//获取当前设备类型   

	if (target == kTargetIpad||target == kTargetIphone||target == kTargetMacOS){//如果IOS系统的获取苹果的
		 scene = startScene::createScene();
	}else{
		scene = AndroidStartup::createScene();
	}


    // create a scene. it's an autorelease object
	/*auto scene = startScene::createScene();*/

    // run
    director->runWithScene(scene);


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
    // if you use SimpleAudioEngine, it must be pause
	audioPlayer::getInstance()->pauseAllMusicEffect();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
	 NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
	audioPlayer::getInstance()->resumeAllMusicEffect();
}
 