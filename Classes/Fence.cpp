#include "Fence.h"
#include "Const.h"

Fence::Fence()
{
}


Fence::~Fence()
{

}

bool Fence::init()
{
	if (Sprite::initWithSpriteFrameName("ui/fence.png"))
	{

		return true;
	}
	return false;
}