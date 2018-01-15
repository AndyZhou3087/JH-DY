#include "UnlockLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
UnlockLayer::UnlockLayer()
{

}


UnlockLayer::~UnlockLayer()
{
}

bool UnlockLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("unlockLayer.csb");
	this->addChild(csbnode);

	m_unlockbg = (cocos2d::ui::Widget*)csbnode->getChildByName("unlockbg");
	m_unlockbg->setScale(0);
	//m_unlockbg->runAction(Sequence::create(ScaleTo::create(0.3f, 0.7f), ScaleTo::create(0.7f, 1.0f), NULL));
	m_unlockbg->runAction(ScaleTo::create(0.5f, 1.0f));
	m_title = (cocos2d::ui::Widget*)csbnode->getChildByName("unlocktitle");
	m_title->setVisible(false);

	m_desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	m_desc->setVisible(false);

	m_addrname = (cocos2d::ui::Text*)csbnode->getChildByName("addrname");
	m_addrname->setVisible(false);

	std::map<std::string, MapData>::iterator it;

	std::string addrnamestr;
	for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	{
		std::string mapid = GlobalData::map_maps[it->first].strid;

		std::vector<std::string> tmp;

		CommonFuncs::split(mapid, tmp, "-");
		int mapchapter = atoi(tmp[0].substr(1, tmp[0].size() - 1).c_str());
		if (mapchapter == GlobalData::getUnlockChapter())
		{
			addrnamestr.append(GlobalData::map_maps[mapid].cname);
			addrnamestr.append(CommonFuncs::gbk2utf("，"));
		}
	}
	m_addrname->setString(addrnamestr.substr(0, addrnamestr.length() - 3));
	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(UnlockLayer::showText), 0.6f);

	return true;
}

UnlockLayer* UnlockLayer::create()
{
	UnlockLayer *pRet = new UnlockLayer();
	if (pRet && pRet->init())
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

void UnlockLayer::showText(float dt)
{
	m_title->setOpacity(0);
	m_title->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	m_desc->setOpacity(0);
	m_desc->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	m_addrname->setOpacity(0);
	m_addrname->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	this->scheduleOnce(schedule_selector(UnlockLayer::removeself), 3.5f);
}

void UnlockLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}