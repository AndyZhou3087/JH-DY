#ifndef __HTTPUTIL_H__
#define __HTTPUTIL_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
typedef enum
{
	GET = 0,
	POST
}HTTPTYPE;

typedef std::function<void(std::string retdata, int code, std::string extenddata)> HttpCBFunc;

#define httputil_calback(__selector__,__target__) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

class HttpUtil: public Ref
{
public:
	static HttpUtil* getInstance();
	void doData(std::string url, HttpCBFunc http_cb, std::string filename = ""/*数据写入文件，不需要写文件传""*/, HTTPTYPE method = GET, std::string postdata = "", std::string extenddata = "");
private:
	HttpCBFunc http_cbfunc;
	std::string m_filename;
	std::string m_tag;
private:
	HttpUtil();

	~HttpUtil();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *resp);
};
#endif