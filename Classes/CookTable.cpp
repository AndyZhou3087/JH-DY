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

void CookTable::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (TIMESCALE* getActionBarTime()));
}