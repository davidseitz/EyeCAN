#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>
#include <eyeCanServer.h>


using namespace httplib;
using json = nlohmann::ordered_json;

void EyeCANServer::initServer() {

    // Set pre-routing handler to allow CORS for api requests
    svr.set_pre_routing_handler([this](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        if (req.method == "OPTIONS") {
            res.status = 204;
            return Server::HandlerResponse::Handled;
        }
        return Server::HandlerResponse::Unhandled;
    });

    initApiEndpoints();

    svr.Get("/", [this](const Request&, Response& res) {
        res.set_content("Here runs the application", "text/plain");
    });

    std::cout << "Server is running on http://localhost:5255/\n";
    svr.listen("0.0.0.0", 5255);
}

void EyeCANServer::initApiEndpoints() {

    //TODO Rework check for swagger io
    std::string exists = readFile("static/api/index.html"); // Check if index for swagger io exists

    if (exists == "<h1>404 Not Found</h1><p>File not found.</p>") { // If not found, use JSON file as api definition
        std::cout << "Using JSON file as API definition\n";
        svr.Get("/api/v1/", [](const Request& req, Response& res) {
            std::string json = readFile("static/api/eyecan-api-definition.json");
            res.set_content(json, "application/json");
        });
    } else {
        svr.set_mount_point("/api/v1", "static/api");
    }

    initKnowledgebaseEndpoints();
    initFilterEndpoints();
    initDatasetEndpoints();

    svr.Get("/api/v1/stop", [&](const Request&, Response& res) {
        res.set_content("Server is shutting down", "text/plain");
        svr.stop();
    });
}

void EyeCANServer::initKnowledgebaseEndpoints(){
    svr.Post("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                // TODO Save the knowledgebase article
                // Set ID for testing
                request_json["id"] = "2";

                // Send JSON response
                res.status = 201; // Created
                res.set_content(request_json.dump(), "application/json");
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Put("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                // TODO Update the knowledgebase article

                // Send JSON response
                res.set_content(request_json.dump(), "application/json");
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Delete("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        std::string uuid = req.get_param_value("uuid");

        // TODO Delete the knowledgebase article

        res.status = 204 ; // No Content
        res.set_content("The Article you deleted with id: " + uuid, "text/plain");
    });

    svr.Get("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        std::string name = req.get_param_value("page");

        // TODO Get the knowledgebase article

        res.status = 200;
        res.set_content("The Page you requested: " + name, "text/plain");
    });
}

void EyeCANServer::initFilterEndpoints() {
    svr.Post("/api/v1/filters", [this](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                const int status = ruleHandler.create(request_json);

                // Send JSON response
                if (status != 201)
                {
                    res.status = status; // Error
                }else
                {
                    res.status = status; // Created successfully
                    res.set_content(request_json.dump(), "application/json");
                }
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Put("/api/v1/filters", [this](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                const int status = ruleHandler.edit(request_json, request_json["id"]);

                res.status = status;
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Delete("/api/v1/filters", [this](const Request& req, Response& res) {
        const std::string uuid = req.get_param_value("uuid");

        const int status = ruleHandler.remove(uuid);

        res.status = status;
    });

    svr.Get("/api/v1/filters", [this](const Request& req, Response& res) {
        const std::string val = req.get_param_value("page");
        int page = -1;
        try{
            page = std::stoi(val);
        }catch (std::invalid_argument& e) {
            res.status = 400;
            res.set_content("Invalid page number", "text/plain");
            return;
        }
        json response;
        const int status = ruleHandler.get(page,response);

        if (status != 200)
        {
            res.status = status;
        }
        else
        {
            res.status = status;
            res.body = response.dump();
        }
    });

    /*
    svr.Get("/api/v1/useFilter", [](const Request& req, Response& res) {
        std::string name = req.get_param_value("page");

        // TODO Use filter

        res.set_content("The Page you requested: " + name, "text/plain");
    });
    */
}

void EyeCANServer::initDatasetEndpoints()
{
    svr.Post("/api/v1/data", [](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                // TODO Save the dataset
                // Set ID for testing
                request_json["id"] = "1";

                // Send JSON response
                res.status = 201; // Created
                res.set_content(request_json.dump(), "application/json");
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Put("/api/v1/data", [](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                // TODO Update the dataset

                // Send JSON response
                res.set_content(request_json.dump(), "application/json");
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Delete("/api/v1/data", [](const Request& req, Response& res) {
        std::string uuid = req.get_param_value("uuid");

        // TODO Delete the dataset
        res.status = 204 ; // No Content
        res.set_content("The Dataset you deleted with id: " + uuid, "text/plain");
    });

    svr.Get("/api/v1/data", [](const Request& req, Response& res) {
        std::string name = req.get_param_value("page");

        // TODO Get the dataset

        res.set_content("The Page you requested: " + name, "text/plain");
    });
}

std::string EyeCANServer::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return "<h1>404 Not Found</h1><p>File not found.</p>";
    }
    std::ostringstream ss;
    ss << file.rdbuf();  // Read binary file correctly
    return ss.str();
}
