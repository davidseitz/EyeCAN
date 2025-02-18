#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include <eyecanserver.h>

using namespace httplib;
using json = nlohmann::json;

void EyeCANServer::initServer() {

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

    std::string exists = readFile("static/api/index.html");

    if (exists == "<h1>404 Not Found</h1><p>File not found.</p>") {
        svr.Get("/api/v1/", [](const Request& req, Response& res) {
            std::string yaml = readFile("static/api/eyecan-api-defintion.json");
            res.set_content(yaml, "application/json");
        });
    } else {
        svr.set_mount_point("/api/v1", "static/api");
    }

    svr.Post("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                // Extract values from JSON
                std::string title = request_json["title"];
                int date = request_json["date"];
                std::string author = request_json["author"];
                std::string content = request_json["content"];

                // Create a response JSON object
                json response_json;
                response_json["id"] = "1";
                response_json["title"] = title;
                response_json["date"] = date;
                response_json["author"] = author;
                response_json["content"] = content;

                // Send JSON response
                res.set_content(response_json.dump(), "application/json");
            } catch (json::parse_error& e) {
                res.status = 400; // Bad Request
                res.set_content("{\"error\": \"Invalid JSON\"}", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content("{\"error\": \"Expected JSON format\"}", "application/json");
        }
    });

    svr.Put("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        res.set_content("WIP", "text/plain");
    });

    svr.Delete("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        std::string uuid = req.get_param_value("uuid");
        res.set_content("The Article you deleted with id: " + uuid, "text/plain");
    });

    svr.Get("/api/v1/knowledgebase", [](const Request& req, Response& res) {
        std::string name = req.get_param_value("page");
        res.set_content("The Page you requested: " + name, "text/plain");
    });

    svr.Get("/api/v1/stop", [&](const Request&, Response& res) {
        res.set_content("Server is shutting down", "text/plain");
        svr.stop();
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
