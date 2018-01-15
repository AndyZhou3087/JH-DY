/********************************************************************
* 练功房
*********************************************************************/

#ifndef _EXERCISE_ROOM_H_
#define _EXERCISE_ROOM_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class ExerciseRoom :public Building
{
public:
	ExerciseRoom();
	~ExerciseRoom();

public:
	/****************************
	闭关操作
	@param minute 
	@param exminute 消耗真实时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(ExerciseRoom);
};
#endif

