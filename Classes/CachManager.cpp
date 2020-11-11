#include "CachManager.h"
#include "UserRecord.h"


static CachManager *_cashManager=nullptr; 

CachManager* CachManager::getInstance()
{
	if (_cashManager==nullptr){
		_cashManager = new CachManager();
		_cashManager->autorelease();
	}
	return _cashManager;
}

void CachManager::freshCach()
{
	cleanCach();


	SpriteFrameCache* cache =  SpriteFrameCache::getInstance();

	//取章节，关卡
	int _cha  = UserRecord::getInstance()->getSectionNum();
	int _post = UserRecord::getInstance()->getSceneNum();

	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	Value currentSection(_cha);
	Value currentScene(_post);
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	int monsterCount = monster->count();
	Value currentRound;
	Value currentEnemy;
	for (int i=1;i!=monsterCount+1;++i)
	{
		currentRound = i;
		__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
		__Dictionary* enemy = dynamic_cast<__Dictionary*>(round->objectForKey("enemy"));
		for (int j=1;j!=enemy->count()+1;++j){
			currentEnemy = j;
			__Dictionary* enemyIndex = dynamic_cast<__Dictionary*>(enemy->objectForKey(currentEnemy.asString()));
			int enemyType = dynamic_cast<__String*>(enemyIndex->objectForKey("type"))->intValue();

			//load plist
			switch (enemyType)
			{
			case 1:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_010.png","td_enemy_010.plist","td_enemy_01.ExportJson");
				break;
			case 2:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_020.png","td_enemy_020.plist","td_enemy_02.ExportJson");
				break;
			case 3:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_030.png","td_enemy_030.plist","td_enemy_03.ExportJson");
				break;
			case 4:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_040.png","td_enemy_040.plist","td_enemy_04.ExportJson");
				break;
			case 5:
			
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_050.png","td_enemy_050.plist","td_enemy_05.ExportJson");
				break;
			case 6:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_060.png","td_enemy_060.plist","td_enemy_06.ExportJson");
				break;
			case 7:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_070.png","td_enemy_070.plist","td_enemy_07.ExportJson");
				break;
			case 8:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_080.png","td_enemy_080.plist","td_enemy_08.ExportJson");
				break;
			case 9:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_090.png","td_enemy_090.plist","td_enemy_09.ExportJson");
				break;
			case 10:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_100.png","td_enemy_100.plist","td_enemy_10.ExportJson");
				break;
			case 11:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_110.png","td_enemy_110.plist","td_enemy_11.ExportJson");
				break;
			case 12:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_120.png","td_enemy_120.plist","td_enemy_12.ExportJson");
				break;
			case 13:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_130.png","td_enemy_130.plist","td_enemy_13.ExportJson");
				break;
			case 14:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_140.png","td_enemy_140.plist","td_enemy_14.ExportJson");
				break;
			case 15:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_150.png","td_enemy_150.plist","td_enemy_15.ExportJson");
				break;
			case 16:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_160.png","td_enemy_160.plist","td_enemy_16.ExportJson");
				break;
			case 17:
				cache->addSpriteFramesWithFile("td_shanDianQiu_effect0.plist"); //ITEM17-19攻击特效
				cache->addSpriteFramesWithFile("td_enemy_boss_remote_effect.plist");//这个是魔法王攻击的特效(17-19也用)
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_170.png","td_enemy_170.plist","td_enemy_17.ExportJson");
				break;
			case 18:
				cache->addSpriteFramesWithFile("td_shanDianQiu_effect0.plist"); //ITEM17-19攻击特效
				cache->addSpriteFramesWithFile("td_enemy_boss_remote_effect.plist");//这个是魔法王攻击的特效(17-19也用)
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_180.png","td_enemy_180.plist","td_enemy_18.ExportJson");
				break;
			case 19:
				cache->addSpriteFramesWithFile("td_shanDianQiu_effect0.plist"); //ITEM17-19攻击特效
				cache->addSpriteFramesWithFile("td_enemy_boss_remote_effect.plist");//这个是魔法王攻击的特效(17-19也用)
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_190.png","td_enemy_190.plist","td_enemy_19.ExportJson");
				break;
			case 20:
				cache->addSpriteFramesWithFile("td_enemy_boss_remote_effect.plist");//这个是魔法王攻击的特效(17-19也用)
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_200.png","td_enemy_200.plist","td_enemy_20.ExportJson");
				break;
			case 21:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_210.png","td_enemy_210.plist","td_enemy_21.ExportJson");
				break;
			default:
				break;
			}
		}
	}

	//公用特效
	cache->addSpriteFramesWithFile("tx_death.plist"); //死亡特效的 
	cache->addSpriteFramesWithFile("td_scene_Effect1.plist"); //塔的死亡特效
	cache->addSpriteFramesWithFile("td_candy_machine.plist");//糖果机
	cache->addSpriteFramesWithFile("td_Magic2_Effect.plist");//魔法技能-变羊的特效
	cache->addSpriteFramesWithFile("image/gameObjsNoCDeffect.plist","image/gameObjsNoCDeffect.png");

	//自身携带兵种特效
	std::map<int,string> _units = UserRecord::getInstance()->getTakeUnits();

	int objType;
	int selfType;
	for (auto iter=_units.begin();iter!=_units.end();++iter){

		string __units(iter->second);
		if (__units.find("war")!=string::npos){
			objType = TYPE_WARRIOR;
		}else if (__units.find("tower")!=string::npos){
			objType = TYPE_TOWER;
		}else if (__units.find("trap")!=string::npos){
			objType = TYPE_TRAP;
		}else
			return;

		selfType = atoi(String::createWithFormat("%c",__units.at(__units.size()-1))->getCString());

		if (objType==TYPE_WARRIOR){
			switch (selfType)
			{
			case 1:
				break;
			case 2:
				cache->addSpriteFramesWithFile("td_war_02_bomb.plist");//远程兵的
				break;
			case 3:
				cache->addSpriteFramesWithFile("td_war_fly_effect.plist");//飞行兵的爆炸特效
				break;
			case 4:
				cache->addSpriteFramesWithFile("td_war_04_bomb0.plist");//自爆兵的
				break;
			default:
				break;
			}
		}else if (objType==TYPE_TOWER){
			switch (selfType)
			{
			case 1:
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_tower_01_a_daodan0.png","td_tower_01_a_daodan0.plist","td_tower_01_a_daodan.ExportJson");
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_tower_01_b_daodan0.png","td_tower_01_b_daodan0.plist","td_tower_01_b_daodan.ExportJson");
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_tower_01_c_daodan0.png","td_tower_01_c_daodan0.plist","td_tower_01_c_daodan.ExportJson");

				cache->addSpriteFramesWithFile("tower_paodan_bom_effect.plist");
				cache->addSpriteFramesWithFile("tower_atk_zidan.plist");//tower 1 攻击特效
				break;
			case 2:
			    ArmatureDataManager::getInstance()->addArmatureFileInfo("tower_02_effect0.png","tower_02_effect0.plist","tower_02_effect.ExportJson");
				break;
			case 3:
				break;
			default:
				break;
			}
		}else if (objType==TYPE_TRAP){
			switch (selfType)
			{
			case 1:
// 				cache->addSpriteFramesWithFile("td_trap_01_a.plist");
// 				cache->addSpriteFramesWithFile("td_trap_01_b.plist");
// 				cache->addSpriteFramesWithFile("td_trap_01_c.plist");
// 				break;
			case 2:
				
// 				cache->addSpriteFramesWithFile("td_trap_02_a.plist");
// 				cache->addSpriteFramesWithFile("td_trap_02_b.plist");
// 				cache->addSpriteFramesWithFile("td_trap_02_c.plist");
				break;
			case 3:
// 				cache->addSpriteFramesWithFile("td_trap_03_a.plist");
// 				cache->addSpriteFramesWithFile("td_trap_03_b.plist");
// 				cache->addSpriteFramesWithFile("td_trap_03_c.plist");
 				cache->addSpriteFramesWithFile("td_trap_03_baozha0.plist");//爆炸显示的效果
				break;
			case 4:
// 				cache->addSpriteFramesWithFile("td_trap_04_a.plist");
// 				cache->addSpriteFramesWithFile("td_trap_04_b.plist");
// 				cache->addSpriteFramesWithFile("td_trap_04_c.plist");
				break;
			case 5:
// 				cache->addSpriteFramesWithFile("td_trap_05_a.plist");
// 				cache->addSpriteFramesWithFile("td_trap_05_b.plist");
// 				cache->addSpriteFramesWithFile("td_trap_05_c.plist");
				break;
			default:
				break;
			}
		}
	}
}

void CachManager::cleanCach()
{
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	TextureCache::getInstance()->removeUnusedTextures();
	SpriteFrameCache::getInstance()->purgeSharedSpriteFrameCache();
	TextureCache::getInstance()->purgeSharedTextureCache();
	ArmatureDataManager::getInstance()->purge();
}