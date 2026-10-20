#include "../SimpleIni.h"
#include "gtest/gtest.h"
#include <algorithm>

class TestQuotes : public ::testing::Test {
protected:
  void SetUp() override;

protected:
  CSimpleIniA ini;
  std::string input;
  std::string expect;
  std::string output;
};

void TestQuotes::SetUp() { ini.SetUnicode(); }

TEST_F(TestQuotes, TestEmpty) {
  ini.SetQuotes(true);

  input = "[section]\n"
          "key1 = \"\"\n"
          "key2 = \n";

  // no need to preserve quotes for empty data
  expect = "[section]\n"
           "key1 = \n"
           "key2 = \n";

  const char *result;
  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  result = ini.GetValue("section", "key1");
  ASSERT_STREQ(result, "");

  result = ini.GetValue("section", "key2");
  ASSERT_STREQ(result, "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);

  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

TEST_F(TestQuotes, TestEmptyDisabled) {
  ini.SetQuotes(false);

  input = "[section]\n"
          "key1 = \"\"\n"
          "key2 = \n";

  const char *result;
  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  result = ini.GetValue("section", "key1");
  ASSERT_STREQ(result, "\"\"");

  result = ini.GetValue("section", "key2");
  ASSERT_STREQ(result, "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);

  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(input.c_str(), output.c_str());
}

TEST_F(TestQuotes, TestGeneral) {
  ini.SetQuotes(true);

  input = "[section]\n"
          "key1 = foo\n"
          "key2 = \"foo\"\n"
          "key3 =  foo \n"
          "key4 = \" foo \"\n"
          "key5 = \"foo\n"
          "key6 = foo\"\n"
          "key7 =  foo \" foo \n"
          "key8 =  \" foo \" foo \" \n";

  expect = "[section]\n"
           "key1 = foo\n"
           "key2 = foo\n"
           "key3 = foo\n"
           "key4 = \" foo \"\n"
           "key5 = \"foo\n"
           "key6 = foo\"\n"
           "key7 = foo \" foo\n"
           "key8 = \" foo \" foo \"\n";

  const char *result;
  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  result = ini.GetValue("section", "key1");
  ASSERT_STREQ(result, "foo");

  result = ini.GetValue("section", "key2");
  ASSERT_STREQ(result, "foo");

  result = ini.GetValue("section", "key3");
  ASSERT_STREQ(result, "foo");

  result = ini.GetValue("section", "key4");
  ASSERT_STREQ(result, " foo ");

  result = ini.GetValue("section", "key5");
  ASSERT_STREQ(result, "\"foo");

  result = ini.GetValue("section", "key6");
  ASSERT_STREQ(result, "foo\"");

  result = ini.GetValue("section", "key7");
  ASSERT_STREQ(result, "foo \" foo");

  result = ini.GetValue("section", "key8");
  ASSERT_STREQ(result, " foo \" foo ");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);

  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

TEST_F(TestQuotes, TestGeneralDisabled) {
  ini.SetQuotes(false);

  input = "[section]\n"
          "key1 = foo\n"
          "key2 = \"foo\"\n"
          "key3 =  foo \n"
          "key4 = \" foo \"\n"
          "key5 = \"foo\n"
          "key6 = foo\"\n"
          "key7 =  foo \" foo \n"
          "key8 =  \" foo \" foo \" \n";

  expect = "[section]\n"
           "key1 = foo\n"
           "key2 = \"foo\"\n"
           "key3 = foo\n"
           "key4 = \" foo \"\n"
           "key5 = \"foo\n"
           "key6 = foo\"\n"
           "key7 = foo \" foo\n"
           "key8 = \" foo \" foo \"\n";

  const char *result;
  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);

  result = ini.GetValue("section", "key1");
  ASSERT_STREQ(result, "foo");

  result = ini.GetValue("section", "key2");
  ASSERT_STREQ(result, "\"foo\"");

  result = ini.GetValue("section", "key3");
  ASSERT_STREQ(result, "foo");

  result = ini.GetValue("section", "key4");
  ASSERT_STREQ(result, "\" foo \"");

  result = ini.GetValue("section", "key5");
  ASSERT_STREQ(result, "\"foo");

  result = ini.GetValue("section", "key6");
  ASSERT_STREQ(result, "foo\"");

  result = ini.GetValue("section", "key7");
  ASSERT_STREQ(result, "foo \" foo");

  result = ini.GetValue("section", "key8");
  ASSERT_STREQ(result, "\" foo \" foo \"");

  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

// SetQuotes(true) strips an outer quote pair on load, but the writer only
// wraps values with leading or trailing whitespace. A stored value that
// itself begins and ends with a quote therefore loses that pair on every
// save/load cycle. The writer must emit the already-accepted ""foo"" form.
#define K1_VALUE "\"foo\""
#define K2_VALUE "\"\""
#define K3_VALUE "\"a\"b\""
#define K4_VALUE " foo "
#define K5_VALUE "\""
TEST_F(TestQuotes, TestQuotedValueRoundTrip) {
  ini.SetQuotes(true);

  ASSERT_EQ(ini.SetValue("section", "k1", K1_VALUE), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k2", K2_VALUE), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k3", K3_VALUE), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k4", K4_VALUE), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k5", K5_VALUE), SI_INSERTED);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k1 = \"" K1_VALUE "\"\n"
           "k2 = \"" K2_VALUE "\"\n"
           "k3 = \"" K3_VALUE "\"\n"
           "k4 = \"" K4_VALUE "\"\n"
           "k5 = " K5_VALUE "\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetQuotes(true);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);

  ASSERT_STREQ(reload.GetValue("section", "k1"), K1_VALUE);
  ASSERT_STREQ(reload.GetValue("section", "k2"), K2_VALUE);
  ASSERT_STREQ(reload.GetValue("section", "k3"), K3_VALUE);
  ASSERT_STREQ(reload.GetValue("section", "k4"), K4_VALUE);
  ASSERT_STREQ(reload.GetValue("section", "k5"), K5_VALUE);
}
#undef K1_VALUE
#undef K2_VALUE
#undef K3_VALUE
#undef K4_VALUE
#undef K5_VALUE

// Quotes are checked before multiline on save. A quoted single-line value
// must still wrap; a value with newlines must stay on the multiline path
// so the loader does not strip quotes from its first and last lines.
#define QUOTED "\"foo\""
#define MULTILINE "foo\nbar"
#define QUOTED_MULTILINE "\"foo\nbar\""
#define SPACED " foo "
#define QUOTED_TAG "\"<<<END\""
TEST_F(TestQuotes, TestQuotedValueWithMultiline) {
  ini.SetQuotes(true);
  ini.SetMultiLine(true);

  ASSERT_EQ(ini.SetValue("section", "quoted", QUOTED), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "nl", MULTILINE), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "quotednl", QUOTED_MULTILINE), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "ws", SPACED), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "tag", QUOTED_TAG), SI_INSERTED);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "quoted = \"" QUOTED "\"\n"
           "nl = <<<END_OF_TEXT\n" MULTILINE "\n"
           "END_OF_TEXT\n"
           "quotednl = <<<END_OF_TEXT\n" QUOTED_MULTILINE "\n"
           "END_OF_TEXT\n"
           "ws = \"" SPACED "\"\n"
           "tag = \"" QUOTED_TAG "\"\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetQuotes(true);
  reload.SetMultiLine(true);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);

  ASSERT_STREQ(reload.GetValue("section", "quoted"), QUOTED);
  ASSERT_STREQ(reload.GetValue("section", "nl"), MULTILINE);
  ASSERT_STREQ(reload.GetValue("section", "quotednl"), QUOTED_MULTILINE);
  ASSERT_STREQ(reload.GetValue("section", "ws"), SPACED);
  ASSERT_STREQ(reload.GetValue("section", "tag"), QUOTED_TAG);
}
#undef QUOTED
#undef MULTILINE
#undef QUOTED_MULTILINE
#undef SPACED
#undef QUOTED_TAG

// A quoted value loaded via the multiline syntax is still just data that
// begins and ends with a quote, so save must wrap it rather than emit
// <<<END_OF_TEXT again.
#define QUOTED "\"foo\""
TEST_F(TestQuotes, TestQuotedValueLoadedFromMultiline) {
  ini.SetQuotes(true);
  ini.SetMultiLine(true);

  input = "[section]\n"
          "k = <<<END\n" QUOTED "\n"
          "END\n";
  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);
  ASSERT_STREQ(ini.GetValue("section", "k"), QUOTED);

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k = \"" QUOTED "\"\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetQuotes(true);
  reload.SetMultiLine(true);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);
  ASSERT_STREQ(reload.GetValue("section", "k"), QUOTED);
}
#undef QUOTED

#define QUOTED "\"foo\""
TEST_F(TestQuotes, TestQuotedValueNoSpaces) {
  ini.SetQuotes(true);
  ini.SetSpaces(false);

  ASSERT_EQ(ini.SetValue("section", "k", QUOTED), SI_INSERTED);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k=\"" QUOTED "\"\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetQuotes(true);
  reload.SetSpaces(false);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);
  ASSERT_STREQ(reload.GetValue("section", "k"), QUOTED);
}
#undef QUOTED
