#include "Item.h"
#include "GameLayer.h"

//********************* CCItem **********************//
CCItem::CCItem(ItemType _itemtype)
{
	itemType = _itemtype;
}

CCItem::~CCItem()
{
	this->unscheduleAllCallbacks();
}

bool CCItem::itemInit()
{
	switch (itemType)
	{
	case eBlinkCoin:
		itemBody = Sprite::create("coinani.png", Rect(0, 0, 8, 14));
		break;
	default:
		break;
	}
	itemBody->setAnchorPoint(Vec2(0, 0));
	this->addChild(itemBody);
	itemBody->retain();
	this->setContentSize(Size(8, 14));
	this->setAnchorPoint(Vec2(0.5, 0));
	return true;
}

CCItem* CCItem::create(ItemType _itemtype)
{
	CCItem *pItem = new CCItem(_itemtype);
	if (pItem && pItem->itemInit())
	{
		pItem->autorelease();
		return pItem;
	}
	CC_SAFE_DELETE(pItem);
	return NULL;
}


//********************* CCGadget **********************//
CCGadget::CCGadget():
gadgetBody(NULL)
{
	gadgetState = eGadgetState_nonactive;

	moveOffset = 0.0f;
	ccMoveOffset = 0.0f;
	jumpOffset = 0.0f;
	ccJumpOffset = 0.0f;

	startPos = Vec2::ZERO;
}

CCGadget::~CCGadget()
{

}

void CCGadget::gadgetUpdateH()
{

}

void CCGadget::gadgetUpdateV()
{

}

float CCGadget::getMoveOffset()
{
	return moveOffset;
}

float CCGadget::getJumpOffset()
{
	return jumpOffset;
}

void CCGadget::setStartPos(Point pos)
{
	startPos = pos;
}

Point CCGadget::getStartPos()
{
	return startPos;
}

void CCGadget::setStartFace(int face)
{
	startFace = face;
}

Rect CCGadget::getGadgetRect()
{
	Point pos = this->getPosition();
	return Rect(pos.x - bodySize.width/2, pos.y,
		bodySize.width, bodySize.height);
}

Size CCGadget::getGadgetSize()
{
	return bodySize;
}

GadgetState CCGadget::getGadgetState()
{
	return gadgetState;
}

void CCGadget::checkGadgetState()
{
	Size winSize = Director::getInstance()->getWinSize();
	float tempMaxH = CCGameLayer::getMapMaxH();
	Point pos = this->getPosition();

	if ( (pos.x + bodySize.width/2 - tempMaxH >= 0) &&
		(pos.x - bodySize.width/2 - tempMaxH) <= winSize.width )
	{
		gadgetState = eGadgetState_active;
	}
	else
	{
		if (pos.x + bodySize.width/2 - tempMaxH < 0)
		{
			gadgetState = eGadgetState_over;
			gadgetBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}
		else
		{
			gadgetState = eGadgetState_nonactive;
		}
	}
}




//********************* CCGadgetLadderLR **********************//
CCGadgetLadderLR::CCGadgetLadderLR(float dis)
{
	gadgetType = eGadget_LadderLR;
	gadgetBody = Sprite::create("ladder.png");
	bodySize = Size(48.0f, 8.0f);
	gadgetBody->setAnchorPoint(Vec2(0, 0));
	this->setContentSize(bodySize);
	this->addChild(gadgetBody);
	this->setAnchorPoint(Vec2(0.5f, 0.0f));

	ccMoveOffset = 0.6f;
	leftSide = 0.0f;
	rightSide = 0.0f;
	lrDis = dis;
}

CCGadgetLadderLR::~CCGadgetLadderLR()
{
	this->unscheduleAllCallbacks();
}

void CCGadgetLadderLR::gadgetUpdateH()
{
	Point pos = this->getPosition();
	if (pos.x <= leftSide || pos.x >= rightSide)
	{
		moveOffset *= -1;
	}
}


void CCGadgetLadderLR::launchGadget()
{
	leftSide = startPos.x - lrDis;
	rightSide = startPos.x + lrDis;
	switch (startFace)
	{
	case 0:
		moveOffset = -ccMoveOffset;
		break;
	case 1:
		moveOffset = ccMoveOffset;
		break;
	default:
		break;
	}

	this->scheduleUpdate();
	
}

void CCGadgetLadderLR::update(float dt)
{
	this->checkGadgetState();

	if (gadgetState == eGadgetState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);

		this->gadgetUpdateH();
	}	
}

void CCGadgetLadderLR::setLRdis(float dis)
{
	lrDis = dis;
}


//********************* CCGadgetLadderUD **********************//
CCGadgetLadderUD::CCGadgetLadderUD(float dis)
{
	gadgetType = eGadget_LadderUD;
	gadgetBody = Sprite::create("ladder.png");
	bodySize = Size(48.0f, 8.0f);
	gadgetBody->setAnchorPoint(Vec2(0, 0));
	this->setContentSize(bodySize);
	this->addChild(gadgetBody);
	this->setAnchorPoint(Vec2(0.5f, 0.0f));

	ccJumpOffset = 0.5f;

	upSide = 0.0f;
	downSide = 0.0f;
	udDis = dis;
}

CCGadgetLadderUD::~CCGadgetLadderUD()
{
	this->unscheduleAllCallbacks();
}

void CCGadgetLadderUD::gadgetUpdateV()
{
	Point pos = this->getPosition();
	if (pos.y <= downSide || pos.y >= upSide)
	{
		jumpOffset *= -1;
	}
}

void CCGadgetLadderUD::launchGadget()
{
	upSide = startPos.y + udDis;
	downSide = startPos.y - udDis;
	switch (startFace)
	{
	case 0:
		jumpOffset = ccJumpOffset;
		break;
	case 1:
		jumpOffset = -ccJumpOffset;
		break;
	default:
		break;
	}

	this->scheduleUpdate();
}

void CCGadgetLadderUD::update(float dt)
{
	this->checkGadgetState();

	if (gadgetState == eGadgetState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->gadgetUpdateV();
	}

}

void CCGadgetLadderUD::setUDdis(float dis)
{
	udDis = dis;
}






