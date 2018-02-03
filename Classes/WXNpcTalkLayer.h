

#ifndef __NPCTALKLAYER__
#define __NPCTALKLAYER__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class WXNpcTalkLayer : public Layer
{
public:
	WXNpcTalkLayer();
	~WXNpcTalkLayer();
	bool init(std::vector<std::string> vec_words, std::string npcid);
	static WXNpcTalkLayer* create(std::vector<std::string> vec_words, std::string npcid);
	void showWords();
	void initRandSeed();
	time_t getNowTime();
	long long getNowTimeMs();
	bool isBeforeToday(time_t sec);
	long long getTodayLeftSec();

private:
	void checkWordLblColor(std::string wordstr);
	void showTypeText(float dt);
	void fastShowWord();
	void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);
private:
	cocos2d::ui::Widget* m_talknode;
	Label* m_wordlbl;
	cocos2d::ui::ImageView* npcimg;
	cocos2d::ui::ImageView* heroimg;
	std::vector<std::string> m_wordslist;
	Vec2 startPos;
	int m_wordindex;
	int m_wordcount;
	bool isShowWord;
	Node* lasttalklbl;
};
#endif
