/********************************************************************
* 书架
*********************************************************************/

#ifndef _BOOKSHELF_H_
#define _BOOKSHELF_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class WXBookShelf :public Building
{
public:
	WXBookShelf();
	~WXBookShelf();

public:

	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(WXBookShelf);
};
#endif

