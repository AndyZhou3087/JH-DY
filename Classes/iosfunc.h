//
//  iosfunc.h
//
//  Created by  admin on 16-11-25.
//
//


const char * getuuid();
const char * getbundleid();
const char * getvercode();
void copytoclipboard(char * p);
const void toast(char * info);
void alterView();
void makeNotification();
void UMShare();
const char * getDeviceIDInKeychain();
const char* gbkToUTF8(const char * p);
