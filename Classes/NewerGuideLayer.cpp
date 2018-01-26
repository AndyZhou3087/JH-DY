
#include "NewerGuideLayer.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "AnalyticUtil.h"
#include "GameScene.h"
#include "HomeLayer.h"
#include "BuildingUILayer.h"
#include "CannotTouchLayer.h"
#include "GoWhereLayer.h"
#include "ActionGetLayer.h"
#include "HeroProperNode.h"

std::map<int, std::vector<std::string>> map_words;

std::string descText[][6] = { { "我是老贤，从现在开始，再也没有人照顾你的饮食起居，以后这里就是你的住所", "什么？就这个地方？这里这么破旧，如何住人？", "因为这里长时间无人居住，所以你只能先把屋内建造好，让老夫来教教你怎么修建房屋吧"},
{ "这里可以制作一系列的工具和武器，先制作锻造台吧", "", ""},//1
{ "这里可以查看制作所需要的材料", "", ""},//2
{ "", "", ""},
{ "这么多东西可以制造？", "锻造台等级越高，能制造的东西越多！先造个斧头试试", ""},//4
{ "", "", ""},
{ "", "", ""},
{"现在资源有点不够了，咱们要出去采集资源，不过出门之前先把刚才的工具带上", "", ""},//7
{"", "", ""},
{"", "", ""},
{"", "", ""},
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "世界这么大，去每一个位置都会根据距离消耗不同的时间，切记状态不好时就不要到处去浪了！", "", "" },//14
{ "这里记录时间，注意照顾好自己的生活时间！", "", "" },//15
{ "", "", "" },
{ "", "", "" },
{ "这里是后山有各种资源供你采集，先挖点铁矿吧", "", "" },//18
{ "这是产出铁矿了吗？", "是的，点击拾取按钮就可以获取了，不过记住，你最多只能带五种不同的物品", "" },//19
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "好了，材料够了，现在先把其它两个工具造好吧", "", "" },//41
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "这样就算是造好基础工具了？", "是啊！你看看，你现在家徒四壁，有个好工具你才能更有效率的采集资源来建造房子啊！还等什么呢！快出去收集资源啊！多收集写木材、石矿、铁矿回来！", ""},//45
{ "", "", "" },
{ "慢着！出门前先把所有装备都带上，后山有狼出没，你要小心！", "", "" },//47
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{"感叹号提示着在这个地点的角色有任务找你，快去看一下是什么事情吧，以你现在的实力还打不过这里的任何人，千万不要轻易进行挑战，切记！！！", "", ""},//58
{ "慢！你看看你现在自己的状态，快回去补充一下！", "", "" },//59
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "", "", "" },
{ "难怪我觉得哪里奇怪！", "生命可以通过睡觉恢复、精神不好就要喝酒愉悦一下，饿了就多吃东西，但是生东西最好少吃，受了伤就要尽快治疗，不要让伤势加重，明白了吗？", "" },//65
{ "", "", "" },
{ "可以在仓库里选择食物，吃烤肉填饱肚子", "", "" },//67
{ "可以在仓库里选择药材，使用三七可以治疗内伤，酒可以恢复精神值，需要在酒桌上使用。", "", "" },//68
{ "掉落没有拾取的物品会放在临时存放点，每个地点都有一个，来了记得带回去~~", "", "" },//69
{ "床主要用来恢复你的生命，生命不足的时候记得通过睡觉来恢复！", "", "" },//70
{"厨房主要用来建造食物，人是铁饭是钢，快把厨房造好吧", "", ""},
{ "", "", "" },
{ "酒窖主要用来酿酒，一个人的日子不好熬，你只能与酒为伴了。切记喝酒必须在酒桌上才能喝！", "", "" },
{ "", "", "" },
{ "", "", ""},
{ "", "", "" }
};

NewerGuideLayer* g_NewerGuideLayer = NULL;

std::vector<std::string> NewerGuideLayer::vec_userdata;
bool NewerGuideLayer::isShowing = false;
NewerGuideLayer::NewerGuideLayer()
{
	wordindex = 0;
	iscannext = false;
	isShowing = false;
}

NewerGuideLayer::~NewerGuideLayer()
{
}

NewerGuideLayer* NewerGuideLayer::create(int type, std::vector<Node*> stencilNodes)
{
	NewerGuideLayer *pRet = new NewerGuideLayer();
	if (pRet && pRet->init(type, stencilNodes))
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

bool NewerGuideLayer::init(int step, std::vector<Node*> stencilNodes)
{
	if (!Layer::init())
	{
		return false;
	}
	m_colorlayer = NULL;

	initDialog();
	m_step = step;
	Node* csbnode = CSLoader::createNode("npctalkLayer.csb");
	this->addChild(csbnode);

	m_talknode = (cocos2d::ui::Widget*)csbnode->getChildByName("node");

	cocos2d::ui::Widget* newerguider = (cocos2d::ui::Widget*)m_talknode->getChildByName("npcimg");
	newerguider->setOpacity(150);
	cocos2d::ui::ImageView* heroimg = (cocos2d::ui::ImageView*)m_talknode->getChildByName("heroimg");

	std::string heroidstr = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);

	cocos2d::ui::Text* wordtext = (cocos2d::ui::Text*)m_talknode->getChildByName("text");
	wordtext->setVisible(false);

	m_wordlbl = Label::createWithTTF("", "fonts/STXINGKA.TTF", 26);
	m_wordlbl->setColor(Color3B(0, 0, 0));
	m_wordlbl->setLineBreakWithoutSpace(true);
	m_wordlbl->setMaxLineWidth(610);
	m_wordlbl->setPosition(Vec2(0, -115));
	m_talknode->addChild(m_wordlbl);

	if (m_step == 4 || m_step == 19 || m_step == 45 || m_step == 65)
	{
		heroimg->setOpacity(255);
		newerguider->setOpacity(100);
	}
	else
	{
		heroimg->setOpacity(150);
		newerguider->setOpacity(255);
	}
	if (m_step == 69)
	{
		m_talknode->setPositionY(560);
	}

	if (map_words[step].size() > 0)
	{
		std::string wordstr = map_words[step][0];
		showWord(wordstr);
	}
	else
	{
		m_talknode->setVisible(false);
	}

	if (stencilNodes.size() > 0)
	{
		m_clippingNode = ClippingNode::create();
		m_clippingNode->setInverted(true);//设置底板可见
		m_clippingNode->setAlphaThreshold(0.5f);//设置透明度Alpha值为0
		this->addChild(m_clippingNode, -1);

		int al = 200;
		if (map_words[step].size() <= 0)
			al = 0;
		m_colorlayer = LayerColor::create(Color4B(0, 0, 0, al));
		m_clippingNode->addChild(m_colorlayer);

		Node* stencil = Node::create();
		for (unsigned int i = 0; i < stencilNodes.size(); i++)
		{
			if (i < vec_userdata.size())
			{
				std::string path = StringUtils::format("ui/%s.png", vec_userdata[i].c_str());
				Sprite* cnode = Sprite::createWithSpriteFrameName(path);
				cnode->setPosition(stencilNodes[i]->getParent()->convertToWorldSpace(stencilNodes[i]->getPosition()));
				//float scalex = stencilNodes[i]->getContentSize().width / cnode->getContentSize().width;
				//float scaley = stencilNodes[i]->getContentSize().height / cnode->getContentSize().height;
				//cnode->setScale(scalex + 0.1f, scaley + 0.1f);
				cnode->runAction(RepeatForever::create(Blink::create(4.0f, 5)));
				stencil->addChild(cnode);
			}
		}
		m_clippingNode->setStencil(stencil);

		showAnim(stencilNodes[stencilNodes.size() - 1]->getParent()->convertToWorldSpace(stencilNodes[stencilNodes.size() - 1]->getPosition()));
	}
	else
	{
		m_colorlayer = LayerColor::create(Color4B(0, 0, 0, 200));
		this->addChild(m_colorlayer, -1);
	}
	isallclick = false;
	if (m_step == 0 || m_step == 2 || m_step == 4 || m_step == 15 || m_step == 19 || m_step == 45 || m_step == 47 || m_step == 59 || m_step == 65)
		isallclick = true;

	if (isallclick)
	{
		iscannext = true;
	}

	//layer 点击事件，屏蔽下层事件
	m_listener = EventListenerTouchOneByOne::create();
	m_listener->setSwallowTouches(true);

	m_listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		Vec2 point = Director::getInstance()->convertToGL(touch->getLocationInView());//获得当前触摸的坐标 
		starPos = touch->getLocation();
		if (!isallclick && stencilNodes.size() > 0)
		{
			Vec2 vec = stencilNodes[stencilNodes.size() - 1]->getParent()->convertToWorldSpace(stencilNodes[stencilNodes.size() - 1]->getPosition());

			auto rect = Rect(vec.x - stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.width / 2, vec.y - stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.height / 2, stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.width, stencilNodes[stencilNodes.size() - 1]->getBoundingBox().size.height);
			if (rect.containsPoint(point))//如果触点处于rect中  
			{
				iscannext = true;
				m_listener->setSwallowTouches(false);
			}
		}
		return true;
	};
	m_listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		if (iscannext)
		{
			wordindex++;

			int size = map_words[m_step].size();
			if (wordindex >= size)
			{
				if (m_step == 0)
				{
					removeSelf();
					HomeLayer* homelayer = (HomeLayer*)g_gameLayer->getChildByName("homelayer");
					if (homelayer != NULL)
						homelayer->checkNewerGuide();
				}
				else if (m_step == 2)
				{
					removeSelf();
					BuildingUILayer* buildlayer = (BuildingUILayer*)g_gameLayer->getChildByName("builduilayer");
					if (buildlayer != NULL)
						buildlayer->checkNewerGuide();
				}
				else if (m_step == 4)
				{
					removeSelf();
					BuildingUILayer* buildlayer = (BuildingUILayer*)g_gameLayer->getChildByName("builduilayer");
					if (buildlayer != NULL)
						buildlayer->checkNewerGuide();
				}
				else if (m_step == 15)
				{
					removeSelf();
					GoWhereLayer* golayer = (GoWhereLayer*)g_gameLayer->getChildByName("gowherelayer");
					if (golayer != NULL)
						golayer->showNewerGuide(16);
				}
				else if (m_step == 19)
				{
					removeSelf();
					ActionGetLayer* alayer = (ActionGetLayer*)g_gameLayer->getChildByName("ActionGetLayer");
					if (alayer != NULL)
						alayer->showNewerGuide(20);
				}
				else if (m_step == 47)
				{
					removeSelf();
					HeroProperNode* heroProperNode = (HeroProperNode*)g_gameLayer->getChildByName("OutDoor")->getChildByName("csbnode")->getChildByName("HeroProperNode");
					if (heroProperNode != NULL)
						heroProperNode->showNewerGuide(48);
				}
				else if (m_step == 59)
				{
					removeSelf();
					TopBar* topbar = (TopBar*)g_gameLayer->getChildByName("topbar");
					if (topbar != NULL)
						topbar->showNewerGuide(60);
				}
				else if (m_step == 65)
				{
					removeSelf();
					HomeLayer* homelayer = (HomeLayer*)g_gameLayer->getChildByName("homelayer");
					if (homelayer != NULL)
						homelayer->checkNewerGuide();
				}
				else
				{
					removeSelf();
				}
			}
			else
			{
				if (m_step == 4 || m_step == 19 || m_step == 45 || m_step == 65)
				{
					if (wordindex % 2 == 0)
					{
						heroimg->setOpacity(255);
						newerguider->setOpacity(150);
					}
					else
					{
						heroimg->setOpacity(150);
						newerguider->setOpacity(255);
					}
				}
				else
				{
					if (wordindex % 2 == 0)
					{
						heroimg->setOpacity(150);
						newerguider->setOpacity(255);
					}
					else
					{
						heroimg->setOpacity(255);
						newerguider->setOpacity(150);
					}
				}

				showWord(map_words[m_step][wordindex]);
			}
		}
		return;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
    GameDataSave::getInstance()->setIsNewerGuide(step, 0);

#ifdef ANALYTICS
	if (step == 0)
		AnalyticUtil::onEvent("newerstart");
	else if (step == 12)
		AnalyticUtil::onEvent("firstout");
	else if (step == 46)
		AnalyticUtil::onEvent("secondout");
	else if (step == 58)
		AnalyticUtil::onEvent("newerend");
#endif

	return true;
}

void NewerGuideLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

bool NewerGuideLayer::checkifNewerGuide(int index)
{
	bool ret = false;
	ret = GameDataSave::getInstance()->getIsNewerGuide(index);
	if (!ret)
	{
		vec_userdata.clear();
	}
	return ret;
}

void NewerGuideLayer::showAnim(Vec2 pos)
{

	if (map_words[m_step].size() <= 0)
	{
		Sprite* quan = Sprite::create("images/newerguide/newerquan.png");
		quan->setPosition(pos);
		quan->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 0.8f), ScaleTo::create(0.5f, 1.0f), NULL)));
		this->addChild(quan, 1);
	}

	auto sj = Sprite::create("images/newerguide/newersj0.png");
	sj->setAnchorPoint(Vec2(0, 1));
	sj->setPosition(pos.x, pos.y);
	this->addChild(sj, 1);

	//创建帧动画序列，名词形式
	auto animation = Animation::create();
	for (int i = 0; i< 2; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "images/newerguide/newersj%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	//设置帧动画属性
	animation->setDelayPerUnit(2.0f / 6);//每一帧停留的时间
	animation->setRestoreOriginalFrame(true);//播放完后回到第一帧
	auto animate = Animate::create(animation);
	sj->runAction(RepeatForever::create(animate));

	if (m_step == 1 || m_step >= 70)
	{
		auto hammer = Sprite::create("images/newerguide/newerhammer0.png");
		hammer->setPosition(pos.x + 20, pos.y + 20);
		this->addChild(hammer);

		//创建帧动画序列，名词形式
		auto animation1 = Animation::create();
		for (int i = 0; i < 2; i++)
		{
			char szName[100] = { 0 };
			sprintf(szName, "images/newerguide/newerhammer%d.png", i);
			animation1->addSpriteFrameWithFile(szName);
		}
		//设置帧动画属性
		animation1->setDelayPerUnit(2.0f / 6);//每一帧停留的时间
		animation1->setRestoreOriginalFrame(true);//播放完后回到第一帧
		auto animate1 = Animate::create(animation1);
		hammer->runAction(RepeatForever::create(animate1));
	}

}

void NewerGuideLayer::pushUserData(std::string strdata)
{
	vec_userdata.push_back(strdata);
}

void NewerGuideLayer::showWord(std::string wordstr)
{
	std::string utf8word = CommonFuncs::gbk2utf(wordstr.c_str());
	m_wordlbl->setString(utf8word);

	std::vector<std::string> vec_resname;
	vec_resname.push_back(CommonFuncs::gbk2utf("木材"));
	vec_resname.push_back(CommonFuncs::gbk2utf("石矿"));
	vec_resname.push_back(CommonFuncs::gbk2utf("铁矿"));
	vec_resname.push_back(CommonFuncs::gbk2utf("切记喝酒必须在酒桌上才能喝！"));
	
	std::vector<std::string>::iterator it;
	for (unsigned int i = 0;i < vec_resname.size(); i++)
	{
		std::string resname = vec_resname[i];
		std::size_t findpos = utf8word.find(resname);
		if (findpos != std::string::npos)
		{
			int sindex = findpos / 3;
			int len = resname.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				m_wordlbl->getLetter(i)->setColor(Color3B(204, 4, 4));
			}
		}
	}
}

void NewerGuideLayer::initDialog()
{
	if (map_words.size() <= 0)
	{
		int total = sizeof(descText) / sizeof(descText[0]);
		for (int i = 0; i < total; i++)
		{
			for (int m = 0; m < 6; m++)
			{
				if (descText[i][m].length() > 0)
				{
					map_words[i].push_back(descText[i][m]);
				}
			}
		}
	}
}

void NewerGuideLayer::removeSelf()
{
	vec_userdata.clear();
	isShowing = false;
	this->removeFromParentAndCleanup(true);
	g_NewerGuideLayer = NULL;
}