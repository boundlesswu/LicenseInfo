#include <time.h>
#include <string.h>

#ifndef _LICENSE_INFO_H_
#define _LICENSE_INFO_H_

struct licenseInfo
{
public:
    time_t starTime; //产品license发布时间戳，默认为生成license的时间
public:
    time_t endTime; //产品license中止时间戳，默认为生成license的时间加上产品有效时间
public:
    unsigned long validTime; //产品license有效时间（秒数）
public:
    time_t loginTime; //上次登录时间戳，第一次使用前设置为0，然后每次使用更新该时间戳，每隔一个小时更新一遍
public:
    unsigned long maxChannelNum; //产品支持最大的视频路数
public:
    unsigned long maxCallNum;//产品支持最大的最大的调用次数
public:
    unsigned long functionBit; //产品功能设置位
//public:
//    int WriterLicenseFile(const char *outputFileName, const char *machineInfo, licenseInfo *pl, const char* key);
//public:
//    int ReadLicenseFile(const char *outputFileName, char *md5sum, licenseInfo *pl, const char* key);
public:
    int WriterLicenseFile(const char *outputFileName, const char *machineInfo,  const char* key);
public:
    int ReadLicenseFile(const char *outputFileName, char *md5sum,const char* key);
public:
    int UpdateLicenseFile(const char *outputFileName, const char* key);
public:
    int UpdateLicenseFile(const char *outputFileName);
public:
    void display();
public:
    std::string time2string(time_t* tt){
      char buf[64]={0};
      strftime(buf, 64, "%Y-%m-%d %H:%M:%S", localtime(tt));
      return buf;
    }

};
#endif
