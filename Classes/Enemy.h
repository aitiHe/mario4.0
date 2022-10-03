#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCEnemy : public Node
{
public:
	CCEnemy();
	virtual ~CCEnemy();

	void setTileCoord(Point _tileCoord);
	Point getTileCoord();
	void setEnemyPos(Point _enemyPos);
	Point getEnemyPos();
	void setBodySize(Size _size);
	Size getBodySize();

	void setMoveOffset(float _moveoffset);
	float getMoveOffset();

	void setccMoveOffset(float _ccmoveoffset);
	float getccMoveOffset();

	EnemyType getEnemyType();

	virtual Rect getEnemyRect();

	// 专门用来开启并展示怪物动画以及移动，碰撞检测的函数
	// 纯虚函数，因为CCEnemy不具体表示哪一类怪物，具体启动细节要根据具体怪物来定
	virtual void launchEnemy() = 0;
	virtual void enemyCollistionH();
	virtual void enemyCollistionV();

	// 和怪物状态控制相关的
	void setEnemyState(EnemyState _state);
	EnemyState getEnemyState();
	void checkState();
	virtual void stopEnemyUpdate();

	// 判断当前类型的怪物是否与主角相碰
	virtual EnemyVSHero checkCollisionWithHero();
	// 检测怪物是否被主角踩死，蘑菇乌龟是可以被这么踩死的，但是食人花就不行
	//virtual bool checkKilled();
	// 怪物被主角踩死时调用的函数
	virtual void forKilledByHero();
	void setNonVisibleForKilledByHero();

	// 被子弹干死调用的函数
	virtual void forKilledByBullet();
	void setNonVisibleForKilledByBullet();

protected:
	EnemyType enemyType;  // 用来表示怪物的类型
	Sprite *enemyBody;  // 展示怪物的精灵
	Size bodySize;      // 怪物的尺寸
	SpriteFrame *enemyLifeOver;  // 用来表示怪物挂掉的精灵帧
	SpriteFrame *overByArrow;    // 被箭射死的精灵帧

	Point tileCoord;   // 用来表示怪物出生点的地图坐标系下的
	Point enemyPos;    // GL坐标系下的，可以直接设置怪物的位置

	marioDirection startFace;  // 刚开启怪物状态时，怪物的朝向

	// 水平方向移动控制变量
	float moveOffset;
	float ccMoveOffset;
	// 竖直方向上移动控制变量
	float jumpOffset;
	float ccJumpOffset;

	// 标识怪物的当前状态
	// 规定，屏幕中的怪物属于活跃状态，屏幕之前或之后的均为非活跃状态，再或者是死亡
	EnemyState enemyState;

};

// 怪物蘑菇（以前一直认为是白菜）
class CCEnemyMushroom : public CCEnemy
{
public:
	CCEnemyMushroom();
	~CCEnemyMushroom();

	// 实现父类的纯虚函数
	void launchEnemy();

	//void enemyCollistionH();
	//void enemyCollistionV();

	void update(float dt);
};

// 吃人花，从管道中冒出来的，需要借助图层的管道层的遮挡关系实现吃人花的隐藏效果
class CCEnemyFlower : public CCEnemy
{
public:
	CCEnemyFlower();
	~CCEnemyFlower();

	// 实现父类的纯虚函数
	void launchEnemy();

	void update(float dt);
	// 因为食人花的相碰算法和之前的蘑菇乌龟等不同，需要重写这个虚函数
	EnemyVSHero checkCollisionWithHero();

	// 重写获取矩形函数
	Rect getEnemyRect();

protected:
	Point startPos;  // 标识食人花刚出生时的位置
};

// 乌龟，地上走的那种
class CCEnemyTortoise : public CCEnemy
{
public:
	CCEnemyTortoise(int _startface);
	~CCEnemyTortoise();

	// 实现父类的纯虚函数
	void launchEnemy();
	// 重写水平方向碰撞检测，因为乌龟有在水平方向上的调头
	void enemyCollistionH();
	void update(float dt);

private:
	SpriteFrame *leftFace;
	SpriteFrame *rightFace;
};

// 乌龟，地上走的，但会往复运动类型
class CCEnemyTortoiseRound : public CCEnemy
{
public:
	CCEnemyTortoiseRound(float dis);
	~CCEnemyTortoiseRound();

	void launchEnemy();

	// 重写父类的水平方向更新，而且这种乌龟也只需要水平方向的更新
	//void enemyCollistionH();

	void update(float dt);

	void setRoundDis(float dis);
	float getRoundDis();
private:
	float roundDis;  // 水平方向上往复的距离

	void reRight();
	void reLeft();
};

// 会飞的乌龟
class CCEnemyTortoiseFly : public CCEnemy
{
public:
	CCEnemyTortoiseFly(float dis);
	~CCEnemyTortoiseFly();

	void launchEnemy();

	void update(float dt);

	void setFlyDis(float dis);
	float getFlyDis();

private:
	float flyDis;
};

// 火串
class CCEnemyFireString : public CCEnemy
{
public:
	CCEnemyFireString(float _begAngle, float _time);
	~CCEnemyFireString();

	// 启动火串后就是旋转，没有状态检测，火串启动起来后就是活跃状态，并且过程中不变
	void launchEnemy();

	// 重写父类的停止怪物更新，因为火串有三个精灵
	void stopEnemyUpdate();

	// 火串与主角的碰撞检测有很大不同
	EnemyVSHero checkCollisionWithHero();

	// 这两个重写函数中为空
	void forKilledByBullet();
	void forKilledByHero();	

	void update(float dt);

private:
	Sprite *enemyBody2;  // 另外两个火球精灵
	Sprite *enemyBody3;

	Size fireSize;  // 单个火球的尺寸，在于主角的碰撞检测中这个是起主要检测作用的
	Vector<Node*> arrayFire;  // 三个火球的数组

	float begAngle;  // 启动旋转前火串所处的角度
	float time;      // 火串旋转一周所需要的时间

	double angle;  // 任意时刻，火串以逆时针方向的角度，整数值
	double PI;  // 旋转所用的圆周率

	void launchFireString();
};

// 普普通通的飞鱼
class CCEnemyFlyFish : public CCEnemy
{
public:
	CCEnemyFlyFish(float _offsetH, float _offsetV, float _duration);
	~CCEnemyFlyFish();

	void launchEnemy();
	void update(float dt);
	void checkState();

	// 飞鱼从下方冲上来不存在被主角踩死，所以只要两个矩形相交即为碰到主角
	EnemyVSHero checkCollisionWithHero();

	// 下面被主角踩死的函数为空，但是被子弹射中的情况还是用基类的
	void forKilledByHero();

private:
	float offsetH;  // 飞鱼水平方向上偏移
	float offsetV;  // 竖直方向上偏移
	float offsetDuration;  // 偏移持续时间
	bool isFlying;    // 是否是飞行状态，防止一个地方无休止的发射

	void flyInSky();
	void reSetNotInSky();
};

// Boss
class CCEnemyBoss : public CCEnemy
{
public:
	CCEnemyBoss();
	~CCEnemyBoss();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void enemyCollistionH();

	void update(float dt);

private:

	float leftSide;
	float rightSide;

	void moveLeft();
	void moveRight();
};

// Boss发出的子弹，也被算成一种怪物
class CCEnemyBossFire : public CCEnemy
{
public:
	CCEnemyBossFire();
	~CCEnemyBossFire();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void update(float dt);
private:

};


// 藏在管道或是其他障碍物后面的，可以冒出一定数量蘑菇的怪物
class CCEnemyAddMushroom : public CCEnemy
{
public:
	CCEnemyAddMushroom(int _addnum);
	~CCEnemyAddMushroom();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByHero();

	void update(float dt);
private:
	int addNums;  // 一共要增加蘑菇的个数
	bool isAddable;  // 每隔一段时间增加一个怪物，用于控制怪物增加速度

	void addMushroom();
	void reSetNonAddable();  // 增加完蘑菇后重新设置为可以增加
};


// 魂斗罗炮台
class CCEnemyBattery : public CCEnemy
{
public:
	CCEnemyBattery(float delay);
	~CCEnemyBattery();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();  // 这种炮台只能被火球打爆
	void forKilledByHero();  // 炮台不能被主角踩死

	void update(float dt);

private:

	bool isFireable;  // 用于控制发射子弹的频度
	float fireDelay;  // 两发子弹之间的时间间隔

	void addBatteryBullet();
	void reSetNonFireable();  // 重新设置为不可发射子弹

	void stopAndClear();

	Point firePos;   // 发射出来的子弹的初始位置
};


// 炮台发出的子弹，小白点
class CCEnemyBatteryBullet : public CCEnemy
{
public:
	CCEnemyBatteryBullet();
	~CCEnemyBatteryBullet();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void update(float dt);
private:

};


// 乌云怪物，可以在主角靠近乌云时掉落闪电
class CCEnemyDarkCloud : public CCEnemy
{
public:
	CCEnemyDarkCloud(float _delay, int _type);
	~CCEnemyDarkCloud();

	void launchEnemy();

	void update(float dt);

	void forKilledByBullet();  // 天上的乌云不会被子弹打中
	void forKilledByHero();    // 也不会被主角踩死，两个函数均为空

private:
	float dropRegion;  // 掉落闪电的区间 一般固定
	float leftSide;    // 掉落的左边界
	float rightSide;   // 右边界

	bool isDropable;   // 控制是否可以掉落乌云的变量

	float delay;       // 两个闪电之间的时间间隔  需要在对象层设定

	void addLighting();
	void reSetDropable();

	int type;   // 乌云的大小型号 0 1 2分别代表小中大

	SpriteFrame *dark;   // 乌云掉落闪电的一瞬展示的
	SpriteFrame *normal; // 正常的白云
	void reSetNormal();
};


// 闪电
class CCEnemyLighting : public CCEnemy
{
public:
	CCEnemyLighting();
	~CCEnemyLighting();

	EnemyVSHero checkCollisionWithHero();

	void launchEnemy();

	void checkState();

	void update(float dt);

	void forKilledByHero();
	void forKilledByBullet();

private:

};



#endif
