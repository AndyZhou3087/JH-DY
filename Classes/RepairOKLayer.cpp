#include "RepairOKLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
RepairOKLayer::RepairOKLayer()
{

}


RepairOKLayer::~RepairOKLayer()
{
}

bool RepairOKLayer::init(PackageData* pdata, int type)
{
	m_type = type;
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("repairOKLayer.csb");
	this->addChild(csbnode);

	m_unlockbg = (cocos2d::ui::Widget*)csbnode->getChildByName("unlockbg");
	m_unlockbg->setScale(0);

	m_unlockbg->runAction(ScaleTo::create(0.5f, 1.0f));
	m_title = (cocos2d::ui::Text*)csbnode->getChildByName("oknode")->getChildByName("name");
	m_title->setString(GlobalData::map_allResource[pdata->strid].cname);
	m_title->setVisible(false);

	m_repairokquan = (cocos2d::ui::Widget*)csbnode->getChildByName("oknode")->getChildByName("repairokquan");
	m_repairokquan->setVisible(false);

	m_desc = (cocos2d::ui::Text*)csbnode->getChildByName("oknode")->getChildByName("desc");
	std::string strslv = StringUtils::format("强化+%d", pdata->slv);
	m_desc->setString(CommonFuncs::gbk2utf(strslv.c_str()));
	m_desc->setVisible(false);
	
	m_repairImgbox = (cocos2d::ui::ImageView*)csbnode->getChildByName("oknode")->getChildByName("repair");
	m_repairImgbox->setVisible(false);

	cocos2d::ui::ImageView* m_repairImg = (cocos2d::ui::ImageView*)m_repairImgbox->getChildByName("img");

	std::string qustr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[pdata->strid].qu);
	m_repairImgbox->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
	m_repairImgbox->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());

	std::string repairResStr = StringUtils::format("ui/%s.png", pdata->strid.c_str());
	m_repairImg->loadTexture(repairResStr, cocos2d::ui::TextureResType::PLIST);
	m_repairImg->setContentSize(Sprite::createWithSpriteFrameName(repairResStr)->getContentSize());

	m_okimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("oknode")->getChildByName("okimg");
	std::string str;
	if (type == 1)
		str = "ui/repairoktext.png";
	else
		str = "ui/strengthoktext.png";
	m_okimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	m_okimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	m_okimg->setVisible(false);


	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(RepairOKLayer::showText), 0.6f);

	return true;
}

RepairOKLayer* RepairOKLayer::create(PackageData* pdata, int type)
{
	RepairOKLayer *pRet = new RepairOKLayer();
	if (pRet && pRet->init(pdata, type))
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

void RepairOKLayer::showText(float dt)
{
	m_repairokquan->setOpacity(0);
	m_repairokquan->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	
	m_title->setOpacity(0);
	m_title->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	if (m_type == 2)
	{
		m_desc->setOpacity(0);
		m_desc->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	}

	m_okimg->setOpacity(0);
	m_okimg->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));

	m_repairImgbox->setOpacity(0);
	m_repairImgbox->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));

	this->scheduleOnce(schedule_selector(RepairOKLayer::removeself), 3.5f);
}

void RepairOKLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}