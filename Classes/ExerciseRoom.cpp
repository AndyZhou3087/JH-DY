#include "ExerciseRoom.h"
#include "Nature.h"
#include "Hero.h"
#include "Const.h"

extern Nature* g_nature;
extern Hero* g_hero;

ExerciseRoom::ExerciseRoom()
{
}


ExerciseRoom::~ExerciseRoom()
{

}

bool ExerciseRoom::init()
{
	if (Sprite::initWithSpriteFrameName("ui/exerciseroom.png"))
	{

		return true;
	}
	return false;
}

void ExerciseRoom::action(int minute, int exminute)
{

}