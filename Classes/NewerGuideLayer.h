

#ifndef __NEWERGUIDELAYER__
#define __NEWERGUIDELAYER__

#include "cocos2d.h"
USING_NS_CC;

#define NEWERLAYERZOER 1999
class NewerGuideLayer : public Layer
{
public:
	NewerGuideLayer();
	~NewerGuideLayer();
	bool init(int type, std::vector<Node*> stencilNodes);
	virtual void onEnterTransitionDidFinish();
	static NewerGuideLayer* create(int step, std::vector<Node*> stencilNodes);
	static bool checkifNewerGuide(int index);
	static void pushUserData(std::string strdata);
	static bool isShowing;
private:
	ClippingNode* m_clippingNode;
	int m_step;
	static bool isNewerGuide[65];
	Node* m_talknode;
	LayerColor *m_colorlayer;
private:
	void showAnim(Vec2 pos);
	void showWord(std::string wordstr);
	void initDialog();
	void removeSelf();
	Vec2 starPos;
	Label* m_wordlbl;
	int wordindex;
	bool isallclick;
	bool iscannext;
	static std::vector<std::string> vec_userdata;
	cocos2d::EventListenerTouchOneByOne* m_listener;
};
extern NewerGuideLayer* g_NewerGuideLayer;
#endif
