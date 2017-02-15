#include "CommonFuncs.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../cocos2d//external//win32-specific/icon/include/iconv.h"
#endif
int CommonFuncs::split(const std::string& str, std::vector<std::string>& ret_, std::string sep)
{
	if (str.empty())
	{
		return 0;
	}

	std::string tmp;
	std::string::size_type pos_begin = 0;//str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;

	while (pos_begin != std::string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		//if (!tmp.empty())
		//{
			ret_.push_back(tmp);
			tmp.clear();
		//}
	}


	return 0;
}

void CommonFuncs::StringReplace(std::string &strBase, std::string strSrc, std::string strDes)
{
	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
}

int CommonFuncs::code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);

	/*
	#else
	if (iconv(cd, &inbuf, &inlen, &outbuf, &outlen) == -1)
	{
		iconv_close(cd);
		return  - 1;
	}
	*/
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	
#endif
	return 0;
}

std::string CommonFuncs::gbk2utf(const char *inbuf)
{
	std::string strRet;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];

	if (code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	strRet = inbuf;
#endif
	return strRet;
}
