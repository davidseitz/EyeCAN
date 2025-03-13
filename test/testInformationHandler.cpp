#include <gtest/gtest.h>
#include "informationHandler.h"

// Concrete subclass for testing purposes
class TestInformationHandler : public InformationHandler {
public:
    TestInformationHandler() {
        localEyeCANPath += "test/";
    }

    int create(json& info) override {
        // Only for test purposes
        return 0;
    }

    int get(int page, json& response) override {
        // Only for test purposes
        return {};
    }

    // Public wrapper methods to access protected functions
    int testSaveToFile(json& info) {
        return saveToFile(info);
    }

    int testGenerateUUID(std::string& uuid) {
        return generateUuid(uuid);
    }

    int testGetFiles(const int page, std::list<json>& information, json& response) const {
        return getFiles(page, information, response);
    }

    std::string getLocalEyeCANPath() {
        return localEyeCANPath;
    }
};

class InformationHandlerTest : public ::testing::Test {
protected:
    TestInformationHandler infoHandler;

    void SetUp() override {
        std::filesystem::create_directories(infoHandler.getLocalEyeCANPath());
    }

    void TearDown() override {
        std::filesystem::remove_all(infoHandler.getLocalEyeCANPath());
    }
};

TEST_F(InformationHandlerTest, SaveToFileTest) {
    // Test save to file
    json info = {{"id", "test_id"}, {"data", "test_data"}};
    int result = infoHandler.testSaveToFile(info);
    EXPECT_EQ(result, 201); // Check if the file was created successfully
    EXPECT_TRUE(std::filesystem::exists(infoHandler.getLocalEyeCANPath() + "test_id.json")); // Check if the file exists

}

TEST_F(InformationHandlerTest, GenerateUUIDTest) {
    std::string uuid;
    int result = infoHandler.testGenerateUUID(uuid);
    EXPECT_EQ(result, 0); // Check if the UUID was generated successfully
    EXPECT_FALSE(uuid.empty()); // Check if the UUID is not empty
}

TEST_F(InformationHandlerTest, GetFilesTest) {
    // Save test files
    for (int i = 0; i < 11; i++) {
        json info = {{"id", "test_id" + std::to_string(i)}, {"data", "test_data" + std::to_string(i)}};
        infoHandler.testSaveToFile(info);
    }

    // Test get files
    std::list<json> information;
    json response;
    int result = infoHandler.testGetFiles(1, information, response);
    EXPECT_EQ(result, 0); // Check if the files were read successfully
    EXPECT_EQ(response["lastPage"], false); // Check if it is not the last page
    EXPECT_EQ(information.size(), 10); // Check if the correct number of files were read

    // Test last page
    information.clear();
    result = infoHandler.testGetFiles(2, information, response);
    EXPECT_EQ(result, 0); // Check if the files were read successfully
    EXPECT_EQ(response["lastPage"], true); // Check if it is the last page
    EXPECT_EQ(information.size(), 1); // Check if the correct number of files were read

    // Test negative page number
    information.clear();
    result = infoHandler.testGetFiles(-1, information, response);
    EXPECT_EQ(result, 0); // Check if the files were read successfully
    EXPECT_EQ(information.size(), 0); // Check if no files were read

}