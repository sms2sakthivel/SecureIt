#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <termios.h>
#include <unistd.h>
class Utils {
public:
  static void echo(bool on = true);
  static std::string encrypt(std::string pwd, std::string plain_txt);
  static std::string decrypt(std::string pwd, std::string cipher_txt);
  static std::string sha3_256(std::string pwd);
};

#endif // _UTILS_H