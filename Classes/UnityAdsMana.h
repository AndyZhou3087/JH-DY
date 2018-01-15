//
//  UnityAdsMana.hpp
//  kuxx-hllm_v1.1.1
//
//  Created by Hailin Rao on 2016/12/6.
//
//

#ifndef UnityAdsMana_hpp
#define UnityAdsMana_hpp

/*typedef enum AdsWhere {
    adsWhereRevive = 0,
    adsSuccess,1
    adsRemoveAds,2
    adsDailyAds,3
    adsOverAddTime,4
    adsStartChooseProp,5
    adsShop,6
    adsBuyLife 7
    adsAccount 8
} enumAdsWhere;*/

void initAds();
void showAds(int adswhere, int coin, int gem, int hp);
bool isAdsReady();
//bool hasRemovedAds();
void alterView();

#endif /* UnityAdsMana_hpp */
