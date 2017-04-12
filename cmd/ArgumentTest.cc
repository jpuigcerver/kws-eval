#include <gtest/gtest.h>

#include "cmd/Argument.h"

using kws::cmd::Argument;

TEST(ArgumentTest, IsBoolean) {
  int arg_int_val = -1;
  Argument<int> arg_int("int", "int help", &arg_int_val);
  EXPECT_FALSE(arg_int.IsBoolean());

  bool arg_bool_val = true;
  Argument<bool> arg_bool("bool", "bool help", &arg_bool_val);
  EXPECT_TRUE(arg_bool.IsBoolean());
}

TEST(ArgumentTest, Help) {
  int arg_int_val = -1;
  Argument<int> arg_int("int", "int help", &arg_int_val);
  EXPECT_EQ("int : int help (type = int32_t)", arg_int.Help());
  EXPECT_EQ("int        : int help (type = int32_t)", arg_int.Help(10));

  bool arg_bool_val = true;
  Argument<bool> arg_bool("bool", "bool help", &arg_bool_val);
  EXPECT_EQ("bool : bool help (type = bool)", arg_bool.Help());
  EXPECT_EQ("bool       : bool help (type = bool)", arg_bool.Help(10));
}

TEST(ArgumentTest, Parse) {
  int arg_int_val = -1;
  Argument<int> arg_int("int", "int help", &arg_int_val);
  EXPECT_TRUE(arg_int.Parse("-1244"));
  EXPECT_EQ(-1244, arg_int_val);
  EXPECT_FALSE(arg_int.Parse("-12.4"));
  EXPECT_FALSE(arg_int.Parse("12 asdsad"));

  bool arg_bool_val = true;
  Argument<bool> arg_bool("bool", "bool help", &arg_bool_val);
  EXPECT_FALSE(arg_bool.Parse(""));     // Boolean arguments CANNOT omit value
  EXPECT_TRUE(arg_bool.Parse("false"));
  EXPECT_EQ(false, arg_bool_val);
  EXPECT_TRUE(arg_bool.Parse("true"));
  EXPECT_EQ(true, arg_bool_val);
  EXPECT_TRUE(arg_bool.Parse("0"));
  EXPECT_EQ(false, arg_bool_val);
  EXPECT_TRUE(arg_bool.Parse("1"));
  EXPECT_EQ(true, arg_bool_val);
  EXPECT_FALSE(arg_bool.Parse("FALSE"));
  EXPECT_FALSE(arg_bool.Parse("TRUE"));
  EXPECT_FALSE(arg_bool.Parse("1234"));
  EXPECT_FALSE(arg_bool.Parse("-inf"));
}