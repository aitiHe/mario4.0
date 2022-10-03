#ifndef _ANIMATION_MANAGER_H__
#define _ANIMATION_MANAGER_H__

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class AnimationManager
{
public:
	void initAnimationMap();
	void preLoadEffectAndMusic();  // �������ֺ���Ч

	Animation* getAnimation(AnimationType key);
	Animate* createAnimate(const char *key);
	Animate* createAnimate(AnimationType key);

	void setSelectLevel();
	void setMusicSwitch();

	static AnimationManager* getInstance();
	static AnimationManager *_instance;
private:
	// �漰�����ǵĺܹ���ľ���֡������������
	Animation* createAnimationByAnimationType(AnimationType key);
};

#define sAnimationMgr AnimationManager::getInstance()
#endif