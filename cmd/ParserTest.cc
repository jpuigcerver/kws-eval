#include <gtest/gtest.h>

#include "cmd/Parser.h"

using kws::cmd::Parser;

TEST(ParserTest, Constructor) {
  Parser p("prog");
  EXPECT_EQ("Usage: prog\n", p.Help());
}

TEST(ParserTest, RegisterOption) {
  Parser p("prog");
  int opt_int_val = -1;
  bool opt_bool_val = false;
  std::string opt_str_val = "";

  // Option registration may fail if the option name was already taken.
  EXPECT_TRUE(p.RegisterOption("int", "help for int option", &opt_int_val));
  EXPECT_FALSE(p.RegisterOption("int", "this will be ignored", &opt_int_val));
  EXPECT_TRUE(p.RegisterOption("bool", "help for bool option", &opt_bool_val));
  EXPECT_TRUE(p.RegisterOption("str", "help for str option", &opt_str_val));

  // Options are shown in alphabetical order
  EXPECT_EQ("Usage: prog [ --bool ] [ --int ] [ --str ]\n", p.Help());
}

TEST(Parser, RegisterArgument) {
  Parser p("prog");
  int opt_int_val = -1;
  bool opt_bool_val = false;
  std::string opt_str_val = "";

  p.RegisterArgument("int", "help for int option", &opt_int_val);
  p.RegisterArgument("bool", "help for bool option", &opt_bool_val);
  p.RegisterArgument("str", "help for str option", &opt_str_val);

  EXPECT_EQ("Usage: prog int bool str\n", p.Help());
}

TEST(Parser, RegisterOptArgument) {
  Parser p("prog");
  int opt_int_val = -1;
  bool opt_bool_val = false;
  std::string opt_str_val = "";

  p.RegisterOptArgument("int", "help for int option", &opt_int_val);
  p.RegisterOptArgument("bool", "help for bool option", &opt_bool_val);
  p.RegisterOptArgument("str", "help for str option", &opt_str_val);

  EXPECT_EQ("Usage: prog [ int [ bool [ str ] ] ]\n", p.Help());
}