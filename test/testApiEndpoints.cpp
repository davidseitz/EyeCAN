#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <ruleHandler.h>

using json = nlohmann::ordered_json;

class ApiTest : public ::testing::Test {
protected:
    RuleHandler rh;
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// TEST Knowledgebase API endpoints
// TODO Implement tests for Knowledgebase API endpoints
/*
TEST_F(ApiTest, GetKnowledgebaseTest) {
    httplib::Client client("http://localhost:5255");
    auto res = client.Get("/api/v1/knowledgebase?page=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);

    // Parse JSON response
    //json response_json = json::parse(res->body);
    EXPECT_EQ((std::string) res->body, "The Page you requested: 1");
}
*/
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

TEST_F(ApiTest, PostFilterTest) {
    httplib::Client client("http://localhost:5255");
    const json testRule = {
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

    // Test Content-Type is not application/json
    auto res = client.Post("/api/v1/filters", testRule.dump(),"application/test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 415); // Unsupported Media Type

    // Test wrong json
    res = client.Post("/api/v1/filters", "}}}This is not json","application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400); // Bad Request

    // Test successfully created rule
    res = client.Post("/api/v1/filters", testRule.dump(),"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 201); // Created

    json response = json::parse(res->body);
    std::string id = response["id"];

    int status = std::filesystem::exists(rh.getEyeCANPath()+id+".json");
    EXPECT_EQ(status, 1); // Ensure file exists

    // Clean up
    std::filesystem::remove(rh.getEyeCANPath()+id+".json");
}

TEST_F(ApiTest, PutFilterTest) {
    httplib::Client client("http://localhost:5255");
    const json testRule = {
        {"id","a1c0bdd4-a228-40a0-889a-516ff7b42393"},
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

    // Test Content-Type is not application/json
    auto res = client.Put("/api/v1/filters", testRule.dump(),"application/test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 415); // Unsupported Media Type

    // Test wrong json
    res = client.Put("/api/v1/filters", "}}}This is not json","application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400); // Bad Request

    // Test file not found
    res = client.Put("/api/v1/filters", testRule.dump(),"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 404); // Unsupported Media Type

    // Create file to edit
    std::string id = testRule["id"];
    std::ofstream fileToEdit(rh.getEyeCANPath()+id+".json");
    fileToEdit << "test";
    fileToEdit.close();

    // Test successfully edited rule
    res = client.Put("/api/v1/filters", testRule.dump() ,"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200); // Successfully edited rule

    std::string fileContent;
    std::ifstream editedFile(rh.getEyeCANPath()+id+".json");
    std::string content((std::istreambuf_iterator<char>(editedFile)), std::istreambuf_iterator<char>());
    editedFile.close();
    json expected = json::parse(content);
    EXPECT_EQ(expected.dump(), testRule.dump()); // Ensure file content is the same as the testRule

    // Clean up
    std::filesystem::remove(rh.getEyeCANPath()+id+".json");
}

TEST_F(ApiTest, DeleteFilterTest) {
    httplib::Client client("http://localhost:5255");

    // Invalid uuid
    auto res = client.Delete("/api/v1/filters?uuid=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // File not found
    std::string testUuid = "a1c0bdd4-a228-40a0-889a-516ff7b42393";
    res = client.Delete("/api/v1/filters?uuid="+testUuid);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 404);

    // Create a file to delete
    std::ofstream file(rh.getEyeCANPath()+testUuid+".json");
    file << "test";
    file.close();

    // Successfully deleted file
    res = client.Delete("/api/v1/filters?uuid="+testUuid);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 204);
}

TEST_F(ApiTest, GetFilterTest) {
    // Create test files
    std::list<std::string> rulesToDelete = {};
    json rulePayload = {
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
    json expectedOutput = {
        {"page", 1},
        {"lastPage", false},
        {"filters", json::array()}
    };
    for (int i = 0; i < 11; ++i) {
        json correctRule = rulePayload;
        rh.create(correctRule);

        json formatedRule = {};
        formatedRule["id"] = correctRule["id"];
        formatedRule["title"] = rulePayload["title"];
        formatedRule["description"] = rulePayload["description"];
        formatedRule["signals"] = rulePayload["signals"];
        formatedRule["settings"] = rulePayload["settings"];

        rulesToDelete.push_back(correctRule["id"]);
        if( i > 0 ){
            expectedOutput["filters"].insert(expectedOutput["filters"].begin(),formatedRule);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Ensure different last write time
    }

    // Test page is not integer
    httplib::Client client("http://localhost:5255");
    auto res = client.Get("/api/v1/filters?page=test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // Test page is wrong
    res = client.Get("/api/v1/filters?page=-1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // Test page is correct get 10 files and not last page
    res = client.Get("/api/v1/filters?page=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->body, expectedOutput.dump());

    // Test page is correct get 1 file and last page
    res = client.Get("/api/v1/filters?page=2");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);

    // Generate expected output
    expectedOutput["page"] = 2;
    expectedOutput["lastPage"] = true;
    expectedOutput["filters"] = json::array();
    json formatedRule = {};
    formatedRule["id"] = rulesToDelete.front();
    formatedRule["title"] = rulePayload["title"];
    formatedRule["description"] = rulePayload["description"];
    formatedRule["signals"] = rulePayload["signals"];
    formatedRule["settings"] = rulePayload["settings"];
    expectedOutput["filters"].push_back(formatedRule);

    EXPECT_EQ(res->body, expectedOutput.dump());

    // Test page is correct get 0 files
    res = client.Get("/api/v1/filters?page=3");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 204);

    // Clean up
    for (const auto& rule : rulesToDelete) {
        std::filesystem::remove(rh.getEyeCANPath() + rule + ".json");
    }
}

// TEST Dataset API endpoints

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
