//
// Created by robin on 28.02.2025.
//
#include <gtest/gtest.h>
//#include <cpr/cpr.h>

TEST(APIEndpoints, TestAPIEndpoints) {
    //cpr::Response r = cpr::Get(cpr::Url{"http://localhost:5255/api/v1/knowledgebase?page=1"});
    EXPECT_EQ(200, 200);
    EXPECT_EQ("Test", "The Page you requested: 1");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}