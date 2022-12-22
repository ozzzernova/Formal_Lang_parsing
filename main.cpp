#include <iostream>
#include <fstream>

#include "../include/LR1.h"

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
  LRAlgorithm lr(nonterminal, terminal, separator, epsilon, start, rules);
  bool correct_lr = lr.BuildLRTable();
  if (correct_lr) {
    std::cout << "Enter your word to check if it belongs to the grammar\nWhen you want to finish, enter \"!exit!\"\n";
    std::cin >> word;
    while (word != "!exit!") {
      std::cout << word << (lr.IsBelongToGrammar(word) ? " belongs" : " does not belong") << " to the grammar\n";
      std::cin >> word;
    }
  } else {
    std::cout << "Sorry, your grammar is not LR, but it's nice, don't worry about it\n";
  }
  return 0;
}