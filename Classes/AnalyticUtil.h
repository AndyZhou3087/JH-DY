

#ifndef __ANALYTICUTIL__
#define __ANALYTICUTIL__

#include <string>
#include <map>
using namespace std;
class AnalyticUtil
{
public:
    static void startLevel(const char * level);
    static void failLevel(const char * level);
    static void finishLevel(const char * level);
    static void pay(const char * item ,int money, int number);
    static void buy(const char * item, int number, float gem);
    static void use(const char * item, int number);
    static void bonus(const char * item, int num, int type);
    static void onEvent(const char * eventId, const char * label = NULL);
    static void onEvent(const char * eventId, std::map<string, string> data);
    static void setPlayerLevel(int level);
private:
};

#endif
