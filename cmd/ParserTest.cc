#include <gtest/gtest.h>

#include "cmd/Parser.h"

using kws::cmd::Parser;

TEST(ParserTest, Constructor) {
  Parser p("prog");
  EXPECT_EQ("Usage: prog\n", p.Help());

  Parser p2("prog2", "  This is the description of\n  this program.");
  EXPECT_EQ(
      "Usage: prog2\n\n"
      "Description:\n"
      "  This is the description of\n"
      "  this program.\n", p2.Help());

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
  EXPECT_EQ("Usage: prog [ options ]\n\n"
            "Options:\n"
            "  --bool : help for bool option (type = bool, value = \"false\")\n"
            "  --int  : help for int option (type = int32_t, value = \"-1\")\n"
            "  --str  : help for str option (type = string, value = \"\")\n",
            p.Help());
}

TEST(Parser, RegisterArgument) {
  Parser p("prog");
  int opt_int_val = -1;
  bool opt_bool_val = false;
  std::string opt_str_val = "";

  p.RegisterArgument("int", "help for int option", &opt_int_val);
  p.RegisterArgument("bool", "help for bool option", &opt_bool_val);
  p.RegisterArgument("str", "help for str option", &opt_str_val);

  // Arguments description is given in order.
  EXPECT_EQ(
      "Usage: prog int bool str\n\n"
      "Arguments:\n"
      "  int  : help for int option (type = int32_t)\n"
      "  bool : help for bool option (type = bool)\n"
      "  str  : help for str option (type = string)\n", p.Help());
}

TEST(Parser, RegisterOptArgument) {
  Parser p("prog");
  int opt_int_val = -1;
  bool opt_bool_val = false;
  std::string opt_str_val = "";

  p.RegisterOptArgument("int", "help for int option", &opt_int_val);
  p.RegisterOptArgument("bool", "help for bool option", &opt_bool_val);
  p.RegisterOptArgument("str", "help for str option", &opt_str_val);

  EXPECT_EQ(
      "Usage: prog [ int [ bool [ str ] ] ]\n\n"
          "Arguments:\n"
          "  int  : help for int option (type = int32_t)\n"
          "  bool : help for bool option (type = bool)\n"
          "  str  : help for str option (type = string)\n", p.Help());
}

TEST(Parser, ParseWithNoOptions) {
  // No options given, OK.
  int opt_int_val = -1;
  bool opt_bool_val = false;

  Parser p("prog");
  EXPECT_TRUE(p.RegisterOption("bool", "", &opt_bool_val));
  EXPECT_TRUE(p.RegisterOption("int", "", &opt_int_val));
  EXPECT_TRUE(p.Parse(1, nullptr));
  EXPECT_EQ(-1, opt_int_val);
  EXPECT_EQ(false, opt_bool_val);
}

TEST(Parser, ParseOptionBool) {
  // Bool option with no explicit value.
  {
    const char* cmdline[] = {
        "prog",
        "--bool",
    };
    bool opt_bool_val = false;
    Parser p(cmdline[0]);
    p.RegisterOption("bool", "", &opt_bool_val);
    EXPECT_TRUE(p.Parse(2, cmdline));
    EXPECT_EQ(true, opt_bool_val);
  }
  // Bool option with explicit value (true).
  {
    const char* cmdline[] = {
        "prog",
        "--bool", "true",
    };
    bool opt_bool_val = false;
    Parser p(cmdline[0]);
    p.RegisterOption("bool", "", &opt_bool_val);
    EXPECT_TRUE(p.Parse(3, cmdline));
    EXPECT_EQ(true, opt_bool_val);
  }
  // Bool option with explicit value (false).
  {
    const char* cmdline[] = {
        "prog",
        "--bool", "false",
    };
    bool opt_bool_val = true;
    Parser p(cmdline[0]);
    p.RegisterOption("bool", "", &opt_bool_val);
    EXPECT_TRUE(p.Parse(3, cmdline));
    EXPECT_EQ(false, opt_bool_val);
  }
  // Bool option with wrong value.
  {
    const char* cmdline[] = {
        "prog",
        "--bool", "12314",
    };
    bool opt_bool_val = false;
    Parser p(cmdline[0]);
    p.RegisterOption("bool", "", &opt_bool_val);
    ::testing::internal::CaptureStderr();
    EXPECT_FALSE(p.Parse(3, cmdline));
    EXPECT_EQ("ERROR: Invalid value for option '--bool'\n",
              ::testing::internal::GetCapturedStderr());
  }
}

TEST(Parser, ParseOptionString) {
  std::string str1 = "", str2 = "";
  Parser p("prog");
  EXPECT_TRUE(p.RegisterOption("str1", "", &str1));
  EXPECT_TRUE(p.RegisterOption("str2", "", &str2));
  {
    const char* cmdline[] = {
        "prog",
        "--str1", "this value",
        "--str2", "  whitespaces are preserved ",
    };
    EXPECT_TRUE(p.Parse(5, cmdline));
    EXPECT_EQ("this value", str1);
    EXPECT_EQ("  whitespaces are preserved ", str2);
  }
  {
    // This parsing should fail, since values for --str2 is missing
    const char* cmdline[] = {
        "prog",
        "--str1", "this value",
        "--str2",
    };
    ::testing::internal::CaptureStderr();
    EXPECT_FALSE(p.Parse(4, cmdline));
    EXPECT_EQ("ERROR: Invalid value for option '--str2'\n",
              ::testing::internal::GetCapturedStderr());
  }
}
