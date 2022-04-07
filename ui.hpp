#ifndef _UI_HPP
#define _UI_HPP

#include "json.hpp"
#include "secure_it.hpp"
#include <string>
class UI {
private:
  SecureIt _sec;

public:
  void add_credential(nlohmann::json &input_data);
  void modify_credential(nlohmann::json &input_data, int id);
  void delete_credential(nlohmann::json &input_data, int id);
  void add_vault();
  void modify_vault(nlohmann::json &input_data, std::string vault_name);
  void run();
  void admin_ui();
  void user_ui();
};

#endif // _UI_HPP