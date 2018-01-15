#include "BookDetailsLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "Hero.h"
#include "SoundManager.h"
#include "Const.h"

BookDetailsLayer::BookDetailsLayer()
{

}

BookDetailsLayer::~BookDetailsLayer()
{

}

bool BookDetailsLayer::init(BookData* bookdata)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	m_csbnode = CSLoader::createNode("bookDetailsLayer.csb");
	this->addChild(m_csbnode);

	m_okbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("okbtn");
	m_okbtn->addTouchEventListener(CC_CALLBACK_2(BookDetailsLayer::onOk, this));

	cocos2d::ui::Text* resname = (cocos2d::ui::Text*)m_csbnode->getChildByName("namelbl");

	resname->setString(GlobalData::map_allResource[bookdata->strid].cname);

	cocos2d::ui::Text* qulbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("qulbl");

	cocos2d::ui::ImageView* resbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("buildsmall");
	std::string qustr = "ui/buildsmall.png";
	int qu = -1;
	if (bookdata->type == WEAPON || bookdata->type == PROTECT_EQU)
	{
		qu = GlobalData::map_equips[bookdata->strid].qu;
	}
	else if (bookdata->type == N_GONG || bookdata->type == W_GONG)
	{
		qu = GlobalData::map_wgngs[bookdata->strid].qu;
	}
	if (qu > 0)
	{
		qustr = StringUtils::format("ui/qubox%d.png", qu);
		qulbl->setVisible(true);
		qulbl->setString(CommonFuncs::gbk2utf(qudesc[qu - 1].c_str()));
		qulbl->setTextColor(qucolor[qu - 1]);
	}

	resbox->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
	resbox->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());

	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)resbox->getChildByName("Image");

	std::string str = StringUtils::format("ui/%s.png", bookdata->strid.c_str());
	resimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	resimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::Text* resdesc = (cocos2d::ui::Text*)m_csbnode->getChildByName("desclbl");
	resdesc->setString(GlobalData::map_allResource[bookdata->strid].desc);

	std::string maxlvstr;
	std::string tmpstr;
	if (bookdata->type == WEAPON || bookdata->type == PROTECT_EQU)
	{
		if (bookdata->type == WEAPON)
		{
			tmpstr = StringUtils::format("攻击力增加%d", GlobalData::map_equips[bookdata->strid].atk);
		}
		else if (bookdata->type == PROTECT_EQU)
		{
			tmpstr = StringUtils::format("防御力增加%d", GlobalData::map_equips[bookdata->strid].df);
		}

		cocos2d::ui::Text* maxlvtext = (cocos2d::ui::Text*)m_csbnode->getChildByName("maxlvtext");
		maxlvtext->setString(CommonFuncs::gbk2utf("品质："));
		maxlvstr = qulbl->getString();
	}
	else if (bookdata->type == N_GONG || bookdata->type == W_GONG)
	{
		int maxlv = GlobalData::map_wgngs[bookdata->strid].maxlv;
		maxlvstr = StringUtils::format("%d", maxlv);

		cocos2d::ui::Text* crittext = (cocos2d::ui::Text*)m_csbnode->getChildByName("crittext");
		crittext->setVisible(true);

		cocos2d::ui::Text* critlbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("critlbl");
		critlbl->setVisible(true);

		cocos2d::ui::Text* skilltext = (cocos2d::ui::Text*)m_csbnode->getChildByName("skilltext");
		cocos2d::ui::Text* skillname = (cocos2d::ui::Text*)m_csbnode->getChildByName("skillname");
		cocos2d::ui::Text* skilldesc = (cocos2d::ui::Text*)m_csbnode->getChildByName("skilldesc");

		std::string critstr;
		if (bookdata->type == W_GONG)
		{
			tmpstr = StringUtils::format("攻击力增加%d", GlobalData::map_wgngs[bookdata->strid].vec_bns[maxlv-1]);
			crittext->setString(CommonFuncs::gbk2utf("最大暴击率："));
			critstr = StringUtils::format("%.4f%%", GlobalData::map_wgngs[bookdata->strid].vec_cirt[maxlv - 1]);
		}
		else if (bookdata->type == N_GONG)
		{
			tmpstr = StringUtils::format("防御力增加%d", GlobalData::map_wgngs[bookdata->strid].vec_bns[maxlv - 1]);
			crittext->setString(CommonFuncs::gbk2utf("最大闪避率："));
			critstr = StringUtils::format("%.4f%%", GlobalData::map_wgngs[bookdata->strid].vec_dodge[maxlv - 1]);
		}
		critlbl->setTextColor(Color4B(204, 4, 4, 255));
		critlbl->setString(critstr);
		int skilltype = GlobalData::map_wgngs[bookdata->strid].skilltype;
		if (skilltype > S_SNONE)
		{
			skilltext->setVisible(true);
			skillname->setString(GlobalData::map_gfskills[skilltype].name);
			std::string sdesc = skilldesc->getString();
			sdesc.append(GlobalData::map_gfskills[skilltype].desc1);
			skilldesc->setString(sdesc);
		}

	}
	cocos2d::ui::Text* maxlv = (cocos2d::ui::Text*)m_csbnode->getChildByName("maxlv");
	maxlv->setString(maxlvstr);
	maxlv->setTextColor(Color4B(204, 4, 4, 255));

	cocos2d::ui::Text* bns = (cocos2d::ui::Text*)m_csbnode->getChildByName("bns");
	bns->setString(CommonFuncs::gbk2utf(tmpstr.c_str()));
	bns->setTextColor(Color4B(204, 4, 4, 255));

	cocos2d::ui::Text* status = (cocos2d::ui::Text*)m_csbnode->getChildByName("status");

	if (g_hero->getMeHas(bookdata->strid) != NULL)
	{
		tmpstr = "已拥有";
		status->setTextColor(Color4B(27, 141, 0, 255));
	}
	else
	{
		std::string mixid = GlobalData::getMixGF();

		bool isinmixSec = false;
		if (mixid.length() > 0)
		{
			for (unsigned int n = 0; n < GlobalData::map_MixGfData[mixid].vec_secgf.size(); n++)
			{
				if (bookdata->strid.compare(GlobalData::map_MixGfData[mixid].vec_secgf[n]) == 0)
				{
					isinmixSec = true;
					break;
				}
			}
		}
		if (!isinmixSec)
			tmpstr = "未拥有";
		else
		{
			tmpstr = "已拥有";
			status->setTextColor(Color4B(27, 141, 0, 255));
		}
	}

	status->setString(CommonFuncs::gbk2utf(tmpstr.c_str()));

	bool isnpchas = false;
	std::string wherestr;
	wherestr.append(CommonFuncs::gbk2utf("打败"));
	std::map<std::string, NpcData>::iterator it;
	for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
	{
		NpcData ndata = GlobalData::map_npcs[it->first];
		for (unsigned int i = 0; i < ndata.winres.size(); i++)
		{
			if (ndata.winres[i].compare(bookdata->strid) == 0)
			{
				wherestr.append(ndata.name);
				wherestr.append(CommonFuncs::gbk2utf("、"));
				isnpchas = true;
			}
		}
	}

	if (isnpchas)
	{
		wherestr = wherestr.substr(0, wherestr.length() - 3);
		wherestr.append(CommonFuncs::gbk2utf("有机会获得"));
		Label* wherelbl = Label::createWithTTF(wherestr, "fonts/STXINGKA.TTF", 25);
		wherelbl->setAnchorPoint(Vec2(0, 1));
		wherelbl->setLineBreakWithoutSpace(true);
		wherelbl->setMaxLineWidth(350);
		wherelbl->setString(wherestr);
		wherelbl->setColor(Color3B(0, 0, 0));
		wherelbl->setPosition(m_csbnode->getChildByName("where")->getPosition());
		m_csbnode->addChild(wherelbl);
		//NPC名称颜色 红色
		std::map<std::string, NpcData>::iterator it;
		for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
		{
			std::string npcname = GlobalData::map_npcs[it->first].name;
			std::size_t findpos;
			std::string temp = wherestr;
			int count = 0;
			while (1){
				findpos = temp.find(npcname);
				if (findpos != std::string::npos){
					temp = temp.substr(findpos + npcname.length(), temp.length() - npcname.length());
					int sindex = (wherestr.length() - temp.length() - npcname.length()) / 3;
					int len = npcname.length() / 3;
					for (int i = sindex; i < sindex + len; i++)
					{
						wherelbl->getLetter(i)->setColor(Color3B(230, 35, 35));
					}
				}
				else
					break;
			}
		}
	}
	else
	{
		cocos2d::ui::Text* wherelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("where");
		wherelbl->setString(CommonFuncs::gbk2utf("可在锻造台制造"));
	}

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

BookDetailsLayer* BookDetailsLayer::create(BookData* bookdata)
{
	BookDetailsLayer *pRet = new BookDetailsLayer();
	if (pRet && pRet->init(bookdata))
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

void BookDetailsLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void BookDetailsLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}