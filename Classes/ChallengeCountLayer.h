#ifndef _CHALLENGECOUNT_LAYER_H_
#define _CHALLENGECOUNT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class ChallengeCountLayer :public Layer
{
public:
	ChallengeCountLayer();
	~ChallengeCountLayer();

	bool init(int* wincount, int winnpccount, bool isRevive = false);
	static ChallengeCountLayer* create(int* wincount, int winnpccount, bool isRevive = false);


	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);

private:
	int* m_wincount;
	int m_winnpcount;
	cocos2d::ui::Text* valuelbl[3];
	cocos2d::ui::TextBMFont* totalwincountlbl;
	cocos2d::ui::Button* selectbtn[3];
	int lastclickindex;
	bool m_isRevive;
private:
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onHeroimg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);


};
#endif

