#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "GameEnum.h"

// 存储游戏的进度与相应数据，在游戏过程中进行更新与加载
class Global
{
public:
	Global();
	~Global();

	int currentLevel;
	void setCurrentLevel(int level);
	void reSetLevel();
	int getCurrentLevel();
	void currentLevelPlusOne();

	int totalLevels;
	int getTotalLevels();

	//ToMainMenuFor whyToMainMenu;  // 记录为什么返回主菜单，或是因为死，或是因为通过本关
	//void setWhyToMainMenu(ToMainMenuFor why);
	//ToMainMenuFor getForToMainMenu();

	BodyType currentHeroType;  // 当前主角的身型状态
	void setCurrentHeroType(BodyType _type);
	BodyType getCurrentHeroType();

	BulletType currentBulletType;  // 当前主角使用的子弹类型
	void setCurrentBulletType(BulletType _type);
	BulletType getCurrentBulletType();

	// 马里奥一共的生命条数
	int lifeNum;
	void lifeNumPlusOne();
	void lifeNumCutOne();
	void setCurrentLifeNum(int num);
	int getCurrentLifeNum();

	static Global  *_globalInstance;
	static Global* getGlobalInstance();
};


#endif