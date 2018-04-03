#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include "LicenseInfo.h"

using namespace std;
void printUsage(){
  std::cout<<"usage ：geneateLicense  $licensefile $serverInfofile "<<
    "$starttime $validTime $maxChannelNum" << std::endl;
}

int readsum5file(const char* file, unsigned char* output){
  ifstream ifs;
  ifs.open(file, ios::in);
  if (!ifs) {
    printf(" Open input %s file failed!\n", file);
    return -1;
  }
  string strLine;
  while (!ifs.eof()) {
    char temp[1024];
    memset(temp, '\0', 1024);
    ifs.getline(temp, 1024);
    strLine += temp;
  }
  ifs.close();
  strcpy((char *) output, (char *) strLine.c_str());
}

int main(int argc,char* argv[]){
  if(argc < 5){
    cout<<"parameter error"<<endl;
    printUsage();
    return -1;
  }
  char* licensefile = argv[1];
  char* serverInfofile = argv[2];
  licenseInfo lI;
  istringstream is(argv[3]);
  is >> lI.starTime;
  istringstream is2(argv[4]);
  is2 >> lI.validTime;
  istringstream is3(argv[5]);
  is3 >> lI.maxChannelNum;
  lI.endTime = lI.starTime + lI.validTime;
  lI.loginTime = 0L;
  lI.functionBit = 0L;
  lI.maxCallNum = 0L;
  char serviceinfo[64]={0};
  if(readsum5file(serverInfofile, (unsigned char *) serverInfofile) < 0){
    cout<< "read service info error" <<endl;
    return -1;
  }
  ::WriterLicenseFile(licensefile,serverInfofile,&lI, NULL);

  char serviceinfo2[64]={0};
  licenseInfo lI2;
  ::ReadLicenseFile(licensefile,serviceinfo2,&lI2,NULL);
  cout <<  "md5sum "<< serviceinfo2 << endl;
  display(&lI2);
  return 0;


}