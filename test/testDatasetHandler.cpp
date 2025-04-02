#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <datasetHandler.h>

using json = nlohmann::ordered_json;

class DatasetHandlerTest : public ::testing::Test {
protected:
    DatasetHandler datasetHandler;
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(DatasetHandlerTest, createDatasetTest) {

    // Test json is wrong
    json wrongArticle = {};
    int result = datasetHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);
    EXPECT_EQ(wrongArticle["id"],nullptr);

    // Test title is too long
    wrongArticle.clear();
    wrongArticle = {
        {"title", std::string (129, 'a')},
        {"description", "1741777379"},
        {"selectedSignalValues", "Test Author"},
        {"signals", "This is a test article"}
    };
    result = datasetHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);

    // Test description is too long
    wrongArticle.clear();
    wrongArticle = {
        {"title", "test"},
        {"description", std::string (301, 'a')},
        {"selectedSignalValues", json::array()},
        {"signals", json::array()}
    };
    result = datasetHandler.create(wrongArticle);
    EXPECT_EQ(result, 400);

    // Test json is correct
    json correctDataset = {
        {"title", "Test Dataset"},
        {"description", "Test description"},
        {"selectedSignalValues", json::array()},
        {"signals", json::array()}
    };

    result = datasetHandler.create(correctDataset);
    EXPECT_EQ(result, 201);
    EXPECT_EQ(correctDataset.contains("id"), true);
    EXPECT_EQ(static_cast<std::string>(correctDataset["id"]).size(), 36);
    EXPECT_EQ(correctDataset.contains("title"), true);
    EXPECT_EQ(static_cast<std::string>(correctDataset["title"]), "Test Dataset");
    EXPECT_EQ(correctDataset.contains("date"), true);
    EXPECT_EQ(static_cast<std::string>(correctDataset["description"]), "Test description");
    EXPECT_EQ(correctDataset.contains("author"), true);
    EXPECT_EQ(static_cast<std::string>(correctDataset["selectedSignalValues"]), static_cast<std::string>(json::array()));
    EXPECT_EQ(correctDataset.contains("content"), true);
    EXPECT_EQ(static_cast<std::string>(correctDataset["signals"]), static_cast<std::string>(json::array()));

    const std::string id = correctDataset["id"];
    EXPECT_TRUE(std::filesystem::exists(datasetHandler.getEyeCANPath() + id + ".json"));

    std::ifstream file(datasetHandler.getEyeCANPath() + (std::string) correctDataset["id"] + ".json");
    json fileJson;
    file >> fileJson;
    file.close();
    EXPECT_EQ(fileJson.dump(), correctDataset.dump());

    // Clean up
    std::filesystem::remove(datasetHandler.getEyeCANPath() + id + ".json");
}


TEST_F(DatasetHandlerTest, getDatasetTest) {
    // Create test files
    std::list<std::string> datasetToDelete = {};
    for (int i = 0; i < 11; ++i) {
        json correctArticle = {
            {"title", "Test Dataset"},
            {"description", "Test description"},
            {"selectedSignalValues", json::array()},
            {"signals", json::array()}
        };
        datasetHandler.create(correctArticle);
        datasetToDelete.push_back(correctArticle["id"]);
    }

    // Test page is wrong
    json response;
    int result = datasetHandler.get(-1, response);
    EXPECT_EQ(result, 400);

    // Test page is correct get 10 files and not last page
    response.clear();
    result = datasetHandler.get(1, response);
    EXPECT_EQ(result, 200);
    EXPECT_EQ(response["page"], 1);
    EXPECT_EQ(response["lastPage"], false);
    EXPECT_EQ(response["datasets"].size(), 10);

    // Test page is correct get 1 file and last page
    response.clear();
    result = datasetHandler.get(2, response);
    EXPECT_EQ(result, 200);
    EXPECT_EQ(response["page"], 2);
    EXPECT_EQ(response["lastPage"], true);
    EXPECT_EQ(response["datasets"].size(), 1);

    // Test page is correct get 0 files
    response.clear();
    result = datasetHandler.get(3, response);
    EXPECT_EQ(result, 204);
    EXPECT_EQ(response["page"], 3);
    EXPECT_EQ(response["lastPage"], true);
    EXPECT_EQ(response["datasets"].size(), 0);

    // Clean up
    for (const auto& datasets : datasetToDelete) {
        std::filesystem::remove(datasetHandler.getEyeCANPath() + datasets + ".json");
    }

    // Test corrupted file
    response.clear();
    std::ofstream file(datasetHandler.getEyeCANPath() + "no_json.json");
    file << "}No json{";
    file.close();
    result = datasetHandler.get(1, response);
    EXPECT_EQ(result, 400);

    // Clean up
    std::filesystem::remove(datasetHandler.getEyeCANPath() + "no_json.json");
}