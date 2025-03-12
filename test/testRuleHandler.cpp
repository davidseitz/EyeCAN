#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <ruleHandler.h>

using json = nlohmann::ordered_json;

class RuleHandlerTest : public ::testing::Test {
protected:
    RuleHandler ruleHandler;
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(RuleHandlerTest, createRuleTest) {
    // Test json is wrong
    json wrongRule = {};
    int result = ruleHandler.create(wrongRule);
    EXPECT_EQ(result, 400);
    EXPECT_EQ(wrongRule["id"],nullptr);

    // Test json is correct
    json correctRule = {
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
    result = ruleHandler.create(correctRule);
    EXPECT_EQ(result, 201);
    const std::string id = correctRule["id"];
    EXPECT_TRUE(std::filesystem::exists(ruleHandler.getEyeCANPath() + id + ".json"));

    // Clean up
    std::filesystem::remove(ruleHandler.getEyeCANPath() + id + ".json");
}


TEST_F(RuleHandlerTest, getRulesTest) {
    // Create test files
    std::list<std::string> rulesToDelete = {};
    for (int i = 0; i < 11; ++i) {
        json correctRule = {
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
        ruleHandler.create(correctRule);
        rulesToDelete.push_back(correctRule["id"]);
    }

    // Test page is wrong
    json response;
    int result = ruleHandler.get(-1, response);
    EXPECT_EQ(result, 400);

    // Test page is correct get 10 files and not last page
    result = ruleHandler.get(1, response);
    EXPECT_EQ(result, 200);
    EXPECT_EQ(response["page"], 1);
    EXPECT_EQ(response["lastPage"], false);
    EXPECT_EQ(response["filters"].size(), 10);

    // Test page is correct get 1 file and last page
    response.clear();
    result = ruleHandler.get(2, response);
    EXPECT_EQ(result, 200);
    EXPECT_EQ(response["page"], 2);
    EXPECT_EQ(response["lastPage"], true);
    EXPECT_EQ(response["filters"].size(), 1);

    // Test page is correct get 0 files
    response.clear();
    result = ruleHandler.get(3, response);
    EXPECT_EQ(result, 204);
    EXPECT_EQ(response["page"], 3);
    EXPECT_EQ(response["lastPage"], true);
    EXPECT_EQ(response["filters"].size(), 0);

    // Clean up
    for (const auto& rule : rulesToDelete) {
        std::filesystem::remove(ruleHandler.getEyeCANPath() + rule + ".json");
    }

    // Test corrupted file
    response.clear();
    std::ofstream file(ruleHandler.getEyeCANPath() + "no_json.json");
    file << "}No json{";
    file.close();
    result = ruleHandler.get(1, response);
    EXPECT_EQ(result, 400);

    // Clean up
    std::filesystem::remove(ruleHandler.getEyeCANPath() + "no_json.json");
}