#ifndef _COMMONFUNCS_H_
#define _COMMONFUNCS_H_
#include <iostream>
#include "cocos2d.h"
#include <algorithm>
class CommonFuncs
{
public:
	static int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",");
	static void StringReplace(std::string &strBase, std::string strSrc, std::string strDes);
	static std::string gbk2utf(const char *inbuf);
private:
	static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);
};

#endif
