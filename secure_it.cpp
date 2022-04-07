#include "secure_it.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "json.hpp"
#include "utils.hpp"
#include <fstream>

bool SecureIt::is_new() {
  std::fstream file(DATA_FILE);
  if (!file) {
    return true;
  } else {
    file.close();
    return false;
  }
}

bool SecureIt::create_user() {
  if (_current_user.get_name().compare("admin") == 0 && _current_user.assess_login()) {
    std::cout << "*********** Create User ***********" << std::endl;
    std::string uName;
    std::string pwd;

    std::cout << USER_NAME_KEY << " : ";
    std::cin >> uName;

    std::cout << PASSWORD_KEY << " : ";
    Utils::echo(false);
    std::cin >> pwd;
    std::cout << std::endl;
    Utils::echo(true);

    nlohmann::json user_data;
    user_data[USER_NAME_KEY] = uName;
    user_data[PASSWORD_KEY] = pwd;
    user_data[DATA_KEY] = {};
    data[uName] = Utils::encrypt(pwd, user_data.dump(4));
    write();
    return true;
  } else {
    return false;
  }
}
bool SecureIt::create_admin() {
  std::cout << "*********** Administrator Configuration ***********" << std::endl;
  std::string pwd;
  std::cout << PASSWORD_KEY << " : ";
  Utils::echo(false);
  std::cin >> pwd;
  std::cout << std::endl;
  Utils::echo(true);
  nlohmann::json user_data;
  user_data[USER_NAME_KEY] = ADMIN_USER;
  user_data[PASSWORD_KEY] = pwd;
  user_data[DATA_KEY] = {};
  data[ADMIN_USER] = Utils::encrypt(pwd, user_data.dump(4));
  std::string dump = data.dump(4);
  std::fstream file(DATA_FILE, std::ios::in | std::ios::out | std::ios::trunc);
  if (file) {
    file.write(dump.c_str(), dump.size());
    file.close();
    return true;
  } else {
    return false;
  }
}

std::vector<std::string> SecureIt::list_users() {
  std::vector<std::string> vec;
  if (_current_user.get_name().compare("admin") == 0 && _current_user.assess_login()) {
    for (auto &[key, value] : data.items())
      vec.push_back(key);
  }
  return vec;
}
bool SecureIt::delete_user(std::string user_name) {
  if (_current_user.get_name().compare("admin") == 0 && _current_user.assess_login() && data.contains(user_name)) {
    data.erase(user_name);
    return true;
  }
  return false;
}

bool SecureIt::login() {
  std::cout << "\n********** Please Login **********" << std::endl;
  std::string uName;
  std::string pwd;
  std::cout << USER_NAME_KEY << " : ";
  std::cin >> uName;
  std::cout << PASSWORD_KEY << " : ";
  Utils::echo(false);
  std::cin >> pwd;
  std::cout << std::endl;
  Utils::echo(true);
  if (data.contains(uName)) {
    _current_user.set_cipher_data(data[uName]);
    return _current_user.authenticate(pwd);
  } else {
    std::cout << "\nLogin Failed, User '" << uName << "' not found...!" << std::endl;
    return false;
  }
  return true;
}

void SecureIt::read() {
  std::fstream file(DATA_FILE);
  if (!file) {
    std::cerr << "Could not open file data.bin." << std::endl;
  }
  std::stringstream ss;
  ss << file.rdbuf();
  file.close();
  std::string file_data = ss.str();
  data = nlohmann::json::parse(file_data);
}

void SecureIt::write() {
  nlohmann::json user_data = _current_user.get();
  data[_current_user.get_name()] = Utils::encrypt(user_data[PASSWORD_KEY], user_data.dump());
  std::fstream file(DATA_FILE, std::ios::in | std::ios::out | std::ios::trunc);
  if (!file) {
    std::cerr << "Could not open file data.bin." << std::endl;
  }
  std::string dump = data.dump(4);
  file.write(dump.c_str(), dump.size());
  file.close();
}