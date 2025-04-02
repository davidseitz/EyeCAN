//
// Created by eyecandevelopment on 02.04.25.
//
// test/testDataFusior.cpp

#include "dataFusior.h"
#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

// Test fixture for DataFusior
class DataFusiorTest : public ::testing::Test {
protected:
    DataFusior* fusior;

    void SetUp() override {
        fusior = new DataFusior();
    }

    void TearDown() override {
        delete fusior;
    }
};

// Test constructor
TEST_F(DataFusiorTest, Constructor) {
    ASSERT_TRUE(fusior != nullptr);
}

// Test readAllData method
TEST_F(DataFusiorTest, ReadAllData) {
    // Use stubs for file paths
    std::string dbcFilepath = "../dbcExamples/Vehicle.dbc";
    std::string mdfFilepath = "../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4";

    // Call the method
    fusior->readAllData(dbcFilepath, mdfFilepath);

    // Check if jsonData is populated
    nlohmann::ordered_json jsonData = fusior->getFusedData();
    ASSERT_FALSE(jsonData["signals"].empty());
}

// Test getFusedData method
TEST_F(DataFusiorTest, GetFusedData) {
    // Use stubs for file paths
    std::string dbcFilepath = "../dbcExamples/Vehicle.dbc";
    std::string mdfFilepath = "../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4";

    // Call the method
    fusior->readAllData(dbcFilepath, mdfFilepath);

    // Get the fused data
    nlohmann::ordered_json fusedData = fusior->getFusedData();

    // Check if the returned data is not empty
    ASSERT_FALSE(fusedData.empty());
}

// Test destructor
TEST_F(DataFusiorTest, Destructor) {
    // Use stubs for file paths
    std::string dbcFilepath = "../dbcExamples/Vehicle.dbc";
    std::string mdfFilepath = "../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4";

    // Call the method
    fusior->readAllData(dbcFilepath, mdfFilepath);

    // Check if the file is created and contains data
    std::ifstream inFile("../mf4Examples/fusedData.json");
    ASSERT_TRUE(inFile.is_open());

    nlohmann::ordered_json jsonData;
    inFile >> jsonData;
    ASSERT_FALSE(jsonData.empty());
    inFile.close();
}

