#include "AES.h"
#include "LicenseInfo.h"


int WriterLicenseFile(const char *outputFileName,const char *machineInfo, licenseInfo *pl, const char* key= nullptr){
  if(key == nullptr){
    AES aes;
    return aes.WriterLicenseFile(machineInfo,pl, outputFileName);

  }else{
    AES aes((unsigned char *) key);
    return aes.WriterLicenseFile(machineInfo,pl, outputFileName);
  }
}

int ReadLicenseFile(const char *outputFileName, char *md5sum,licenseInfo *pl, const char* key= nullptr){
  if(key == nullptr){
    AES aes;
    return aes.ReadLicenseFile(outputFileName, pl, md5sum);
  }else{
    AES aes((unsigned char *) key);
    return aes.ReadLicenseFile(outputFileName, pl, md5sum);
  }
}

int UpdateLicenseFile(const char *outputFileName,const char* key= nullptr){
  char md5sum[32+1] = {0};
  licenseInfo al;
  int ret = ReadLicenseFile(outputFileName,md5sum,&al,key);
  if(ret < 0) {
    return ret;
  }
  al.loginTime = time(NULL);
  return WriterLicenseFile(outputFileName,md5sum,&al,key);
}

int UpdateLicenseFile(const char *outputFileName){
  return UpdateLicenseFile(outputFileName,NULL);
}