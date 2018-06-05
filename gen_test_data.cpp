#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "invalid input" << std::endl;
  }

  int n = std::stoi(argv[1]);

  std::ofstream ofs("data.txt");

  for (int i = 0; i < n; i++) {
    std::string str;
    for (int j = 0; j < 3; j++) {
      char c = 'a' + rand() % 26;
      str.push_back(c);
    }
    ofs << str << ',' << 1 << std::endl;
  }

  return 0;
}
