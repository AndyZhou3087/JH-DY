#include "AnalyticUtil.h"
#include "Const.h"
#ifdef ANALYTICS
#include "MobClickCpp.h"

void AnalyticUtil::startLevel(const char * level) {
        umeng::MobClickCpp::startLevel(level);
    }
    
    void AnalyticUtil::failLevel(const char * level) {
        umeng::MobClickCpp::failLevel(level);
    }
    
    void AnalyticUtil::finishLevel(const char * level) {
        umeng::MobClickCpp::finishLevel(level);
    }
    
    void AnalyticUtil::pay(const char * item ,int money, int number)  {
        umeng::MobClickCpp::pay((double)money, 1, item, number, (double)0);
    }
    
    void AnalyticUtil::buy(const char * item, int number, float gem)
    {
        umeng::MobClickCpp::buy(item, number, (double)gem);
    }
    
    void AnalyticUtil::use(const char * item, int number) {
        umeng::MobClickCpp::use(item, number, (double)0);
    }
    
    void AnalyticUtil::bonus(const char * item, int num, int type)
    {
        umeng::MobClickCpp::bonus(item, num, (double)0, type);
    }
    
    void AnalyticUtil::onEvent(const char * eventId, const char * label) {
        umeng::MobClickCpp::event(eventId, label);
    }
    
    void AnalyticUtil::onEvent(const char * eventId, std::map<string, string> data) {
        umeng::eventDict dict;
        map<string, string>::iterator iter;
        for (iter = data.begin(); iter!=data.end(); iter++)
        {
            dict[iter->first] = dict[iter->second];
        }
        umeng::MobClickCpp::event(eventId, &dict);
    }
    
    void AnalyticUtil::setPlayerLevel(int level)
    {
        umeng::MobClickCpp::setUserLevel(level);
    }
#endif
    
