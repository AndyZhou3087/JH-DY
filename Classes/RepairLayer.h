#ifndef _REPAIR_LAYER_H_
#define _REPAIR_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class RepairLayer :public Layer
{
public:
	RepairLayer();
	~RepairLayer();

	bool init(int type);
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	static RepairLayer* create(int type);//type:1--修理；2--强化

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updataUI();
	void onResItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRepairItem(cocos2d::Ref* pSender);
	void showAnim();
	void finishAnim(Ref* pSender);
	void delayShowOkLayer(float dt);
	void delayCanClick(float dt);
	void dofail();
private:
	cocos2d::ui::Widget* repairbox;
	cocos2d::ui::Widget* needResbox[3];
	cocos2d::ui::ImageView* needResImg[3];
	cocos2d::ui::Text* needResCountLbl[3];
	cocos2d::ui::ImageView* repairResImg;
	cocos2d::ui::ImageView* repairResBoxImg;
	std::vector<PackageData*> vec_RepairData;
	std::vector<std::string> repairneedres;
	int getMyPackageResCount(std::string strid);
	cocos2d::ui::Button *m_actionBtn;
	cocos2d::ui::Text* succrandtext;
	cocos2d::ui::Widget* failSprite;
	bool resIsEnough;
	int m_type;
	int useluckstone;
	int succrnd;
	int clickindex;
	std::vector<Node*> vec_repairItem;
};
#endif

