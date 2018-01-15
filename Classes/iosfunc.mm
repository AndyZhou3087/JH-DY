//
//  iosfunc.m
//
//  Created by  admin on 16-11-25.
//
//

#import <Foundation/Foundation.h>
#import "iosfunc.h"
#include "MBProgressHUD.h"
#import <UShareUI/UShareUI.h>
#include "GlobalData.h"
#import "LZKeychain.h"

const char * getuuid()
{
    NSString * uuid = [[NSUUID UUID] UUIDString];
    return [uuid UTF8String];
}

const char * getbundleid()
{
    return [[[NSBundle mainBundle]bundleIdentifier] UTF8String];
}

const char * getvercode()
{
    return [[[NSBundle mainBundle]objectForInfoDictionaryKey:@"CFBundleShortVersionString"] UTF8String];
}


void setUserDefaultsBool(char * key, bool value) {
    NSUserDefaults * usedef = [NSUserDefaults standardUserDefaults];
    [usedef setBool:value forKey:[NSString stringWithUTF8String:key]];
}

bool getUserDefaultsBool() {
    NSUserDefaults * usedef = [NSUserDefaults standardUserDefaults];
    bool a = [usedef boolForKey:@"noads"];
    return a;
}

void copytoclipboard(char * p) {
    //把char*转换成OC的NSString
    NSString *nsMessage= [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    
    //获得iOS的剪切板
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    
    //改变剪切板的内容
    pasteboard.string = nsMessage;
    toast("QQ号码已经复制到剪贴板！");
}

const void toast(char * info)
{
    UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    MBProgressHUD * HUD = [[MBProgressHUD alloc] initWithView:s.view];
    [s.view addSubview:HUD];
    HUD.label.text = [NSString stringWithUTF8String:info];
    HUD.mode = MBProgressHUDModeText;
    
    //指定距离中心点的X轴和Y轴的偏移量，如果不指定则在屏幕中间显示
    //    HUD.yOffset = 150.0f;
    //    HUD.xOffset = 100.0f;
    
    [HUD showAnimated:YES whileExecutingBlock:^{
        sleep(1.5);
    } completionBlock:^{
        [HUD removeFromSuperview];
        [HUD release];
        //HUD = NULL;
    }];
}

void alterView() {
    /*UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"评论" message:@"求求你给个好评吧！" delegate:nil cancelButtonTitle:@"拒绝" otherButtonTitles:@"下次再说", nil];
     alert.alertViewStyle = UIAlertViewStyleDefault;
     [alert show];*/
    /*UIActionSheet * sheet = [[UIActionSheet alloc] initWithTitle:@"求评论" delegate:nil cancelButtonTitle:@"残忍拒绝" destructiveButtonTitle:@"下次再说" otherButtonTitles:@"乐意接受", nil];
     UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
     [sheet showInView:];*/
    UIViewController * s = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    UIAlertController * alert = [UIAlertController alertControllerWithTitle:@"大侠！帮个忙吧！" message:@"大侠！您一生行侠仗义，给个好评吧！" preferredStyle:UIAlertControllerStyleAlert];
    //NSMutableAttributedString *hogan = [[NSMutableAttributedString alloc] initWithString:@"玩的这么爽"];
    //[hogan addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:20.0] range:NSMakeRange(0, 20)];
    //[alert setValue:hogan forKey:@"attributedTitle"];
    alert.popoverPresentationController.barButtonItem = s.navigationItem.leftBarButtonItem;
    UIAlertAction * ac = [UIAlertAction actionWithTitle:@"好说好说" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"itms-apps://itunes.apple.com/cn/app/id1215576714?mt=8&action=write-review"]];

        GlobalData::setNoComments(true);
    }];
    //[ac setValue:[UIColor greenColor] forKey:@"_titleTextColor"];
    [alert addAction:ac];
    [alert addAction:[UIAlertAction actionWithTitle:@"下次再说" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
    }]];
    UIAlertAction * ac3 = [UIAlertAction actionWithTitle:@"懒得理你" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        GlobalData::setNoComments(true);
    }];
    [alert addAction:ac3];
    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 8.3)
        [ac3 setValue:[UIColor grayColor] forKey:@"_titleTextColor"];
    [s presentViewController:alert animated:YES completion:nil];
}

void makeNotification()
{
    // 1.创建本地通知
    UILocalNotification *localNote = [[UILocalNotification alloc] init];
    
    // 1.1.设置什么时间弹出
    localNote.fireDate = [NSDate dateWithTimeIntervalSinceNow:1];
    
    // 1.2.设置弹出的内容
    localNote.alertBody = @"闭关已结束！";
    
    // 1.3.设置锁屏状态下,显示的一个文字
    localNote.alertAction = @"快点打开";
    
    // 1.4.显示启动图片
    localNote.alertLaunchImage = @"Icons/Icon-28.png";
    
    // 1.5.是否显示alertAction的文字(默认是YES)
    localNote.hasAction = YES;
    
    // 1.6.设置音效
    localNote.soundName =UILocalNotificationDefaultSoundName;
    
    // 1.7.应用图标右上角的提醒数字
    localNote.applicationIconBadgeNumber =1;
    
    // 1.8.设置UserInfo来传递信息
    localNote.userInfo = @{@"alertBody" : localNote.alertBody,@"applicationIconBadgeNumber" : @(localNote.applicationIconBadgeNumber)};
    
    // 2.调度通知
    [[UIApplication sharedApplication] scheduleLocalNotification:localNote];
}

void UMShare()
{
    [UMSocialUIManager showShareMenuViewInWindowWithPlatformSelectionBlock:^(UMSocialPlatformType platformType, NSDictionary *userInfo) {
        // 根据获取的platformType确定所选平台进行下一步操作
    }];
}

const char * getDeviceIDInKeychain()
{
    NSString * uuid = [LZKeychain getDeviceIDInKeychain];
    return [uuid UTF8String];
}

const char* gbkToUTF8(const char * p) {
    //把char*转换成OC的NSString
    NSString *nsstr = [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    NSString *utf8str = [nsstr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return [utf8str UTF8String];
}

