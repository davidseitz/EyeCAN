#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <articleHandler.h>

using json = nlohmann::ordered_json;

class ArticleHandlerTest : public ::testing::Test {
protected:
    ArticleHandler articleHandler;
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(ArticleHandlerTest, createArticleTest) {

    // Test json is wrong
    json wrongArticle = {};
    int result = articleHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);
    EXPECT_EQ(wrongArticle["id"],nullptr);

    // Test title is too long
    wrongArticle.clear();
    wrongArticle = {
        {"title", std::string (129, 'a')},
        {"date", "1741777379"},
        {"author", "Test Author"},
        {"content", "This is a test article"}
    };
    result = articleHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);

    // Test author is too long
    wrongArticle.clear();
    wrongArticle = {
        {"title", "test"},
        {"date", "1741777379"},
        {"author", std::string (513, 'a')},
        {"content", "This is a test article"}
    };
    result = articleHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);

    // Test content is too long
    wrongArticle.clear();
    wrongArticle = {
        {"title", "test"},
        {"date", "1741777379"},
        {"author", "Test Author"},
        {"content", std::string (10001, 'a')}
    };
    result = articleHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);

    // Test json is correct
    json correctArticle = {
        {"title", "Test Article"},
        {"date", "1741777379"},
        {"author", "Test Author"},
        {"content", "This is a test article"}
    };
    result = articleHandler.create(correctArticle);
    EXPECT_EQ(result, 201);
    EXPECT_EQ(correctArticle.contains("id"), true);
    EXPECT_EQ(static_cast<std::string>(correctArticle["id"]).size(), 36);
    EXPECT_EQ(correctArticle.contains("title"), true);
    EXPECT_EQ(static_cast<std::string>(correctArticle["title"]), "Test Article");
    EXPECT_EQ(correctArticle.contains("date"), true);
    EXPECT_EQ(static_cast<std::string>(correctArticle["date"]), "1741777379");
    EXPECT_EQ(correctArticle.contains("author"), true);
    EXPECT_EQ(static_cast<std::string>(correctArticle["author"]), "Test Author");
    EXPECT_EQ(correctArticle.contains("content"), true);
    EXPECT_EQ(static_cast<std::string>(correctArticle["content"]), "This is a test article");

    const std::string id = correctArticle["id"];
    EXPECT_TRUE(std::filesystem::exists(articleHandler.getEyeCANPath() + id + ".json"));

    std::ifstream file(articleHandler.getEyeCANPath() + (std::string) correctArticle["id"] + ".json");
    json fileJson;
    file >> fileJson;
    file.close();
    EXPECT_EQ(fileJson.dump(), correctArticle.dump());

    // Clean up
    std::filesystem::remove(articleHandler.getEyeCANPath() + id + ".json");
}


TEST_F(ArticleHandlerTest, getArticlesTest) {
    // Create test files
    std::list<std::string> articlesToDelete = {};
    for (int i = 0; i < 11; ++i) {
        json correctArticle = {
            {"title", "Test Article"},
            {"date", "1741777379"},
            {"author", "Test Author"},
            {"content", "This is a test article"}
        };
        articleHandler.create(correctArticle);
        articlesToDelete.push_back(correctArticle["id"]);
    }

    // Test page is wrong
    json response;
    int result = articleHandler.get(-1, response);
    EXPECT_EQ(result, 400);

    // Test page is correct get 10 files and not last page
    response.clear();
    result = articleHandler.get(1, response);
    EXPECT_EQ(result, 200);
    EXPECT_EQ(response["page"], 1);
    EXPECT_EQ(response["lastPage"], false);
    EXPECT_EQ(response["articles"].size(), 10);

    // Test page is correct get 1 file and last page
    response.clear();
    result = articleHandler.get(2, response);
    EXPECT_EQ(result, 200);
    EXPECT_EQ(response["page"], 2);
    EXPECT_EQ(response["lastPage"], true);
    EXPECT_EQ(response["articles"].size(), 1);

    // Test page is correct get 0 files
    response.clear();
    result = articleHandler.get(3, response);
    EXPECT_EQ(result, 204);
    EXPECT_EQ(response["page"], 3);
    EXPECT_EQ(response["lastPage"], true);
    EXPECT_EQ(response["articles"].size(), 0);

    // Clean up
    for (const auto& article : articlesToDelete) {
        std::filesystem::remove(articleHandler.getEyeCANPath() + article + ".json");
    }

    // Test corrupted file
    response.clear();
    std::ofstream file(articleHandler.getEyeCANPath() + "no_json.json");
    file << "}No json{";
    file.close();
    result = articleHandler.get(1, response);
    EXPECT_EQ(result, 400);

    // Clean up
    std::filesystem::remove(articleHandler.getEyeCANPath() + "no_json.json");
}