#ifndef FORMAL_LANGUAGES_PARSING_LR1_H
#define FORMAL_LANGUAGES_PARSING_LR1_H

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_set>

#include "Grammar.h"

class LRAlgorithm : protected Grammar {
 public:
  using Grammar::Grammar;
  bool BuildLRTable();
  bool IsBelongToGrammar(const std::string& to_find);

 private:
  struct Action;
  struct GrammarRule;
  const char end_symbol_ = '$';
  std::vector<std::map<char, Action>> LR_table_;
  std::vector<std::map<char, int>> action_counter_;
  std::vector<std::map<char, int>> go_to_;
  std::vector<std::set<GrammarRule>> grammar_;
  bool CheckWord(const std::string& to_find);
  void BuildSets();
  std::set<GrammarRule> Closure(std::set<GrammarRule>& grammar);
  std::vector<char> First(char start, const std::string& rule);
  void NextSymbol(char state, std::vector<char>& next_term);
  std::set<GrammarRule> GoTo(char symbol, std::set<GrammarRule>& current);
};

struct LRAlgorithm::Action {
  std::string action;
  int ind = -1;
  char start = '.';
};

struct LRAlgorithm::GrammarRule {
  char left_part = '.';
  std::string rule;
  char next_symbol = '.';
  int point_position = 0;
  bool operator<(const GrammarRule& sec) const;
  bool operator==(const GrammarRule& sec) const;
};

#endif //FORMAL_LANGUAGES_PARSING_LR1_H