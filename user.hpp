#ifndef _USER_H
#define _USER_H

#include "json.hpp"
#include <string>

class User {
  nlohmann::json _data;
  std::string _cipher_data;
  bool _logged_in = false;

public:
  User(){};
  User(nlohmann::json data) : _data(data){};
  bool is_logged_in() { return _logged_in; }
  void set_data(nlohmann::json data) { _data = data; };
  void set_cipher_data(std::string data) { _cipher_data = data; };
  void set_login_status(bool flag) { _logged_in = flag; };
  nlohmann::json get() { return _data; };

  bool assess_login();
  bool authenticate(std::string pwd);

  std::string get_name();
  std::string get_all_vault_data();
  std::vector<std::string> get_all_vault_names();
  void add(std::string key, nlohmann::json data_);
  void add(std::string key, std::string data_);
  void remove(std::string key);
  std::string get(std::string key);
  nlohmann::json get_vault(std::string name);
};

#endif // _USER_H