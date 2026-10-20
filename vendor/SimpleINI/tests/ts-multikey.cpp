#include "../SimpleIni.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

namespace {

std::vector<std::string> AllValues(const CSimpleIniA &ini, const char *section,
                                   const char *key) {
  CSimpleIniA::TNamesDepend vals;
  ini.GetAllValues(section, key, vals);
  std::vector<std::string> out;
  for (const auto &e : vals) {
    out.emplace_back(e.pItem ? e.pItem : "");
  }
  return out;
}

} // namespace

class TestMultiKey : public ::testing::Test {
protected:
  void SetUp() override;

protected:
  CSimpleIniA ini;
  std::string input;
  std::string expect;
  std::string output;
};

void TestMultiKey::SetUp() { ini.SetUnicode(); }

// Duplicate keys are kept when enabled and collapsed to the last value
// when disabled. GetValue returns the first stored entry.
#define FIRST "first"
#define SECOND "second"
TEST_F(TestMultiKey, TestEnabled) {
  ini.SetMultiKey(true);

  input = "[section]\n"
          "k = " FIRST "\n"
          "k = " SECOND "\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  bool hasMultiple = false;
  ASSERT_STREQ(ini.GetValue("section", "k", nullptr, &hasMultiple), FIRST);
  ASSERT_TRUE(hasMultiple);

  const auto vals = AllValues(ini, "section", "k");
  ASSERT_EQ(vals.size(), 2u);
  ASSERT_EQ(vals[0], FIRST);
  ASSERT_EQ(vals[1], SECOND);

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(input.c_str(), output.c_str());
}

TEST_F(TestMultiKey, TestDisabled) {
  ini.SetMultiKey(false);

  input = "[section]\n"
          "k = " FIRST "\n"
          "k = " SECOND "\n";
  expect = "[section]\n"
           "k = " SECOND "\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  bool hasMultiple = true;
  ASSERT_STREQ(ini.GetValue("section", "k", nullptr, &hasMultiple), SECOND);
  ASSERT_FALSE(hasMultiple);
  ASSERT_EQ(AllValues(ini, "section", "k").size(), 1u);

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(expect.c_str(), output.c_str());
}

TEST_F(TestMultiKey, TestSetValueAppends) {
  ini.SetMultiKey(true);

  ASSERT_EQ(ini.SetValue("section", "k", FIRST), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k", SECOND), SI_UPDATED);
  ASSERT_STREQ(ini.GetValue("section", "k"), FIRST);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k = " FIRST "\n"
           "k = " SECOND "\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetMultiKey(true);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);

  const auto vals = AllValues(reload, "section", "k");
  ASSERT_EQ(vals.size(), 2u);
  ASSERT_EQ(vals[0], FIRST);
  ASSERT_EQ(vals[1], SECOND);
}

TEST_F(TestMultiKey, TestForceReplace) {
  ini.SetMultiKey(true);

  ASSERT_EQ(ini.SetValue("section", "k", FIRST), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k", SECOND, nullptr, true), SI_UPDATED);
  ASSERT_STREQ(ini.GetValue("section", "k"), SECOND);
  ASSERT_EQ(AllValues(ini, "section", "k").size(), 1u);
}

TEST_F(TestMultiKey, TestToggledOffBeforeSave) {
  ini.SetMultiKey(true);

  input = "[section]\n"
          "k = " FIRST "\n"
          "k = " SECOND "\n";
  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  ini.SetMultiKey(false);
  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  // Save uses GetAllValues, so disabling the flag drops every extra copy.
  expect = "[section]\n"
           "k = " FIRST "\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());
}
#undef FIRST
#undef SECOND

#define Q1 " foo "
#define Q2 " bar "
TEST_F(TestMultiKey, TestWithQuotes) {
  ini.SetMultiKey(true);
  ini.SetQuotes(true);

  input = "[section]\n"
          "k = \"" Q1 "\"\n"
          "k = \"" Q2 "\"\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  const auto vals = AllValues(ini, "section", "k");
  ASSERT_EQ(vals.size(), 2u);
  ASSERT_EQ(vals[0], Q1);
  ASSERT_EQ(vals[1], Q2);

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(input.c_str(), output.c_str());
}
#undef Q1
#undef Q2

#define ML1 "first\nline"
#define ML2 "second\nline"
TEST_F(TestMultiKey, TestWithMultiLine) {
  ini.SetMultiKey(true);
  ini.SetMultiLine(true);

  ASSERT_EQ(ini.SetValue("section", "k", ML1), SI_INSERTED);
  ASSERT_EQ(ini.SetValue("section", "k", ML2), SI_UPDATED);

  SI_Error rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

  expect = "[section]\n"
           "k = <<<END_OF_TEXT\n" ML1 "\n"
           "END_OF_TEXT\n"
           "k = <<<END_OF_TEXT\n" ML2 "\n"
           "END_OF_TEXT\n";
  ASSERT_STREQ(expect.c_str(), output.c_str());

  CSimpleIniA reload;
  reload.SetUnicode();
  reload.SetMultiKey(true);
  reload.SetMultiLine(true);
  rc = reload.LoadData(output);
  ASSERT_EQ(rc, SI_OK);

  const auto vals = AllValues(reload, "section", "k");
  ASSERT_EQ(vals.size(), 2u);
  ASSERT_EQ(vals[0], ML1);
  ASSERT_EQ(vals[1], ML2);
}
#undef ML1
#undef ML2

TEST_F(TestMultiKey, TestWithAllowKeyOnly) {
  ini.SetMultiKey(true);
  ini.SetAllowKeyOnly(true);

  input = "[section]\n"
          "k\n"
          "k\n"
          "k = val\n";

  SI_Error rc = ini.LoadData(input);
  ASSERT_EQ(rc, SI_OK);

  const auto vals = AllValues(ini, "section", "k");
  ASSERT_EQ(vals.size(), 3u);
  ASSERT_EQ(vals[0], "");
  ASSERT_EQ(vals[1], "");
  ASSERT_EQ(vals[2], "val");
  ASSERT_STREQ(ini.GetValue("section", "k"), "");

  rc = ini.Save(output);
  ASSERT_EQ(rc, SI_OK);
  output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());
  ASSERT_STREQ(input.c_str(), output.c_str());
}
