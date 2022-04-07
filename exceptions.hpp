#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H
#include <exception>
#include <string>
class Exception : public std::exception {
public:
  std::string reason;

public:
  Exception(std::string msg) { reason = msg; };
  const char *what() { return reason.c_str(); };
};

class NotFound : public Exception {
public:
  NotFound(std::string msg) : Exception(msg){};
};

class NotLoggedIn : public Exception {
public:
  NotLoggedIn(std::string msg) : Exception(msg){};
};

class UserNotInitialized : public Exception {
public:
  UserNotInitialized(std::string msg) : Exception(msg){};
};

#endif // _EXCEPTIONS_H