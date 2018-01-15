//
//  IOSPurchase.h
//  kuxx-hllm_v1.1.1
//
//  Created by Hailin Rao on 2016/12/16.
//
//

#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@interface IOSPurchase : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    NSString * _productId;
}

@end
