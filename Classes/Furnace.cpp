#include "Furnace.h"
#include "Nature.h"
#include "Const.h"
#include "HintBox.h"
#include "CommonFuncs.h"
extern Nature* g_nature;

Furnace::Furnace()
{
}


Furnace::~Furnace()
{

}

bool Furnace::init()
{
	if (Sprite::initWithSpriteFrameName("ui/furnace.png"))
	{

		return true;
	}
	return false;
}
void Furnace::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}
void Furnace::action(int minute, int exminute)
{
	//生火
	extime = exminute;
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (TIMESCALE* getActionBarTime()));

	this->scheduleOnce(schedule_selector(Furnace::onfire), getActionBarTime());
}


void Furnace::shake(Node * node) {
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
void Furnace::onfire(float dt)
{
	//温度+15°
	g_nature->makewarm(extime);
	
}