#include "IOSPurchaseWrap.h"
#include "IOSPurchase.h"
//#import "TalkingDataGA.h"

IOSPurchase * iosPurchase;
void buy(char * productId) {
    [iosPurchase buy:[NSString stringWithUTF8String:productId]];
    NSString * eventName = [NSString stringWithFormat:@"buy%@", [NSString stringWithUTF8String:productId]];
    //[TalkingDataGA onEvent:eventName eventData:nil];
}

void initBuy() {
    iosPurchase = [[IOSPurchase alloc] init];
    [iosPurchase initMyBuy];
}

void restoreBuy() {
    [iosPurchase restore];
}
