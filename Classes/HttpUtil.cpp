#include "HttpUtil.h"

HttpUtil::HttpUtil()
{
	m_httpcallback = NULL;
}

HttpUtil::~HttpUtil()
{
	m_httpcallback = NULL;
}

HttpUtil* HttpUtil::getInstance()
{
	HttpUtil* httputil = new HttpUtil();
	httputil->autorelease();
	httputil->retain();
	return httputil;
}

void HttpUtil::doData(std::string url, HTTPCOMPLETE_CALLBACK http_cb, std::string filename, HTTPTYPE method, std::string postdata, std::string tag)
{
	m_httpcallback = http_cb;
	m_filename = filename;
	m_tag = tag;
	HttpClient* httpClient = HttpClient::getInstance();
	HttpRequest* httpReq = new HttpRequest();
	HttpRequest::Type httpmethod = HttpRequest::Type::GET;
	if (method == POST)
	{
		httpmethod = HttpRequest::Type::POST;
		httpReq->setRequestData(postdata.c_str(), postdata.length());
	}

	httpReq->setRequestType(httpmethod);
	httpReq->setUrl(url.c_str());
	httpReq->setResponseCallback(CC_CALLBACK_2(HttpUtil::onHttpRequestCompleted, this));
	//httpReq->setResponseCallback(ccHttpRequestCallback(onHttpRequestCompleted));
	httpClient->setTimeoutForConnect(10);
	httpClient->send(httpReq);
	httpReq->release();
	httpReq = NULL;
}

void HttpUtil::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *resp)
{
	std::string retdata;
	int retcode = 0;
	HttpResponse* response = (HttpResponse*)resp;
	//    判断是否响应成功
	if (!response->isSucceed())
	{
		retcode = -1;
	}
	else
	{
		std::vector<char> *buffer = NULL;
		buffer = response->getResponseData();
		std::string tag = response->getHttpRequest()->getTag();
		if (buffer == NULL)
		{
			retcode = -2;
		}
		else
		{
			int length = buffer->size();
			for (int i = 0; i < length; ++i)
			{
				retdata += (*buffer)[i];
			}
			retdata += '\0';
		}

		// 数据转存文件
		if (m_filename.length() > 0 && buffer != NULL)
		{
			std::string path = FileUtils::getInstance()->getWritablePath() + m_filename;
			std::string data(buffer->begin(), buffer->end());

			//保存到本地文件
			//log("path: %s", path.c_str());
			FILE *fp = fopen(path.c_str(), "wb+");
			fwrite(data.c_str(), 1, buffer->size(), fp);
			fclose(fp);
		}
	}
	if (m_httpcallback != NULL)
		m_httpcallback(retdata, retcode, m_tag);

	release();
}