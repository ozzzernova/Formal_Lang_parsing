#include "Grammar.h"

Grammar::Grammar(const std::string& nonterminals, const std::string& terminals, char separator, char epsilon, char start, const std::vector<std::string>& rules) {
  SetNonTerminals(nonterminals);
  SetTerminals(terminals);
  SetSeparator(separator);
  SetEpsilon(epsilon);
  SetStartSymbol(start);
  for (auto& rule : rules) {
    AddRule(rule);
  }
};

void Grammar::SetNonTerminals(const std::string& non_terminals) {
  for (auto& symbol : non_terminals) {
    if (symbol != ' ') {
      non_terminals_.emplace(symbol);
    }
  }
}

void Grammar::SetTerminals(const std::string &terminals) {
  for (auto& symbol : terminals) {
    if (symbol != ' ') {
      terminals_.emplace(symbol);
    }
  }
}

void Grammar::AddRule(const std::string& rule) {
  grammar_rules_[rule[0]].emplace_back(rule.begin() + 2, rule.end());
}

void Grammar::SetSeparator(char separator) {
  separator_ = separator;
}

void Grammar::SetEpsilon(char epsilon) {
  epsilon_ = epsilon;
  terminals_.emplace(epsilon_);
}

void Grammar::SetStartSymbol(char start) {
  start_ = start;
  non_terminals_.emplace(start);
}