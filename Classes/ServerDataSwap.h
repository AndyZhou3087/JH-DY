/********************************************************************
*服务器数据交互
*********************************************************************/
#ifndef __SERVERDATASWAP__
#define __SERVERDATASWAP__
#include "cocos2d.h"

class ServerDataDelegateProtocol;

class ServerDataSwap
{
public:

	static ServerDataSwap* init(ServerDataDelegateProtocol* delegateProtocol = NULL);

	void postOneData(std::string userid, int tag = 0);
	void postAllData();
	void getAllData();
	void propadjust();
	void modifyNickName(std::string nickname);
	void vipSuccNotice(std::string gid);
	void vipIsOn(int heroid);
	void isGetVip(std::vector<std::string> vipids);
	void updateFreeReviveCount();
	void getServerTime();
	void getRankData(std::string orderby);
	void getannouncement();
	void setDelegate(ServerDataDelegateProtocol *delegateProtocol);
	static bool isGetingData();

	void createFaciton(std::string name, int lvlimit, int sexlimit, std::string desc);
	void getFactionList();
	void requestFaction(int factionid);
	void getFactionMembers(int factionid);
	void modifyFaciton(int factionid, std::string name, int lvlimit, int sexlimit, std::string desc);
	void joinFaction(int factionid, int requesterId, int requestertype);
	void kickFaction(int factionid, int requesterId, int requestertype);
	void promotionFaction(int factionid, int requesterId, int requestertype, int position);
	void leaveFaction(int actiontype, int factionid, int herotype);
	void contributionFaction(int factionid, int contribution, int herotype);

	void cancelFaction(int factionid);
	void refuseFaction(int factionid, int requesterId, int requestertype);
	void getlotteryData(int actiontype);

	void getCoinpoolData();
	void playCoinpoolData();

	void getCoupons(std::string codestr);

	void getChallengeranklist();

	void getMyFihgterData(std::string fightplayerid, int fightplayertype);

	void getFightCount(int matchtype, int count);

	void getChallengeResult(int myrank, std::string fightplayerid, int fightplayertype, int fightrank, int win);

	void getKajuanAction();

	void getKajuanAwardList();

	void getMyMatchInfo();

	void getMatchFight();

	void getMatchFightResult(std::string fightplayerid, int score);

	void getHSLJRankData();

	void getHSLJRewardData();

	void getCommonData();
private:
	ServerDataDelegateProtocol *m_pDelegateProtocol;//接口对象
	static bool isdoing;
	ServerDataSwap();
	~ServerDataSwap();
	ServerDataSwap(ServerDataDelegateProtocol *pDelegateProtocol);
	void release();
private:
	void httpPostOneDataCB(std::string retdata, int code, std::string tag);
	void httpGetAllDataCB(std::string retdata, int code, std::string tag);
	void httpPropadJustDataCB(std::string retdata, int code, std::string tag);
	void httpModifyNickNameCB(std::string retdata, int code, std::string tag);
	void httpVipIsOnCB(std::string retdata, int code, std::string tag);
	void httpGetServerTimeCB(std::string retdata, int code, std::string tag);
	void httpGetRankDataCB(std::string retdata, int code, std::string tag);
	void httpGetAnnouncementCB(std::string retdata, int code, std::string tag);
	void httpIsGetVipCB(std::string retdata, int code, std::string tag);
	void httpBlankCB(std::string retdata, int code, std::string tag);
	void httpCreateFactionCB(std::string retdata, int code, std::string tag);
	void httpGetFactionListCB(std::string retdata, int code, std::string tag);
	void httpRequestFactionListCB(std::string retdata, int code, std::string tag);
	void httpGetFactionMemberCB(std::string retdata, int code, std::string tag);
	void httpJionFactionCB(std::string retdata, int code, std::string tag);
	void httpKickFactionCB(std::string retdata, int code, std::string tag);

	void httpPromotionFactionCB(std::string retdata, int code, std::string tag);
	void httpLeaveFactionCB(std::string retdata, int code, std::string tag);
	void httpModifyFactionCB(std::string retdata, int code, std::string tag);
	void httpContributionFactionCB(std::string retdata, int code, std::string tag);

	void httpCancelFactionCB(std::string retdata, int code, std::string tag);

	void httpRefuseFactionCB(std::string retdata, int code, std::string tag);

	void httpLotteryCB(std::string retdata, int code, std::string tag);

	void httpGetCoinpoolCB(std::string retdata, int code, std::string tag);

	void httpPlayCoinpoolCB(std::string retdata, int code, std::string tag);

	void httpGetCouponsCB(std::string retdata, int code, std::string tag);

	void httpGetChallengeranklistCB(std::string retdata, int code, std::string tag);

	void httpGetMyFihgterDataCB(std::string retdata, int code, std::string tag);

	void httpGetFightCountCB(std::string retdata, int code, std::string tag);

	void httpGetChallengeResultCB(std::string retdata, int code, std::string tag);

	void httpGetKajuanActionCB(std::string retdata, int code, std::string tag);

	void httpGetKajuanAwardListCB(std::string retdata, int code, std::string tag);

	void httpGetMyMatchInfoCB(std::string retdata, int code, std::string tag);

	void httpGetMatchFightCB(std::string retdata, int code, std::string tag);

	void httpGetMatchFightResultCB(std::string retdata, int code, std::string tag);

	void httpGetHSLJRankDataCB(std::string retdata, int code, std::string tag);

	void httpGetHSLJRewardDataCB(std::string retdata, int code, std::string tag);

	void httpGetCommonDataCB(std::string retdata, int code, std::string tag);

};

class ServerDataDelegateProtocol
{
public:
	virtual ~ServerDataDelegateProtocol(){};

	/**
	* @brief 数据成功
	*/
	virtual void onSuccess() {};

	virtual void onErr(int errcode) {};
};

#endif
