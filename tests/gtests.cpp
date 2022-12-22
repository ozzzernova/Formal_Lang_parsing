#include <iostream>
#include <gtest/gtest.h>
#include "../include/LR1.h"

GTEST_API_ int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(CorrectBracketSequences, StandardSequnces) {
  LRAlgorithm lr("S", "( ) [ ] { }", '>', 'e', 'S', {"S>e", "S>(S)S", "S>[S]S", "S>{S}S"});
  ASSERT_EQ(true, lr.BuildLRTable());
  ASSERT_EQ(true, lr.IsBelongToGrammar("()()"));
  ASSERT_EQ(false, lr.IsBelongToGrammar(")"));
  ASSERT_EQ(true, lr.IsBelongToGrammar("e"));
  ASSERT_EQ(false, lr.IsBelongToGrammar(")()("));
  ASSERT_EQ(false, lr.IsBelongToGrammar("([)]"));
  ASSERT_EQ(true, lr.IsBelongToGrammar("{()[()]}([]())"));
}

TEST(GrammarFromControlWork, FirstGrammar) {
  LRAlgorithm lr("STU", "bc", '>', 'e', 'S', {"S>T", "S>b", "T>bUcb", "T>e", "U>Sb", "U>b"});
  ASSERT_EQ(false, lr.BuildLRTable());
}

TEST(GrammarFromControlWork, SecondGrammar) {
  LRAlgorithm lr("STU", "abc", '>', 'e', 'S', {"S>aTa", "S>b", "T>bSab", "T>b", "S>Ucb", "S>c", "U>aS", "U>c"});
  ASSERT_EQ(false, lr.BuildLRTable());
}

TEST(ITMO_CASE, LRGrammar) {
  LRAlgorithm lr("SC", "cd", '>', 'e', 'S', {"S>CC", "C>cC", "C>d"});
  ASSERT_EQ(true, lr.BuildLRTable());
  ASSERT_EQ(true, lr.IsBelongToGrammar("cccdccccccd"));
  ASSERT_EQ(false, lr.IsBelongToGrammar("e"));
  ASSERT_EQ(true, lr.IsBelongToGrammar("cdd"));
}