#include "ui.hpp"
#include <iostream>

void UI::admin_ui() {
  int option = 0, vault_idx = 0;
  std::string user_name;
  char y_n;
  while (true) {
    std::cout << "\n\n1. Create user"
              << "\n2. Delete user"
              << "\n3. List user"
              << "\n4. Add vault"
              << "\n5. Modify vault"
              << "\n6. Delete vault"
              << "\n7. List vaults"
              << "\n8. Save"
              << "\n9. Logout"
              << "\n10. Exit" << std::endl;
    std::cout << "\nSelect option : ";
    std::cin >> option;
    std::vector<std::string> vec;
    std::string tmp_str;
    nlohmann::json vault;
    switch (option) {
    case 1:
      _sec.create_user();
      break;
    case 2:
      vec = _sec.list_users();
      for (int i = 0; i < vec.size(); i++)
        tmp_str.append("\t" + std::to_string(i + 1) + ". " + vec[i] + "\n");
      std::cout << tmp_str << std::endl;
      std::cout << "Select User :";
      std::cin >> vault_idx;
      user_name = vec[vault_idx - 1];
      std::cout << "\n Are you sure your want to delete user '" << user_name << "' ? [Y/N]: ";
      y_n = (char)getchar();
      std::cout << y_n << std::endl;
      if (std::tolower(y_n) == 'y') {
        if (_sec.delete_user(user_name))
          std::cout << "User deletion successful...!" << std::endl;
      } else {
        std::cout << "User deletion failed...!" << std::endl;
      }
      break;
    case 3:
      vec = _sec.list_users();
      for (int i = 0; i < vec.size(); i++)
        tmp_str.append("\t" + std::to_string(i + 1) + ". " + vec[i] + "\n");
      std::cout << tmp_str << std::endl;
      break;
    case 4:
      add_vault();
      break;
    case 5:
      vec = _sec.get_current_user().get_all_vault_names();
      for (int i = 0; i < vec.size(); i++)
        tmp_str.append("\t" + std::to_string(i + 1) + ". " + vec[i] + "\n");
      std::cout << tmp_str << std::endl;
      std::cout << "Select valut :";
      std::cin >> vault_idx;
      vault = _sec.get_current_user().get_vault(vec[vault_idx - 1]);
      modify_vault(vault, vec[vault_idx - 1]);
      break;
    case 6:
      vec = _sec.get_current_user().get_all_vault_names();
      for (int i = 0; i < vec.size(); i++)
        tmp_str.append("\t" + std::to_string(i + 1) + ". " + vec[i] + "\n");
      std::cout << tmp_str << std::endl;
      std::cout << "Select valut :";
      std::cin >> vault_idx;
      _sec.get_current_user().remove(vec[vault_idx - 1]);
      break;
    case 7:
      std::cout << _sec.get_current_user().get_all_vault_data() << std::endl;
      break;
    case 8:
      _sec.write();
      break;
    case 9:
      _sec.logout();
      std::cout << "Logged out...!" << std::endl;
      return;
    case 10:
      exit(0);
      break;
    default:
      std::cout << "Invalid Operation Selected" << std::endl;
      std::cin.clear();
      std::cin.ignore(INT8_MAX, '\n');
      break;
    }
  }
}
void UI::user_ui() {
  int option = 0, vault_idx = 0;
  while (true) {
    std::cout << "\n\n1. Add vault"
              << "\n2. Modify vault"
              << "\n3. Delete vault"
              << "\n4. List vaults"
              << "\n5. Save"
              << "\n6. Logout"
              << "\n7. Exit" << std::endl;
    std::cout << "\nSelect option : ";
    std::cin >> option;
    std::vector<std::string> vec;
    std::string tmp_str;
    nlohmann::json vault;
    switch (option) {
    case 1:
      add_vault();
      break;
    case 2:
      vec = _sec.get_current_user().get_all_vault_names();
      for (int i = 0; i < vec.size(); i++)
        tmp_str.append("\t" + std::to_string(i + 1) + ". " + vec[i] + "\n");
      std::cout << tmp_str << std::endl;
      std::cout << "\nSelect valut :";
      std::cin >> vault_idx;
      vault = _sec.get_current_user().get_vault(vec[vault_idx - 1]);
      modify_vault(vault, vec[vault_idx - 1]);
      break;
    case 3:
      vec = _sec.get_current_user().get_all_vault_names();
      for (int i = 0; i < vec.size(); i++)
        tmp_str.append("\t" + std::to_string(i + 1) + ". " + vec[i] + "\n");
      std::cout << tmp_str << std::endl;
      std::cout << "\nSelect valut :";
      std::cin >> vault_idx;
      _sec.get_current_user().remove(vec[vault_idx - 1]);
      break;
    case 4:
      std::cout << _sec.get_current_user().get_all_vault_data() << std::endl;
      break;
    case 5:
      _sec.write();
      break;
    case 6:
      _sec.logout();
      std::cout << "Logged out...!" << std::endl;
      return;
    case 7:
      exit(0);
      break;
    default:
      std::cout << "Invalid Operation Selected" << std::endl;
      std::cin.clear();
      std::cin.ignore(INT8_MAX, '\n');
      break;
    }
  }
}

void UI::run() {

  while (_sec.is_new()) {
    if (!_sec.create_admin()) { // Create Administrator if there is no user.
      std::cout << "Administrator creator failed. Exiting..." << std::endl;
      exit(-1);
    }
  }

  _sec.read();

  while (!_sec.is_logged_in()) {
    _sec.login();
    _sec.read();
  }

  while (true) {
    if (!_sec.is_logged_in()) {
      _sec.login();
      _sec.read();
    } else {
      if (_sec.is_admin()) {
        admin_ui();
      } else {
        user_ui();
      }
    }
  }
}

void UI::add_credential(nlohmann::json &input_data) {
  nlohmann::json cred;
  int option = 0;
  bool continue_loop = true;
  std::string attr_name, attr_value;
  cred["cred_id"] = input_data.size() + 1;
  while (continue_loop) {
    std::cout << "\t1. Add attribute"
              << "\t2. Delete attribute"
              << "\t3. List attributes"
              << "\t4. Done"
              << "\t5. Cancel" << std::endl;
    std::cout << "Select option : ";
    std::cin >> option;
    switch (option) {
    case 1:
      attr_name.clear();
      attr_value.clear();
      std::cout << "Attribute Name : ";
      std::cin >> attr_name;
      std::cout << "Attribute Value : ";
      std::cin >> attr_value;
      cred[attr_name] = attr_value;
      break;
    case 2:
      attr_name.clear();
      std::cout << "Attribute Name : ";
      std::cin >> attr_name;
      cred.erase(attr_name);
      break;
    case 3:
      std::cout << cred.dump(4) << std::endl;
      break;
    case 4:
      input_data.push_back(cred);
      return;
    case 5:
      return;
    default:
      std::cout << "Invalid option Selected...!" << std::endl;
      std::cin.clear();
      std::cin.ignore(INT8_MAX, '\n');
      break;
    }
  }
}
void UI::modify_credential(nlohmann::json &input_data, int id) {
  nlohmann::json cred = input_data[id - 1];
  int option = 0;
  bool continue_loop = true;
  std::string attr_name, attr_value;
  while (continue_loop) {
    std::cout << "\t1. Add attribute"
              << "\t2. Delete attribute"
              << "\t3. List attributes"
              << "\t4. Done"
              << "\t5. Cancel" << std::endl;
    std::cout << "\nSelect option : ";
    std::cin >> option;
    switch (option) {
    case 1:
      attr_name.clear();
      attr_value.clear();
      std::cout << "Attribute Name : ";
      std::cin >> attr_name;
      std::cout << "Attribute Value : ";
      std::cin >> attr_value;
      cred[attr_name] = attr_value;
      break;
    case 2:
      attr_name.clear();
      std::cout << "Attribute Name : ";
      std::cin >> attr_name;
      cred.erase(attr_name);
      break;
    case 3:
      std::cout << cred.dump(4) << std::endl;
      break;
    case 4:
      input_data[id - 1] = cred;
      return;
    case 5:
      return;
    default:
      std::cout << "Invalid option Selected...!" << std::endl;
      std::cin.clear();
      std::cin.ignore(INT8_MAX, '\n');
      break;
    }
  }
}

void UI::delete_credential(nlohmann::json &input_data, int cred_id) {
  bool found = false;
  for (int i = 0; i < input_data.size(); i++) {
    if (found) {
      input_data[i]["cred_id"] = i;
    }
    if (input_data[i]["cred_id"] == cred_id) {
      input_data.erase(i);
      found = true;
    }
  }
}

void UI::add_vault() {
  nlohmann::json credentials = nlohmann::json::array();
  nlohmann::json cred;
  int option = 0;
  std::string vault_name;
  std::cout << "Enter Vault Name : ";
  std::cin >> vault_name;
  int id = 0;

  while (true) {
    std::cout << "\n\n1. Add Credential"
              << "\n2. Modify Credential"
              << "\n3. Delete Credential"
              << "\n4. List Credential"
              << "\n5. Done"
              << "\n6. Cancel" << std::endl;
    std::cout << "\nSelect option : ";
    std::cin >> option;
    switch (option) {
    case 1:
      add_credential(credentials);
      break;
    case 2:
      std::cout << credentials.dump(4) << std::endl;
      std::cout << "\nSelect Credential id: ";
      std::cin >> id;
      modify_credential(credentials, id);
      break;
    case 3:
      std::cout << credentials.dump(4) << std::endl;
      std::cout << "\nSelect Credential id: ";
      std::cin >> id;
      delete_credential(credentials, id);
      break;
    case 4:
      std::cout << credentials.dump(4) << std::endl;
      break;
    case 5:
      _sec.get_current_user().add(vault_name, credentials);
      return;
    case 6:
      return;
    default:
      std::cout << "Invalid option Selected...!" << std::endl;
      std::cin.clear();
      std::cin.ignore(INT8_MAX, '\n');
      break;
    }
  }
}
void UI::modify_vault(nlohmann::json &input_data, std::string vault_name) {
  nlohmann::json cred;
  int option = 0;
  int id = 0;

  while (true) {
    std::cout << "\n\n1. Add Credential"
              << "\n2. Modify Credential"
              << "\n3. Delete Credential"
              << "\n4. List Credential"
              << "\n5. Done"
              << "\n6. Cancel" << std::endl;
    std::cout << "\nSelect option : ";
    std::cin >> option;
    switch (option) {
    case 1:
      add_credential(input_data);
      break;
    case 2:
      std::cout << input_data.dump(4) << std::endl;
      std::cout << "\nSelect Credential id: ";
      std::cin >> id;
      modify_credential(input_data, id);
      break;
    case 3:
      std::cout << input_data.dump(4) << std::endl;
      std::cout << "\nSelect Credential id: ";
      std::cin >> id;
      delete_credential(input_data, id);
      break;
    case 4:
      std::cout << input_data.dump(4) << std::endl;
      break;
    case 5:
      _sec.get_current_user().add(vault_name, input_data);
      return;
    case 6:
      return;
    default:
      std::cout << "Invalid option Selected...!" << std::endl;
      std::cin.clear();
      std::cin.ignore(INT8_MAX, '\n');
      break;
    }
  }
}
