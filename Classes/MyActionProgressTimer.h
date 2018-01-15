
#ifndef __MY_ACTION_CCPROGRESS_TIMER_H__
#define __MY_ACTION_CCPROGRESS_TIMER_H__

#include "2d/CCActionInterval.h"
#include "cocos2d.h"
NS_CC_BEGIN

class MyProgressTo : public ActionInterval
{
public:

	static MyProgressTo* create(float duration, float percent);

    //
    // Overrides
    //
	virtual MyProgressTo* clone() const override;
	virtual MyProgressTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    void update(float time);
    
	MyProgressTo();
	~MyProgressTo();

    /** 
     * @brief Initializes with a duration and destination percentage. 
     * @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
     * @param percent Specify the destination percentage.
     * @return If the creation success, return true; otherwise, return false.
     */
    bool initWithDuration(float duration, float percent);

protected:
    float _to;
    float _from;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(MyProgressTo);
};


class MyProgressFromTo : public ActionInterval
{
public:

	static MyProgressFromTo* create(float duration, float fromPercentage, float toPercentage);

    //
    // Overrides
    //
	virtual MyProgressFromTo* clone() const override;
	virtual MyProgressFromTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    void update(float time);
    
	MyProgressFromTo();
	~MyProgressFromTo();

    /**
     * @brief Initializes the action with a duration, a "from" percentage and a "to" percentage.
     * @param duration Specify the duration of the ProgressFromTo action. It's a value in seconds.
     * @param fromPercentage Specify the source percentage.
     * @param toPercentage Specify the destination percentage.
     * @return If the creation success, return true; otherwise, return false.
     */
    bool initWithDuration(float duration, float fromPercentage, float toPercentage);

protected:
    float _to;
    float _from;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(MyProgressFromTo);
};

// end of actions group
/// @}

NS_CC_END

#endif // __MY_ACTION_CCPROGRESS_TIMER_H__
