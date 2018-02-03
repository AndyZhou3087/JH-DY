#include "WXBookShelf.h"

#include "Const.h"


WXBookShelf::WXBookShelf()
{
}


WXBookShelf::~WXBookShelf()
{

}

bool WXBookShelf::init()
{
	if (Sprite::initWithSpriteFrameName("ui/bookshelf.png"))
	{

		return true;
	}
	return false;
}

void WXBookShelf::action(int minute, int exminute)
{

}