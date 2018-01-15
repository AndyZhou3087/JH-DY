/********************************************************************
*rapidjson json操作
*********************************************************************/
#ifndef JSON_JSON_H_INCLUDED
#define JSON_JSON_H_INCLUDED

#include "cocos2d.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

/****************************
获取JSON的字符串格式
@para apidjson::Document对象
@return JSON的字符串
*****************************/
std::string JsonWriter(rapidjson::Document& doc);

/****************************
解析JSON数据
@para 需要解析的JSON字符串
@para rapidjson::Document对象
@return 解析是否成功
*****************************/
bool JsonReader(std::string& strData, rapidjson::Document& doc);
 
/****************************
从文件读JSON数据
@name 文件绝对文件名
*****************************/
rapidjson::Document ReadJsonFile(const std::string& name);

/****************************
写入JSON数据到文件
@para 写入的文件绝对文件名
@para rapidjson::Document对象
*****************************/
void WriteJsonFile(const std::string& name, rapidjson::Document& doc);

#endif // JSON_JSON_H_INCLUDED
