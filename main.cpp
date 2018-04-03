#include <iostream>
#include <cstdio>

#include "AES.h"
#include "LicenseInfo.h"

void print(unsigned char* state)
{
  int i;
  for(i=0; i<16; i++)
  {
    printf("%s%X ",state[i]>15 ? "" : "0", state[i]);
  }
  printf("\n");
}

int main() {
  licenseInfo info;
  info.starTime = time(NULL);
  info.endTime = info.starTime + 3600*24*365;
  info.validTime = 3600*24*365;
  info.loginTime = time(NULL);
  info.maxCallNum = 500000L;
  info.maxChannelNum =  64;
  info.functionBit = 0x678;

  display(&info);
  const char md5sum[]="c3d9d2be5a2c083ec7845ae3db8a2ad9";
  const char licenseFileName[]="license.dat";

  ::WriterLicenseFile(licenseFileName,md5sum,&info, NULL);
  licenseInfo licenseInfo1 ;
  char machineInfo[32+1] = {0};

  ::ReadLicenseFile(licenseFileName, machineInfo,&licenseInfo1, NULL);
  std::cout << "ms5sum:" <<machineInfo<<std::endl;
  display(&licenseInfo1);
  return 0;
}


