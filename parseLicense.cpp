#include <cstdio>
#include <iostream>
#include "LicenseInfo.h"

using namespace std;
void printUsage(){
  std::cout<<"usage ：parseLicense  $licensefile" << std::endl;
}
int main(int argc,char* argv[]){
  if(argc < 2){
    cout<<"parameter error"<<endl;
    printUsage();
    return -1;
  }
  char* licensefile = argv[1];
  char serviceinfo2[64]={0};
  licenseInfo lI2;
  ::ReadLicenseFile(licensefile,serviceinfo2,&lI2,NULL);
  cout <<  "md5sum "<< serviceinfo2 << endl;
  display(&lI2);
  return 0;
}