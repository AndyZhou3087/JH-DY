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

class WXActivitScene :public Layer
{
public:
	WXActivitScene();
	~WXActivitScene();

	bool init(std::string imagepath, std::string content);
	void loadTime();
	static cocos2d::Scene* createScene(std::string imagepath, std::string content);
	void on1sTimer();
private:
	void initBg();
	static WXActivitScene* create(std::string imagepath, std::string content);
	void popself(float dt);
	void saveTime();
	void getRndRes(float dt);

	void initA();
	void checkstoleData(float dt);
	void saveTempResData();
	inline long getCountDown() { return m_countdown; }
private:
	std::vector<PackageData> m_stoleData;
	cocos2d::ui::Text* m_text;
	long m_lastSaveTime;

	std::map<std::string, std::vector<PackageData>> map_tempdata;
	std::string m_tempmapid;
	long m_countdown;
};
#endif

