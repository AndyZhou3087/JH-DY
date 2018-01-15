#include "SepcialNpcLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "FightLayer.h"
#include "HintBox.h"
SepcialNpcLayer::SepcialNpcLayer()
{

}


SepcialNpcLayer::~SepcialNpcLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);
}

SepcialNpcLayer* SepcialNpcLayer::create(std::string addrname)
{
	SepcialNpcLayer *pRet = new SepcialNpcLayer();
	if (pRet && pRet->init(addrname))
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

bool SepcialNpcLayer::init(std::string addrid)
{
	m_addrstr = addrid;
	m_csbnode = CSLoader::createNode("specialNpcLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(SepcialNpcLayer::onBack, this));

	cocos2d::ui::Widget* fightbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("fightbtn");
	fightbtn->addTouchEventListener(CC_CALLBACK_2(SepcialNpcLayer::onFight, this));

	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.jpg", GlobalData::map_maps[addrid].tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
	return true;
}
void SepcialNpcLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void SepcialNpcLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void SepcialNpcLayer::onFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_addrstr.compare("m13-1") == 0 && GlobalData::getUnlockChapter() < 13)
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("解锁第13章方可挑战！挑战失败，复活后可继续挑战，挑战过程中可通过胜点购买属性，中途退出也可获得收益。"));
			g_gameLayer->addChild(hbox, 5);
			return;
		}

		GlobalData::map_maps[m_addrstr].npcs.clear();
		std::map<std::string, NpcData>::iterator it;
		for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
		{
			NpcData ndata = GlobalData::map_npcs[it->first];
			if (ndata.lv >= 10)
				GlobalData::map_maps[m_addrstr].npcs.push_back(ndata.id);
		}
		if (g_gameLayer != NULL)
			g_gameLayer->addChild(FightLayer::create(m_addrstr, "n005"), 4, "fightlayer");
		//this->removeFromParentAndCleanup(true);
	}
}