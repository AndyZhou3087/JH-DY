#include "BranchMissionLayer.h"
#include "CommonFuncs.h"
#include "GlobalData.h"
#include "GameDataSave.h"

BranchMissionLayer::BranchMissionLayer()
{
}


BranchMissionLayer::~BranchMissionLayer()
{

}

bool BranchMissionLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("branchMissionLayer.csb");
	this->addChild(csbnode);
	
	cocos2d::ui::Text* mapTxt = (cocos2d::ui::Text*)csbnode->getChildByName("maptxt");
	cocos2d::ui::Text* npcTxt = (cocos2d::ui::Text*)csbnode->getChildByName("npctxt");

	cocos2d::ui::Widget* onDropbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("dropbtn");
	onDropbtn->addTouchEventListener(CC_CALLBACK_2(BranchMissionLayer::onDropMisson, this));

	cocos2d::ui::Widget* onClosebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	onClosebtn->addTouchEventListener(CC_CALLBACK_2(BranchMissionLayer::onClose, this));


	std::string mid = GlobalData::getCurBranchPlotMissison();

	int subindex = GlobalData::map_BranchPlotMissionItem[mid].subindex;
	PlotMissionData pd = GlobalData::map_BranchPlotMissionData[mid][subindex];
	npcTxt->setString(GlobalData::map_npcs[pd.snpc].name);

	std::map<std::string, MapData>::iterator it;

	for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	{
		for (unsigned int i = 0; i < GlobalData::map_maps[it->first].npcs.size(); i++)
		{
			if (GlobalData::map_maps[it->first].npcs[i].compare(pd.snpc) == 0)
			{
				mapTxt->setString(GlobalData::map_maps[it->first].cname);
				break;
			}
		}
		
	}

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

BranchMissionLayer* BranchMissionLayer::create()
{
	BranchMissionLayer *pRet = new BranchMissionLayer();
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

void BranchMissionLayer::onDropMisson(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string curmid = GlobalData::getCurBranchPlotMissison();
		if (curmid.length() > 0)
		{
			if (GlobalData::map_BranchPlotMissionItem[curmid].count > 0)
				GlobalData::map_BranchPlotMissionItem[curmid].count--;
			GlobalData::map_BranchPlotMissionItem[curmid].time = GlobalData::map_BranchPlotMissionItem[curmid].maxtime;

			int subindex = GlobalData::map_BranchPlotMissionItem[curmid].subindex;
			GlobalData::map_BranchPlotMissionData[curmid][subindex].status = M_NONE;

			GlobalData::saveBranchPlotMissionStatus("", 0);
			GameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
		}
		this->removeFromParentAndCleanup(true);
	}
}

void BranchMissionLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}