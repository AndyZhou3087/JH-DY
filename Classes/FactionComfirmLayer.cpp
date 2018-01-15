#include "FactionComfirmLayer.h"
#include "SoundManager.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "MD5.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "GameScene.h"
#include "FactionMainLayer.h"
#include "FactionMemberLayer.h"

bool FactionComfirmLayer::init(FactionListData *fldata)
{
    if (!Layer::init()) 
	{
		return false;
    }
	f_action = F_NONE;
	m_fldata = fldata;
	Node* csbnode = CSLoader::createNode("factionComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(FactionComfirmLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(FactionComfirmLayer::onCancel, this));

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	std::string str;
	if (GlobalData::mytitle == 1)
	{
		str = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("是否确认花费50金元宝解散").c_str(), m_fldata->factionname.c_str(), CommonFuncs::gbk2utf("？").c_str());
	}
	else
	{
		str = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("是否确认退出").c_str(), m_fldata->factionname.c_str(), CommonFuncs::gbk2utf("？").c_str());

	}
	desctext->setString(str);
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

FactionComfirmLayer* FactionComfirmLayer::create(FactionListData *fldata)
{
	FactionComfirmLayer *pRet = new FactionComfirmLayer();
	if (pRet && pRet->init(fldata))
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

void FactionComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::mytitle == 1)
		{
			int mygold = GlobalData::getMyGoldCount();

			if (mygold >= 50)
			{
				if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					GlobalData::dataIsModified = true;
					GlobalData::setMyGoldCount(0);
					HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}

				f_action = F_RELEASE;
				WaitingProgress* waitbox = WaitingProgress::create("处理中...");
				Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
				ServerDataSwap::init(this)->leaveFaction(1, m_fldata->id, g_hero->getHeadID());
			}
			else
			{
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！！"));
				Director::getInstance()->getRunningScene()->addChild(hint, 1);
				removSelf();
			}
		}
		else
		{
			f_action = F_LEAVE;

			WaitingProgress* waitbox = WaitingProgress::create("加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->leaveFaction(0, m_fldata->id, g_hero->getHeadID());
		}
	}
}

void FactionComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void FactionComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void FactionComfirmLayer::onSuccess()
{
	if (f_action == F_RELEASE)
	{
		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 50);
		int usegold = GlobalData::getUseGold() + 50;
		GlobalData::setUseGold(usegold);
	}
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	GlobalData::myFaction = 0;
	GlobalData::mytitle = 0;
	f_action = F_NONE;
	removSelf();

	FactionMemberLayer* flayer = (FactionMemberLayer*)g_gameLayer->getChildByName("factionmemberlayer");
	if (flayer != NULL)
	{
		flayer->removeFromParentAndCleanup(true);
	}

	FactionMainLayer* fmlayer = (FactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmlayer != NULL)
	{
		fmlayer->getFactionListData();
	}
}

void FactionComfirmLayer::onErr(int errcode)
{
	f_action = F_NONE;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据处理异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box,1);
	removSelf();
}

