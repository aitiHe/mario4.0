#ifndef _GAME_ENUM_H_
#define _GAME_ENUM_H_

// ������ʾ�����ĵĻ�ȡ���ͣ�����ʾ�˵�ǰ����µ�״̬
// ����µ�״̬:
// eRight��       ����״̬����������״̬�У�
// eLeft:         �����ߵ�״̬��
// eJumpLeft:     ������Ծ״̬
// eJumpRight:    ������Ծ״̬

enum marioDirection
{
	eLeft,
	eRight,
	eJumpLeft,
	eJumpRight,
	eNormalRight,
	eNormalLeft,
	eFireTheHole
};

enum GameElement
{
	eElement_GameLayer,
	eElement_GameMap
};

// ����µ�����ߴ�
enum BodyType
{
	eBody_Normal,
	eBody_Small,
	eBody_Fireable
};

// ��������
enum AnimationType
{
	eAniLeft,
	eAniRight,
	eAniLeftSmall,
	eAniRightSmall,
	eAniLeftFire,
	eAniRightFire,
	eAniBlinkCoin,
	eAniMushroom,
	eAniflower,
	eAniTortoiseLeft,
	eAniTortoiseRight,
	eAniTortoiseFly,
	eAniRotatedFireBall,
	eAniBoomedFireBall,
	eAniFireActionR,
	eAniFireActionL,
	eAniFlyFishR,
	eAniFlyFishL,
	eAniArrowBroken,
	eAniSmallDie,
	eAniNormalDie,
	eAniFireDie,
	eAniBossMoveLeft,
	eAniBossMoveRight,
	eAniBossFireLeft,
	eAniBossFireRight,
	eAniBatteryBoom,   // �궷����̨����
	eAniLighting,      // ���綯��
	eAniArrowLeft,
	eAniArrowRight,
	eAniArrowDie,
	eAniArrowActionL,
	eAniArrowActionR
};

// ��ϷԪ�����ͣ���˸�Ľ�ҵ�
enum ItemType
{
	eBlinkCoin
};

// ��ͼ��ͼ��Ԫ�ص����࣬��������Щ����ײ�����Ҫ��
enum TileType
{
	eTile_Land,   // �ذ�
	eTile_Block,  // ��ҵ�С���
	eTile_Pipe,   // �ܵ�
	eTile_Trap,   // ����
	eTile_Coin,   // ���
	eTile_NoneH,  // ����ˮƽ������δ������ײ��
	eTile_NoneV,   // ����ֱ������δ������ײ��
	eTile_Flagpole  // ���
};

// ��������
enum EnemyType
{
	eEnemy_mushroom,
	eEnemy_flower,
	eEnemy_tortoise,
	eEnemy_tortoiseRound,
	eEnemy_tortoiseFly,
	eEnemy_fireString,
	eEnemy_flyFish,
	eEnemy_Boss,
	eEnemy_BossFire,  // Boss������ӵ�
	eEnemy_AddMushroom,   // ����ð��һ����Ģ���Ĺ���
	eEnemy_Battery,        // �궷�������̨
	eEnemy_BatteryBullet,           // �궷����̨�������ӵ�
	eEnemy_DarkCloud,           // ����
	eEnemy_Lighting             // ����
};

// ����״̬���л�Ծ���ǻ�Ծ������
enum EnemyState
{
	eEnemyState_active,
	eEnemyState_nonactive,
	eEnemyState_over
};

// ������������ײ���Ľ�����й��ﱻ�����ģ������Ǳ����������ģ�����û����ײ����
enum EnemyVSHero
{
	eVS_enemyKilled,
	eVS_heroKilled,
	eVS_nonKilled
};

// �ӵ�����
enum BulletType
{
	eBullet_common,
	eBullet_arrow
};

// �ӵ��Ļ״̬���շ���ȥ���ӵ��ǻ�ģ����Ǵ򵽹��������������ϰ����Ϊ�ǻ״̬
enum BulletState
{
	eBulletState_active,
	eBulletState_nonactive
};

// С���������
enum GadgetType
{
	eGadget_LadderUD,  // ��������
	eGadget_LadderLR   // ��������

};

// С�����״̬���뿪��Ļ����Զ����ٵ�
enum GadgetState
{
	eGadgetState_active,
	eGadgetState_nonactive,
	eGadgetState_over
};

// ������Ϸ���淵�ص����˵�ʱ��ԭ��
enum ToMainMenuFor
{
	efor_PassFailure,
	efor_PassSuccess,
	efor_Non,
	efor_StartGame,
	efor_GameOver,
	efor_PassAll
};

#endif