#include <iostream>
#include <gtest/gtest.h>
#include "../include/Earley.h"

GTEST_API_ int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(CorrectBracketSequences, StandardSequnces) {
  Earley earley("S", "( ) [ ] { }", '>', 'e', 'S', {"S>e", "S>(S)S", "S>[S]S", "S>{S}S"});
  ASSERT_EQ(true, earley.IsBelongToGrammar("()()"));
  ASSERT_EQ(false, earley.IsBelongToGrammar(")"));
  ASSERT_EQ(true, earley.IsBelongToGrammar("e"));
  ASSERT_EQ(false, earley.IsBelongToGrammar(")()("));
  ASSERT_EQ(false, earley.IsBelongToGrammar("([)]"));
  ASSERT_EQ(true, earley.IsBelongToGrammar("{()[()]}([]())"));
}

TEST(GrammarFromControlWork, FirstGrammar) {
  Earley earley("STU", "bc", '>', 'e', 'S', {"S>T", "S>b", "T>bUcb", "T>e", "U>Sb", "U>b"});
  ASSERT_EQ(true, earley.IsBelongToGrammar("e"));
  ASSERT_EQ(true, earley.IsBelongToGrammar("bbcb"));
  ASSERT_EQ(true, earley.IsBelongToGrammar("bbbbbcbbcbbcb"));
  ASSERT_EQ(false, earley.IsBelongToGrammar("bbbbcb"));
}

TEST(GrammarFromControlWork, SecondGrammar) {
  Earley earley("STU", "abc", '>', 'e', 'S', {"S>aTa", "S>b", "T>bSab", "T>b", "S>Ucb", "S>c", "U>aS", "U>c"});
  ASSERT_EQ(false, earley.IsBelongToGrammar("e"));
  ASSERT_EQ(true, earley.IsBelongToGrammar("ababcbaba"));
  ASSERT_EQ(true, earley.IsBelongToGrammar("accb"));
}