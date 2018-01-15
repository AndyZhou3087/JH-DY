//
//  IOSPurchase.m
//  kuxx-hllm_v1.1.1
//
//  Created by Hailin Rao on 2016/12/16.
//
//

#import <Foundation/Foundation.h>
#import "IOSPurchase.h"
#import "GlobalData.h"
#import "ShopLayer.h"
#include "HintBox.h"
#include "Const.h"

@interface IOSPurchase ()

@end

@implementation IOSPurchase

- (void) initMyBuy {
    _productId = @"";
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

- (void) buy:(NSString *) productId {
    _productId = productId;
    //[self requestProductData];
    //GlobalData::setHasBuy(true);
    //[[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    SKPayment * payment = [SKPayment paymentWithProductIdentifier:productId];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void) restore {
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

/*- (void) requestProductData {
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    NSArray * product = nil;
    product = [[NSArray alloc] initWithObjects:@"b001",@"b002",@"b003",@"b004",@"b005",@"b006",@"b007",@"b008",@"b009",@"b010",@"b014",nil];
    NSSet * nsset = [NSSet setWithArray:product];
    SKProductsRequest * request = [[SKProductsRequest alloc] initWithProductIdentifiers:nsset];
    //[request autorelease];
    request.delegate = self;
    [request start];
}*/

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response NS_AVAILABLE_IOS(3_0) {
    NSArray * myProduct = response.products;
    //NSLog(@"aaa product id=%@", response.invalidProductIdentifiers);
    //NSLog(@"aaa count=%d", (int)[myProduct count]);
    for (SKProduct * product in myProduct) {
        //NSLog(@"product info");
        //NSLog(@"des=%@", [product description]);
        //NSLog(@"title=%@", product.localizedTitle);
        //NSLog(@"ldes=%@", product.localizedDescription);
        //NSLog(@"price=%@", product.price);
        //NSLog(@"id=%@", product.productIdentifier);
        if ([product.productIdentifier isEqualToString:_productId]) {
            SKPayment * payment = [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
            break;
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray<SKPaymentTransaction *> *)transactions NS_AVAILABLE_IOS(3_0) {
    NSLog(@"aaa updatedTransactions");
    UIAlertView * alertView;
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                //NSLog(@"aaa transaction complelete");
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                //NSString * pid = transaction.payment.productIdentifier;
                alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"购买成功" delegate:nil cancelButtonTitle:NSLocalizedString(@"close", nil) otherButtonTitles:nil, nil];
                [alertView show];
                //NSLog(@"aaa transiden=%@", transaction.transactionIdentifier);
                
                for (int i=1; i<4; i++) {
                    if ([_productId isEqualToString:[NSString stringWithUTF8String:payCode[i].c_str()]])
                        GlobalData::setUnlockHero(i, true);
                }
                ShopLayer::setMessage(PAY_SUCC);
                /*if (GlobalData::getHasBuy() && [transaction.payment.productIdentifier isEqualToString:_productId]) {
                    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
                    GlobalData::setHasBuy(false);
                }*/
                break;
            case SKPaymentTransactionStateFailed:
                NSLog(@"aaa transaction failed");
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"购买失败" delegate:nil cancelButtonTitle:NSLocalizedString(@"close", nil) otherButtonTitles:nil, nil];
                [alertView show];
                /*if (GlobalData::getHasBuy() && [transaction.payment.productIdentifier isEqualToString:_productId]) {
                    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
                    GlobalData::setHasBuy(false);
                }*/
                ShopLayer::setMessage(PAY_FAIL);
                break;
            case SKPaymentTransactionStateRestored:
                NSLog(@"aaa transaction restored");
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"恢复成功" delegate:nil cancelButtonTitle:NSLocalizedString(@"close", nil) otherButtonTitles:nil, nil];
                [alertView show];
                for (int i=1; i<4; i++) {
                    if ([transaction.payment.productIdentifier isEqualToString:[NSString stringWithUTF8String:payCode[i].c_str()]])
                        GlobalData::setUnlockHero(i, true);
                }
                /*if (GlobalData::getHasBuy() && [transaction.payment.productIdentifier isEqualToString:_productId]) {
                    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
                    GlobalData::setHasBuy(false);
                }*/
                break;
        }
        
    }
    //[[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}

- (void)requestDidFinish:(SKRequest *)request NS_AVAILABLE_IOS(3_0) {
    //NSLog(@"aaa didfinish");
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error NS_AVAILABLE_IOS(3_0) {
    //NSLog(@"aaa didfailwitherror");
}

@end
