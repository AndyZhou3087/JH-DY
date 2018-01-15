

#ifndef __NPCTALKLAYER__
#define __NPCTALKLAYER__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class NpcTalkLayer : public Layer
{
public:
	NpcTalkLayer();
	~NpcTalkLayer();
	bool init(std::vector<std::string> vec_words, std::string npcid);
	static NpcTalkLayer* create(std::vector<std::string> vec_words, std::string npcid);
	void showWords();
private:
	void checkWordLblColor(std::string wordstr);
	void showTypeText(float dt);
	void fastShowWord();
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
