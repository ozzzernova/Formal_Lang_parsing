#ifndef FORMAL_LANGUAGES_PARSING_GRAMMAR_H
#define FORMAL_LANGUAGES_PARSING_GRAMMAR_H

#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>
#include <map>

class Grammar {
 public:
  Grammar() = default;
  Grammar(const std::string& nonterminals, const std::string& terminals, char separator, char epsilon, char start, const std::vector<std::string>& rules);

 protected:
  std::map<char, std::vector<std::string>> grammar_rules_;
  std::unordered_set<char> non_terminals_;
  std::unordered_set<char> terminals_;
  char separator_ = '>';
  char epsilon_ = 'e';
  char start_ = 'S';
  void SetNonTerminals(const std::string& non_terminals);
  void SetTerminals(const std::string& terminals);
  void AddRule(const std::string& rule);
  void SetSeparator(char separator);
  void SetEpsilon(char epsilon);
  void SetStartSymbol(char start);
};

#endif //FORMAL_LANGUAGES_PARSING_GRAMMAR_H