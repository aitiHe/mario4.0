#include "Bullet.h"
#include "AnimationManager.h"
#include "Hero.h"
#include "GameMap.h"
#include "GameLayer.h"

// ****************** CCBullet ******************** //
CCBullet::CCBullet():
bulletBody(NULL)
{
	Point heroPos = Hero::getHeroInstance()->getPosition();
	Size heroSize = Hero::getHeroInstance()->getContentSize();
	startPos = Vec2(heroPos.x, heroPos.y + heroSize.height/2);
}

CCBullet::~CCBullet()
{

}

void CCBullet::setBodySize(Size _size)
{
	bodySize = _size;
}
Size CCBullet::getBodySize()
{
	return bodySize;
}

void CCBullet::setStartPos(Point pos)
{
	startPos = pos;
}
Point CCBullet::getStartPos()
{
	return startPos;
}

BulletType CCBullet::getBulletType()
{
	return bulletType;
}

BulletState CCBullet::getBulletState()
{
	return bulletState;
}

void CCBullet::checkBulletState()
{

}

Rect CCBullet::getBulletRect()
{
	Point pos = this->getPosition();
	return Rect(pos.x - bodySize.width/2, pos.y, bodySize.width, bodySize.height);
}

void CCBullet::forKilledEnemy()
{

}

// ****************** CCBulletCommon ******************** //
CCBulletCommon::CCBulletCommon()
{
	bulletType = eBullet_common;
	bulletState = eBulletState_active;
	isLand = false;

	bodySize = Size(10, 10);
	bulletBody = Sprite::create("fireRight.png", Rect(0, 0, 10, 10));
	bulletBody->setAnchorPoint(Vec2(0, 0));
	this->setContentSize(bodySize);
	this->addChild(bulletBody);
	this->setAnchorPoint(Vec2(0.5f, 0.0f));

	moveOffset = 0.0f;
	ccMoveOffset = 5.0f;
	jumpOffset = 0.0f;
	ccJumpOffset = 0.3f;

	switch (Hero::getHeroInstance()->face)
	{
	case eRight:
		moveOffset = ccMoveOffset;
		break;
	case eLeft:
		moveOffset = -ccMoveOffset;
		break;
	default:
		break;
	}
}

CCBulletCommon::~CCBulletCommon()
{
	this->unscheduleAllCallbacks();
}

void CCBulletCommon::launchBullet()
{
	bulletBody->runAction(RepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));
	
	this->scheduleUpdate();
}

// 子弹的两侧碰撞检测
// 如果两侧碰到了land，block，pipe等会自动爆炸掉
void CCBulletCommon::commonBulletCollisionH()
{
	Point currentPos = this->getPosition();

	// 判断子弹是否会射出屏幕
	float leftSide = currentPos.x - bodySize.width/2;
	float rightSide = currentPos.x + bodySize.width/2;
	float mapMaxH = CCGameLayer::getMapMaxH();
	Size winSize = Director::getInstance()->getWinSize();
	if (fabs(leftSide - mapMaxH) <= 8)
	{
		this->showBoom();
		return ;
	}

	// 判断是否射出地图右侧
	float mapRightSide = CCGameMap::getGameMap()->mapSize.width*
		CCGameMap::getGameMap()->tileSize.width;
	if (fabs(rightSide - mapRightSide) <= 8)
	{
		this->showBoom();
		return ;
	}

	if (leftSide - mapMaxH >= winSize.width)
	{
		bulletBody->stopAllActions();
		this->unscheduleUpdate();
		this->autoClear();
	}

	// 右侧
	Point rightCollision = Vec2(currentPos.x + bodySize.width/2, currentPos.y + bodySize.height/2);
	Point rightTilecoord = CCGameMap::getGameMap()->positionToTileCoord(rightCollision);
	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->showBoom();
		return ;
		break;
	}

	// 左侧
	Point leftCollision = Vec2(currentPos.x - bodySize.width/2, currentPos.y + bodySize.height/2);
	Point leftTilecoord = CCGameMap::getGameMap()->positionToTileCoord(leftCollision);
	tileType = CCGameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->showBoom();
		return ;
		break;
	}
}

// 因为子弹只会往下落，所以这里仅仅做脚底的判断
// 脚底如果接触到land，block，会自动反弹，但是脚底碰到pipe也会爆炸
void CCBulletCommon::commonBulletCollisionV()
{
	Point currentPos = this->getPosition();

	Point downCollision = currentPos;
	Point downTilecoord = CCGameMap::getGameMap()->positionToTileCoord(downCollision);
	downTilecoord.y += 1;
	Point downPos = CCGameMap::getGameMap()->tilecoordToPosition(downTilecoord);
	downPos = Vec2(currentPos.x, downPos.y + CCGameMap::getGameMap()->getTileSize().height);

	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(downTilecoord);
	switch (tileType)
	{
	case eTile_Land:
		isLand = true;
		jumpOffset = 3.0f;
		this->setPosition(downPos);
		return ;
		break;
	case eTile_Pipe:
	case eTile_Block:
		this->showBoom();
		return ;
		break;
	case eTile_Trap:
		{
			bulletBody->stopAllActions();
			this->unscheduleUpdate();
			this->autoClear();
			return ;
			break;
		}
	default:
		break;
	}

	jumpOffset -= ccJumpOffset;
}

void CCBulletCommon::update(float dt)
{
	if (bulletState == eBulletState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->commonBulletCollisionH();
		this->commonBulletCollisionV();
	}
}

void CCBulletCommon::showBoom()
{	
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	bulletBody->runAction(Sequence::create(sAnimationMgr->createAnimate(eAniBoomedFireBall), 
		CallFunc::create(
            [&](){
                 bulletState = eBulletState_nonactive;
                 this->setVisible(false);
    }  ), NULL));
    
}

void CCBulletCommon::autoClear()
{	
	bulletState = eBulletState_nonactive;
	this->setVisible(false);
}

void CCBulletCommon::forKilledEnemy()
{
	this->showBoom();
}


// ****************** CCBulletArrow ******************** //
CCBulletArrow::CCBulletArrow()
{
	bulletType = eBullet_arrow;
	bulletState = eBulletState_active;

	bodySize = Size(16, 16);
	bulletBody = Sprite::create("arrow.png");
	bulletBody->setAnchorPoint(Vec2(0, 0));
	this->setContentSize(bodySize);
	this->addChild(bulletBody);
	this->setAnchorPoint(Vec2(0.5f, 0.5f));

	ccMoveOffset = 6.0f;
	ActionInstant *flipX = FlipX::create(true);

	switch (Hero::getHeroInstance()->face)
	{
	case eRight:
		moveOffset = ccMoveOffset;
		break;
	case eLeft:
		moveOffset = -ccMoveOffset;
		bulletBody->runAction(flipX);
		break;
	default:
		break;
	}

}

CCBulletArrow::~CCBulletArrow()
{
	this->unscheduleAllCallbacks();
}

void CCBulletArrow::launchBullet()
{
	this->scheduleUpdate();
}

void CCBulletArrow::arrowBulletCollisionH()
{
	Point currentPos = this->getPosition();
	
	// 判断子弹是否会射出屏幕
	float leftSide = currentPos.x - bodySize.width/2;
	float rightSide = currentPos.x + bodySize.width/2;
	float mapMaxH = CCGameLayer::getMapMaxH();
	Size winSize = Director::getInstance()->getWinSize();
	if (fabs(leftSide - mapMaxH) <= 8)
	{
		this->broken();
		return ;
	}
	// 判断是否射出地图右侧
	float mapRightSide = CCGameMap::getGameMap()->mapSize.width*
		CCGameMap::getGameMap()->tileSize.width;
	if (fabs(rightSide - mapRightSide) <= 8)
	{
		this->broken();
		return ;
	}
	
	if (leftSide - mapMaxH >= winSize.width)
	{
		bulletBody->stopAllActions();
		this->stopAllActions();
		this->unscheduleUpdate();
		this->autoClear();
	}

	// 右侧
	Point rightCollision = Vec2(currentPos.x + bodySize.width/2, currentPos.y);
	Point rightTilecoord = CCGameMap::getGameMap()->positionToTileCoord(rightCollision);
	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->broken();
		return ;
		break;
	}

	// 左侧
	Point leftCollision = Vec2(currentPos.x - bodySize.width/2, currentPos.y);
	Point leftTilecoord = CCGameMap::getGameMap()->positionToTileCoord(leftCollision);
	tileType = CCGameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->broken();
		return ;
		break;
	}
}

void CCBulletArrow::forKilledEnemy()
{
	bulletState = eBulletState_nonactive;
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	this->setVisible(false);
}

void CCBulletArrow::update(float dt)
{
	if (bulletState == eBulletState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;

		this->setPosition(currentPos);
		this->arrowBulletCollisionH();
	}
}

Rect CCBulletArrow::getBulletRect()
{
	Point pos = this->getPosition();
	return Rect(pos.x - 6, pos.y - 5, 12, 10);
}

void CCBulletArrow::autoClear()
{
	bulletState = eBulletState_nonactive;
	this->setVisible(false);
}

void CCBulletArrow::broken()
{
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();

	bulletBody->runAction(Sequence::create(sAnimationMgr->createAnimate(eAniArrowBroken),
		CallFunc::create(
                         [&](){
                               bulletState = eBulletState_nonactive;
                               this->setVisible(false);
                         }), NULL));
}
