/********************************************************************
* 修复强化成功 UI界面
*********************************************************************/
#ifndef _REPAIROK_LAYER_H_
#define _REPAIROK_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;

class RepairOKLayer :public Layer
{
public:
	RepairOKLayer();
	~RepairOKLayer();

	virtual bool init(PackageData* pdata, int type);
	static RepairOKLayer* create(PackageData* pdata, int type);
private:
	void showText(float dt);
	void removeself(float dt);

private:
	cocos2d::ui::Widget* m_unlockbg;
	cocos2d::ui::Widget* m_repairokquan;
	cocos2d::ui::Text* m_title;
	cocos2d::ui::ImageView* m_okimg;
	cocos2d::ui::ImageView* m_repairImgbox;
	cocos2d::ui::Text* m_desc;
	int m_type;
};
#endif

