
#ifndef __BOOK_DETAILS_LAYER__
#define __BOOK_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BookShelfLayer.h"
USING_NS_CC;

class BookDetailsLayer : public Layer
{
public:
	BookDetailsLayer();
	~BookDetailsLayer();
	bool init(BookData* bookdata);
	static BookDetailsLayer* create(BookData* bookdata);
	void removSelf();

private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);


private:
	cocos2d::ui::Button* m_okbtn;
	Node* m_csbnode;
};

#endif
