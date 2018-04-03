///////////////////////////////
// http://mingcn.cnblogs.com //
//  xelz CopyRight (c) 2010  //
///////////////////////////////

#include "AES.h"

using namespace std;

AES::AES() {
  CopyConst();
  unsigned char vorxCommonKey[] =
    {
      0x2b, 0x7e, 0x15, 0x16,
      0x28, 0xae, 0xd2, 0xa6,
      0xab, 0xf7, 0x15, 0x88,
      0x09, 0xcf, 0x4f, 0x3c
    };
  KeyExpansion(vorxCommonKey, w);
}

AES::AES(unsigned char *key) {
  CopyConst();
  KeyExpansion(key, w);
}

AES::~AES() {

}

int AES::EncryptLicenseInfo(licenseInfo *input,unsigned char *output) {
  int nLength = sizeof(licenseInfo);
  int spaceLength = 16 - (nLength % 16);
  unsigned char *pBuffer = new unsigned char[nLength + spaceLength];
  memset(pBuffer, '\0', nLength + spaceLength);
  memcpy(pBuffer, input, sizeof(licenseInfo));
  Cipher((void *) pBuffer, nLength + spaceLength);
  char *pOut = new char[2 * (nLength + spaceLength)+1];
  memset(pOut, '\0', 2 * (nLength + spaceLength)+1);
  Byte2Hex(pBuffer, nLength + spaceLength, pOut);
  int cpyLen = strlen(pOut);
  strncpy((char *) output, pOut, cpyLen);
  output[cpyLen+1] = '\0';
  delete[] pBuffer;
  pBuffer = nullptr;
  delete[] pOut;
  pOut = nullptr;
  return 0;
  //return (unsigned char *) pOut;
}

unsigned char *AES::Cipher(unsigned char *input) {
  unsigned char state[4][4];
  int i, r, c;

  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      state[r][c] = input[c * 4 + r];
    }
  }

  AddRoundKey(state, w[0]);

  for (i = 1; i <= 10; i++) {
    SubBytes(state);
    ShiftRows(state);
    if (i != 10)MixColumns(state);
    AddRoundKey(state, w[i]);
  }

  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      input[c * 4 + r] = state[r][c];
    }
  }

  return input;
}

unsigned char *AES::InvCipher(unsigned char *input) {
  unsigned char state[4][4];
  int i, r, c;

  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      state[r][c] = input[c * 4 + r];
    }
  }

  AddRoundKey(state, w[10]);
  for (i = 9; i >= 0; i--) {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, w[i]);
    if (i) {
      InvMixColumns(state);
    }
  }

  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      input[c * 4 + r] = state[r][c];
    }
  }

  return input;
}

void *AES::Cipher(void *input, int length) {
  unsigned char *in = (unsigned char *) input;
  int i;
  if (!length) {
    while (*(in + length++));
    in = (unsigned char *) input;
  }
  for (i = 0; i < length; i += 16) {
    Cipher(in + i);
  }
  return input;
}

int AES::DecryptLicenseInfo(unsigned char *input,licenseInfo* out) {
  //int length = sizeof(licenseInfo);
  int nLength = strlen((const char *) input) / 2;
  unsigned char *pBuffer = new unsigned char[nLength];
  memset(pBuffer, '\0', nLength);
  Hex2Byte((const char *) input, strlen((const char *) input), pBuffer);
  licenseInfo * pl = (licenseInfo *) InvCipher(pBuffer, sizeof(licenseInfo));
  memcpy(out,pl,sizeof(licenseInfo));
  delete[] pBuffer;
  pBuffer = nullptr;
  return 0;
}

void *AES::InvCipher(void *input, int length) {
  unsigned char *in = (unsigned char *) input;
  int i;
  for (i = 0; i < length; i += 16) {
    InvCipher(in + i);
  }
  return input;
}

int AES::WriterLicenseFile(const char *first, const char *second, const char *outputFileName) {
  fstream ofs;
  ofs.open(outputFileName);
  if (!ofs) {
    printf("%s Open output file failed!", outputFileName);
    return -1;
  }
  std::string strLine = std::string(first) + std::string(second);
  //ofs.write(strLine.c_str(),strLine.length());
  ofs << strLine;
  ofs.close();
  return 0;
}

int AES::WriterLicenseFile(const char *machineInfo, licenseInfo *input, const char *outputFileName) {
  unsigned char second[1024] = {0};
  EncryptLicenseInfo(input,second);
  //unsigned char *estring = EncryptLicenseInfo(input);
  //strncpy((char *) second, (const char *) estring,strlen((const char *) estring));
  //delete[] estring;
  return WriterLicenseFile(machineInfo, (const char *) second, outputFileName);
}

int AES::ReadLicenseFile(const char *licenseFileName, licenseInfo *pl, char *md5sum) {
  ifstream ifs;
  // Open file:
  ifs.open(licenseFileName, ios::in);
  if (!ifs) {
    printf(" Open input %s file failed!\n", licenseFileName);
    return -1;
  }
// Read config data:

  string strLine;
  while (!ifs.eof()) {
    char temp[1024];
    memset(temp, '\0', 1024);
    ifs.getline(temp, 1024);
    //ifs.read(temp, 1000);
    strLine += temp;
  }
  ifs.close();
  strcpy(md5sum, (char *) strLine.substr(0, 32).c_str());
  string licensestring = strLine.substr(32, strLine.length());
  //std::cout << licensestring << std::endl;
  DecryptLicenseInfo((unsigned char *) licensestring.c_str(), pl);
  //memcpy(pl, DecryptLicenseInfo((unsigned char *) licensestring.c_str()), sizeof(struct licenseInfo));
  return 0;
}

void AES::KeyExpansion(unsigned char *key, unsigned char w[][4][4]) {
  int i, j, r, c;
  unsigned char rc[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      w[0][r][c] = key[r + c * 4];
    }
  }
  for (i = 1; i <= 10; i++) {
    for (j = 0; j < 4; j++) {
      unsigned char t[4];
      for (r = 0; r < 4; r++) {
        t[r] = j ? w[i][r][j - 1] : w[i - 1][r][3];
      }
      if (j == 0) {
        unsigned char temp = t[0];
        for (r = 0; r < 3; r++) {
          t[r] = Sbox[t[(r + 1) % 4]];
        }
        t[3] = Sbox[temp];
        t[0] ^= rc[i - 1];
      }
      for (r = 0; r < 4; r++) {
        w[i][r][j] = w[i - 1][r][j] ^ t[r];
      }
    }
  }
}

unsigned char AES::FFmul(unsigned char a, unsigned char b) {
  unsigned char bw[4];
  unsigned char res = 0;
  int i;
  bw[0] = b;
  for (i = 1; i < 4; i++) {
    bw[i] = bw[i - 1] << 1;
    if (bw[i - 1] & 0x80) {
      bw[i] ^= 0x1b;
    }
  }
  for (i = 0; i < 4; i++) {
    if ((a >> i) & 0x01) {
      res ^= bw[i];
    }
  }
  return res;
}

void AES::SubBytes(unsigned char state[][4]) {
  int r, c;
  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      state[r][c] = Sbox[state[r][c]];
    }
  }
}

void AES::ShiftRows(unsigned char state[][4]) {
  unsigned char t[4];
  int r, c;
  for (r = 1; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      t[c] = state[r][(c + r) % 4];
    }
    for (c = 0; c < 4; c++) {
      state[r][c] = t[c];
    }
  }
}

void AES::MixColumns(unsigned char state[][4]) {
  unsigned char t[4];
  int r, c;
  for (c = 0; c < 4; c++) {
    for (r = 0; r < 4; r++) {
      t[r] = state[r][c];
    }
    for (r = 0; r < 4; r++) {
      state[r][c] = FFmul(0x02, t[r])
                    ^ FFmul(0x03, t[(r + 1) % 4])
                    ^ FFmul(0x01, t[(r + 2) % 4])
                    ^ FFmul(0x01, t[(r + 3) % 4]);
    }
  }
}

void AES::AddRoundKey(unsigned char state[][4], unsigned char k[][4]) {
  int r, c;
  for (c = 0; c < 4; c++) {
    for (r = 0; r < 4; r++) {
      state[r][c] ^= k[r][c];
    }
  }
}

void AES::InvSubBytes(unsigned char state[][4]) {
  int r, c;
  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      state[r][c] = InvSbox[state[r][c]];
    }
  }
}

void AES::InvShiftRows(unsigned char state[][4]) {
  unsigned char t[4];
  int r, c;
  for (r = 1; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      t[c] = state[r][(c - r + 4) % 4];
    }
    for (c = 0; c < 4; c++) {
      state[r][c] = t[c];
    }
  }
}

void AES::InvMixColumns(unsigned char state[][4]) {
  unsigned char t[4];
  int r, c;
  for (c = 0; c < 4; c++) {
    for (r = 0; r < 4; r++) {
      t[r] = state[r][c];
    }
    for (r = 0; r < 4; r++) {
      state[r][c] = FFmul(0x0e, t[r])
                    ^ FFmul(0x0b, t[(r + 1) % 4])
                    ^ FFmul(0x0d, t[(r + 2) % 4])
                    ^ FFmul(0x09, t[(r + 3) % 4]);
    }
  }
}
