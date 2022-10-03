#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "GameEnum.h"

// �洢��Ϸ�Ľ�������Ӧ���ݣ�����Ϸ�����н��и��������
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

	//ToMainMenuFor whyToMainMenu;  // ��¼Ϊʲô�������˵���������Ϊ����������Ϊͨ������
	//void setWhyToMainMenu(ToMainMenuFor why);
	//ToMainMenuFor getForToMainMenu();

	BodyType currentHeroType;  // ��ǰ���ǵ�����״̬
	void setCurrentHeroType(BodyType _type);
	BodyType getCurrentHeroType();

	BulletType currentBulletType;  // ��ǰ����ʹ�õ��ӵ�����
	void setCurrentBulletType(BulletType _type);
	BulletType getCurrentBulletType();

	// �����һ������������
	int lifeNum;
	void lifeNumPlusOne();
	void lifeNumCutOne();
	void setCurrentLifeNum(int num);
	int getCurrentLifeNum();

	static Global  *_globalInstance;
	static Global* getGlobalInstance();
};


#endif