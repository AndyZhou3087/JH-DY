#include "PaySelectLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "Const.h"
#include "AnalyticUtil.h"
#include "ShopLayer.h"

static std::string goodsDesc[] = { "", "三少爷角色", "江小白角色", "阿青角色", "60元宝", "140元宝", "400元宝", "1080元宝", "荣耀月卡", "贵族月卡", "尊贵月卡", "限时礼包" };

PaySelectLayer::PaySelectLayer()
{
}


PaySelectLayer::~PaySelectLayer()
{

}

bool PaySelectLayer::init(int payindex)
{
	m_payindex = payindex;
	Node* csbnode = CSLoader::createNode("paySelectLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(PaySelectLayer::onCancel, this));

	cocos2d::ui::Button* qqpaybtn = (cocos2d::ui::Button*)csbnode->getChildByName("qqpaybtn");
	qqpaybtn->addTouchEventListener(CC_CALLBACK_2(PaySelectLayer::onPay, this));
	qqpaybtn->setTag(0);

	cocos2d::ui::Button* wxpaybtn = (cocos2d::ui::Button*)csbnode->getChildByName("wxpaybtn");
	wxpaybtn->addTouchEventListener(CC_CALLBACK_2(PaySelectLayer::onPay, this));
	wxpaybtn->setTag(1);

	cocos2d::ui::Text* content = (cocos2d::ui::Text*)csbnode->getChildByName("content");

	content->setString(CommonFuncs::gbk2utf(goodsDesc[payindex].c_str()));

	cocos2d::ui::Text* price = (cocos2d::ui::Text*)csbnode->getChildByName("price");

	std::string pricestr = StringUtils::format("¥%d.00", buyprice[payindex]);
	price->setString(CommonFuncs::gbk2utf(pricestr.c_str()));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

PaySelectLayer* PaySelectLayer::create(int payindex)
{
	PaySelectLayer *pRet = new PaySelectLayer();
	if (pRet && pRet->init(payindex))
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

void PaySelectLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer::setMessage(PAY_FAIL);
		this->removeFromParentAndCleanup(true);
	}
}
void PaySelectLayer::onPay(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

	//CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (node->getTag() == 1)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			ShopLayer::setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", payCode[m_payindex].c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/PayAction", "pay", "(Ljava/lang/String;I)V"))
			{
				jstring str1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str1, m_payindex);
			}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//payCode
			buy((char*)payCode[m_payindex].c_str());
#endif
		}
	}
}