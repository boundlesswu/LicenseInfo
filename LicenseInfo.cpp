#include "AES.h"
#include "LicenseInfo.h"


int licenseInfo::WriterLicenseFile(const char *outputFileName, const char *machineInfo, const char *key = nullptr) {
  if (key == nullptr) {
    AES aes;
    return aes.WriterLicenseFile(machineInfo, this, outputFileName);

  } else {
    AES aes((unsigned char *) key);
    return aes.WriterLicenseFile(machineInfo, this, outputFileName);
  }
}

int licenseInfo::ReadLicenseFile(const char *outputFileName, char *md5sum, const char *key = nullptr) {
  if (key == nullptr) {
    AES aes;
    return aes.ReadLicenseFile(outputFileName, this, md5sum);
  } else {
    AES aes((unsigned char *) key);
    return aes.ReadLicenseFile(outputFileName, this, md5sum);
  }
}

int licenseInfo::UpdateLicenseFile(const char *outputFileName, const char *key = nullptr) {
  char md5sum[32 + 1] = {0};
  int ret = ReadLicenseFile(outputFileName, md5sum, key);
  if (ret < 0) {
    return ret;
  }
  loginTime = time(NULL);
  return WriterLicenseFile(outputFileName, md5sum, key);
}

int licenseInfo::UpdateLicenseFile(const char *outputFileName) {
  return UpdateLicenseFile(outputFileName, NULL);
}

void licenseInfo::display() {
  std::cout << "starTime:" << this->starTime << " " << time2string(&(this->starTime))  <<std::endl;
  std::cout << "endTime:" << this->endTime << " " << time2string(&(this->endTime)) << std::endl;
  std::cout << "validTime:" << this->validTime << std::endl;
  std::cout << "loginTime:" << this->loginTime  << " " << time2string(&(this->loginTime)) << std::endl;
  std::cout << "maxChannelNum:" << this->maxChannelNum << "   maxCallNum:" << this->maxCallNum << std::endl;
  std::cout << "functionBit:" << this->functionBit << std::endl;
}
