
#ifndef __BOOK_DETAILS_LAYER__
#define __BOOK_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "WXBookShelfLayer.h"
USING_NS_CC;

class WXBookDetailsLayer : public Layer
{
public:
	WXBookDetailsLayer();
	~WXBookDetailsLayer();
	bool init(BookData* bookdata);
	static WXBookDetailsLayer* create(BookData* bookdata);
	void removSelf();

private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);


private:
	cocos2d::ui::Button* m_okbtn;
	Node* m_csbnode;
};

#endif
