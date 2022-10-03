#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class Hero : public Node
{
public:
	Sprite *mainBody;  // ��Ҫ������¾���
	Sprite *mainTemp;  // ����µ�������Ҫ���ڱ���С�Ķ����ڼ�
	Size norBodySize;  // ��������µ������С(������)
	Size smallSize;    // С�����
	Size currentSize;  // ��ǰ�����
	BodyType bodyType;   // ��ǰ������״̬

	void reSetForSuccess();  // ��С���������ͬʱ�������Ҫִ�еĻ�״̬Ȼ����Ծ�������ĵ�

	SpriteFrame *_normalRight;    // ����״̬�������������
	SpriteFrame *_normalLeft;     // ��������������������
	SpriteFrame *_jumpLeft;  // �����ڿ���ͣ��״̬�£����Ƕ�Ӧ������
	SpriteFrame *_jumpRight; // ����ͬ��

	SpriteFrame *_smallRight;  // ��Ӧ��С��״̬�µ�
	SpriteFrame *_smallLeft;
	SpriteFrame *_smallJumpLeft;
	SpriteFrame *_smallJumpRight;

	SpriteFrame *_normalRightFire;  // �����еĴ����ӵ�״̬����
	SpriteFrame *_normalLeftFire;   // �����д����ӵ�����
	SpriteFrame *_jumpRightFire;
	SpriteFrame *_jumpLeftFire;

	SpriteFrame *_normalRightArrow;  // ���乭����Ƥ��
	SpriteFrame *_normalLeftArrow;
	SpriteFrame *_jumpRightArrow;
	SpriteFrame *_jumpLeftArrow;

	SpriteFrame *_lifeOverSmall;  // С�����������״̬
	SpriteFrame *_lifeOverNormal;
	SpriteFrame *_lifeOverFire;

	bool isDied;   // �Ƿ��Ѿ�����
	void setHeroDie(bool _die);
	bool isHeroDied();

	//void dieForEnemy();  // ���������������ִ�еĺ��������ֻ�ǻ�����״�ľ���֡
	void dieForTrap();   // ����µ���������Ҫִ��һ�����ƺ����ƵĶ�������ʧ��
	void reSetNonVisible();

	marioDirection state;  // ��ʾ��ǰ���ǵ�״̬
	marioDirection statePre;  // ��ʶ���ǵ���һ��״̬
	marioDirection face;   // ����µĳ���
	bool isFlying;   // ������Ƿ��ڿ��з���״̬

	bool bulletable;  // �����Ƿ���Է����ӵ������������������е�һ������
	bool isBulletable();

	BulletType currentBulletType;  // ��ǰ��������ʹ�õ��ӵ�����
	void setBulletType(BulletType _type);
	BulletType getBulletType();

	// �Ƿ�վ�����ӵ�С�����
	bool gadgetable;
	void setGadgetable(bool _gadgetable);
	bool isGadgetable();

	void fireAction();
	void reSetStateForFired();

	// ������ʾ����һ���˵�UP����
	Label *pLabelUp;

	static Hero* create();

	void setHeroState(marioDirection state);
	marioDirection getHeroState();	

	Hero();
	~Hero();

	void onEnter();
	void onExit();

	void changeForGotAddLifeMushroom();
	void changeForGotMushroom();
	void changeForGotEnemy();

	void setBodyType(BodyType _bodytype);
	Size getCurrentSize();
	BodyType getCurrentBodyType();

	void setSafeTime(bool _issafe);
	bool getIsSafeTime();

	static Hero *_heroInstance;
	static Hero* getHeroInstance();
private:
	bool heroInit();
	void clearLabelUp();
	void setHeroTypeForSmall();
	void setHeroTypeForNormal();

	bool isSafeTime;  // ��ʶ�����Ƿ��ڰ�ȫ�ڣ����˱�С��5�����3���ڲ��ܹ�����ײ���
	void reSetSafeTime();
};

#endif
