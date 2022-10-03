#include "Global.h"

Global* Global::_globalInstance;
Global* Global::getGlobalInstance()
{
	if (_globalInstance == 0)
	{
		_globalInstance = new Global();
	}
	return _globalInstance;
}

Global::Global()
{
	currentLevel = 1;

	totalLevels = 8;

	//whyToMainMenu = efor_StartGame;

	currentHeroType = eBody_Small;

	currentBulletType = eBullet_common;

	lifeNum = 3;
}

Global::~Global()
{

}

void Global::currentLevelPlusOne()
{
	++currentLevel;
}

void Global::reSetLevel()
{
	currentLevel = 1;
}

int Global::getCurrentLevel()
{
	return currentLevel;
}

int Global::getTotalLevels()
{
	return totalLevels;
}

//void Global::setWhyToMainMenu(ToMainMenuFor why)
//{
//	whyToMainMenu = why;
//}
//
//ToMainMenuFor Global::getForToMainMenu()
//{
//	return whyToMainMenu;
//}

void Global::setCurrentHeroType(BodyType _type)
{
	currentHeroType = _type;
}

BodyType Global::getCurrentHeroType()
{
	return currentHeroType;
}

void Global::setCurrentBulletType(BulletType _type)
{
	currentBulletType = _type;
}

BulletType Global::getCurrentBulletType()
{
	return currentBulletType;
}

void Global::lifeNumPlusOne()
{
	++lifeNum;
}

void Global::lifeNumCutOne()
{
	--lifeNum;
}

void Global::setCurrentLifeNum(int num)
{
	lifeNum = num;
}

int Global::getCurrentLifeNum()
{
	return lifeNum;
}

void Global::setCurrentLevel( int level )
{
	currentLevel = level;
}
