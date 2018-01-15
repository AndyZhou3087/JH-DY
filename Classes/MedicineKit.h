
/********************************************************************
*	 药箱
*********************************************************************/

#ifndef _MEDICINEKIT_H_
#define _MEDICINEKIT_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class MedicineKit :public Building
{
public:
	MedicineKit();
	~MedicineKit();

public:
	/****************************
	制药操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(MedicineKit);
};
#endif

