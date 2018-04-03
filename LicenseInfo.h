#include <time.h>

#ifndef _LICENSE_INFO_H_
#define _LICENSE_INFO_H_

struct licenseInfo{
    time_t starTime; //产品license发布时间戳，默认为生成license的时间
    time_t endTime; //产品license中止时间戳，默认为生成license的时间加上产品有效时间
    unsigned long validTime; //产品license有效时间（秒数）
    time_t loginTime; //上次登录时间戳，第一次使用前设置为0，然后每次使用更新该时间戳，每隔一个小时更新一遍
    unsigned long maxChannelNum; //产品支持最大的视频路数
    unsigned long maxCallNum;//产品支持最大的最大的调用次数
    unsigned long functionBit; //产品功能设置位
};


int WriterLicenseFile(const char *outputFileName,const char *machineInfo, licenseInfo *pl, const char* key);

int ReadLicenseFile(const char *outputFileName, char *md5sum,licenseInfo *pl, const char* key);

int UpdateLicenseFile(const char *outputFileName);

inline void display(licenseInfo* p){
  std::cout<<"starTime:"<<p->starTime<<"   endTime:" << p->endTime<<std::endl;
  std::cout<<"validTime:"<<p->validTime<<"   loginTime:" << p->loginTime<<std::endl;
  std::cout<<"maxChannelNum:"<<p->maxChannelNum<<"   maxCallNum:" << p->maxCallNum<<std::endl;
  std::cout<<"functionBit:"<<p->functionBit<<std::endl;
}
#endif