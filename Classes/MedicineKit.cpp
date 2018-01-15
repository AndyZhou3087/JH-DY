#include "MedicineKit.h"
#include "Nature.h"
#include "Const.h"
#include "GameScene.h"

MedicineKit::MedicineKit()
{

}


MedicineKit::~MedicineKit()
{

}

bool MedicineKit::init()
{
	if (Sprite::initWithSpriteFrameName("ui/medicinekit.png"))
	{

		return true;
	}
	return false;
}

void MedicineKit::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}