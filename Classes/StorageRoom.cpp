#include "StorageRoom.h"
#include "Const.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "GlobalData.h"

std::map<int, std::vector<PackageData>> StorageRoom::map_storageData;
StorageRoom::StorageRoom()
{
}


StorageRoom::~StorageRoom()
{

}

bool StorageRoom::init()
{
	if (Sprite::initWithSpriteFrameName("ui/storageroom.png"))
	{

		return true;
	}
	return false;
}

void StorageRoom::save()
{
	std::string str;

	std::map<int, std::vector<PackageData>>::iterator it; 
	for (it = map_storageData.begin(); it != map_storageData.end(); ++it)
	{
		int size = map_storageData[it->first].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = map_storageData[it->first][j];
			std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", sdata.strid.c_str(), sdata.type, sdata.count, sdata.extype, sdata.lv, sdata.exp, sdata.goodvalue, sdata.slv, sdata.tqu);
			str.append(idstr);
		}
	}
	GameDataSave::getInstance()->setStorageData(str.substr(0, str.length() - 1));
}

void StorageRoom::loadStorageData()
{
	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = map_storageData.begin(); it != map_storageData.end(); ++it)
	{
		map_storageData[it->first].clear();
	}
	map_storageData.clear();

	std::string strval = GameDataSave::getInstance()->getStorageData();
	std::vector<std::string> tmp;
	CommonFuncs::split(strval, tmp, ";");

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		std::vector<std::string> tmp2;
		CommonFuncs::split(tmp[i], tmp2, "-");

		PackageData sdata;
		sdata.strid = tmp2[0];
		if (sdata.strid.length() <= 0)
			continue;
		sdata.type = atoi(tmp2[1].c_str());

		//修改商城购买江湖大礼包类型错了
		if (sdata.strid.compare("w023") == 0)
			sdata.type = W_GONG;
		else if (sdata.strid.compare("x019") == 0)
			sdata.type = N_GONG;

		sdata.count = atoi(tmp2[2].c_str());
		sdata.extype = GlobalData::getResExType(sdata.strid);//atoi(tmp2[3].c_str());
		sdata.lv = atoi(tmp2[4].c_str());

		sdata.exp = atoi(tmp2[5].c_str());
		sdata.goodvalue = atoi(tmp2[6].c_str());

		if (tmp2.size() >= 9)
		{
			sdata.slv = atoi(tmp2[7].c_str());
			sdata.tqu = atoi(tmp2[8].c_str());
		}

		bool ishassame = false;
		if (sdata.type == WEAPON || sdata.type == PROTECT_EQU)
		{
			std::vector<PackageData>::iterator it;
			for (it = map_storageData[sdata.type].begin(); it != map_storageData[sdata.type].end(); it++)
			{
				if (it->strid.compare(sdata.strid) == 0)
				{
					it->goodvalue += sdata.goodvalue;
					if (it->goodvalue > 1000)
						it->goodvalue = 1000;
					if (it->slv < sdata.slv)
						it->slv = sdata.slv;
					if (it->lv < sdata.lv)
						it->lv = sdata.lv;
					ishassame = true;
				}
			}
		}

		if (ishassame)
			continue;

		map_storageData[sdata.type].push_back(sdata);
	}

}

void StorageRoom::add(PackageData data)
{
	int typesize = map_storageData[data.type].size();
	int i = 0;
	for (i = 0; i < typesize; i++)
	{
		PackageData *sdata = &map_storageData[data.type][i];
		if (data.strid.compare(sdata->strid) == 0 && (data.type == FOOD || data.type == MEDICINAL || data.type == RES_1 || data.type == RES_2))
		{
			sdata->count += data.count;
			break;
		}
		
	}
	if (i == typesize)
		map_storageData[data.type].push_back(data);
	StorageRoom::save();
	
	if (data.strid.compare("80") == 0)
	{
		GlobalData::doAchive(A_3, StorageRoom::getCountById("80"));
	}
}

void StorageRoom::use(std::string strid, int count)
{
	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = map_storageData.begin(); it != map_storageData.end(); ++it)
	{
		for (unsigned int i = 0; i < map_storageData[it->first].size(); i++)
		{
			PackageData *sdata = &map_storageData[it->first][i];
			if (strid.compare(sdata->strid) == 0)
			{
				if (sdata->type == FOOD || sdata->type == MEDICINAL || sdata->type == RES_1 || sdata->type == RES_2)
				{
					sdata->count -= count;
					if (sdata->count <= 0)
					{
						map_storageData[it->first].erase(map_storageData[it->first].begin() + i);
					}
				}
				else
				{
					map_storageData[it->first].erase(map_storageData[it->first].begin() + i);
				}
				break;
			}
		}
	}
	StorageRoom::save();

	std::string mixid = GlobalData::getMixGF();
	if (mixid.length() > 0)
	{
		MixGfData mdata = GlobalData::map_MixGfData[mixid];

		if (mdata.mastergf.compare(strid) == 0)
		{
			for (unsigned int n = 0; n < mdata.vec_secgf.size(); n++)
			{
				if (StorageRoom::getCountById(mdata.vec_secgf[n]) <= 0)
				{
					PackageData data;
					WG_NGData gfdata = GlobalData::map_wgngs[mdata.vec_secgf[n]];
					data.strid = gfdata.id;
					data.count = 1;
					data.lv = gfdata.maxlv - 1;
					data.type = gfdata.type - 1;
					data.extype = gfdata.extype;
					StorageRoom::add(data);
				}
			}
			GlobalData::setMixGF("");
		}
	}
}

void StorageRoom::use(PackageData data)
{
	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = map_storageData.begin(); it != map_storageData.end(); ++it)
	{
		for (unsigned int i = 0; i < map_storageData[it->first].size(); i++)
		{
			PackageData *sdata = &map_storageData[it->first][i];
			if (data.strid.compare(sdata->strid) == 0 && data.goodvalue == sdata->goodvalue)
			{
				if (sdata->type == FOOD || sdata->type == MEDICINAL || sdata->type == RES_1 || sdata->type == RES_2)
				{
					sdata->count -= data.count;
					if (sdata->count <= 0)
					{
						map_storageData[it->first].erase(map_storageData[it->first].begin() + i);
					}
				}
				else
				{
					map_storageData[it->first].erase(map_storageData[it->first].begin() + i);
				}
				break;
			}
		}
	}
	StorageRoom::save();
}

int StorageRoom::getCountById(std::string strid)
{
	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = map_storageData.begin(); it != map_storageData.end(); ++it)
	{
		for (unsigned int i = 0; i < map_storageData[it->first].size(); i++)
		{
			PackageData *sdata = &map_storageData[it->first][i];
			if (strid.compare(sdata->strid) == 0)
			{
				return sdata->count;
			}
		}
	}
	return 0;
}