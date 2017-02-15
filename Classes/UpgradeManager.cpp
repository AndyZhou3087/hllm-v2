/****************************************************************************
 版权:
 作者:
 日期：
 内容: 在线升级数据，下载文件为zip，下载后自动解压（解压后自动删除zip文件）,该类支持断点续传.
 ****************************************************************************/

#include "UpgradeManager.h"

#include <stdio.h>
#include <vector>
#include <thread>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//win32 需要做以下操作
//1.只要在直接调用curl库的项目上加上两个预编译选项：
//BUILDING_LIBCURL; HTTP_ONLY
//2.加上三个链接库
#pragma comment(lib, "libcurl_imp.lib")
#include <curl/include/win32/curl/curl.h>
#include <curl/include/win32/curl/easy.h>
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <asm/io.h>
#include <unistd.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <curl/include/android/curl/curl.h>
#include <curl/include/android/curl/easy.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <curl/include/ios/curl/curl.h>
#include <curl/include/ios/curl/easy.h>
#endif
#endif

#include "cocos2d.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include "base/CCUserDefault.h"
#include "platform/CCFileUtils.h"

#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include "unzip/unzip.h"
#endif

USING_NS_CC;
using namespace std;

/* 下载文件名 */
#define TEMP_PACKAGE_FILE_NAME "temp.zip"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

UpgradeManager::UpgradeManager()
{
}

UpgradeManager::~UpgradeManager()
{
}

UpgradeManager::UpgradeManager(const char* packageUrl /* = NULL */, const char* storagePath /* = NULL */):
m_sPackageUrl(packageUrl),
m_sStoragePath(storagePath),
m_pUMDelegateProtocol(nullptr),
m_nTimeOut(0),
m_pCurl(nullptr),
m_lLocalFileLenth(0L)
{
	checkStoragePath();
}

void UpgradeManager::setPackageUrl(const char* packageUrl)
{
	m_sPackageUrl = packageUrl;
}

void UpgradeManager::setStoragePath(const char* storagePath)
{
	m_sStoragePath = storagePath;
}

void UpgradeManager::setDelegate(UpgradeManagerDelegateProtocol *delegateProtocol)
{
	m_pUMDelegateProtocol = delegateProtocol;
}

void UpgradeManager::setConnectionTimeout(unsigned int timeout)
{
	m_nTimeOut = timeout;
}

int assetsManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	static int percent = 0;
	auto manager = static_cast<UpgradeManager*>(ptr);


	int tmp = 0;
	if (0 < totalToDownload)
	{
		tmp = (int)((nowDownloaded + manager->m_lLocalFileLenth) / (totalToDownload + manager->m_lLocalFileLenth) * 100);
	}
	else
	{
		//tmp = (int)(nowDownloaded / totalToDownload * 100);
		tmp = 0;
	}

	if (percent != tmp)
	{
		percent = tmp;
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			
			if (manager->m_pUMDelegateProtocol)
			{
				manager->m_pUMDelegateProtocol->onProgress(nowDownloaded + manager->m_lLocalFileLenth, totalToDownload + manager->m_lLocalFileLenth);
			}
		});

		CCLOG("wpj --> downloading... %d%%", percent);
	}

	return 0;
}

void UpgradeManager::checkStoragePath()
{
	if (m_sStoragePath.size() > 0 && m_sStoragePath[m_sStoragePath.size() - 1] != '/')
	{
		m_sStoragePath.append("/");
	}
}

void UpgradeManager::createStoragePath()
{
	size_t startIndex = 0;

	size_t index = m_sStoragePath.find("/", startIndex);

	while (index != std::string::npos)
	{
		const string dir = m_sStoragePath.substr(0, index);

		FILE *out = fopen(dir.c_str(), "r");

		if (!out)
		{
			if (!createDirectory(dir.c_str()))
			{
				CCLOG("can not create directory %s", dir.c_str());
				return;
			}
			else
			{
				CCLOG("create directory %s", dir.c_str());
			}
		}
		else
		{
			fclose(out);
		}

		startIndex = index + 1;

		index = m_sStoragePath.find("/", startIndex);

	}
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *fp = (FILE*)userdata;
	size_t written = fwrite(ptr, size, nmemb, fp);
	return written;
}

bool UpgradeManager::downLoad()
{
	const string outFileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
	CCLOG("downLoad path %s", outFileName.c_str());

	createStoragePath();

	FILE *fp = NULL;//fopen(outFileName.c_str(), "wb");

	if(0 == access(outFileName.c_str(), 0))
	{ 
		// 以二进制形式追加
		fp = fopen(outFileName.c_str(), "ab+");
	}
	else
	{ 
		// 二进制写
		fp = fopen(outFileName.c_str(), "wb");
	}

	if (fp == NULL)
	{
		// 如果文件初始化失败进行返回
		return false;
	}

	if (! fp)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]
		{
			if (this->m_pUMDelegateProtocol)
			{
				this->m_pUMDelegateProtocol->onError(ErrorCode::CREATE_FILE);
			}
		});
		CCLOG("wpj --> can not create file %s", outFileName.c_str());
		return false;
	}
	m_lLocalFileLenth = getLocalFileLength();
	m_pCurl = curl_easy_init();
	// Download pacakge
	CURLcode res;
	curl_easy_setopt(m_pCurl, CURLOPT_URL, m_sPackageUrl.c_str());//下载url
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, downLoadPackage);//写文件回调方法
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, fp); // 写入文件对象
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, assetsManagerProgressFunc);//下载进度回调方法
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);// 传入本类对象
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(m_pCurl, CURLOPT_LOW_SPEED_LIMIT, 1L);//设置“低限速”
	curl_easy_setopt(m_pCurl, CURLOPT_LOW_SPEED_TIME, 5L);//设置“低限时”
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1 );
	curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, m_lLocalFileLenth);  // 从本地大小位置进行请求数据
//	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, m_nTimeOut);//超时这个数额几秒钟后读操作


	res = curl_easy_perform(m_pCurl);
	curl_easy_cleanup(m_pCurl);
	if (res != 0)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]
		{
			if (this->m_pUMDelegateProtocol)
			{
				this->m_pUMDelegateProtocol->onError(ErrorCode::NETWORK);
			}
		});
		CCLOG("wpj --> error when download package");
		fclose(fp);
		return false;
	}

	CCLOG("wpj --> succeed downloading package %s", m_sPackageUrl.c_str());

	fclose(fp);
	return true;
}

long UpgradeManager::getLocalFileLength()
{
	string filePathName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
	FILE *fp = fopen(filePathName.c_str(), "r");
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fclose(fp);
	CCLOG("file length %ld", length);
	return length;
}

void UpgradeManager::setSearchPath()
{
	vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
	vector<string>::iterator iter = searchPaths.begin();
	searchPaths.insert(iter, m_sStoragePath);
	FileUtils::getInstance()->setSearchPaths(searchPaths);
}

void UpgradeManager::downloadAndUncompress()
{
	do
	{
		if (!downLoad())
		{
			break;
		}

		// 解压zip文件.
		if (!uncompress())
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]
			{
// 				UserDefault::getInstance()->setStringForKey(this->keyOfDownloadedVersion().c_str(),"");
// 				UserDefault::getInstance()->flush();
				if (this->m_pUMDelegateProtocol)
				{
					this->m_pUMDelegateProtocol->onError(ErrorCode::UNCOMPRESS);
				}
			});
			break;
		}

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] 
		{

			// Record new version code.
			//UserDefault::getInstance()->setStringForKey(this->keyOfVersion().c_str(), this->_version.c_str());

			// Unrecord downloaded version code.
// 			UserDefault::getInstance()->setStringForKey(this->keyOfDownloadedVersion().c_str(), "");
// 			UserDefault::getInstance()->flush();

			// Set resource search path.
			this->setSearchPath();

			// Delete unloaded zip file.
			string zipfileName = this->m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
			if (remove(zipfileName.c_str()) != 0)
			{
				CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
			}

			if (this->m_pUMDelegateProtocol)
			{
				this->m_pUMDelegateProtocol->onSuccess();
			}
		});

	} while (0);
//	_isDownloading = false;
}

bool UpgradeManager::uncompress()
{
	// Open the zip file
	string outFileName = m_sStoragePath + TEMP_PACKAGE_FILE_NAME;
	unzFile zipfile = unzOpen(outFileName.c_str());
	if (! zipfile)
	{
		CCLOG("can not open downloaded zip file %s", outFileName.c_str());
		return false;
	}

	// Get info about the zip file
	unz_global_info global_info;
	if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
	{
		CCLOG("can not read file global info of %s", outFileName.c_str());
		unzClose(zipfile);
		return false;
	}

	// Buffer to hold data read from the zip file
	char readBuffer[BUFFER_SIZE];

	CCLOG("start uncompressing");

	// Loop to extract all files.
	uLong i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info fileInfo;
		char fileName[MAX_FILENAME];
		if (unzGetCurrentFileInfo(zipfile,
			&fileInfo,
			fileName,
			MAX_FILENAME,
			nullptr,
			0,
			nullptr,
			0) != UNZ_OK)
		{
			CCLOG("can not read file info");
			unzClose(zipfile);
			return false;
		}

		const string fullPath = m_sStoragePath + fileName;

		// Check if this entry is a directory or a file.
		const size_t filenameLength = strlen(fileName);
		if (fileName[filenameLength-1] == '/')
		{
			// Entry is a direcotry, so create it.
			// If the directory exists, it will failed scilently.
			if (!createDirectory(fullPath.c_str()))
			{
				CCLOG("can not create directory %s", fullPath.c_str());
				unzClose(zipfile);
				return false;
			}
		}
		else
		{
			//There are not directory entry in some case.
			//So we need to test whether the file directory exists when uncompressing file entry
			//, if does not exist then create directory
			const string fileNameStr(fileName);

			size_t startIndex=0;

			size_t index = fileNameStr.find("/",startIndex);

			while(index != std::string::npos)
			{
				const string dir=m_sStoragePath + fileNameStr.substr(0,index);

				FILE *out = fopen(dir.c_str(), "r");

				if(!out)
				{
					if (!createDirectory(dir.c_str()))
					{
						CCLOG("can not create directory %s", dir.c_str());
						unzClose(zipfile);
						return false;
					}
					else
					{
						CCLOG("create directory %s",dir.c_str());
					}
				}
				else
				{
					fclose(out);
				}

				startIndex=index+1;

				index=fileNameStr.find("/",startIndex);

			}



			// Entry is a file, so extract it.

			// Open current file.
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				CCLOG("can not open file %s", fileName);
				unzClose(zipfile);
				return false;
			}

			// Create a file to store current file.
			FILE *out = fopen(fullPath.c_str(), "wb");
			if (! out)
			{
				CCLOG("can not open destination file %s", fullPath.c_str());
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return false;
			}

			// Write current file content to destinate file.
			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
				if (error < 0)
				{
					CCLOG("can not read zip file %s, error code is %d", fileName, error);
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				if (error > 0)
				{
					fwrite(readBuffer, error, 1, out);
				}
			} while(error > 0);

			fclose(out);
		}

		unzCloseCurrentFile(zipfile);

		// Goto next entry listed in the zip file.
		if ((i + 1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				CCLOG("can not read next file");
				unzClose(zipfile);
				return false;
			}
		}
	}

	CCLOG("end uncompressing");
	unzClose(zipfile);

	return true;
}

bool UpgradeManager::createDirectory(const char *path)
{
	return FileUtils::getInstance()->createDirectory(path);

}

void UpgradeManager::update()
{
// 	if (_isDownloading) return;
// 
// 	_isDownloading = true;

	// 1. Urls of package and version should be valid;
	// 2. Package should be a zip file.
	if (m_sPackageUrl.size() == 0 || std::string::npos == m_sPackageUrl.find(".zip"))
	{
		CCLOG("no package url, or the package is not a zip file");
		//_isDownloading = false;
		return;
	}

	// Check if there is a new version.
// 	if (! checkUpdate())
// 	{
// 		_isDownloading = false;
// 		return;
// 	}

	// Is package already downloaded?
	//_downloadedVersion = UserDefault::getInstance()->getStringForKey(keyOfDownloadedVersion().c_str());

	auto t = std::thread(&UpgradeManager::downloadAndUncompress, this);
	t.detach();
}