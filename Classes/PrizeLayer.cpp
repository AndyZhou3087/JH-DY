#include "PrizeLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "MyPackage.h"
#include "StorageRoom.h"
#include "HuafeiAwardListLayer.h"

const std::string prizerwd[] = {"g20", "80020", "82005"};

PrizeLayer::PrizeLayer()
{
}


PrizeLayer::~PrizeLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}

bool PrizeLayer::init()
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("prizeLayer.csb");
	this->addChild(csbnode);

	//关闭按钮
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(PrizeLayer::onClose, this));

	m_okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	m_okbtn->addTouchEventListener(CC_CALLBACK_2(PrizeLayer::onOk, this));

	cocos2d::ui::Widget* huafeienter = (cocos2d::ui::Widget*)csbnode->getChildByName("huafeienter");
	huafeienter->addTouchEventListener(CC_CALLBACK_2(PrizeLayer::onHuafeiEnter, this));

	m_input = (cocos2d::ui::TextField*)csbnode->getChildByName("input");
	m_input->setString("");
	m_input->addEventListener(CC_CALLBACK_2(PrizeLayer::textFieldEvent, this));
	
	cocos2d::ui::Widget* inputbox = (cocos2d::ui::Widget*)csbnode->getChildByName("box");
	m_edit = cocos2d::ui::EditBox::create(Size(380, 44), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui/blank.png"));
	m_edit->setPosition(Point(inputbox->getPosition()));
	m_edit->setFontColor(Color3B::BLACK);
	m_edit->setPlaceHolder(CommonFuncs::gbk2utf("请输入兑换码:").c_str());
	m_edit->setPlaceholderFontSize(30);
	m_edit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	m_edit->setPlaceholderFontColor(Color3B(112, 116, 109));
	m_edit->setMaxLength(20);
	m_edit->setVisible(false);
	m_edit->setDelegate(this);
	this->addChild(m_edit);

#if 1//(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_input->setVisible(false);
	m_edit->setVisible(true);
#endif
	//layer 点击事件，屏蔽下层事件

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


void PrizeLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void PrizeLayer::onHuafeiEnter(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		HuafeiAwardListLayer* layer = HuafeiAwardListLayer::create();
		this->addChild(layer);
	}
}

void PrizeLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{

}

void PrizeLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{

}

void PrizeLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void PrizeLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{

}


void PrizeLayer::textFieldEvent(Ref * pSender, cocos2d::ui::TextField::EventType type)
{
	std::string str;
	cocos2d::ui::TextField * textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		break;
	}
}


void PrizeLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string codestr;
		
		codestr = m_input->getString();
#if 1//(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		codestr = m_edit->getText();
#else
		codestr = m_input->getString();
#endif
		WaitingProgress* waitbox = WaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getCoupons(codestr);
	}
}

void PrizeLayer::showRwd()
{
	int startx = 180;
	int spacex = 170;
	int starty = 580;

	for (int i = 0; i < 3; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");
		box->setPosition(Vec2(startx + i*spacex, starty));
		this->addChild(box);

		std::string resstr;
		std::string strcount;
		std::string namstr;

		std::string resid = prizerwd[i];
		int intresid = atoi(resid.c_str());

		if (resid.compare(0, 1, "g") == 0)
		{
			strcount = StringUtils::format("x%d", atoi(resid.substr(1).c_str()));
			namstr = CommonFuncs::gbk2utf("金元宝");
			resstr = "ui/gd0.png";
		}
		else
		{
			resstr = StringUtils::format("ui/%d.png", intresid / 1000);
			strcount = StringUtils::format("x%d", intresid % 1000);
			std::string ridstr = StringUtils::format("%d", intresid / 1000);
			namstr = GlobalData::map_allResource[ridstr].cname;
		}

		Sprite* res = Sprite::createWithSpriteFrameName(resstr);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);

		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(1, 0.5));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + box->getContentSize().width / 2 - 10, 540));
		this->addChild(coutlbl);

		Label * namelbl = Label::createWithTTF(namstr, "fonts/STXINGKA.TTF", 26);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getPositionX(), 490));
		this->addChild(namelbl);
	}
}

void PrizeLayer::addRes()
{
	for (int i = 0; i < 3; i++)
	{
		if (prizerwd[i].compare(0, 1, "g") == 0)
		{
			int count = atoi(prizerwd[i].substr(1).c_str());
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + count);
		}
		else
		{
			int intresid = atoi(prizerwd[i].c_str());
			std::string resid = StringUtils::format("%d", intresid / 1000);

			PackageData pdata;
			pdata.strid = resid;
			pdata.count = intresid % 1000;
			pdata.type = GlobalData::getResType(resid);
			pdata.extype = GlobalData::getResExType(resid);
			StorageRoom::add(pdata);
		}
	}
}

void PrizeLayer::onSuccess()
{
	m_okbtn->setEnabled(false);
	m_edit->setEnabled(false);
	showRwd();
	addRes();

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf("兑换成功，请到仓库中查看！"));
	this->addChild(hintbox);
}

void PrizeLayer::onErr(int errcode)
{
	std::string str;
	if (errcode == -3)
	{
		str = "兑换失败！已经兑换过了，每人只有兑换一次！";
	}
	else
	{
		str = "兑换失败！无效的兑换码！";
	}

    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf(str.c_str()));
	this->addChild(hintbox);
}
