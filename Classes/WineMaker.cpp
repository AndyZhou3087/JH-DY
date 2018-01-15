#include "WineMaker.h"
#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;

WineMaker::WineMaker()
{

}


WineMaker::~WineMaker()
{

}

bool WineMaker::init()
{
	if (Sprite::initWithSpriteFrameName("ui/winemaker.png"))
	{

		return true;
	}
	return false;
}

void WineMaker::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}