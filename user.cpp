#include "user.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "utils.hpp"

#include <iostream>

bool User::assess_login() {
  if (_logged_in)
    return true;
  else
    throw NotLoggedIn("Not logged in, Please Login...!");
}

std::string User::get_name() {
  if (_data.size() != 0) {
    if (_data.contains(USER_NAME_KEY))
      return _data[USER_NAME_KEY];
    else
      throw NotFound("Username not found");
  } else
    throw UserNotInitialized("User not initialized");
}

bool User::authenticate(std::string pwd) {
  set_login_status(false);

  try {
    _data = nlohmann::json::parse(Utils::decrypt(pwd, _cipher_data));
  } catch (std::exception &e) {
    std::cout << "\nLogin Failed, Incorrect Password...!" << std::endl;
    return false;
  }

  if (_data.contains(PASSWORD_KEY)) {
    if (pwd.compare(_data[PASSWORD_KEY]) == 0) {
      set_login_status(true);
      std::cout << "\nLogin Success...!" << std::endl;
      return true;
    } else {
      set_login_status(false);
      std::cout << "\nLogin Failed, Incorrect Password...!" << std::endl;
      return false;
    }
  } else {
    std::cout << "\nLogin Failed, Incorrect Password...!" << std::endl;
    return false;
  }
}

void User::add(std::string key, nlohmann::json data_) {
  if (assess_login() && _data.contains(DATA_KEY)) {
    _data[DATA_KEY][key] = data_;
  } else {
    _data[DATA_KEY] = {};
    _data[DATA_KEY][key] = data_;
  }
}

void User::add(std::string key, std::string data_) {
  if (assess_login() && _data.contains(DATA_KEY)) {
    _data[DATA_KEY][key] = data_;
  } else {
    _data[DATA_KEY] = {};
    _data[DATA_KEY][key] = data_;
  }
}

void User::remove(std::string key) {
  if (assess_login() && _data.contains(DATA_KEY)) {
    _data[DATA_KEY].erase(key);
  }
}

std::string User::get(std::string key) {
  if (assess_login() && _data.contains(DATA_KEY) && _data[DATA_KEY].contains(key)) {
    return _data[DATA_KEY][key].dump(4);
  } else {
    return std::string("");
  }
}

std::string User::get_all_vault_data() {
  if (assess_login() && _data.contains(DATA_KEY)) {
    return _data[DATA_KEY].dump(4);
  }
  return std::string("");
}

std::vector<std::string> User::get_all_vault_names() {
  std::vector<std::string> vec;
  if (assess_login() && _data.contains(DATA_KEY)) {
    for (auto &[key, value] : _data[DATA_KEY].items())
      vec.push_back(key);
  }
  return vec;
}

nlohmann::json User::get_vault(std::string name) {
  if (assess_login() && _data.contains(DATA_KEY) && _data[DATA_KEY].contains(name)) {
    return _data[DATA_KEY][name];
  } else {
    return {};
  }
}