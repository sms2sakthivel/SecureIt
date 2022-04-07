#include "ui.hpp"
#include <iostream>

int main(int argc, char **argv) {
  while (true) {
    UI ui;
    try {
      ui.run();
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}