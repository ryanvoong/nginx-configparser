#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

TEST(NginxConfigTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ(statement.ToString(0), "foo bar;\n");
}

class NginxStringConfigTest : public ::testing::Test {
protected:
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {  
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statement";
  EXPECT_EQ("foo", out_config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
  EXPECT_TRUE(ParseString(""));
}

TEST_F(NginxStringConfigTest, InvalidConfig) {
  EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("server { listen 80; }"));
}

TEST_F(NginxStringConfigTest, EmptyNestedConfig) {
  EXPECT_TRUE(ParseString("server {}"));
}

TEST_F(NginxStringConfigTest, UnbalancedNestedConfig) {
  EXPECT_FALSE(ParseString("server { listen 80;"));
}

TEST_F(NginxStringConfigTest, DoublyNestedConfig) {
  EXPECT_TRUE(ParseString("server { port { listen 80; } }"));
}

TEST_F(NginxStringConfigTest, MultipleDoublyNestedConfig) {
  EXPECT_TRUE(ParseString("server { port { listen 80; } foo { bar; } }"));
}
