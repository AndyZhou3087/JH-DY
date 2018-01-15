/********************************************************************
* 开场场景
*********************************************************************/
#ifndef _STORY_SCENE_H_
#define _STORY_SCENE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class StoryScene :public Layer
{
public:
	StoryScene();
	~StoryScene();

	virtual bool init(int type);
	static cocos2d::Scene* createScene(int type);//0-开场动画，1-结束动画
	
private:
	static StoryScene* create(int type);
	void showClickText(float dt);
	void delayShowNextStory(float dt);
	void showNextScene(float dt);
private:
	Node* m_csbnode;
	Label* m_wordlbl;
	int clickcount;
	bool isCanClick;
	int m_heroindex;
	cocostudio::timeline::ActionTimeline* m_action;
	int m_type;//0-开场动画，1-结束动画
};
#endif

