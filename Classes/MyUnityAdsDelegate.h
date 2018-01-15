//
//  ViewController.h
//  testunityads
//
//  Created by Hailin Rao on 2016/12/6.
//  Copyright © 2016年 Hailin Rao. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <UnityAds/UnityAds.h>
#import <VungleSDK/VungleSDK.h>
#import <GoogleMobileAds/GoogleMobileAds.h>
//@import GoogleMobileAds;

@interface MyUnityAdsDelegate : NSObject <UnityAdsDelegate, VungleSDKDelegate, GADInterstitialDelegate> {
    NSInteger _adsWhere;
    NSInteger _awardCoin;
    NSInteger _awardGem;
    NSInteger _awardHp;
    GADInterstitial * interstitial;
    VungleSDK * vungleSdk;
}

- (IBAction)showUnityAds:(NSInteger)adswhere icoin:(NSInteger)coin igem:(NSInteger)gem ihp:(NSInteger)hp;
- (IBAction)showVungleAds:(NSInteger)adswhere icoin:(NSInteger)coin igem:(NSInteger)gem ihp:(NSInteger)hp;
- (IBAction)showAdMobAds:(NSInteger)adswhere icoin:(NSInteger)coin igem:(NSInteger)gem ihp:(NSInteger)hp;
- (bool) isReady;
- (bool) unityIsReady;
- (bool) vungleIsReady;
- (bool) admobIsReady;
@end

