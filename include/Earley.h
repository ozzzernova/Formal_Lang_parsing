#ifndef FORMAL_LANGUAGES_PARSING_EARLEY_H
#define FORMAL_LANGUAGES_PARSING_EARLEY_H

#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>
#include <map>

#include "../include/Grammar.h"

class Earley : protected Grammar {
 public:
  using Grammar::Grammar;
  bool IsBelongToGrammar(const std::string &word);

 private:
  struct Situation;
  struct SituationComparator;
  std::vector<std::set<Situation, SituationComparator>> situation_layers_;
  void Scan(size_t position, char current_letter);
  void Complete(size_t position, std::set<Situation, SituationComparator> &new_set,
                std::set<Situation, SituationComparator> &old_set);
  void Predict(size_t position, std::set<Situation, SituationComparator>& new_set,
               std::set<Situation, SituationComparator>& old_set);
  bool EarleyAlgorithm(const std::string& input_word);
};

struct Earley::Situation {
  char start = '0';
  std::string rule;
  size_t point_position = 0;
  size_t layer_position = 0;
  size_t len = 0;
  Situation() = default;
  Situation(char start, std::string str, size_t point, size_t layer, char epsilon);
};

struct Earley::SituationComparator {
  bool operator()(const Situation& first, const Situation& second) const;
};

#endif //FORMAL_LANGUAGES_PARSING_EARLEY_H
