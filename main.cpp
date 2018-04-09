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

  info.display();
  const char md5sum[]="c3d9d2be5a2c083ec7845ae3db8a2ad9";
  const char licenseFileName[]="license.dat";

  info.WriterLicenseFile(licenseFileName,md5sum, NULL);
  licenseInfo licenseInfo1 ;
  char machineInfo[32+1] = {0};

  licenseInfo1.ReadLicenseFile(licenseFileName, machineInfo, NULL);
  std::cout << "ms5sum:" <<machineInfo<<std::endl;
  licenseInfo1.display();
  return 0;
}


