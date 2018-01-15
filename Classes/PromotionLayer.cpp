#include "PromotionLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "WaitingProgress.h"
#include "HintBox.h"
#include "FactionMemberLayer.h"

PromotionLayer::PromotionLayer()
{
	m_select = -1;
}


PromotionLayer::~PromotionLayer()
{

}

PromotionLayer* PromotionLayer::create(FactionMemberData *data, Node* target)
{
	PromotionLayer *pRet = new PromotionLayer();
	if (pRet && pRet->init(data, target))
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

bool PromotionLayer::init(FactionMemberData *data, Node* target)
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("promotionLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(PromotionLayer::onOk, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(PromotionLayer::onBack, this));

	//checkbox

	for (int i = 0; i < 3; i++)
	{
		std::string str = StringUtils::format("check%d", i + 1);
		m_checkBox[i] = (cocos2d::ui::CheckBox*)csbnode->getChildByName(str);
		m_checkBox[i]->addEventListener(CC_CALLBACK_2(PromotionLayer::checkBoxCallback, this));
		m_checkBox[i]->setTag(i + 2);
	}
	m_target = target;
	m_memberdata = data;
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



void PromotionLayer::checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type)
{
	Node* checkbtn = (Node*)pSender;
	int tag = checkbtn->getTag();
	switch(type)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		case cocos2d::ui::CheckBox::EventType::SELECTED://选中
			for (int i = 0; i < 3; i++)
			{
				if (i != tag - 2)
				{
					m_checkBox[i]->setSelected(false);
				}
				else
				{
					m_select = tag;
				}
			}
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED://不选中
			m_select = -1;
			break;
		default:
			break;
	}
}

void PromotionLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (m_select == m_memberdata->position || m_select < 0)
		{
			this->removeFromParentAndCleanup(true);
			return;
		}
		WaitingProgress* waitbox = WaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->promotionFaction(m_memberdata->factionid, m_memberdata->userid, m_memberdata->herotype, m_select);
	}
}

void PromotionLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void PromotionLayer::onSuccess()
{
	FactionMemberItem* mbitem = (FactionMemberItem*)m_target;
	mbitem->updatePosition(m_select);
	this->removeFromParentAndCleanup(true);
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void PromotionLayer::onErr(int errcode)
{
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox* hintbox;
	if (errcode == -4)
	{
		if (m_select == 2)
			hintbox = HintBox::create(CommonFuncs::gbk2utf("任命失败！ 副帮主人数已超限，可提升帮派等级增加副帮主人数！"));
		else
			hintbox = HintBox::create(CommonFuncs::gbk2utf("任命失败！ 长老人数已超限，可提升帮派等级增加长老人数！"));
	}
	else
	{
		hintbox = HintBox::create(CommonFuncs::gbk2utf("任命失败！请检查网络连接或者稍后重试！"));
	}
	this->addChild(hintbox);
}
