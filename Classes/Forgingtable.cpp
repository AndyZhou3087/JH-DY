#include "Forgingtable.h"

#include "Nature.h"
#include "Const.h"
extern Nature* g_nature;

Forgingtable::Forgingtable()
{
}


Forgingtable::~Forgingtable()
{

}


bool Forgingtable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/forgingtable.png"))
	{

		return true;
	}
	return false;
}

void Forgingtable::initA()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_top = CSLoader::createNode("GameTopLayer.csb");
	addChild(m_top, 0);
	m_top->ignoreAnchorPointForPosition(false);
	m_top->setAnchorPoint(Vec2(0.5, 1));
	m_top->setPosition(size.width * 0.5, size.height);

	auto m_bottom = CSLoader::createNode("GameBottomLayer.csb");
	m_bottom->ignoreAnchorPointForPosition(false);
	m_bottom->setAnchorPoint(Vec2(0.5, 0));
	m_bottom->setPosition(size.width * 0.5, 0);
	addChild(m_bottom, 1);
}

void Forgingtable::action(int minute, int exminute)
{
	//锻造
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}

void Forgingtable::initBg()
{
	const Size size = Director::getInstance()->getVisibleSize();
	auto m_bg = Sprite::create("");
	addChild(m_bg, 0);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	auto m_bgGround = Sprite::create("");
	addChild(m_bgGround, 1);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create("");
	addChild(tree);
	tree->setAnchorPoint(Vec2(1, 0.5));
	tree->setPosition(size.width, 120);
}