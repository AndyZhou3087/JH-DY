#include "BookShelf.h"

#include "Const.h"


BookShelf::BookShelf()
{
}


BookShelf::~BookShelf()
{

}

bool BookShelf::init()
{
	if (Sprite::initWithSpriteFrameName("ui/bookshelf.png"))
	{

		return true;
	}
	return false;
}

void BookShelf::action(int minute, int exminute)
{

}