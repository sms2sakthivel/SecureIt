#include "utils.hpp"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha3.h>
#include <iostream>

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void Utils::echo(bool on) {
  struct termios settings;
  tcgetattr(STDIN_FILENO, &settings);
  settings.c_lflag = on ? (settings.c_lflag | ECHO) : (settings.c_lflag & ~(ECHO));
  tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

std::string Utils::encrypt(std::string pwd, std::string plain_txt) {
  std::string digest = sha3_256(pwd), cipher;
  CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
  memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

  for (int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++) {
    key[i] = digest[i];
    iv[i] = digest[i + CryptoPP::AES::DEFAULT_KEYLENGTH];
  }

  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;
  e.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv, CryptoPP::AES::BLOCKSIZE);

  CryptoPP::StringSource s(
      plain_txt, true,
      new CryptoPP::StreamTransformationFilter(e,
                                               new CryptoPP::StringSink(cipher)) // StreamTransformationFilter
  );                                                                             // StringSource
  // Convert from Bytes to String
  size_t size = cipher.size();
  std::string ret_str;
  for (int i = 0; i < size; ++i) {
    ret_str.append(1, hexmap[(cipher[i] & 0xF0) >> 4]);
    ret_str.append(1, hexmap[cipher[i] & 0x0F]);
  }
  return ret_str;
}

std::string Utils::decrypt(std::string pwd, std::string cipher_txt) {
  std::string digest = sha3_256(pwd), plain_txt;
  CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
  memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
  memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

  // SHA3-256 Hash Total Length 256 bits. Assign it to key and IV. Key = 1-128 bits, IV = 129-156 bits
  for (int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++) {
    key[i] = digest[i];
    iv[i] = digest[i + CryptoPP::AES::DEFAULT_KEYLENGTH];
  }

  size_t cipher_size = cipher_txt.size() / 2 + cipher_txt.size() % 2;
  CryptoPP::byte cipher[cipher_size];

  // Convert from String to Bytes
  for (unsigned int i = 0, j = 0; i < cipher_txt.length(); i += 2, j += 1) {
    std::string byteString = cipher_txt.substr(i, 2);
    CryptoPP::byte byte = (CryptoPP::byte)strtol(byteString.c_str(), NULL, 16);
    cipher[j] = byte;
  }
  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d;
  d.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv, CryptoPP::AES::BLOCKSIZE);

  CryptoPP::StringSource s(
      cipher, cipher_size, true,
      new CryptoPP::StreamTransformationFilter(d,
                                               new CryptoPP::StringSink(plain_txt)) // StreamTransformationFilter
  );                                                                                // StringSource
  return plain_txt;
}
std::string Utils::sha3_256(std::string pwd) {
  CryptoPP::SHA3_256 hash;
  std::string digest;
  CryptoPP::StringSource(pwd, true, new CryptoPP::HashFilter(hash, new CryptoPP::StringSink(digest)));
  return digest;
}