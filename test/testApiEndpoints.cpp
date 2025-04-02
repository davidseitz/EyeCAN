#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <ruleHandler.h>
#include <articleHandler.h>
#include <datasetHandler.h>

using json = nlohmann::ordered_json;

class ApiTest : public ::testing::Test {
protected:
    RuleHandler rh;
    ArticleHandler ah;
    DatasetHandler dh;
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// TEST Knowledgebase API endpoints

TEST_F(ApiTest, PostKnowledgebaseTest) {
    httplib::Client client("http://localhost:5255");
    const json testArticle = {
        {"title", "Test Article"},
        {"date", "1741777379"},
        {"author", "Test Author"},
        {"content", "This is a test article"}
    };

    // Test Content-Type is not application/json
    auto res = client.Post("/api/v1/knowledgebase", testArticle.dump(),"application/test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 415); // Unsupported Media Type

    // Test wrong json
    res = client.Post("/api/v1/knowledgebase", "}}}This is not json","application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400); // Bad Request

    // Test successfully created rule
    res = client.Post("/api/v1/knowledgebase", testArticle.dump(),"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 201); // Created

    json response = json::parse(res->body);
    std::string id = response["id"];

    int status = std::filesystem::exists(ah.getEyeCANPath()+id+".json");
    EXPECT_EQ(status, 1); // Ensure file exists

    // Clean up
    std::filesystem::remove(ah.getEyeCANPath()+id+".json");
}

TEST_F(ApiTest, PutKnowledgebaseTest) {
    httplib::Client client("http://localhost:5255");
    const json testArticle = {
            {"id","a1c0bdd4-a228-40a0-889a-516ff7b42393"},
            {"title", "Test Article"},
            {"date", "1741777379"},
            {"author", "Test Author"},
            {"content", "This is a test article"}
    };

    // Test Content-Type is not application/json
    auto res = client.Put("/api/v1/knowledgebase", testArticle.dump(),"application/test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 415); // Unsupported Media Type

    // Test wrong json
    res = client.Put("/api/v1/knowledgebase", "}}}This is not json","application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400); // Bad Request

    // Test file not found
    res = client.Put("/api/v1/knowledgebase", testArticle.dump(),"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 404); // Unsupported Media Type

    // Create file to edit
    std::string id = testArticle["id"];
    std::ofstream fileToEdit(ah.getEyeCANPath()+id+".json");
    fileToEdit << "test";
    fileToEdit.close();

    // Test successfully edited rule
    res = client.Put("/api/v1/knowledgebase", testArticle.dump() ,"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200); // Successfully edited rule

    std::string fileContent;
    std::ifstream editedFile(ah.getEyeCANPath()+id+".json");
    std::string content((std::istreambuf_iterator<char>(editedFile)), std::istreambuf_iterator<char>());
    editedFile.close();
    json expected = json::parse(content);
    EXPECT_EQ(expected.dump(), testArticle.dump()); // Ensure file content is the same as the testArticle

    // Clean up
    std::filesystem::remove(ah.getEyeCANPath()+id+".json");
}

TEST_F(ApiTest, DeleteKnowledgebaseTest) {
    httplib::Client client("http://localhost:5255");

    // Invalid uuid
    auto res = client.Delete("/api/v1/knowledgebase?uuid=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // File not found
    std::string testUuid = "a1c0bdd4-a228-40a0-889a-516ff7b42393";
    res = client.Delete("/api/v1/knowledgebase?uuid="+testUuid);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 404);

    // Create a file to delete
    std::ofstream file(ah.getEyeCANPath()+testUuid+".json");
    file << "test";
    file.close();

    // Successfully deleted file
    res = client.Delete("/api/v1/knowledgebase?uuid="+testUuid);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 204);
}

TEST_F(ApiTest, GetKnowledgebaseTest) {
    // Create test files
    std::list<std::string> articlesToDelete = {};
    json articlePayload = {
        {"title", "Test Article"},
        {"date", "1741777379"},
        {"author", "Test Author"},
        {"content", "This is a test article"}
    };
    json expectedOutput = {
        {"page", 1},
        {"lastPage", false},
        {"articles", json::array()}
    };
    for (int i = 0; i < 11; ++i) {
        json correctArticle = articlePayload;
        ah.create(correctArticle);

        json formatedArticle = {};
        formatedArticle["id"] = correctArticle["id"];
        formatedArticle["title"] = articlePayload["title"];
        formatedArticle["date"] = articlePayload["date"];
        formatedArticle["author"] = articlePayload["author"];
        formatedArticle["content"] = articlePayload["content"];

        articlesToDelete.push_back(correctArticle["id"]);
        if( i > 0 ){
            expectedOutput["articles"].insert(expectedOutput["articles"].begin(),formatedArticle);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Ensure different last write time
    }

    // Test page is not integer
    httplib::Client client("http://localhost:5255");
    auto res = client.Get("/api/v1/knowledgebase?page=test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // Test page is wrong
    res = client.Get("/api/v1/knowledgebase?page=-1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // Test page is correct get 10 files and not last page
    res = client.Get("/api/v1/knowledgebase?page=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->body, expectedOutput.dump());

    // Test page is correct get 1 file and last page
    res = client.Get("/api/v1/knowledgebase?page=2");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);

    // Generate expected output
    expectedOutput["page"] = 2;
    expectedOutput["lastPage"] = true;
    expectedOutput["articles"] = json::array();
    json formatedArticle = {};
    formatedArticle["id"] = articlesToDelete.front();
    formatedArticle["title"] = articlePayload["title"];
    formatedArticle["date"] = articlePayload["date"];
    formatedArticle["author"] = articlePayload["author"];
    formatedArticle["content"] = articlePayload["content"];
    expectedOutput["articles"].push_back(formatedArticle);

    EXPECT_EQ(res->body, expectedOutput.dump());

    // Test page is correct get 0 files
    res = client.Get("/api/v1/knowledgebase?page=3");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 204);

    // Clean up
    for (const auto& article : articlesToDelete) {
        std::filesystem::remove(ah.getEyeCANPath() + article + ".json");
    }
}

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
    EXPECT_EQ(res->status, 404); // File not found

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

TEST_F(ApiTest, PostDatasetTest) {
    httplib::Client client("http://localhost:5255");

    // Test Content-Type is not application/json
    auto res = client.Post("/api/v1/data", "","application/test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 415); // Unsupported Media Type

    // Test wrong json
    res = client.Post("/api/v1/data", "}}}This is not multipart data","multipart/form-data");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400); // Bad Request

    // Test successfully created rule
    const std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    const std::string multipartData =
        "--" + boundary + "\r\n" +
        "Content-Disposition: form-data; name=\"title\"\r\n\r\n" +
        "Test Dataset\r\n" +
        "--" + boundary + "\r\n" +
        "Content-Disposition: form-data; name=\"description\"\r\n\r\n" +
        "Test description\r\n" +
        "--" + boundary + "\r\n" +
        "Content-Disposition: form-data; name=\"mf4\"; filename=\"test.mf4\"\r\n" +
        "Content-Type: application/octet-stream\r\n\r\n" +
        "mf4 file content\r\n" +
        "--" + boundary + "\r\n" +
        "Content-Disposition: form-data; name=\"dbc\"; filename=\"test.dbc\"\r\n" +
        "Content-Type: application/octet-stream\r\n\r\n" +
        "dbc file content\r\n" +
        "--" + boundary + "--\r\n";

    res = client.Post("/api/v1/data", multipartData, "multipart/form-data; boundary=" + boundary);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 201); // Created

    json response = json::parse(res->body);
    std::string id = response["id"];

    int status = std::filesystem::exists(dh.getEyeCANPath()+id+".json");
    EXPECT_EQ(status, 1); // Ensure file exists

    // Clean up
    std::filesystem::remove(dh.getEyeCANPath()+id+".json");
}

TEST_F(ApiTest, PutDatasetTest) {
    httplib::Client client("http://localhost:5255");
    const json testDataset = {
        {"id","a1c0bdd4-a228-40a0-889a-516ff7b42393"},
        {"title", "Test Dataset"},
        {"description", "Test description"},
        {"selectedSignalValues", json::array()},
        {"signals", json::array()}
    };

    // Test Content-Type is not application/json
    auto res = client.Put("/api/v1/data", testDataset.dump(),"application/test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 415); // Unsupported Media Type

    // Test wrong json
    res = client.Put("/api/v1/data", "}}}This is not json","application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400); // Bad Request

    // Test file not found
    res = client.Put("/api/v1/data", testDataset.dump(),"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 404); // Not found

    // Create file to edit
    std::string id = testDataset["id"];
    std::ofstream fileToEdit(dh.getEyeCANPath()+id+".json");
    fileToEdit << "test";
    fileToEdit.close();

    // Test successfully edited rule
    res = client.Put("/api/v1/data", testDataset.dump() ,"application/json");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200); // Successfully edited rule

    std::string fileContent;
    std::ifstream editedFile(dh.getEyeCANPath()+id+".json");
    std::string content((std::istreambuf_iterator<char>(editedFile)), std::istreambuf_iterator<char>());
    editedFile.close();
    json expected = json::parse(content);
    EXPECT_EQ(expected.dump(), testDataset.dump()); // Ensure file content is the same as the testDataset

    // Clean up
    std::filesystem::remove(dh.getEyeCANPath()+id+".json");
}

TEST_F(ApiTest, DeleteDatasetTest) {
    httplib::Client client("http://localhost:5255");

    // Invalid uuid
    auto res = client.Delete("/api/v1/data?uuid=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // File not found
    std::string testUuid = "a1c0bdd4-a228-40a0-889a-516ff7b42393";
    res = client.Delete("/api/v1/data?uuid="+testUuid);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 404);

    // Create a file to delete
    std::ofstream file(dh.getEyeCANPath()+testUuid+".json");
    file << "test";
    file.close();

    // Successfully deleted file
    res = client.Delete("/api/v1/data?uuid="+testUuid);

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 204);
}

TEST_F(ApiTest, GetDatasetTest) {
    // Create test files
    std::list<std::string> datasetsToDelete = {};
    json datasetPayload = {
        {"title", "Test Dataset"},
        {"description", "Test description"},
        {"selectedSignalValues", json::array()},
        {"signals", json::array()}
    };
    json expectedOutput = {
        {"page", 1},
        {"lastPage", false},
        {"datasets", json::array()}
    };
    for (int i = 0; i < 11; ++i) {
        json correctDataset = datasetPayload;
        dh.create(correctDataset);

        json formatedDataset = {};
        formatedDataset["id"] = correctDataset["id"];
        formatedDataset["title"] = datasetPayload["title"];
        formatedDataset["description"] = datasetPayload["description"];
        formatedDataset["selectedSignalValues"] = datasetPayload["selectedSignalValues"];
        formatedDataset["signals"] = datasetPayload["signals"];

        datasetsToDelete.push_back(correctDataset["id"]);
        if( i > 0 ){
            expectedOutput["datasets"].insert(expectedOutput["datasets"].begin(),formatedDataset);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Ensure different last write time
    }

    // Test page is not integer
    httplib::Client client("http://localhost:5255");
    auto res = client.Get("/api/v1/data?page=test");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // Test page is wrong
    res = client.Get("/api/v1/data?page=-1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 400);

    // Test page is correct get 10 files and not last page
    res = client.Get("/api/v1/data?page=1");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->body, expectedOutput.dump());

    // Test page is correct get 1 file and last page
    res = client.Get("/api/v1/data?page=2");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 200);

    // Generate expected output
    expectedOutput["page"] = 2;
    expectedOutput["lastPage"] = true;
    expectedOutput["datasets"] = json::array();
    json formatedDataset = {};
    formatedDataset["id"] = datasetsToDelete.front();
    formatedDataset["title"] = datasetPayload["title"];
    formatedDataset["description"] = datasetPayload["description"];
    formatedDataset["selectedSignalValues"] = datasetPayload["selectedSignalValues"];
    formatedDataset["signals"] = datasetPayload["signals"];
    expectedOutput["datasets"].push_back(formatedDataset);

    EXPECT_EQ(res->body, expectedOutput.dump());

    // Test page is correct get 0 files
    res = client.Get("/api/v1/data?page=3");

    ASSERT_NE(res, nullptr);  // Ensure response is not null
    EXPECT_EQ(res->status, 204);

    // Clean up
    for (const auto& dataset : datasetsToDelete) {
        std::filesystem::remove(dh.getEyeCANPath() + dataset + ".json");
    }
}