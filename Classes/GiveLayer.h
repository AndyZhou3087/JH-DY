#ifndef _GIVE_LAYER_H_
#define _GIVE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;
class GiveLayer :public Layer
{
public:
	GiveLayer();
	~GiveLayer();

	bool init(std::string npcid, std::string addrid);
	virtual void onEnterTransitionDidFinish();
	static GiveLayer* create(std::string npcid, std::string addrid);
	void updataMyGoodsUI();
	void updataGiveGoodsUI();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updata();
	void onGiveGoodsItem(cocos2d::Ref* pSender);
	void onMyGoodsItem(cocos2d::Ref* pSender);
	void updateMyGoods(PackageData data);
	void updateGiveGoods(PackageData data);

	void checkValue();

	void doGiveMission();

	void giveRes(std::vector<std::string> vec_res);

	void removeSelf(float dt);
private:
	cocos2d::ui::ScrollView* m_giveGoodsSrollView;
	cocos2d::ui::ScrollView* m_myGoodsSrollView;
	cocos2d::ui::Widget* m_givebtn;
	int lastMyGoodsSrollViewHeight;
	int lastGiveGoodsSrollViewHeight;
	std::string m_npcid;
	std::vector<PackageData> myGoodsData;
	std::vector<PackageData> myGiveData;
	cocos2d::ui::Text* friendlylbl;
	int friendly;
	int giveval;
	std::string m_addrid;
	cocos2d::ui::Widget *m_backbtn;
};
#endif

