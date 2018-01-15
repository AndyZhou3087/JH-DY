//
//  ViewController.m
//  testunityads
//
//  Created by Hailin Rao on 2016/12/6.
//  Copyright © 2016年 Hailin Rao. All rights reserved.
//

#import "MyUnityAdsDelegate.h"
#import "GlobalData.h"
#import "GameScene.h"
#import "SoundManager.h"
#import "HintBox.h"


@interface MyUnityAdsDelegate ()

@end

@implementation MyUnityAdsDelegate

- (void)initUnityAds {
    if (![UnityAds isInitialized]) {
        if ([UnityAds isSupported]) {
            [UnityAds setDebugMode:false];
            [UnityAds initialize:@"1264569" delegate:self testMode:false];//
        }
    }
}

- (bool) unityIsReady {
    return [UnityAds isReady];
}

- (bool) vungleIsReady {
    return [vungleSdk isAdPlayable];
}

- (bool) admobIsReady {
    return interstitial.isReady;
}

- (bool) isReady {
    if ([self unityIsReady] || [self vungleIsReady])
        return true;
    return false;
}

- (IBAction)showUnityAds:(NSInteger)adswhere icoin:(NSInteger)coin igem:(NSInteger)gem ihp:(NSInteger)hp {
    if ([UnityAds isReady]) {
        _adsWhere = adswhere;
        _awardCoin = coin;
        _awardGem = gem;
        _awardHp = hp;
        UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
        [UnityAds show:s placementId:@"video"];
    }
}

- (IBAction)showVungleAds:(NSInteger)adswhere icoin:(NSInteger)coin igem:(NSInteger)gem ihp:(NSInteger)hp {
    _adsWhere = adswhere;
    _awardCoin = coin;
    _awardGem = gem;
    _awardHp = hp;
    [vungleSdk setDelegate:self];
    [vungleSdk setLoggingEnabled:false];
    if ([vungleSdk isAdPlayable]) {
        NSError * error;
        UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
        if (adswhere == 8) {
            //NSDictionary * options = @{VunglePlayAdOptionKeyIncentivized:@false};
            [vungleSdk playAd:s error:&error];
        }
        else {
            NSDictionary * options = @{VunglePlayAdOptionKeyIncentivized:@true};
            [vungleSdk playAd:s withOptions:options error:&error];
        }
    }
}

- (IBAction)showAdMobAds:(NSInteger)adswhere icoin:(NSInteger)coin igem:(NSInteger)gem ihp:(NSInteger)hp {
    _adsWhere = adswhere;
    _awardCoin = coin;
    _awardGem = gem;
    _awardHp = hp;
    
    if (interstitial.isReady) {
        UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
        [interstitial presentFromRootViewController:s];
    }
}

- (void)initVungleAds {
    NSString * appID = @"584a218898d2beed0e00005f";
    vungleSdk = [VungleSDK sharedSDK];
    [vungleSdk startWithAppId:appID];
}

- (void)unityAdsReady:(NSString *)placementId {
    //NSLog(@"aaaready %@", placementId);
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString *)message {
    //NSLog(@"aaadiderror %@", message);
}

- (void)unityAdsDidStart:(NSString *)placementId {
    //NSLog(@"aaadidstart %@", placementId);
}

- (void)initAdmobAds {
    //正式：ca-app-pub-6582621014015154/9096120625
    //测试：ca-app-pub-3940256099942544/4411468910
    interstitial = [[GADInterstitial alloc] initWithAdUnitID:@"ca-app-pub-6582621014015154~2172810626"];
    interstitial.delegate = self;
    GADRequest * request = [GADRequest request];
    //request.testDevices = @[ kGADSimulatorID, @"6FD4B90E-6B37-533F-B16F-CEB9F8F7984" ];
    [interstitial loadRequest:request];
}

- (void)unityAdsDidFinish:(NSString *)placementId
          withFinishState:(UnityAdsFinishState)state {
    NSLog(@"aaadidfinish %@", placementId);
    //([placementId isEqualToString:@"rewardedVideo"] || [placementId isEqualToString:@"rewardedVideoZone"])
    if (state == kUnityAdsFinishStateCompleted) {
        [self didAfterCompleted:_adsWhere];
        //[self rewardPlayerForWatchingAd];
    }
}

- (void)vungleSDKAdPlayableChanged:(BOOL)isAdPlayable {
    if (isAdPlayable) {
        NSLog(@"aaaAn ad is available for playback");
    } else {
        NSLog(@"aaaNo ads currently available for playback");
    }
}

- (void)vungleSDKwillShowAd {
    NSLog(@"aaaAn ad is about to be played!");
    //Use this delegate method to pause animations, sound, etc.
}

- (void)vungleSDKwillCloseAdWithViewInfo:(NSDictionary *)viewInfo willPresentProductSheet:(BOOL)willPresentProductSheet {
    NSLog(@"ViewInfo Dictionary:");
    // viewInfo dictioanry contains completedView, playTime and didDownload information
    // willPresentProductSheet always returns NO
    for(NSString * key in [viewInfo allKeys]) {
        NSLog(@"aaa%@ : %@", key, [[viewInfo objectForKey:key] description]);
    }
    NSNumber * isCompleted = [viewInfo objectForKey:@"completedView"];
    int value = [isCompleted intValue];
    if (value == 1) {
        [self didAfterCompleted:_adsWhere];
    }
}

- (void)vungleSDKwillCloseProductSheet:(id)productSheet {
    NSLog(@"aaaThe user has downloaded an advertised application and is now returning to the main app");
    //This method can be used to resume animations, sound, etc. if a user was presented a product sheet earlier
}

/*adsWhereRevive = 0,
adsSuccess,1
adsRemoveAds,2
adsDailyAds,3
adsOverAddTime,4
adsStartChooseProp,5
adsShop,6
adsBuyLife 7*/

- (void)didAfterCompleted:(NSInteger)adsWhere {
    switch (adsWhere) {
        case 0:
            //g_gameScene->ReviveOK();
            break;
        case 1:
            //GlobalData::setGemCount(GlobalData::getGemCount() + _awardGem);
            //GlobalData::setHpCount(GlobalData::getHpCount() + _awardHp);
            //GlobalData::setGoldCount(GlobalData::getGoldCount() + _awardCoin);
            break;
        case 2:
            if ([[[NSUserDefaults standardUserDefaults] objectForKey:@"noads"] isEqualToString:@"0"]) {
                [[NSUserDefaults standardUserDefaults] setObject:@"1" forKey:@"noads"];
            }
            break;
        case 3: {
            /*NSUserDefaults * userdef = [NSUserDefaults standardUserDefaults];
            NSString * adsDay = [userdef stringForKey:@"dailyadsday"];
            NSInteger adsCount = [userdef integerForKey:@"dailyadscount"];
            NSDateFormatter * formatter = [[NSDateFormatter alloc] init];
            [formatter setDateFormat:@"dd"];
            NSString * day = [formatter stringFromDate:[NSDate date]];
            if (![adsDay isEqualToString:day] || adsCount < 3) {
                [userdef setObject:day forKey:@"dailyadsday"];
                [userdef setInteger:adsCount forKey:@"dailyadscount"];
                adsCount++;*/
            NSInteger adsCount = GlobalData::getDailyAdsCount();
                /*if (adsCount == 1) {
                    GlobalData::setPromptProp(GlobalData::getPromptProp() + 1);
                    NSString * tips = @"奖励1个提示道具";
                    HintBox::showToast([tips UTF8String]);
                }
                else if (adsCount == 2) {
                    GlobalData::setRefreshProp(GlobalData::getRefreshProp() + 1);
                    NSString * tips = @"奖励1个重排道具";
                    HintBox::showToast([tips UTF8String]);
                }
                else if (adsCount == 3) {
                    GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + 1);
                    NSString * tips = @"奖励1个加时道具";
                    HintBox::showToast([tips UTF8String]);
                }*/
                GlobalData::setDailyAdsCount(GlobalData::getDailyAdsCount() + 1);
            //}
        }
            break;
        case 4:
            //g_gameScene->useClock();
            //g_gameScene->gameResume();
            break;
        case 5:
            break;
        case 6: {
            /*GlobalData::setGemCount(GlobalData::getGemCount() + 5);
            SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);
            NSString * tips = @"已获得5个钻石";
            HintBox::showToast([tips UTF8String]);
            GlobalData::setFreeGemAdsCount(GlobalData::getFreeGemAdsCount() + 1);*/
        }
            break;
        case 7:
            //GlobalData::setHpCount(GlobalData::getHpCount() + _awardHp);
            break;
        default:
            break;
            
    }
    
}

- (void)interstitial:(GADInterstitial *)interstitial didFailToReceiveAdWithError:(nonnull GADRequestError *)error {
    
}

- (void)interstitialDidReceiveAd:(GADInterstitial *)ad {
    NSLog(@"admob interstitialDidReceiveAd!");
    //[self didAfterCompleted:_adsWhere];
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)ad {
    NSLog(@"admob finish!");
    
    [self didAfterCompleted:_adsWhere];
}

@end
