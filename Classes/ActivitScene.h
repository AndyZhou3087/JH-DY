/********************************************************************
* 过场场景
*********************************************************************/
#ifndef _ACTIVITY_SCENE_H_
#define _ACTIVITY_SCENE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyPackage.h"
USING_NS_CC;

class ActivitScene :public Layer
{
public:
	ActivitScene();
	~ActivitScene();

	bool init(std::string imagepath, std::string content);
	static cocos2d::Scene* createScene(std::string imagepath, std::string content);
private:
	static ActivitScene* create(std::string imagepath, std::string content);
	void popself(float dt);
	void getRndRes(float dt);
	void checkstoleData(float dt);
	void saveTempResData();
private:
	std::vector<PackageData> m_stoleData;
	cocos2d::ui::Text* m_text;
	std::map<std::string, std::vector<PackageData>> map_tempdata;
	std::string m_tempmapid;
};
#endif

