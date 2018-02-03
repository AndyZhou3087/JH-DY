#include "WXExerciseRoom.h"
#include "Nature.h"
#include "Hero.h"
#include "Const.h"

extern Nature* g_nature;
extern Hero* g_hero;

WXExerciseRoom::WXExerciseRoom()
{
}


WXExerciseRoom::~WXExerciseRoom()
{

}
void WXExerciseRoom::jumpDown(cocos2d::Node *node, float dt) {
	if (nullptr == node) {
		return;
	}

	const float originY = node->getPositionY();
	node->setPositionY(originY + dt);

	ActionInterval *action = Sequence::create(
		MoveBy::create(0.2, Vec2(0, -dt - 10)),
		MoveBy::create(0.2, Vec2(0, 20)),
		MoveBy::create(0.1, Vec2(0, -18)),
		MoveBy::create(0.1, Vec2(0, 13)),
		MoveBy::create(0.1, Vec2(0, -5)),


		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 0.98, 1, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	node->runAction(action);
}
bool WXExerciseRoom::init()
{
	if (Sprite::initWithSpriteFrameName("ui/exerciseroom.png"))
	{

		return true;
	}
	return false;
}

void WXExerciseRoom::action(int minute, int exminute)
{

}


bool WXExerciseRoom::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height / size.width;
	if (rate >= 1.49) {
		return true;
	}
	else {
		return false;
	}
}
