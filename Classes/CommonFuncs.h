#ifndef _COMMONFUNCS_H_
#define _COMMONFUNCS_H_
#include <iostream>
#include "cocos2d.h"
#include <algorithm>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
class CommonFuncs
{
public:
	/****************************
	字符串分割
	@param str 字符串
	@param ret_ 分割后的字符串vector
	@param sep 分割的符号
	****************************/
	static int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",");

	/****************************
	字符串替换
	@param s 源字符串
	@param t 需要替换的字符串
	@param w 替换成的字符串
	@return 替换后的字符串
	****************************/
	static std::string replace_all(std::string & s, std::string const & t, std::string const & w);

	/****************************
	字符串转换UTF8
	@param inbuf 字符串
	@return 转换后的字符串
	****************************/
	static std::string gbk2utf(const char *inbuf);

	/****************************
	按钮点击
	****************************/
	static void BtnAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	图片变灰操作
	****************************/
	static void changeGray(cocos2d::Node* node);

	/****************************
	图片变灰还原操作
	****************************/
	static void removeGray(cocos2d::Node* node);

private:

	/****************************
	字符串转换
	@param from_charset 转换前字符编码
	@param to_charset 转换后字符编码
	@param inbuf 需要转换的字符串

	@param inbuf 需要转换的字符串的长度
	@param outbuf 转换后的字符串
	@param outlen 转换后的字符长度
	****************************/
	static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);

};

#endif
