#include "../SimpleIni.h"
#include "gtest/gtest.h"
#include <algorithm>

class TestKeyOnly : public ::testing::Test {
protected:
  void SetUp() override;

protected:
  CSimpleIniA ini;
  std::string input;
  std::string expect;
  std::string output;
};

void TestKeyOnly::SetUp() { ini.SetUnicode(); }

// Lines without '=' are dropped unless AllowKeyOnly is on. Empty values
// with an equals sign are then written back as a bare key.
#define STRING "string"
#define BARE "plain text"
TEST_F(TestKeyOnly, TestDisabled) {
  ini.SetAllowKeyOnly(false);

  input = "[section]\n"
          "k1 = " STRING "\n"
          "k2 = \n"
          "k3\n" BARE "\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  ASSERT_STREQ(ini.GetValue("section", "k1"), STRING);
  ASSERT_STREQ(ini.GetValue("section", "k2"), "");
  ASSERT_FALSE(ini.KeyExists("section", "k3"));
  ASSERT_FALSE(ini.KeyExists("section", BARE));
}

TEST_F(TestKeyOnly, TestEnabled) {
  ini.SetAllowKeyOnly(true);

  input = "[section]\n"
          "k1 = " STRING "\n"
          "k2 = \n"
          "k3\n" BARE "\n";
  expect = "[section]\n"
           "k1 = " STRING "\n"
           "k2\n"
           "k3\n" BARE "\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  ASSERT_STREQ(ini.GetValue("section", "k1"), STRING);
  ASSERT_STREQ(ini.GetValue("section", "k2"), "");
  ASSERT_STREQ(ini.GetValue("section", "k3"), "");
  ASSERT_STREQ(ini.GetValue("section", BARE), "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

// FindEntry used to leave a_pVal pointing at the previous key's value, so
// a key-only line inherited that value instead of being empty.
TEST_F(TestKeyOnly, TestDoesNotInheritPreviousValue) {
  ini.SetAllowKeyOnly(true);

  input = "[section]\n"
          "k1 = " STRING "\n"
          "k2\n";
  expect = "[section]\n"
           "k1 = " STRING "\n"
           "k2\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  ASSERT_STREQ(ini.GetValue("section", "k1"), STRING);
  ASSERT_STREQ(ini.GetValue("section", "k2"), "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

#define EMPTY ""
TEST_F(TestKeyOnly, TestEmptyRoundTrip) {
  ini.SetAllowKeyOnly(true);

  ASSERT_EQ(ini.SetValue("section", "k", EMPTY), SI_INSERTED);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetAllowKeyOnly(true);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);
  ASSERT_TRUE(reload.KeyExists("section", "k"));
  ASSERT_STREQ(reload.GetValue("section", "k"), EMPTY);
}
#undef EMPTY
#undef STRING
#undef BARE

TEST_F(TestKeyOnly, TestWithSpaces) {
  ini.SetAllowKeyOnly(true);
  ini.SetSpaces(false);

  ASSERT_EQ(ini.SetValue("section", "bare", ""), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "hasval", "x"), SI_INSERTED);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "bare\n"
           "hasval=x\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

// Quotes strip `""` to an empty value. AllowKeyOnly then writes that as a
// bare key, so the quotes do not survive the round trip.
TEST_F(TestKeyOnly, TestWithQuotes) {
  ini.SetAllowKeyOnly(true);
  ini.SetQuotes(true);

  input = "[section]\n"
          "k = \"\"\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);
  ASSERT_STREQ(ini.GetValue("section", "k"), "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

TEST_F(TestKeyOnly, TestWithMultiLineDisabled) {
  ini.SetMultiLine(false);
  ini.SetAllowKeyOnly(true);

  // Without multiline, <<<END is a literal value. The following lines are
  // not consumed as a body; AllowKeyOnly turns them into keys.
  input = "[section]\n"
          "k = <<<END\n"
          "orphan\n"
          "END\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  ASSERT_STREQ(ini.GetValue("section", "k"), "<<<END");
  ASSERT_STREQ(ini.GetValue("section", "orphan"), "");
  ASSERT_STREQ(ini.GetValue("section", "END"), "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  expect = "[section]\n"
           "k = <<<END\n"
           "orphan\n"
           "END\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA consumed;
  consumed.SetUnicode();
  consumed.SetMultiLine(true);
  consumed.SetAllowKeyOnly(true);
  rc = consumed.LoadData(input);
  ASSERT_EQ(rc, SI_OK);
  ASSERT_STREQ(consumed.GetValue("section", "k"), "orphan");
  ASSERT_FALSE(consumed.KeyExists("section", "orphan"));
  ASSERT_FALSE(consumed.KeyExists("section", "END"));
}
