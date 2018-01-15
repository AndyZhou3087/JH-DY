
#include "json.h"
std::string JsonWriter(rapidjson::Document& doc)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	return buffer.GetString();
}

//
bool JsonReader(std::string& strData, rapidjson::Document& doc)
{
	bool result = true;
	doc.Parse<0>(strData.c_str());
	if (doc.HasParseError())  //打印解析错误
	{
		result = false;
	}
	return result;
}


rapidjson::Document ReadJsonFile(const std::string& name)
{
	rapidjson::Document doc;
	std::string filestr = FileUtils::getInstance()->getStringFromFile(name);
	if (filestr.length() > 0)
	{
		JsonReader(filestr, doc);
	}

	return doc;
}

void WriteJsonFile(const std::string& name, rapidjson::Document& doc)
{
	std::string file = name;

	FILE* fp = fopen(file.c_str(), "wt");
	if(fp != NULL)
	{	
		std::string data = JsonWriter(doc);
		fwrite(data.c_str(), 1, data.size(), fp);
		fclose(fp);
	}
}