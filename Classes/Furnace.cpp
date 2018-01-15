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

void Furnace::action(int minute, int exminute)
{
	//生火
	extime = exminute;
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (TIMESCALE* getActionBarTime()));

	this->scheduleOnce(schedule_selector(Furnace::onfire), getActionBarTime());
}
void Furnace::onfire(float dt)
{
	//温度+15°
	g_nature->makewarm(extime);
	
}