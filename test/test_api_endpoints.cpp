#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ApiTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// TEST Knowledgebase API endpoints

TEST_F(ApiTest, GetKnowledgebaseTest) {
    httplib::Client client("http://localhost:5255");
    auto res = client.Get("/api/v1/knowledgebase?page=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);

    // Parse JSON response
    //json response_json = json::parse(res->body);
    EXPECT_EQ((std::string) res->body, "The Page you requested: 1");
}

/*
TEST_F(ApiTest, PostRequestTest) {
    httplib::Client client("http://jsonplaceholder.typicode.com");
    json request_body = { {"title", "foo"}, {"body", "bar"}, {"userId", 1} };

    auto res = client.Post("/posts", request_body.dump(), "application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 201);

    json response_json = json::parse(res->body);
    EXPECT_EQ(response_json["title"], "foo");
    EXPECT_EQ(response_json["body"], "bar");
    EXPECT_EQ(response_json["userId"], 1);
}
*/

// TEST Filter API endpoints

TEST_F(ApiTest, GetFilterTest) {
    httplib::Client client("http://localhost:5255");
    auto res = client.Get("/api/v1/filters?page=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);

    // Parse JSON response
    //json response_json = json::parse(res->body);
    EXPECT_EQ((std::string) res->body, "The Page you requested: 1");
}


