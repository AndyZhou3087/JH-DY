#include "WineTable.h"
#include "Hero.h"

#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;
extern Hero* g_hero;
WineTable::WineTable()
{

}


WineTable::~WineTable()
{

}

bool WineTable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/winetable.png"))
	{

		return true;
	}
	return false;
}

void WineTable::action(int minute, int exminute)
{
	g_hero->drinking();
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}