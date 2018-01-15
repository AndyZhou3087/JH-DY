#include "Nature.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "MD5.h"

int Nature::ReasonCDays = 30;

Nature::Nature()
{
	m_timeinterval = NORMAL_TIMEINTERVAL;
	m_daynight = Night;
	m_ismakewarm = false;
	m_isshowinsect = false;
	m_warmtime = 0;
	m_warmpasttime = 0.0f;
}

Nature::~Nature()
{

}

bool Nature::init()
{
	this->schedule(schedule_selector(Nature::updateData), NORMAL_TIMEINTERVAL * 1.0f / TIMESCALE);
	return true;
}


void Nature::ChangeWeather()
{
	int r = GlobalData::createRandomNum(100);
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		if (r < WeatherWeight[m_reason][i])
		{
			break;
		}
	}
	EWeather w = (EWeather)i;
	if (w != m_weather)
	{
		int c = w - m_weather;
		setWeather(w);
		setTemperature(m_temperature - c * 5);
		g_uiScroll->addEventText(CommonFuncs::gbk2utf(weatherEventText[w].c_str()));
	}


}

void Nature::ChangeReason()
{
	int yudays = m_pastdays % (4 * ReasonCDays);
	EReason r = Spring;
	if (yudays < ReasonCDays)
	{
		r = Spring;
	}
	else if (yudays < ReasonCDays * 2)
	{
		r = Summer;
	}
	else if (yudays < ReasonCDays * 3)
	{
		r = Autumn;
	}
	else
	{
		r = Winter;
	}
	if (r != m_reason)
	{
		setReason(r);
		ChangeTemperature();

		g_uiScroll->addEventText(CommonFuncs::gbk2utf(reasonEventText[r].c_str()));
	}
}


void Nature::makewarm(int extime)
{
	setIsMaKeWarm(true);
	setTemperature(m_temperature + 15);
	m_warmpasttime = 0.0f;
	m_warmtime = extime;
	//this->scheduleOnce(schedule_selector(Nature::makewarmover), extime / TIMESCALE);

	std::string str = StringUtils::format("%d-%d", m_pastdays * 24 * 60 + (int)m_time, extime);
	GlobalData::setMakeWarmConfig(str);
}

void Nature::makewarmover()
{
	//取暖时间到
	setIsMaKeWarm(false);
	m_warmpasttime = 0.0f;
	setTemperature(m_temperature - 15);
	GlobalData::setMakeWarmConfig("");
}


void Nature::ChangeDayNight()
{
	if (m_time < 60 * 6 - 1 || m_time >= 18 * 60)
	{
		if (m_daynight == Day)
		{
			setDayOrNight(Night);
			setTemperature(m_temperature - 5);
		}
	}
	else
	{
		if (m_daynight == Night)
		{
			setDayOrNight(Day);
			//setTemperature(m_temperature + 5);
			ChangeTemperature();
		}
	}
}

void Nature::ChangeTemperature()
{
	int maxr = tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1;
	int  t = tempeRange[m_reason][0] + GlobalData::createRandomNum(maxr);
	if (m_ismakewarm)
		t += 15;
	setTemperature(t);
}

void Nature::setTemperature(int val)
{
	m_temperature = val;

	if (m_temperature < -20)
		m_temperature = 20;
	else if (m_temperature > 25)
		m_temperature = 25;
}

int Nature::getTemperature()
{
	return m_temperature;
}

void Nature::updateData(float dt)
{
	if (GlobalData::g_gameStatus != GAMESTART || GlobalData::isPlayerChallenging)
		return;
	if (g_hero != NULL && g_hero->getIsWDChallenge())
		return;

	m_time += getTimeInterval();

	if (m_time >= 1440.0f)
	{
		if (GlobalData::getMD5LiveDays().compare(md5(m_pastdays)) != 0)
		{
			GlobalData::dataIsModified = true;
			m_pastdays = 0;
		}

		m_pastdays++;

		GlobalData::doAchive(A_11, m_pastdays);
		GlobalData::setMD5LiveDays(md5(m_pastdays));
		m_time -= 1440.0f;
		changeWeatherCount = 0;
		changeWeatherRandow = GlobalData::createRandomNum(24) + 1;
		ChangeReason();
	}
	if (m_ismakewarm)
	{
		//制暖时间
		m_warmpasttime += getTimeInterval();

		if (m_warmpasttime >= m_warmtime)//之前时间到结束
		{
			makewarmover();
		}
	}
	//产生随机数
	int inttime = (int)m_time;
	if (changeWeatherRandow <= 0)
		changeWeatherRandow = GlobalData::createRandomNum(23) + 1;
	if (changeWeatherRandow == inttime / 60)
	{
		changeWeatherCount++;
		if (changeWeatherCount <= 1)//每天变化一次
		{
			ChangeWeather();
		}
	}

	ChangeDayNight();
}