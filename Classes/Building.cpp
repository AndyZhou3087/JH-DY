#include "Building.h"
#include "Nature.h"
#include "Const.h"
#include "GameDataSave.h"

extern Nature* g_nature;

Building::Building()
{
	m_actionBarTime = ACTION_BAR_TIME;
}


Building::~Building()
{

}

void Building::parseData(rapidjson::Value& jsonvalue)
{
	if (jsonvalue.IsObject())
	{
		rapidjson::Value& value = jsonvalue["name"];
		strcpy(data.name, value.GetString());

		value = jsonvalue["cname"];
		strcpy(data.cname, value.GetString());

		data.level = GameDataSave::getInstance()->getBuildLV(data.name);;
		value = jsonvalue["maxlevel"];
		data.maxlevel = atoi(value.GetString());
		value = jsonvalue["needtime"];
		for (int i = 0; i < data.maxlevel; i++)
		{
			data.needtime.push_back(value[i].GetInt());
		}

		value = jsonvalue["needres"];
		for (int i = 0; i < data.maxlevel; i++)
		{
			std::vector<int> res;
			rapidjson::Value& resvalue = value[i];
			int size = resvalue.Size();
			for (int j = 0; j < size; j++)
			{
				int intid = resvalue[j].GetInt();
				if (intid > 0)
					res.push_back(resvalue[j].GetInt());
			}
			data.Res.push_back(res);
		}

		value = jsonvalue["desc"];
		data.desc = value.GetString();
	}

}

void Building::build()
{
	//设置建造时间间隔 eg: 4s 走完进度条，游戏时间走40min
	float time = data.needtime[data.level] * NORMAL_TIMEINTERVAL * 1.0f / (ACTION_BAR_TIME * TIMESCALE);
	g_nature->setTimeInterval(time);
	//完成后，等级+1
	data.level++;
	GameDataSave::getInstance()->setBuildLV(data.name, data.level);

}

void Building::action(int minute, int exminute)
{
	//子类自己实现自己的操作
}