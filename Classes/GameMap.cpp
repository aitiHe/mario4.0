#include "GameMap.h"
#include "AnimationManager.h"
#include "Item.h"
#include "Enemy.h"
#include "GameLayer.h"
#include "Hero.h"
#include "Bullet.h"
#include "Global.h"
#include "AudioEngine.h"

CCGameMap* CCGameMap::_gameMap;
CCGameMap* CCGameMap::getGameMap()
{
	return _gameMap;
}

CCGameMap::CCGameMap()
{
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("superMarioMap.png");
	_brokenCoin = SpriteFrame::createWithTexture(pTexture, Rect(1, 18, 16, 16));
	_brokenCoin->retain();

	pItemCoordArray = PointArray::create(100);
	pItemCoordArray->retain();

//	pSpriteArray = Array::createWithCapacity(4);
//	pSpriteArray->retain();

	pMushroomPointArray = PointArray::create(100);
	pMushroomPointArray->retain();

//	pEnemyArray = Array::create();
//	pEnemyArray->retain();

//	pBulletArray = Array::create();
//	pBulletArray->retain();
//
//	pGadgetArray = Array::create();
//	pGadgetArray->retain();

	pMushSprite = NULL;
	pAddLifeMushroom = NULL;
	pItem = NULL;
	heroInGadget = NULL;

	isBossMap = false;

	_gameMap = this;

	enemyTilePos = Vec2::ZERO;
	pRandomEnemy = NULL;

	// רΪBoss��ͼ׼����
	bridgeTileStartPos = Vec2::ZERO;
	bridgeTileNums = 0;
	pBossEnemy = NULL;
	pPrincess = NULL;
}

CCGameMap::~CCGameMap()
{
	this->unscheduleAllCallbacks();

	//CC_SAFE_DELETE(pItemCoordArray);
	//CC_SAFE_DELETE(pEnemyArray);
	//CC_SAFE_DELETE(pBulletArray);
	//CC_SAFE_DELETE(pMushroomPointArray);

	//CC_SAFE_DELETE(landLayer);
	//CC_SAFE_DELETE(blockLayer);
	//CC_SAFE_DELETE(pipeLayer);
	//CC_SAFE_DELETE(coinLayer);
	//CC_SAFE_DELETE(trapLayer);
	//CC_SAFE_DELETE(cloudLayer);
	//CC_SAFE_DELETE(objectLayer);

	//pItemCoordArray->release();
	//pEnemyArray->release();
}

CCGameMap* CCGameMap::create(const char *tmxFile)
{
	CCGameMap *pGameMap = new CCGameMap();
	if (pGameMap && pGameMap->initWithTMXFile(tmxFile))
	{
		pGameMap->extraInit();
		pGameMap->autorelease();
		return pGameMap;
	}
	CC_SAFE_DELETE(pGameMap);
	return NULL;
}

void CCGameMap::extraInit()
{
	tileSize = this->getTileSize();
	mapSize = this->getMapSize();

	cloudLayer = this->getLayer("cloud");
	blockLayer = this->getLayer("block");
	pipeLayer = this->getLayer("pipe");
	landLayer = this->getLayer("land");
	trapLayer = this->getLayer("trap");
	objectLayer = this->getObjectGroup("objects");
	coinLayer = this->getLayer("coin");
	flagpoleLayer = this->getLayer("flagpole");

	this->initObjects();

	// ������Щ��������Ӧ���ڳ�ʼ��������֮���������
	// �ڳ�ʼ�������Ӱڷŵ�󣬽�����ڵ�ͼ��
	// ����Ҫ�жϸõ�ͼ�ǲ���BossMap
	if (isBossMap)
	{
		pFlag = Sprite::create("axe.png");
		pFlag->retain();
		pPrincess = Sprite::create("princess.png");
		pPrincess->setAnchorPoint(Vec2(0.5f, 0.0f));
		pPrincess->setPosition(Vec2(finalPoint.x + 16, finalPoint.y));
		this->addChild(pPrincess, this->getChildrenCount());
	} 
	else
	{
		pFlag = Sprite::create("flag.png");
		pFlag->retain();
	}

	pFlag->setAnchorPoint(Vec2(0.5f, 0));
	pFlag->setPosition(flagPoint);
	this->addChild(pFlag, this->getChildrenCount());

	this->launchEnemyInMap();

	this->launchGadgetInMap();

	this->scheduleUpdate();
}

void CCGameMap::showFlagMove()
{
	if (isBossMap)
	{
		// ��Boss�أ�˳��ͨ�ض���Ӧ�����ŵ�����ȥ��Boss����ȥ

		this->initBridgeArray();
	} 
	else
	{
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("QiZiLuoXia.ogg");
		MoveBy *pMoveBy = MoveBy::create(2.0f, Vec2(0, -8*16));
		pFlag->runAction(pMoveBy);
	}
}

void CCGameMap::initObjects()
{
	ValueVector tempArray = objectLayer->getObjects();

	float x, y, w, h;
	//Value objPointMap;
	for(auto objPointMap :tempArray)
	{
		ValueMap objPoint = objPointMap.asValueMap();
		int posX = objPoint.at("x").asFloat();
		int posY = objPoint.at("y").asFloat();
		posY -= this->getTileSize().height;
		Point tileXY = this->positionToTileCoord(Vec2(posX, posY));
		std::string name = objPoint.at("name").asString();
		std::string type = objPoint.at("type").asString();

			// ���й���ĳ�ʼ�����ȸ����������ж��ǲ���enemy����ϸ��enemy����		
		if (name == "enemy")
		{
			CCEnemy *pEnemy = NULL;
			if (type == "mushroom")
			{
				pEnemy = new CCEnemyMushroom();
			}
			else if (type == "flower")
			{
				pEnemy = new CCEnemyFlower();
			}
			else if (type == "tortoise")
			{
				// ��ͼ��Ԥ�ȱ�Ǻõ��ڹ궼�������
				pEnemy = new CCEnemyTortoise(0);
			}
			else if (type == "tortoise_round")
			{
				std::string dis = objPoint.at("roundDis").asString();
				int roundDis = atof(dis.c_str());
				pEnemy = new CCEnemyTortoiseRound(roundDis);
			}
			else if (type == "tortoise_fly")
			{
				std::string dis = objPoint.at("flyDis").asString();
				int flyDis = atof(dis.c_str());
				pEnemy = new CCEnemyTortoiseFly(flyDis);
			}
			else if (type == "fire_string")
			{
				std::string str = objPoint.at("begAngle").asString();
				float begAngle = atof(str.c_str());
				str = objPoint.at("time").asString();
				float time = atof(str.c_str());
				pEnemy = new CCEnemyFireString(begAngle, time);
			}
			else if (type == "flyfish")
			{
				std::string str = objPoint.at("offsetH").asString();
				float offsetH = atof(str.c_str());
				str = objPoint.at("offsetV").asString();
				float offsetV = atof(str.c_str());
				str = objPoint.at("duration").asString();
				float duration = atof(str.c_str());

				pEnemy = new CCEnemyFlyFish(offsetH, offsetV, duration);
			}
			else if (type == "boss")
			{
				isBossMap = true;
				pEnemy = new CCEnemyBoss();
				pBossEnemy = pEnemy;
			}
			else if (type == "addmushroom")
			{
				std::string str = objPoint.at("nums").asString();
				int nums = atoi(str.c_str());
					pEnemy = new CCEnemyAddMushroom(nums);
			}
			else if (type == "battery")
			{
				float delay = objPoint.at("delay").asFloat();
				pEnemy = new CCEnemyBattery(delay);
			}
			else if (type == "darkcloud")
			{
				float delay = objPoint.at("delay").asFloat();
				int style = objPoint.at("style").asFloat();
				pEnemy = new CCEnemyDarkCloud(delay, style);
			}

			if (pEnemy != NULL)
			{
				pEnemy->setTileCoord(tileXY);
				pEnemy->setEnemyPos(Vec2(posX, posY));
				EnemyArray.pushBack(pEnemy);
			}
		}
		else if (name == "gadget")
		{
			float dis = objPoint.at("ladderDis").asFloat();
			int val;
			CCGadget *pGadget = NULL;
			if (type == "ladderLR")
			{
				// �����ƶ�������
				pGadget = new CCGadgetLadderLR(dis);
				val = objPoint.at("LorR").asInt();
				pGadget->setStartFace(val);
			}
			else if (type == "ladderUD")
			{
				// �����ƶ�������
				pGadget = new CCGadgetLadderUD(dis);
				val = objPoint.at("UorD").asInt();
				pGadget->setStartFace(val);
			}

					if (pGadget != NULL)
					{
						pGadget->setStartPos(Vec2(posX, posY));
						GadgetArray.pushBack(pGadget);
					}
				}
				else if (name == "mushroom")
				{
					if (type == "MushroomReward")
					{
						// ���˱������ӵ�Ģ����
						pMushroomPointArray->addControlPoint(tileXY);
					}
					else if (type == "MushroomAddLife")
					{
						// ��ͼ�н��е�һ������Ģ����
						addLifePoint = tileXY;
					}
				}
				else if (name == "others")
				{
					if (type == "BirthPoint")
					{
						// ������ڵ�ǰ��ͼ�ϵĳ�����
						marioBirthPos = this->tilecoordToPosition(tileXY);
						marioBirthPos.x += this->getTileSize().width/2;
					}
					else if (type == "flagpoint")
					{
						flagPoint = Vec2(posX, posY);
					}
					else if (type == "finalpoint")
					{
						finalPoint = Vec2(posX, posY);
					}
					else if (type == "bridgestartpos")
					{
						bridgeTileStartPos = tileXY;
					}
				}
			}

}

void CCGameMap::launchEnemyInMap()
{
	CCEnemy *tempEnemy = NULL;
	unsigned int enemyCount = EnemyArray.size();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (CCEnemy *)EnemyArray.at(idx);
		tempEnemy->setPosition(tempEnemy->getEnemyPos());
		switch (tempEnemy->getEnemyType())
		{
		case eEnemy_flower:
		case eEnemy_AddMushroom:
			this->addChild(tempEnemy, 3);
			break;
		default:
			this->addChild(tempEnemy, 7);
			break;
		}
		tempEnemy->launchEnemy();
	}
}

void CCGameMap::launchGadgetInMap()
{
	CCGadget *tempGadget = NULL;
	unsigned int gadgetCount = GadgetArray.size();
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		tempGadget = (CCGadget *)GadgetArray.at(idx);
		tempGadget->setPosition(tempGadget->getStartPos());
		this->addChild(tempGadget, 3);
		tempGadget->launchGadget();
	}
}

void CCGameMap::enemyVSHero()
{
	CCEnemy *tempEnemy = NULL;
	EnemyVSHero vsRet;
	unsigned int enemyCount = EnemyArray.size();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (CCEnemy *)EnemyArray.at(idx);
		if (tempEnemy->getEnemyState() == eEnemyState_active)
		{
			vsRet = tempEnemy->checkCollisionWithHero();
			switch (vsRet)
			{
			case eVS_heroKilled:
				{
					if (! Hero::getHeroInstance()->getIsSafeTime())
					{
						Hero::getHeroInstance()->changeForGotEnemy();
					}
					break;
				}
			case eVS_enemyKilled:
				{
					tempEnemy->forKilledByHero();
					//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("CaiSiGuaiWu.ogg");
					break;
				}
			default:
				break;
			}
		}
	}
}

void CCGameMap::update(float dt)
{
	this->enemyVSHero();
	this->bulletVSEnemy();
}

void CCGameMap::stopUpdateForHeroDie()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = EnemyArray.size();
	CCEnemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (CCEnemy *)EnemyArray.at(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->stopEnemyUpdate();
			switch (pEnemy->getEnemyType())
			{
			case eEnemy_mushroom:
			case eEnemy_tortoise:
				break;
			case eEnemy_flower:
			case eEnemy_flyFish:
			case eEnemy_tortoiseRound:
			case eEnemy_tortoiseFly:
			case eEnemy_fireString:
			//case eEnemy_Boss:
				pEnemy->stopAllActions();
				break;
			}
			
			pEnemy->unscheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = BulletArray.size();
	CCBullet *pBullet = NULL;	
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (CCBullet *)BulletArray.at(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = GadgetArray.size();
	CCGadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (CCGadget *)GadgetArray.at(idx);
		pGadget->stopAllActions();
		pGadget->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

TMXLayer* CCGameMap::getCloudLayer() const
{
	return cloudLayer;
}

TMXLayer* CCGameMap::getLandLayer()const
{
	return landLayer;
}

TMXLayer* CCGameMap::getPipeLayer()const
{
	return pipeLayer;
}

TMXLayer* CCGameMap::getBlockLayer()const
{
	return blockLayer;
}

TMXLayer* CCGameMap::getTrapLayer()const
{
	return trapLayer;
}

TMXObjectGroup* CCGameMap::getObjectLayer()const
{
	return objectLayer;
}

TMXLayer* CCGameMap::getCoinLayer()const
{
	return coinLayer;
}

TMXLayer* CCGameMap::getFlagpoleLayer()const
{
	return flagpoleLayer;
}

Point CCGameMap::getMarioBirthPos()
{
	return marioBirthPos;
}

// ����һ��Tile����ϵ�µĵ㣬�����õ��Ӧ��ͼ�е�ͼ��Ԫ������
TileType CCGameMap::tileTypeforPos(Point tileCoord)
{
	int GID = pipeLayer->getTileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Pipe;
	}
	GID = blockLayer->getTileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Block;
	}
	GID = landLayer->getTileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Land;
	}
	GID = trapLayer->getTileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Trap;
	}
	GID = coinLayer->getTileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Coin;
	}
	GID = flagpoleLayer->getTileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Flagpole;
	}
	return eTile_NoneH;
}

// ��block���� ��Щblockש���ǿ��б�����ģ�
// ��Щש������˸���ʺţ�ͷ��һ�»ᵯ��һ�������
// �ڵ�ͼ��ͼ�������У���blockType����ʾ
// 1: ��˸���ʺ�
// 2: ��ͨ��һ�������ש��
void CCGameMap::breakBlock(Point tileCoord, BodyType _bodyType)
{
	int gID = blockLayer->getTileGIDAt(tileCoord);

	Value pD;
	pD = this->getPropertiesForGID(gID);
	
	if (pD.isNull()==false )
	{
		int value = pD.asValueMap().at("blockType").asInt();
		if (value)
		{
			int blockType = value;
			if (blockType == 2)
			{
				switch (_bodyType)
				{
				case eBody_Normal:
					{
						// ��������:ש�鱻������Ŀ飬Ȼ����ʧ��
						this->showBlockBroken(tileCoord);
						blockLayer->removeTileAt(tileCoord);
					}
					break;
				case eBody_Small:
					{
						// ������С������£�ש��ֻ�������ƶ�һ��Ȼ���ٻָ�λ��
						this->showBlockJump(tileCoord);
						//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.ogg");
					}
					break;
				default:
					break;
				}
			}
			else if (blockType == 1)  // �������ש��
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					// ���統ǰ�����������ʺ�û�б�����							
					pItemCoordArray->addControlPoint(tileCoord);

					// ��֮��Ҫ�ѽ���Ģ���������˸���ʺ�ש����
					// Ӧ�÷�����һ�����εĵط�����Ȼ��blockͼ�㣬�����õ�ȷʵ��ձ�����ͼ��
					//if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
					//{
					//	// �����������Ģ�����Ǹ���ͼ����
					//	this->showAddLifeMushroom(tileCoord);
					//}
					if (mushroomPointContains(tileCoord))
					{
						// �����Ģ����ô��ҾͲ�չʾ��Ծ����
						resetCoinPoint = tileCoord;
						this->resetCoinBlockTexture();
						this->showNewMushroom(tileCoord, _bodyType);
						this->deleteOneMushPointFromArray(tileCoord);
					} 
					else
					{
						// ��ש��������������Ҳ�����
						if (CCRANDOM_0_1() > 0.4f)
						{
							//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
							this->showJumpUpBlinkCoin(tileCoord);
							// �������ͨ�Ľ�ң���ôչʾһ����Ծ����
							this->showCoinJump(tileCoord);
						}
						else
						{
							this->showCoinJump(tileCoord);
							// ͣ��0.2��
							enemyTilePos = tileCoord;
							this->runAction(Sequence::create(DelayTime::create(0.2f),
								CallFunc::create(CC_CALLBACK_0(CCGameMap::randomShowEnemy,this)), NULL));
						}

					}
				}
				else
				{
					//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.ogg");
				}
			}
			else if (blockType == 3)
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					pItemCoordArray->addControlPoint(tileCoord);
					// ���ص�ש�飬һ�������н���Ģ��
					Sprite *pSprite = blockLayer->getTileAt(tileCoord);
					pSprite->setSpriteFrame(_brokenCoin);
					this->showAddLifeMushroom(tileCoord);
				}
			}
		}
	}
	//else
	//{
	//	// Ҫ�����еı������ӵ�Ģ������˸��ש����Ļ�������жϴ���Ҳ����ִ����
	//	//if (mushroomPointContains(tileCoord))
	//	//{
	//	//	this->showNewMushroom(tileCoord, _bodyType);
	//	//	this->deleteOneMushPointFromArray(tileCoord);
	//	//}
	//	// ��������������˵�Ļ�����ô���еĽ���Ģ�������ж϶���ִ�������
	//	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	//	{
	//		// �����������Ģ�����Ǹ���ͼ����
	//		this->showAddLifeMushroom(tileCoord);
	//	}
	//}
}

void CCGameMap::randomShowEnemy()
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wma");

	// �����Ĺ�����Ҳ��һ�����
	if (CCRANDOM_0_1() > 0.5f)
	{
		pRandomEnemy = new CCEnemyMushroom();
		if (CCRANDOM_0_1() > 0.5f)
		{
			// ���ó�ʼ��������
			pRandomEnemy->setMoveOffset(pRandomEnemy->getccMoveOffset());
		}
	} 
	else
	{
		int val = 1;
		if (CCRANDOM_0_1() < 0.5f)
		{
			val = 1;
		}
		pRandomEnemy = new CCEnemyTortoise(val);
	}

	Point pos = this->tilecoordToPosition(enemyTilePos);
	pos.x += tileSize.width/2;

	pRandomEnemy->setPosition(pos);
	this->addChild(pRandomEnemy, blockLayer->getLocalZOrder()-1);

	pRandomEnemy->runAction(Sequence::create(JumpBy::create(0.2f, Vec2(0, 16), 1, 20),
		CallFunc::create(CC_CALLBACK_0(CCGameMap::randomLaunchEnemy,this)), NULL));
	

}

void CCGameMap::randomLaunchEnemy()
{
	EnemyArray.pushBack(pRandomEnemy);
	//pRandomEnemy->setEnemyPos(pos);
	pRandomEnemy->setLocalZOrder(7);
	pRandomEnemy->launchEnemy();
}

bool CCGameMap::itemCoordArrayContains(Point tileCoord)
{
	Point temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pItemCoordArray->count(); ++idx)
	{
		temp = pItemCoordArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

bool CCGameMap::mushroomPointContains(Point tileCoord)
{
	Point temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

void CCGameMap::deleteOneMushPointFromArray(Point tileCoord)
{
	Point temp ;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			pMushroomPointArray->removeControlPointAtIndex(idx);
			break;
		}
	}
}

void CCGameMap::showBlockBroken(Point tileCoord)
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingPoZhuan.ogg");

	Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("singleblock.png");
	SpriteFrame *frame = SpriteFrame::createWithTexture(pTexture, Rect(0, 0, 8, 8));
	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height/2;

	Sprite *pSprite1 = Sprite::createWithSpriteFrame(frame);	
	pSprite1->setPosition(pos);
	spriteArray.pushBack(pSprite1);
	Sprite *pSprite2 = Sprite::createWithSpriteFrame(frame);
	pSprite2->setPosition(pos);
	spriteArray.pushBack(pSprite2);
	Sprite *pSprite3 = Sprite::createWithSpriteFrame(frame);
	pSprite3->setPosition(pos);
	spriteArray.pushBack(pSprite3);
	Sprite *pSprite4 = Sprite::createWithSpriteFrame(frame);
	pSprite4->setPosition(pos);
	spriteArray.pushBack(pSprite4);

	this->addChild(pSprite1);
	this->addChild(pSprite2);
	this->addChild(pSprite3);
	this->addChild(pSprite4);

	ActionInterval *pLeftUp = JumpBy::create(0.2f,
		Vec2(-this->getTileSize().width*2, this->getTileSize().height), 10, 1);
	ActionInterval *pRightUp = JumpBy::create(0.2f, 
		Vec2(this->getTileSize().width*2, this->getTileSize().height), 10, 1);
	ActionInterval *pLeftDown = JumpBy::create(0.2f, 
		Vec2(-this->getTileSize().width*3, -this->getTileSize().height), 5, 1);
	ActionInterval *pRightDown = JumpBy::create(0.2f, 
		Vec2(this->getTileSize().width*3, -this->getTileSize().height), 5, 1);

	pSprite1->runAction(pLeftUp);
	pSprite2->runAction(pRightUp);
	pSprite3->runAction(pLeftDown);
	pSprite4->runAction(Sequence::create(pRightDown,
		CallFunc::create(CC_CALLBACK_0(CCGameMap::clearSpriteArray,this)), NULL));
}

void CCGameMap::showJumpUpBlinkCoin(Point tileCoord)
{
	pItem = CCItem::create(eBlinkCoin);
	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height;
	pItem->setPosition(pos);
	pItem->setVisible(true);
	this->addChild(pItem);

	ActionInterval *pJump = JumpBy::create(0.16f, Vec2(0, this->getTileSize().height),
		this->getTileSize().height*1.5, 1);

	pItem->itemBody->runAction(sAnimationMgr->createAnimate(eAniBlinkCoin));
	pItem->runAction(Sequence::create(pJump, CallFunc::create(
		 CC_CALLBACK_0(CCGameMap::clearItem,this)), NULL));
}

void CCGameMap::showBlockJump(Point tileCorrd)
{
	Sprite *tempSprite = blockLayer->getTileAt(tileCorrd);
	ActionInterval *pJumpBy = JumpBy::create(0.2f, Vec2::ZERO, 
		this->getTileSize().height*0.5, 1);
	tempSprite->runAction(pJumpBy);
}

void CCGameMap::showCoinJump(Point tileCoord)
{
	Sprite *tempSprite = blockLayer->getTileAt(tileCoord);
	ActionInterval *pJumpBy = JumpBy::create(0.2f, Vec2::ZERO, 
		this->getTileSize().height*0.5, 1);

	// ͨ�����еĳ�Ա���������ݲ��������Լ򻯺������йش��εĴ���
	resetCoinPoint = tileCoord;

	tempSprite->runAction(Sequence::create(pJumpBy, 
		CallFunc::create( CC_CALLBACK_0(CCGameMap::resetCoinBlockTexture,this)), NULL));
}

void CCGameMap::resetCoinBlockTexture()
{
	Sprite *coinTile = blockLayer->getTileAt(resetCoinPoint);
	coinTile->setSpriteFrame(_brokenCoin);
}



void CCGameMap::showNewMushroom(Point tileCoord, BodyType _bodyType)
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHuoHua.ogg");
	// ���õ�ǰĢ��������
	mushTileCoord = Vec2(tileCoord.x, tileCoord.y - 1);

	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height/2;

	switch (_bodyType)
	{
	case eBody_Small:
		{
			pMushSprite = Sprite::create("rewardMushroomSet.png", Rect(0, 0, 16, 16));
		}
		break;
	case eBody_Normal:
		{
			pMushSprite = Sprite::create("Tools.png", Rect(0, 0, 18, 18));
		}
		break;
	default:
		break;
	}

	pMushSprite->setPosition(pos);
	this->addChild(pMushSprite);

	MoveBy *pMushJump = MoveBy::create(0.4f, Vec2(0, this->getTileSize().height));
	pMushSprite->runAction(pMushJump);
}

void CCGameMap::showAddLifeMushroom(Point tileCoord)
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wma");

	// ��һ�ε����ý���Ģ��������
	// ��һ�ν�y��������һ����λ����Ϊ�˺�����ж��Ƿ�ٵ�����Ģ������
	addLifePoint = Vec2(tileCoord.x, tileCoord.y - 1);

	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height/2;

	pAddLifeMushroom = Sprite::create("rewardMushroomSet.png", Rect(16, 0, 16, 16));

	pAddLifeMushroom->setPosition(pos);
	this->addChild(pAddLifeMushroom);

	MoveBy *pMushJump = MoveBy::create(0.4f, Vec2(0, this->getTileSize().height));
	pAddLifeMushroom->runAction(pMushJump);
}


void CCGameMap::clearItem()
{
	pItem->removeFromParentAndCleanup(true);
	pItem = NULL;
}

void CCGameMap::clearSpriteArray()
{
	Sprite *pS = NULL;
	for (unsigned int idx = 0; idx < spriteArray.size(); ++idx)
	{
		pS = (Sprite *)spriteArray.at(idx);
		pS->removeFromParentAndCleanup(true);
	}
}

bool CCGameMap::isMarioEatMushroom(Point tileCoord)
{
	if (pMushSprite == NULL)
	{
		return false;
	}
	if (tileCoord.x == mushTileCoord.x && tileCoord.y == mushTileCoord.y)
	{
		pMushSprite->removeFromParentAndCleanup(true);
		mushTileCoord = Vec2::ZERO;
		pMushSprite = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool CCGameMap::isMarioEatAddLifeMushroom(Point tileCoord)
{
	if (pAddLifeMushroom == NULL)
	{
		return false;
	}
	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	{
		pAddLifeMushroom->removeFromParentAndCleanup(true);
		mushTileCoord = Vec2::ZERO;
		pAddLifeMushroom = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

Point CCGameMap::positionToTileCoord(Point pos)
{
	int x = pos.x / this->getTileSize().width;
	int y = (this->getMapSize().height - 1) - pos.y/this->getTileSize().height;
	return Vec2(x, y);
}

Point CCGameMap::tilecoordToPosition(Point tileCoord)
{
	float x = tileCoord.x * this->getTileSize().width;
	float y = (this->getMapSize().height - 1 - tileCoord.y) * this->getTileSize().height;
	return Vec2(x, y);
}

void CCGameMap::createNewBullet()
{
	CCBullet *pBullet = NULL;
	switch (Global::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		pBullet = new CCBulletCommon();
		break;
	case eBullet_arrow:
		pBullet = new CCBulletArrow();
		break;
	default:
		break;
	}
	
    BulletArray.pushBack(pBullet);
	
	pBullet->setPosition(pBullet->getStartPos());
	this->addChild(pBullet, 7);
	pBullet->launchBullet();
}

void CCGameMap::createNewBulletForBoss(Point pos, EnemyType _enemyType)
{
	CCEnemy *pEnemy = NULL;

	switch (_enemyType)
	{
	case eEnemy_BossFire:
		pEnemy = new CCEnemyBossFire();
		break;
	case eEnemy_mushroom:
		pEnemy = new CCEnemyMushroom();
		break;
	case eEnemy_BatteryBullet:
		pEnemy = new CCEnemyBatteryBullet();
		break;
	case eEnemy_Lighting:
		pEnemy = new CCEnemyLighting();
		break;
	default:
		break;
	}

	if (pEnemy != NULL)
	{
		EnemyArray.pushBack(pEnemy);
		pEnemy->setEnemyPos(pos);
		pEnemy->setPosition(pos);
		this->addChild(pEnemy, 7);
		pEnemy->launchEnemy();
	}
}

void CCGameMap::bulletVSEnemy()
{
	unsigned int bulletCount = BulletArray.size();
	unsigned int enemyCount = EnemyArray.size();
	CCBullet *pBullet = NULL;
	CCEnemy *pEnemy = NULL;
	Vector<Node* > delBullet;
	//delBullet->retain();
    Vector<Node* > delEnemy;
	Rect bulletRect;
	Rect enemyRect;

	for (unsigned int idxBullet = 0; idxBullet < bulletCount; ++idxBullet)
	{
		pBullet = (CCBullet *)BulletArray.at(idxBullet);
		if (pBullet->getBulletState() == eBulletState_nonactive)
		{
			delBullet.pushBack(pBullet);
			continue;
		}
		bulletRect = pBullet->getBulletRect();

		for (unsigned int idxEnemy = 0; idxEnemy < enemyCount; ++idxEnemy)
		{
			pEnemy = (CCEnemy *)EnemyArray.at(idxEnemy);
			switch (pEnemy->getEnemyType())
			{
			// ���ǲ��ܱ��κ��ӵ������
			// �궷�޵���̨������ӵ����ᱻ�κ��ӵ����
			case eEnemy_BatteryBullet:
			case eEnemy_fireString:
			case eEnemy_Lighting:
			case eEnemy_DarkCloud:
				continue;
				break;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_Boss)
			{
				// ��ͨ���ӵ���Bossû������
				continue;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_BossFire)
			{
				// ��ͨ�ӵ���Boss�����Ļ���Ҳ��û�����õ�
				continue;
			}

			if (pEnemy->getEnemyState() == eEnemyState_active)
			{
				enemyRect = pEnemy->getEnemyRect();

				if (bulletRect.intersectsRect(enemyRect))
				{
					pBullet->forKilledEnemy();
					//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("HuoQiuDaDaoGuaiWu.ogg");
					pEnemy->forKilledByBullet();
				}
			}
		}
	}

	unsigned int delCount = delBullet.size();
	for (unsigned int idxDel = 0; idxDel < delCount; ++idxDel)
	{
		pBullet = (CCBullet *)delBullet.at(idxDel);
		BulletArray.eraseObject(pBullet, true);
		this->removeChild(pBullet, true);
	}
	//delBullet->release();
}

bool CCGameMap::isHeroInGadget(Point heroPos, float &gadgetLevel)
{
	bool ret = false;
	CCGadget *tempGadget = NULL;
	unsigned int gadgetCount = GadgetArray.size();
	Rect gadgetRect;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		tempGadget = (CCGadget *)GadgetArray.at(idx);
		if (tempGadget->getGadgetState() == eGadgetState_active)
		{
			gadgetRect = tempGadget->getGadgetRect();
			if (gadgetRect.containsPoint(heroPos))
			{
				gadgetLevel = tempGadget->getPosition().y + tempGadget->getGadgetSize().height;
				ret = true;
				this->heroInGadget = tempGadget;
				Hero::getHeroInstance()->setGadgetable(true);
				break;
			}
		}
	}
	return ret;
}

void CCGameMap::initBridgeArray()
{
	bridgeTileNums = 13;

	Sprite *pS = NULL;
	Point bossPos = pBossEnemy->getPosition();
	Point pos;
	for (int i = 0; i < bridgeTileNums; ++i)
	{
		Point tilePos = bridgeTileStartPos;
		tilePos.x += i;

		pS = landLayer->getTileAt(tilePos);
		pS->runAction(MoveBy::create(1.0f, Vec2(0, -60)));

		pos = this->tilecoordToPosition(tilePos);
		if (pos.x >= bossPos.x)
		{
			if (pBossEnemy->getEnemyState() == eEnemyState_active)
			{
				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossDiaoLuoQiaoXia.ogg");
				pBossEnemy->runAction(MoveBy::create(1.0f, Vec2(0, -80)));
			}
		}
	}

	// ���ǰѸ�������Ϊ���ɼ�
	pFlag->setVisible(false);
}

void CCGameMap::pauseGameMap()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = EnemyArray.size();
	CCEnemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (CCEnemy *)EnemyArray.at(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->unscheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = BulletArray.size();
	CCBullet *pBullet = NULL;	
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (CCBullet *)BulletArray.at(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = GadgetArray.size();
	CCGadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (CCGadget *)GadgetArray.at(idx);
		pGadget->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

void CCGameMap::resumeGameMap()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = EnemyArray.size();
	CCEnemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (CCEnemy *)EnemyArray.at(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->scheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = BulletArray.size();
	CCBullet *pBullet = NULL;	
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (CCBullet *)BulletArray.at(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->scheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = GadgetArray.size();
	CCGadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (CCGadget *)GadgetArray.at(idx);
		pGadget->scheduleUpdate();
	}

	this->scheduleUpdate();
}




