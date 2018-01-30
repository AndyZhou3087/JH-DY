#include "CookTable.h"
#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;
CookTable::CookTable()
{

}


CookTable::~CookTable()
{

}

bool CookTable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/cooktable.png"))
	{

		return true;
	}
	return false;
}

void CookTable::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void CookTable::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (TIMESCALE* getActionBarTime()));
}

void CookTable::shake(Node * node) {
	if (NULL == node) {
		return;
	}

	node->runAction(CCSequence::create(
		MoveBy::create(0.02, Vec2(0, 15)),
		MoveBy::create(0.02, Vec2(0, -27)),
		MoveBy::create(0.02, Vec2(0, 22)),
		MoveBy::create(0.02, Vec2(0, -14)),
		MoveBy::create(0.02, Vec2(0, 4)),
		NULL));
}
