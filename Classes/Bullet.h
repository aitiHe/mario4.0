#ifndef _BULLET_
#define _BULLET_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCBullet : public Node
{
public:
	CCBullet();
	virtual ~CCBullet();

	void setBodySize(Size _size);
	Size getBodySize();

	void setStartPos(Point pos);
	Point getStartPos();

	BulletType getBulletType();

	BulletState getBulletState();

	virtual void checkBulletState();

	// 纯虚函数
	virtual void launchBullet() = 0;

	virtual void forKilledEnemy();

	virtual Rect getBulletRect();

protected:
	BulletType bulletType;  // 子弹所属的类型
	Sprite *bulletBody;
	Size bodySize;

	Point startPos;  // 子弹发出点

	BulletState bulletState; // 子弹的活动状态
};

// 传统的普通子弹
class CCBulletCommon : public CCBullet
{
public:
	CCBulletCommon();
	~CCBulletCommon();

	// 实现父类的纯虚函数，启动子弹
	void launchBullet();

	void commonBulletCollisionH();
	void commonBulletCollisionV();

	void update(float dt);

	void forKilledEnemy();
private:
	float moveOffset;
	float ccMoveOffset;

	float jumpOffset;
	float ccJumpOffset;

	bool isLand;  // 火焰子弹的轨迹：先是右下直线然后是地面上的抛物线

	void showBoom();
	void autoClear();
};

// 弓箭
class CCBulletArrow : public CCBullet
{
public:
	CCBulletArrow();
	~CCBulletArrow();

	void launchBullet();

	void forKilledEnemy();

	void update(float dt);

	void arrowBulletCollisionH();

	Rect getBulletRect();
private:
	// 弓箭只有水平方向的移动
	float moveOffset;
	float ccMoveOffset;

	void autoClear();
	void broken();
};

#endif