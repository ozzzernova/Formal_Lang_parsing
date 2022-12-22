#include "LR1.h"

bool LRAlgorithm::GrammarRule::operator<(const LRAlgorithm::GrammarRule& sec) const {
  if (left_part == sec.left_part) {
    if (rule == sec.rule) {
      if (point_position == sec.point_position) {
        return next_symbol < sec.next_symbol;
      }
      return point_position < sec.point_position;
    }
    return rule < sec.rule;
  }
  return left_part < sec.left_part;
}

bool LRAlgorithm::GrammarRule::operator==(const LRAlgorithm::GrammarRule& sec) const {
  return left_part == sec.left_part && rule == sec.rule && point_position == sec.point_position && next_symbol == sec.next_symbol;
}

bool LRAlgorithm::BuildLRTable() {
  BuildSets();
  LR_table_.resize(grammar_.size());
  go_to_.resize(grammar_.size());
  action_counter_.resize(grammar_.size());
  for (int i = 0; i < LR_table_.size(); ++i) {
    for (auto& symbol : terminals_) {
      LR_table_[i][symbol] = {"reject"};
    }
    LR_table_[i][end_symbol_] = {"reject"};
    for (auto& symbol : non_terminals_) {
      go_to_[i][symbol] = -1;
    }
    for (auto& rule : grammar_[i]) {
      if (rule.point_position == rule.rule.size()) {
        if (rule.left_part == '#') {
          if (LR_table_[i].find(end_symbol_) == LR_table_[i].end() || LR_table_[i][end_symbol_].action != "accept") {
            ++action_counter_[i][end_symbol_];
          }
          LR_table_[i][end_symbol_] = {"accept"};
        } else {
          if (LR_table_[i].find(rule.next_symbol) == LR_table_[i].end() || LR_table_[i][rule.next_symbol].action != "reduce") {
            ++action_counter_[i][rule.next_symbol];
          }
          int ind = -1;
          for (int j = 0 ; j < grammar_rules_[rule.left_part].size(); ++j) {
            if (grammar_rules_[rule.left_part][j] == rule.rule) {
              ind = j;
              break;
            }
          }
          LR_table_[i][rule.next_symbol] = {"reduce", ind, rule.left_part};
        }
      } else {
        char cur_symbol = rule.rule[rule.point_position];
        std::set<GrammarRule> go_to = GoTo(cur_symbol, grammar_[i]);
        int ind = -1;
        for (int j = 0; j < grammar_.size(); ++j) {
          if (go_to == grammar_[j]) {
            ind = j;
            break;
          }
        }
        if (non_terminals_.find(cur_symbol) != non_terminals_.end()) {
          if (ind != -1) {
            go_to_[i][cur_symbol] = ind;
          }
        } else {
          if (ind != -1) {
            if (LR_table_[i].find(cur_symbol) == LR_table_[i].end() || LR_table_[i][cur_symbol].action != "shift") {
              ++action_counter_[i][cur_symbol];
            }
            LR_table_[i][cur_symbol] = {"shift", ind};
          }
        }
      }
    }
  }
  for (int i = 0; i < LR_table_.size(); ++i) {
    if (action_counter_[i][end_symbol_] > 1) {
      return false;
    }
    for (auto& symbol : terminals_) {
      if (action_counter_[i][symbol] > 1) {
        return false;
      }
    }
  }
  return true;
}

void LRAlgorithm::NextSymbol(char state, std::vector<char>& next_term) {
  for (auto& rule : grammar_rules_[state]) {
    if (rule[0] == '.') {
      continue;
    }
    if (non_terminals_.find(rule[0]) != non_terminals_.end()) {
      NextSymbol(rule[0], next_term);
      continue;
    }
    next_term.emplace_back(rule[0]);
  }
}

std::set<LRAlgorithm::GrammarRule> LRAlgorithm::GoTo(char symbol, std::set<GrammarRule>& current) {
  std::set<GrammarRule> new_set;
  for (auto& cur_conf : current) {
    if (cur_conf.point_position == cur_conf.rule.size()) {
      continue;
    }
    if (cur_conf.rule[cur_conf.point_position] == symbol) {
      new_set.emplace(GrammarRule{cur_conf.left_part, cur_conf.rule, cur_conf.next_symbol, cur_conf.point_position + 1});
    }
  }
  return Closure(new_set);
}

std::set<LRAlgorithm::GrammarRule> LRAlgorithm::Closure(std::set<GrammarRule>& grammar) {
  std::set<GrammarRule> new_set = grammar;
  bool change = true;
  while (change) {
    change = false;
    for (auto& conf : new_set) {
      if (conf.point_position == conf.rule.size()) {
        continue;
      }
      char start = conf.rule[conf.point_position];
      if (non_terminals_.find(start) != non_terminals_.end()) {
        std::string sub_rule;
        std::vector<char> next_situation;
        if (conf.point_position + 1 < conf.rule.size()) {
          sub_rule = conf.rule.substr(conf.point_position + 1, conf.rule.size());
        }
        next_situation = First(conf.next_symbol, sub_rule);
        for (auto& rule : grammar_rules_[start]) {
          for (auto& next : next_situation) {
            GrammarRule new_rule = {start, rule};
            if (rule == ".") {
              new_rule.rule = "";
            }
            new_rule.next_symbol = next;
            new_rule.point_position = 0;
            if (new_set.find(new_rule) == new_set.end()) {
              new_set.emplace(new_rule);
              change = true;
            }
          }
        }
      }
    }
  }
  return new_set;
}

std::vector<char> LRAlgorithm::First(char start, const std::string& rule) {
  std::vector<char> first_term;
  if (rule.empty()) {
    first_term.emplace_back(start);
    return first_term;
  }
  if (non_terminals_.find(rule[0]) != non_terminals_.end()) {
    NextSymbol(rule[0], first_term);
    return first_term;
  }
  first_term.emplace_back(rule[0]);
  return first_term;
}

void LRAlgorithm::BuildSets() {
  std::set<GrammarRule> start_set;
  std::string start;
  start += start_;
  start_set.emplace(GrammarRule{'#', start, end_symbol_, 0});
  grammar_.emplace_back(Closure(start_set));
  bool change = true;
  while (change) {
    change = false;
    std::vector<std::set<GrammarRule>> new_states;
    for (auto& current : grammar_) {
      for (auto& term : terminals_) {
        std::set<GrammarRule> next = GoTo(term, current);
        if (!next.empty()) {
          if (std::find(grammar_.begin(), grammar_.end(), next) ==
              grammar_.end()) {
            new_states.emplace_back(next);
            change = true;
          }
        }
      }
      for (auto& nonterm : non_terminals_) {
        std::set<GrammarRule> next = GoTo(nonterm, current);
        if (!next.empty()) {
          if (std::find(grammar_.begin(), grammar_.end(), next) == grammar_.end()) {
            new_states.emplace_back(next);
            change = true;
          }
        }
      }
    }
    for (auto& new_state : new_states) {
      grammar_.emplace_back(new_state);
    }
  }
}

bool LRAlgorithm::IsBelongToGrammar(const std::string& to_find) {
  return CheckWord(to_find);
}

bool LRAlgorithm::CheckWord(const std::string& to_find) {
  std::string str = to_find;
  if (to_find.size() == 1 && to_find[0] == epsilon_) {
    str = "";
  }
  str += end_symbol_;
  std::stack<int> stack;
  stack.push(0);
  int ind = 0;
  while (ind < str.size()) {
    int cur_state = stack.top();
    if (terminals_.find(str[ind]) == terminals_.end()) {
      if (str[ind] != end_symbol_) {
        return false;
      }
    }
    if (LR_table_[cur_state][str[ind]].action == "shift") {
      stack.push(LR_table_[cur_state][str[ind]].ind);
      ++ind;
    } else if (LR_table_[cur_state][str[ind]].action == "reduce") {
      std::string cur_rule_right = grammar_rules_[LR_table_[cur_state][str[ind]].start][LR_table_[cur_state][str[ind]].ind];
      if (cur_rule_right == ".") {
        cur_rule_right = "";
      }
      for (int i = 0; i < cur_rule_right.size(); ++i) {
        stack.pop();
      }
      int s = stack.top();
      if (go_to_[s][LR_table_[cur_state][str[ind]].start] == -1) {
        return false;
      }
      stack.push(go_to_[s][LR_table_[cur_state][str[ind]].start]);
    } else if (LR_table_[cur_state][str[ind]].action == "accept") {
      return true;
    } else if (LR_table_[cur_state][str[ind]].action == "reject") {
      return false;
    }
  }
  return false;
}