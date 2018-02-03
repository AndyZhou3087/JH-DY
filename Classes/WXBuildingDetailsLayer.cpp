#include "WXBuildingDetailsLayer.h"
#include "SoundManager.h"
#include "CommonFuncs.h"

WXBuildingDetailsLayer::WXBuildingDetailsLayer()
{

}

WXBuildingDetailsLayer::~WXBuildingDetailsLayer()
{

}


void WXBuildingDetailsLayer::reset() {
	updateBloodBar();
	resetBoss();
}

bool WXBuildingDetailsLayer::init(Building* pdata)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 160));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("buildingDetailsLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(WXBuildingDetailsLayer::onOk, this));


	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("buildsmall")->getChildByName("Image");

	std::string str = StringUtils::format("ui/s%s.png", pdata->data.name);
	resimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	resimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::Text* buildingname = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl");

	buildingname->setString(pdata->data.cname);

	cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)csbnode->getChildByName("desclbl");
	desclbl->setString(pdata->data.desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void WXBuildingDetailsLayer::updateBloodBar() {

	float rate = 0;
	if (rate > 0) {
		rate = 1 - 10 * 1.0f / 5;
	}

	if (nullptr != m_bloodBar) {
		static const Vec2 offSize = m_bloodBar->getTextureRect().origin;
		static const float h = m_bloodBar->getContentSize().height;
		static const float w = m_bloodBar->getContentSize().width;
		float width = rate * w;
		m_bloodBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}


WXBuildingDetailsLayer* WXBuildingDetailsLayer::create(Building* pdata)
{
	WXBuildingDetailsLayer *pRet = new WXBuildingDetailsLayer();
	if (pRet && pRet->init(pdata))
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

void WXBuildingDetailsLayer::updateTime() {

	int s = 100;
	s -= 1;
	if (s <= 10 && s >= 0) {
		
	}

	if (s <= 0) {
		s = 0;
	}
}

void WXBuildingDetailsLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}


void WXBuildingDetailsLayer::resetBoss() {

	if (nullptr != m_boss) {
		m_boss->setPositionY(m_bossOriginPosY + 500);
		m_boss->setOpacity(255);
	}
}

void WXBuildingDetailsLayer::onScoreChange() {

	updateBloodBar();
}

void WXBuildingDetailsLayer::on1sTimer() {

	if (m_bStop) {
		return;
	}

	updateTime();
}

void WXBuildingDetailsLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void WXBuildingDetailsLayer::playBossShowEffect(CallFunc * callback) {
	if (nullptr == m_boss) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	Point midPos = Vec2(size.width * 0.5, size.height * 0.5);
	ccBezierConfig config;
	config.endPosition = Point(m_bossOriginPosX, m_bossOriginPosY);

	config.controlPoint_1 = Point(midPos.x, midPos.y + 50);
	config.controlPoint_2 = Point(midPos.x, midPos.y + 100);


	ActionInterval * showAction = Sequence::create(
		EaseSineOut::create(MoveTo::create(0.8, midPos)),
		ScaleTo::create(0.2, 5, 5, 1),
		Spawn::create(
		ScaleTo::create(0.8, 1, 1, 1),
		BezierTo::create(0.8, config),
		NULL
		),
		CallFunc::create([=](){
		playBossActiveEffect();
	}),
		callback,
		NULL);
	m_boss->runAction(showAction);
}

void WXBuildingDetailsLayer::playBossActiveEffect()
{

}


void WXBuildingDetailsLayer::initBossBombParticleSystem() {
	if (nullptr == m_boss) {
		return;
	}

	m_emitterBomb = ParticleSystemQuad::create("");
	m_emitterBomb->setTexture(Director::getInstance()->getTextureCache()->addImage(""));
	m_boss->addChild(m_emitterBomb, 100);
	m_emitterBomb->setPosition(Vec2(m_boss->getContentSize().width * 0.5, m_boss->getContentSize().height * 0.5));
	m_emitterBomb->stopSystem();
}

