#include "AchiveLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"

AchiveLayer::AchiveLayer()
{

}


AchiveLayer::~AchiveLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


AchiveLayer* AchiveLayer::create()
{
	AchiveLayer *pRet = new AchiveLayer();
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

bool AchiveLayer::init()
{
	Node* csbnode = CSLoader::createNode("achiveLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(AchiveLayer::onBack, this));

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	m_loadlbl = Label::createWithTTF(CommonFuncs::gbk2utf("加载中..."), "fonts/STXINGKA.TTF", 28);
	m_loadlbl->setColor(Color3B(0, 0, 0));
	m_loadlbl->setPosition(Vec2(320, 600));
	this->addChild(m_loadlbl);

	GlobalData::g_gameStatus = GAMEPAUSE;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void AchiveLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	this->scheduleOnce(schedule_selector(AchiveLayer::delayShowData), 0.1f);
}

void AchiveLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void AchiveLayer::delayShowData(float dt)
{
	int size = GlobalData::vec_achiveData.size();

	int itemheight = 140;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	sort(GlobalData::vec_achiveData.begin(), GlobalData::vec_achiveData.end(), larger_callback);

	for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
	{
		AchiveItem* node = AchiveItem::create(&GlobalData::vec_achiveData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width / 2, innerheight - itemheight / 2 - i * itemheight));
		srollView->addChild(node);
	}
	m_loadlbl->setVisible(false);
}

bool AchiveLayer::larger_callback(AchiveData a, AchiveData b)
{
	int needcountA = GlobalData::getAchiveFinishCount(a);
	int needcountB = GlobalData::getAchiveFinishCount(b);
	float percentA = a.finish*100.0f / needcountA;
	float percentB = b.finish*100.0f / needcountB;
	if (percentA > percentB)
		return true;
	else
		return false;
}


AchiveItem::AchiveItem()
{

}
AchiveItem::~AchiveItem()
{

}

bool AchiveItem::init(AchiveData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;
	Node* csbnode = CSLoader::createNode("achiveNode.csb");
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	cocos2d::ui::Text* name = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	name->setString(data->name);

	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	m_barbg = (cocos2d::ui::Widget*)csbnode->getChildByName("finishbg");
	m_bar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("finishbar");

	m_finishtext = (cocos2d::ui::Text*)csbnode->getChildByName("finishtext");

	m_getimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("getimg");
	m_getimg->addTouchEventListener(CC_CALLBACK_2(AchiveItem::onGet, this));
	
	m_getimg->setUserData((void*)data);

	m_gettext = (cocos2d::ui::Text*)m_getimg->getChildByName("statutext");

	for (unsigned int i = 0; i < data->vec_rwd.size(); i++)
	{
		std::string resid = data->vec_rwd[i];
		int count = 1;
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			resid = "gd0";
		}
		else
		{
			int res = atoi(resid.c_str());
			if (res > 0)
			{
				resid = StringUtils::format("%d", res / 1000);
				count = res % 1000;
			}
		}
		std::string respath = StringUtils::format("ui/%s.png", resid.c_str());

		std::string str = StringUtils::format("rwd_%d", i + 1);
		cocos2d::ui::ImageView* rwd = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);
		rwd->loadTexture(respath, cocos2d::ui::Widget::TextureResType::PLIST);
		rwd->setVisible(true);
		std::string countstr = StringUtils::format("x%d", count);
		str = StringUtils::format("count_%d", i + 1);
		cocos2d::ui::Text* rwdcount = (cocos2d::ui::Text*)csbnode->getChildByName(str);
		rwdcount->setString(countstr);
		rwdcount->setVisible(true);
	}
	std::string descstr;
	float finishpercert = 0.0f;
	std::string finishstr;
	needcount = 0;
	if (data->type == A_0 || data->type == A_1 || data->type == A_2 || data->type == A_3 || data->type == A_5 || data->type == A_11)
	{
		descstr = StringUtils::format(data->desc.c_str(), data->vec_para[0].c_str());
	}
	else if (data->type == A_4)
	{
		descstr = StringUtils::format(data->desc.c_str(), GlobalData::map_npcs[data->vec_para[0]].name);
	}
	else if (data->type == A_6)
	{
		for (unsigned int i = 0; i < GlobalData::vec_PlotMissionData.size(); i++)
		{
			if (data->vec_para[0].compare(GlobalData::vec_PlotMissionData[i].id) == 0)
			{
				descstr = StringUtils::format(data->desc.c_str(), GlobalData::map_npcs[GlobalData::vec_PlotMissionData[i].snpc].name);
				break;
			}
		}
	}
	else if (data->type == A_7 || data->type == A_9)
	{
		descstr = StringUtils::format(data->desc.c_str(), GlobalData::map_allResource[data->vec_para[0]].cname.c_str(), data->vec_para[1].c_str());
	}
	else if (data->type == A_8)
	{
		descstr = StringUtils::format(data->desc.c_str(), GlobalData::map_MixGfData[data->vec_para[0]].name.c_str());
	}

	else if (data->type == A_10)
	{
		descstr = StringUtils::format(data->desc.c_str(), data->vec_para[0].c_str(), data->vec_para[1].c_str());
	}
	else if (data->type == A_12)
	{
		descstr = StringUtils::format(data->desc.c_str(), GlobalData::map_npcs[data->vec_para[0]].name, data->vec_para[1].c_str());
	}

	needcount = GlobalData::getAchiveFinishCount(*data);
	int finishcount = data->finish;
	if (finishcount > needcount)
		finishcount = needcount;
	finishstr = StringUtils::format("%d/%d", finishcount, needcount);
	m_bar->setPercent(finishcount*100.0f / needcount);
	m_finishtext->setString(finishstr);
	desc->setString(descstr);
	if (data->finish == -1)
	{
		finish();
	}
	else if (data->finish >= needcount)
	{
		m_getimg->loadTexture("ui/loginrbtn.png", cocos2d::ui::Widget::TextureResType::PLIST);
		m_gettext->setString(CommonFuncs::gbk2utf("领取"));
	}
	else
	{
		m_gettext->setString(CommonFuncs::gbk2utf("未完成"));
	}
	return true;
}

AchiveItem* AchiveItem::create(AchiveData *data)
{
	AchiveItem *pRet = new AchiveItem();
	if (pRet && pRet->init(data))
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
void AchiveItem::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::ImageView * imagebtn = (cocos2d::ui::ImageView*)pSender;
		AchiveData* paradata = (AchiveData*)imagebtn->getUserData();
		if (paradata->finish >= needcount)
		{
			for (unsigned int i = 0; i < paradata->vec_rwd.size(); i++)
			{
				std::string resid = paradata->vec_rwd[i];
				int res = atoi(resid.c_str());
				int count = 1;
				if (res > 0)
				{
					resid = StringUtils::format("%d", res / 1000);
					count = res % 1000;
				}
				if (resid.compare(0, 1, "g") == 0)
				{
					count = atoi(resid.substr(1).c_str());
					GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + count);

				}
				else
				{
					PackageData pdata;
					pdata.strid = resid;
					pdata.count = count;
					pdata.type = GlobalData::getResType(resid);
					pdata.extype = GlobalData::getResExType(resid);

					if (!g_hero->checkifHasGF_Equip(resid) && GlobalData::tempHasGf_Equip(resid).length() <= 0)
						StorageRoom::add(pdata);
				}
			}
			paradata->finish = -1;
			finish();
			GlobalData::saveAchiveData();
		}
		else
		{
			HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf("成就未完成！！"));
			Director::getInstance()->getRunningScene()->addChild(hintbox);
		}
	}
}

void AchiveItem::finish()
{
	m_barbg->setVisible(false);
	m_bar->setVisible(false);
	m_finishtext->setVisible(false);
	m_gettext->setString(CommonFuncs::gbk2utf("已领取"));
	m_getimg->loadTexture("ui/disnpcbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
	m_getimg->setEnabled(false);
}