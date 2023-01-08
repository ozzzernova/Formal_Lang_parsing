#include "../include/Earley.h"
#include <algorithm>

Earley::Situation::Situation(char start, std::string str, size_t point, size_t layer, char epsilon)
  : start(start)
  , rule(std::move(str))
  , point_position(point)
  , layer_position(layer) {
  if (rule.size() == 1 && rule[0] == epsilon) {
    len = 0;
  } else {
    len = rule.size();
  }
}

bool Earley::SituationComparator::operator()(const Situation& first, const Situation& second) const {
  if (first.start == second.start) {
    if (first.layer_position == second.layer_position) {
      if (first.point_position == second.point_position) {
        return first.rule < second.rule;
      }
      return first.point_position < second.point_position;
    }
    return first.layer_position < second.layer_position;
  }
  return first.start < second.start;
}

bool Earley::IsBelongToGrammar(const std::string &word) {
  return EarleyAlgorithm(word);
}

bool Earley::EarleyAlgorithm(const std::string& input_word) {
  situation_layers_.clear();
  situation_layers_.resize(input_word.size() + 1);
  std::string begin("0");
  begin[0] = start_;
  situation_layers_[0].emplace('#', begin, 0, 0, epsilon_);
  char current_letter = '0';
  for (size_t i = 0; i <= input_word.size(); ++i) {
    if (i != 0) { current_letter = input_word[i - 1]; }
    Scan(i, current_letter);
    bool change = true;
    auto old_situations = situation_layers_[i];
    std::set<Situation, SituationComparator> new_situations;
    while (change) {
      Complete(i, new_situations, old_situations);
      Predict(i, new_situations, old_situations);
      change = !new_situations.empty();
      old_situations = std::move(new_situations);
    }
  }
  if (input_word.size() == 1 && input_word[0] == epsilon_) {
     return (situation_layers_[0].find(Situation('#', begin, 1, 0, epsilon_)) != situation_layers_[0].end());
  }
  return (situation_layers_[input_word.size()].find(Situation('#', begin, 1, 0, epsilon_)) != situation_layers_[input_word.size()].end());
}

void Earley::Scan(size_t position, char current_letter) {
  if (position == 0) {
    return;
  }
  for (auto& situation : situation_layers_[position - 1]) {
    if (situation.rule[situation.point_position] == current_letter) {
      situation_layers_[position].emplace(situation.start, situation.rule, situation.point_position + 1, situation.layer_position, epsilon_);
    }
  }
}

void Earley::Complete(size_t position, std::set<Situation, SituationComparator>& new_situations,
                      std::set<Situation, SituationComparator>& old_situations) {
  for (auto& situation : old_situations) {
    if (situation.point_position == situation.len) {
      for (auto& prev_situation : situation_layers_[situation.layer_position]) {
        if (situation.start == prev_situation.rule[prev_situation.point_position]) {
          if (situation_layers_[position].find(Situation(prev_situation.start, prev_situation.rule, prev_situation.point_position + 1, prev_situation.layer_position, epsilon_)) == situation_layers_[position].end()) {
            new_situations.emplace(prev_situation.start, prev_situation.rule, prev_situation.point_position + 1, prev_situation.layer_position, epsilon_);
          }
          situation_layers_[position].emplace(prev_situation.start, prev_situation.rule, prev_situation.point_position + 1, prev_situation.layer_position, epsilon_);
        }
      }
    }
  }
}

void Earley::Predict(size_t position, std::set<Situation, SituationComparator>& new_situations,
                     std::set<Situation, SituationComparator>& old_situations) {
  for (auto& situation : old_situations) {
    if (non_terminals_.find(situation.rule[situation.point_position]) != non_terminals_.end()) {
      for (auto& rule : grammar_rules_[situation.rule[situation.point_position]]) {
        if (situation_layers_[position].find(Situation(situation.rule[situation.point_position], rule, 0, position, epsilon_)) == situation_layers_[position].end()) {
          new_situations.emplace(situation.rule[situation.point_position], rule, 0, position, epsilon_);
        }
        situation_layers_[position].emplace(situation.rule[situation.point_position], rule, 0, position, epsilon_);
      }
    }
  }
}
