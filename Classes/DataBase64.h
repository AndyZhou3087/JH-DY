
/********************************************************************
 *    文件名  :    DATABASE64.H
 *    作者    :    
 *    文件描述:    结合database64算法来加密存档数据
 *********************************************************************/

#ifndef _H_DATABASE64_H_
#define _H_DATABASE64_H_

#include <string>

std::string encryptData(unsigned char const*, unsigned int len);
std::string parseData(std::string const& s);

#endif // _H_DATABASE64_H_
