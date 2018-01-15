#include "ExerciseDoneLayer.h"
#include "CommonFuncs.h"
#include "GlobalData.h"
#include "Const.h"
#include "MyPackage.h"
#include "StorageRoom.h"
#include "GameScene.h"
ExerciseDoneLayer::ExerciseDoneLayer()
{

}


ExerciseDoneLayer::~ExerciseDoneLayer()
{
}

bool ExerciseDoneLayer::init(std::string wgidstr, std::string ngidstr, int hour)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("exerciseDoneLayer.csb");
	this->addChild(csbnode);

	m_unlockbg = (cocos2d::ui::Widget*)csbnode->getChildByName("unlockbg");
	m_unlockbg->setScale(0);
	//m_unlockbg->runAction(Sequence::create(ScaleTo::create(0.3f, 0.7f), ScaleTo::create(0.7f, 1.0f), NULL));
	m_unlockbg->runAction(ScaleTo::create(0.5f, 1.0f));
	m_title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	m_title->setVisible(false);

	m_herotext = (cocos2d::ui::Text*)csbnode->getChildByName("exptext");
	m_herotext->setVisible(false);

	m_wgtext = (cocos2d::ui::Text*)csbnode->getChildByName("wgtext");
	m_wgtext->setVisible(false);

	m_ngtext = (cocos2d::ui::Text*)csbnode->getChildByName("ngtext");
	m_ngtext->setVisible(false);

	exerciseDone(wgidstr, ngidstr, hour);
	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(ExerciseDoneLayer::showText), 0.6f);

	return true;
}

ExerciseDoneLayer* ExerciseDoneLayer::create(std::string wgidstr, std::string ngidstr, int hour)
{
	ExerciseDoneLayer *pRet = new ExerciseDoneLayer();
	if (pRet && pRet->init(wgidstr, ngidstr, hour))
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

void ExerciseDoneLayer::showText(float dt)
{
	m_title->setOpacity(0);
	m_title->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	m_herotext->setOpacity(0);
	m_herotext->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	m_wgtext->setOpacity(0);
	m_wgtext->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));

	m_ngtext->setOpacity(0);
	m_ngtext->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));

	this->scheduleOnce(schedule_selector(ExerciseDoneLayer::removeself), 3.5f);
}

void ExerciseDoneLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void ExerciseDoneLayer::exerciseDone(std::string wgidstr, std::string ngidstr, int hour)
{
	int f_gfexp = 0;
	int f_heroexp = 0;
	std::vector<PackageData*> vec_gfdata;

	std::string gfstrid[] = { wgidstr, ngidstr };
	StorageType gfSType[] = { W_GONG, N_GONG };

	for (int i = 0; i < 2; i++)
	{
		if (gfstrid[i].length() > 0)
		{
			bool isfind = false;
			PackageData * gfdata = NULL;
			gfdata = g_hero->getAtrByType(HeroAtrType(i + H_WG));
			if (gfdata != NULL && gfdata->count > 0 && gfdata->strid.compare(gfstrid[i]) == 0)
			{
				isfind = true;
			}
			else
			{
				for (unsigned int m = 0; m < StorageRoom::map_storageData[gfSType[i]].size(); m++)
				{
					gfdata = &StorageRoom::map_storageData[gfSType[i]][m];
					if (gfdata->strid.compare(gfstrid[i]) == 0)
					{
						isfind = true;
						break;
					}
				}
			}

			if (isfind)
			{
				vec_gfdata.push_back(gfdata);
			}
		}
	}

	for (unsigned int m = 0; m < vec_gfdata.size(); m++)
	{
		std::string gfname = vec_gfdata[m]->strid;
		std::vector<int> vec_gfExp = GlobalData::map_wgngs[gfname].vec_exp;
		int lv = 0;

		int curlv = vec_gfdata[m]->lv;

		//兼容上一个版本，功法等级溢出
		int maxlv = GlobalData::map_wgngs[gfname].maxlv;
		if (curlv >= maxlv)
		{
			curlv = maxlv - 1;
			vec_gfdata[m]->lv = curlv;
		}

		f_gfexp = GlobalData::map_wgngs[gfname].vec_exp.at(curlv) * hour;


		std::string str = StringUtils::format("%s%s+%d", GlobalData::map_allResource[gfname].cname.c_str(), CommonFuncs::gbk2utf("经验值： ").c_str(), f_gfexp);
		if (vec_gfdata[m]->type == W_GONG)
		{
			m_wgtext->setString(str.c_str());
		}
		else if (vec_gfdata[m]->type == N_GONG)
		{
			m_ngtext->setString(str.c_str());
		}

		vec_gfdata[m]->exp += f_gfexp;

		for (unsigned i = curlv; i < vec_gfExp.size(); i++)
		{
			if (vec_gfdata[m]->exp >= vec_gfExp[i])
			{
				lv = i + 1;
				vec_gfdata[m]->exp = vec_gfdata[m]->exp - vec_gfExp[i];
			}
		}
		int gfmaxlv = GlobalData::map_wgngs[gfname].maxlv;
		if (lv > curlv)
		{

			if (lv >= gfmaxlv)
			{
				lv = gfmaxlv - 1;
			}
			vec_gfdata[m]->lv = lv;

		}
	}

	for (unsigned int i = 0; i < GlobalData::vec_achiveData.size(); i++)
	{
		if (GlobalData::vec_achiveData[i].type == A_7)
		{
			std::string astr = GlobalData::vec_achiveData[i].vec_para[0];
			PackageData* mePackageData = g_hero->getMeHas(astr);
			if (mePackageData != NULL && GlobalData::vec_achiveData[i].finish != -1)
			{
				GlobalData::vec_achiveData[i].finish = mePackageData->lv + 1;
				GlobalData::saveAchiveData();
			}
		}

		if (GlobalData::vec_achiveData[i].type == A_10)
		{
			if (GlobalData::vec_achiveData[i].finish != -1)
			{
				int nlv = atoi(GlobalData::vec_achiveData[i].vec_para[0].c_str());
				GlobalData::vec_achiveData[i].finish = g_hero->getGfCountByLv(nlv);
				GlobalData::saveAchiveData();
			}
		}
	}

	int curlv = g_hero->getLVValue();

	std::vector<int> vec_heroExp = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp;

	f_heroexp = vec_heroExp.at(curlv) * hour;

	g_hero->setExpValue(g_hero->getExpValue() + f_heroexp);

	unsigned int i = 0;
	int lv = 0;

	for (i = curlv; i < vec_heroExp.size(); i++)
	{
		if (g_hero->getExpValue() >= vec_heroExp[i])
		{
			lv = i + 1;
			g_hero->setExpValue(g_hero->getExpValue() - vec_heroExp[i]);
		}
	}
	if (lv > curlv)
	{
		int heromaxlv = vec_heroExp.size();
		if (lv >= heromaxlv)
		{
			g_hero->setExpValue(vec_heroExp[heromaxlv - 1]);
			lv = heromaxlv - 1;
			g_hero->setLVValue(lv);
		}
		else
		{
			g_hero->setLVValue(lv);
			g_hero->setLifeValue(g_hero->getMaxLifeValue());
		}
		GlobalData::doAchive(A_2, lv + 1);
	}
	else
	{
		if (g_hero->getLVValue() == vec_heroExp.size() - 1)
		{
			GlobalData::doAchive(A_2, g_hero->getLVValue() + 1);
		}
	}

	std::string str = StringUtils::format("%s经验值： +%d", heroname[g_hero->getHeadID()-1].c_str(), f_heroexp);
	m_herotext->setString(CommonFuncs::gbk2utf(str.c_str()));
}