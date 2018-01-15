#include "Forgingtable.h"

#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;

Forgingtable::Forgingtable()
{
}


Forgingtable::~Forgingtable()
{

}


bool Forgingtable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/forgingtable.png"))
	{

		return true;
	}
	return false;
}

void Forgingtable::action(int minute, int exminute)
{
	//锻造
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}