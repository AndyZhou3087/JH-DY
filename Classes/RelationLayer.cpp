#include "RelationLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"

const std::string relationdesc[] = { "一般", "好友", "师徒", "一般"};
RelationLayer::RelationLayer()
{

}


RelationLayer::~RelationLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


RelationLayer* RelationLayer::create()
{
	RelationLayer *pRet = new RelationLayer();
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

bool RelationLayer::init()
{
	Node* csbnode = CSLoader::createNode("relationLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(RelationLayer::onBack, this));

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

void RelationLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	this->scheduleOnce(schedule_selector(RelationLayer::delayShowData), 0.1f);
}

void RelationLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void RelationLayer::delayShowData(float dt)
{
	int size = GlobalData::map_NPCFriendData.size();

	int itemheight = 90;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	int c = 0;
	std::map<std::string, NPCFriendData>::iterator it;
	for (it = GlobalData::map_NPCFriendData.begin(); it != GlobalData::map_NPCFriendData.end(); ++it)
	{
		std::string npcid = it->first;

		Node* nodeitem = CSLoader::createNode("relationNode.csb");
		nodeitem->setPosition(Vec2(srollView->getContentSize().width / 2, innerheight - c * itemheight - itemheight / 2));
		srollView->addChild(nodeitem);

		cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)nodeitem->getChildByName("head");
		std::string headstr = StringUtils::format("ui/%s.png", npcid.c_str());
		headimg->loadTexture(headstr, cocos2d::ui::Widget::TextureResType::PLIST);
		headimg->setScale(0.32f);

		cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)nodeitem->getChildByName("name");
		npcname->setString(GlobalData::map_npcs[npcid].name);

		cocos2d::ui::ImageView* propeimg = (cocos2d::ui::ImageView*)nodeitem->getChildByName("npcprope");
		std::string propestr = StringUtils::format("ui/npcprope%d.png", GlobalData::map_npcs[npcid].type);
		propeimg->loadTexture(propestr, cocos2d::ui::Widget::TextureResType::PLIST);

		cocos2d::ui::Text* relation = (cocos2d::ui::Text*)nodeitem->getChildByName("relation");
		relation->setString(CommonFuncs::gbk2utf(relationdesc[GlobalData::map_myfriendly[npcid].relation].c_str()));


		int friendly = GlobalData::map_myfriendly[npcid].friendly;

		if (friendly < 0)
			relation->setString(CommonFuncs::gbk2utf("仇人"));

		int maxfriendly = GlobalData::map_NPCFriendData[npcid].maxfriendly;
		if (friendly > maxfriendly)
			friendly = maxfriendly;
		else if (friendly < -maxfriendly)
			friendly = -maxfriendly;

		int per = maxfriendly / 5;
		int count = abs(friendly / per);

		for (int m = 0; m < 5; m++)
		{
			std::string barstr = StringUtils::format("fbar_%d", m);
			cocos2d::ui::LoadingBar* friendbar = (cocos2d::ui::LoadingBar*)nodeitem->getChildByName(barstr);

			if (friendly < 0)
			{
				friendbar->loadTexture("ui/fheart0.png", cocos2d::ui::TextureResType::PLIST);
			}
			else
			{
				friendbar->loadTexture("ui/fheart1.png", cocos2d::ui::TextureResType::PLIST);
			}
			if (m < count)
			{
				friendbar->setPercent(100);
			}
			else if (m == count)
			{
				float percent = fabs(friendly % per * 100.0f / per);
				friendbar->setPercent(percent);
			}
		}


		c++;
	}
	m_loadlbl->setVisible(false);
}