#ifndef _SECUREIT_H
#define _SECUREIT_H

#include "json.hpp"
#include "user.hpp"
#include <iostream>
#include <string>
#include <vector>

class SecureIt {
  nlohmann::json data;
  User _current_user;
  bool _is_exit = false;

public:
  bool is_logged_in() { return _current_user.is_logged_in(); }

  void add(std::string key, std::map<std::string, std::string> data_) { _current_user.add(key, data_); };
  void add(std::string key, std::string data_) { _current_user.add(key, data_); };
  std::string list() { return _current_user.get_all_vault_data(); };
  std::string get(std::string key) { return _current_user.get(key); };
  void remove(std::string key) { _current_user.remove(key); };
  User &get_current_user() { return _current_user; };
  bool is_new();
  bool is_admin() { return _current_user.get_name().compare("admin") == 0; };
  bool create_user();
  bool create_admin();
  bool delete_user(std::string username);
  std::vector<std::string> list_users();
  bool login();
  void logout() { _current_user.set_login_status(false); };
  void read();
  void write();
};

#endif // _SECUREIT_H