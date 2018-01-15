

#ifndef __NEWERGUIDE2LAYER__
#define __NEWERGUIDE2LAYER__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

#define NEWERLAYERZOER 1999
class NewerGuide2Layer : public Layer
{
public:
	NewerGuide2Layer();
	~NewerGuide2Layer();
	bool init(int type, std::vector<Node*> stencilNodes);
	static NewerGuide2Layer* create(int step, std::vector<Node*> stencilNodes);
	static bool checkifNewerGuide(int index);
	void showWords();
private:

	void hightNode(std::vector<Node*> sNodes);
private:
	int m_step;
	cocos2d::ui::Text* wordtext;
	Vec2 startPos;
	int wordindex;
	std::vector<Node*> m_stencilNodes;

};
#endif
