#include "Bed.h"
#include "Nature.h"
#include "Hero.h"
#include "Const.h"

extern Nature* g_nature;
extern Hero* g_hero;

Bed::Bed()
{
}


Bed::~Bed()
{

}

bool Bed::init()
{
	if (Sprite::initWithSpriteFrameName("ui/bed.png"))
	{

		return true;
	}
	return false;
}

void Bed::initA()
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
void Bed::action(int minute, int exminute)
{
	//更新时间刷新的间隔，这样实现消耗游戏时间
	g_nature->setTimeInterval(exminute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
	//
	g_hero->sleep(minute, exminute / 60);
}

void Bed::initBg()
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