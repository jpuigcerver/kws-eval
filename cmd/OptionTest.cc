#include <gtest/gtest.h>

#include "cmd/Option.h"

using kws::cmd::Option;

TEST(OptionTest, IsBoolean) {
  int opt_int_val = -1;
  Option<int> opt_int("--int", "int help", &opt_int_val);
  EXPECT_FALSE(opt_int.IsBoolean());

  bool opt_bool_val = true;
  Option<bool> opt_bool("--bool", "bool help", &opt_bool_val);
  EXPECT_TRUE(opt_bool.IsBoolean());
}

TEST(OptionTest, Help) {
  int opt_int_val = -1;
  Option<int> opt_int("--int", "int help", &opt_int_val);
  EXPECT_EQ("--int : int help (type = int32_t, value = \"-1\")",
            opt_int.Help());
  EXPECT_EQ("--int      : int help (type = int32_t, value = \"-1\")",
            opt_int.Help(10));

  bool opt_bool_val = true;
  Option<bool> opt_bool("--bool", "bool help", &opt_bool_val);
  EXPECT_EQ("--bool : bool help (type = bool, value = \"true\")",
            opt_bool.Help());
  EXPECT_EQ("--bool     : bool help (type = bool, value = \"true\")",
            opt_bool.Help(10));
}

TEST(OptionTest, Parse) {
  int opt_int_val = -1;
  Option<int> opt_int("--int", "int help", &opt_int_val);
  EXPECT_TRUE(opt_int.Parse("-1244"));
  EXPECT_EQ(-1244, opt_int_val);
  EXPECT_FALSE(opt_int.Parse("-12.4"));
  EXPECT_FALSE(opt_int.Parse("12 asdsad"));

  bool opt_bool_val = true;
  Option<bool> opt_bool("--bool", "bool help", &opt_bool_val);
  EXPECT_TRUE(opt_bool.Parse(""));   // Boolean options can omit value
  EXPECT_TRUE(opt_bool_val);
  EXPECT_TRUE(opt_bool.Parse("false"));
  EXPECT_FALSE(opt_bool_val);
  EXPECT_TRUE(opt_bool.Parse("true"));
  EXPECT_TRUE(opt_bool_val);
  EXPECT_TRUE(opt_bool.Parse("0"));
  EXPECT_FALSE(opt_bool_val);
  EXPECT_TRUE(opt_bool.Parse("1"));
  EXPECT_TRUE(opt_bool_val);
  EXPECT_FALSE(opt_bool.Parse("FALSE"));
  EXPECT_FALSE(opt_bool.Parse("TRUE"));
  EXPECT_FALSE(opt_bool.Parse("1234"));
  EXPECT_FALSE(opt_bool.Parse("-inf"));
}