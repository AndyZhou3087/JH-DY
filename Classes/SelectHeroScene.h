#ifndef __SELECTHERO_SCENE_H__
#define __SELECTHERO_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
class SelectHeroScene : public cocos2d::Layer
{
public:
	SelectHeroScene();
	~SelectHeroScene();
    static cocos2d::Scene* createScene();

	virtual void onExit();
    virtual bool init();
    
	CREATE_FUNC(SelectHeroScene);
	void unlockSucc(int index);
	void enterNextScene();
private:
	cocos2d::ui::ImageView* heroimg[4];
	cocos2d::ui::ImageView* selectimg[4];
	cocos2d::ui::Widget* heronameimg[4];
	cocos2d::ui::Widget* lock[4];
	int _lastSelect;
	bool isMoving;
private:
	void onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void clickMoveFinish(float dt);

};
extern SelectHeroScene* g_SelectHeroScene;
#endif
