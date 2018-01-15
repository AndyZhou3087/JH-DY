
#include "MyActionProgressTimer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

NS_CC_BEGIN

#define kLoadingBarCast cocos2d::ui::LoadingBar*

// implementation of MyProgressTo

MyProgressTo::MyProgressTo()
{

}

MyProgressTo::~MyProgressTo()
{

}

MyProgressTo* MyProgressTo::create(float duration, float percent)
{
	MyProgressTo *progressTo = new (std::nothrow) MyProgressTo();
    progressTo->initWithDuration(duration, percent);
    progressTo->autorelease();

	
	return progressTo;
}

bool MyProgressTo::initWithDuration(float duration, float percent)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = percent;

        return true;
    }

    return false;
}

MyProgressTo* MyProgressTo::clone() const
{
    // no copy constructor    
	auto a = new (std::nothrow) MyProgressTo();
    a->initWithDuration(_duration, _to);
    a->autorelease();
    return a;
}

MyProgressTo* MyProgressTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in ProgressTo");
    return nullptr;
}

void MyProgressTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
	_from = ((kLoadingBarCast)(target))->getPercent();
}

void MyProgressTo::update(float time)
{
	((kLoadingBarCast)(_target))->setPercent(_from + (_to - _from) * time);
}

// implementation of ProgressFromTo

MyProgressFromTo::MyProgressFromTo()
{

}

MyProgressFromTo::~MyProgressFromTo()
{

}

MyProgressFromTo* MyProgressFromTo::create(float duration, float fromPercentage, float toPercentage)
{
	MyProgressFromTo *progressFromTo = new (std::nothrow) MyProgressFromTo();
    progressFromTo->initWithDuration(duration, fromPercentage, toPercentage);
    progressFromTo->autorelease();

    return progressFromTo;
}

bool MyProgressFromTo::initWithDuration(float duration, float fromPercentage, float toPercentage)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = toPercentage;
        _from = fromPercentage;

        return true;
    }

    return false;
}

MyProgressFromTo* MyProgressFromTo::clone() const
{
    // no copy constructor    
	auto a = new (std::nothrow) MyProgressFromTo();
    a->initWithDuration(_duration, _from, _to);
    a->autorelease();
    return a;
}


MyProgressFromTo* MyProgressFromTo::reverse() const
{
	return MyProgressFromTo::create(_duration, _to, _from);
}

void MyProgressFromTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
}

void MyProgressFromTo::update(float time)
{
	((kLoadingBarCast)(_target))->setPercent(_from + (_to - _from) * time);
}

NS_CC_END
