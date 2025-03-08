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

    std::list<json> get(int page) override {
        // Only for test purposes
        return {};
    }

    // Public wrapper methods to access protected functions
    int testSaveToFile(json& info) {
        return saveToFile(info);
    }

    int testGenerateUUID(std::string& uuid) {
        return generate_uuid(uuid);
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

    // Test save to file with existing file
    result = infoHandler.testSaveToFile(info);
    EXPECT_EQ(result, 400); // Check if the file was created successfully

}

TEST_F(InformationHandlerTest, GenerateUUIDTest) {
    std::string uuid;
    int result = infoHandler.testGenerateUUID(uuid);
    EXPECT_EQ(result, 0); // Check if the UUID was generated successfully
    EXPECT_FALSE(uuid.empty()); // Check if the UUID is not empty
}