#include <iostream>
#include <fstream>

#include "../include/Earley.h"

int main() {
  std::cout << "Enter name of file with grammar\n";
  std::fstream file_desc;
  std::string file;
  std::cin >> file;
  file_desc.open("../grammar/" + file);
  std::string nonterminal, terminal, rule, word;
  std::vector<std::string> rules;
  char separator, epsilon, start;
  std::getline(file_desc, nonterminal);
  std::getline(file_desc, terminal);
  file_desc >> separator >> epsilon >> start;
  while (file_desc >> rule) {
    rules.emplace_back(rule);
  }
  file_desc.close();
  Earley earley(nonterminal, terminal, separator, epsilon, start, rules);
  std::cout << "Enter your word to check if it belongs to the grammar\nWhen you want to finish, enter \"!exit!\"\n";
  std::cin >> word;
  while (word != "!exit!") {
    std::cout << word << (earley.IsBelongToGrammar(word) ? " belongs" : " does not belong") << " to the grammar\n";
    std::cin >> word;
  }
  return 0;
}
