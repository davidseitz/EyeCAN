#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <ruleHandler.h>

using json = nlohmann::ordered_json;

class RuleHandlerTest : public ::testing::Test {
protected:
    RuleHandler ruleHandler;
    void SetUp() override {
        std::filesystem::create_directories(ruleHandler.getEyeCANPath() + "test/");
    }

    void TearDown() override {
        std::filesystem::remove_all(ruleHandler.getEyeCANPath() + "test/");
    }
};

TEST_F(RuleHandlerTest, createRuleTest) {
    RuleHandler ruleHandler;

    // Test json is wrong
    json wrong_rule = {};
    int result = ruleHandler.create(wrong_rule);
    EXPECT_EQ(result, 400);

    // Test json is correct
    json correct_rule = {
        {"title", "Test Rule"},
        {"description", "This is a test rule"},
        {"signals", {
            {"signal1", "value1"},
            {"signal2", "value2"}
        }},
        {"settings", {
            {"setting1", "value1"},
            {"setting2", "value2"}
        }}
    };
    result = ruleHandler.create(correct_rule);
    EXPECT_EQ(result, 201);
}

/*
TEST_F(ruleHandlerTest, getRulesTest) {
    //TODO getRulesTest
}

 */