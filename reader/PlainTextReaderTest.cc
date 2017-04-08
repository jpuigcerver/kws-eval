#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/DocumentBoundingBox.h"
#include "core/Event.h"
#include "reader/PlainTextReader.h"

using kws::core::DocumentBoundingBox;
using kws::core::Event;
using kws::reader::PlainTextReader;

using testing::ElementsAre;
using testing::IsEmpty;

// TODO: Refactor these tests & classes to remove dependencies from other
// classes during testing.

TEST(PlainTextReader, Integers) {
  PlainTextReader<int> reader;
  {
    // This is OK
    std::istringstream iss(
      "# this is a comment line\n"
      "# this is also a comment line\n"
      "1 2 3\n"
      "\n");
    std::vector<int> v;
    EXPECT_TRUE(reader.Read(&iss, &v));
    EXPECT_THAT(v, ElementsAre(3, 2, 1));  // Elements are sorted!
  }
  {
    // Comment line in between the text, should fail
    std::istringstream iss(
      "1 2 3\n"
      "# this is also a comment line\n"
      "1 2 3\n"
      "\n");
    std::vector<int> v;
    EXPECT_FALSE(reader.Read(&iss, &v));
  }
}

TEST(PlainTextReader, EventDocumentBoundingBox) {
  PlainTextReader<Event<std::string, DocumentBoundingBox<int>>> reader;
  {
    // File with comments but no events.
    std::istringstream iss(
        "# This is a comment line\n"
        "# This is another comment line\n");
    std::vector<Event<std::string, DocumentBoundingBox<int>>> v;
    EXPECT_TRUE(reader.Read(&iss, &v));
    EXPECT_THAT(v, IsEmpty());
  }
  {
    // Valid case
    std::istringstream iss(
        "q1 d1 1 2 3 4\n"
        "q1 d2 4 3 2 1\n");
    std::vector<Event<std::string, DocumentBoundingBox<int>>> v;
    EXPECT_TRUE(reader.Read(&iss, &v));
    const Event<std::string, DocumentBoundingBox<int>> b1(
        "q1", DocumentBoundingBox<int>("d2", 4, 3, 2, 1));
    const Event<std::string, DocumentBoundingBox<int>> b2(
        "q1", DocumentBoundingBox<int>("d1", 1, 2, 3, 4));
    EXPECT_THAT(v, ElementsAre(b1, b2));
  }
  {
    // Valid case with comments
    std::istringstream iss(
        "# this is a comment\n"
        "q1 d1 1 2 3 4\n"
        "q1 d2 4 3 2 1\n");
    std::vector<Event<std::string, DocumentBoundingBox<int>>> v;
    EXPECT_TRUE(reader.Read(&iss, &v));
    const Event<std::string ,DocumentBoundingBox<int>> b1(
        "q1", DocumentBoundingBox<int>("d2", 4, 3, 2, 1));
    const Event<std::string, DocumentBoundingBox<int>> b2(
        "q1", DocumentBoundingBox<int>("d1", 1, 2, 3, 4));
    EXPECT_THAT(v, ElementsAre(b1, b2));
  }
  {
    // Valid case with comments and windows new lines
    std::istringstream iss(
        "# this is a comment\r\n"
        "q1 d1 1 2 3 4\r\n"
        "q1 d2 4 3 2 1\r\n");
    std::vector<Event<std::string, DocumentBoundingBox<int>>> v;
    EXPECT_TRUE(reader.Read(&iss, &v));
    const Event<std::string, DocumentBoundingBox<int>> b1(
        "q1", DocumentBoundingBox<int>("d2", 4, 3, 2, 1));
    const Event<std::string, DocumentBoundingBox<int>> b2(
        "q1", DocumentBoundingBox<int>("d1", 1, 2, 3, 4));
    EXPECT_THAT(v, ElementsAre(b1, b2));
  }
  {
    // Valid case with comments and events in a single line
    std::istringstream iss(
        "# this is a comment\r\n"
        "q1 d1 1 2 3 4 q1 d2 4 3 2 1 q1 d2 2 2 3 4\r\n");
    std::vector<Event<std::string, DocumentBoundingBox<int>>> v;
    EXPECT_TRUE(reader.Read(&iss, &v));
    const Event<std::string, DocumentBoundingBox<int>> b1(
        "q1", DocumentBoundingBox<int>("d2", 4, 3, 2, 1));
    const Event<std::string, DocumentBoundingBox<int>> b2(
        "q1", DocumentBoundingBox<int>("d2", 2, 2, 3, 4));
    const Event<std::string, DocumentBoundingBox<int>> b3(
        "q1", DocumentBoundingBox<int>("d1", 1, 2, 3, 4));
    EXPECT_THAT(v, ElementsAre(b1, b2, b3));
  }
  {
    // Missing info
    std::istringstream iss(
        "# this is a comment\n"
        "q1 d1 1 2 3 4\n"
        "q2 d1 5 6 7 8\n"
        "q1 d2 9 10 11\n"  // <- Missing h coordinate!
        "q1 d1 13 14 15 16\n");
    std::vector<Event<std::string, DocumentBoundingBox<int>>> v;
    EXPECT_FALSE(reader.Read(&iss, &v));
  }
}
