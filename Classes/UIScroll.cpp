#include "UIScroll.h"

#define DIFF 8//添加控件的间隔
#define MAXCOUNT 100
UIScroll::UIScroll()
{
}


UIScroll::~UIScroll()
{
}

UIScroll* UIScroll::create(float width, float height)
{
	UIScroll *pRet = new UIScroll();
	if (pRet && pRet->init(width, height))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool UIScroll::init(float width, float height)
{
	m_width = width;
	m_height = height;


	m_containerLayer = Layer::create();

	scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setPosition(-width / 2, -m_height / 2);
	scrollView->setViewSize(Size(width, height));

	scrollView->setContainer(m_containerLayer);
	scrollView->setContentSize(Size(width, height));
	scrollView->setDelegate(this);

	scrollView->setBounceable(true);

	this->addChild(scrollView);

	return true;
}

void UIScroll::addEventText(std::string text, int fontsize, Color3B color3b)
{
	Label* textlbl = Label::createWithTTF(text, "fonts/STXINGKA.TTF", fontsize);
	textlbl->setLineBreakWithoutSpace(true);
	textlbl->setMaxLineWidth(m_width);
	textlbl->setColor(color3b);
	addEventLabel(textlbl);

}

void UIScroll::addEventLabel(Label* label)
{
	label->setAnchorPoint(Vec2(0, 0));
	label->setPosition(Vec2(0, 0));

	int curlblheight = label->getContentSize().height;

	Vector<Node*> vec_tlbl = scrollView->getContainer()->getChildren();
	int lblcount = vec_tlbl.size();

	if (lblcount >= MAXCOUNT)
	{
		lblcount = MAXCOUNT - 1;
		vec_tlbl.at(0)->removeFromParentAndCleanup(true);
		vec_tlbl.erase(0);
	}

	int totalNodeH = 0;
	for (int i = 0; i < lblcount; i++)
	{
		Node* tlbl = vec_tlbl.at(i);
		tlbl->setPositionY(tlbl->getPositionY() + curlblheight + DIFF);
		totalNodeH += tlbl->getContentSize().height;
	}

	//添加新的内容时，先滑到最下
	scrollView->setContentOffset(Vec2(0, 0));


	int contentH = totalNodeH + lblcount * DIFF + curlblheight;
	if (contentH > scrollView->getViewSize().height)
		scrollView->setContentSize(Size(m_width, contentH));

	m_containerLayer->addChild(label);
}

void UIScroll::clean()
{
	scrollView->setContentSize(Size(m_width, m_height));
	scrollView->setContentOffset(Vec2(0, 0));
	m_containerLayer->removeAllChildrenWithCleanup(true);

}

bool UIScroll::onTouchBegan(Touch * touch, Event *unused_event)
{
	return true;

}

void UIScroll::onTouchMoved(Touch*touch, Event*unused_event)
{

}

void UIScroll::onTouchEnded(Touch*touch, Event *unused_event)
{

}


void UIScroll::scrollViewDidScroll(ScrollView* view)
{

}
