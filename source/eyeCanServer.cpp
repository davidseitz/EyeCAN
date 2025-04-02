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

    if (!std::filesystem::exists("dist/"))
    {
        // Application files not found
        svr.Get("/", [this](const Request&, Response& res) {
            res.status = 404;
            res.set_content("Frontend files not found", "text/plain");
        });
    }else
    {
        // Serve the frontend files
        svr.set_mount_point("/", "dist/");
    }

    std::cout << "Server is running on http://localhost:5255/\n";
    svr.listen("0.0.0.0", 5255);
}

void EyeCANServer::initApiEndpoints() {

    if (!std::filesystem::exists("static/api/"))
    {
        // No description files found
        svr.Get("/api/v1/", [this](const Request&, Response& res) {
            res.status = 404;
            res.set_content("Api description not found", "text/plain");
        });
    }else
    {
        if  (!std::filesystem::exists("static/api/index.html") &&
            !std::filesystem::exists("static/api/eyecan-api-definition.json"))
        {
            // No description file found
            svr.Get("/api/v1/", [this](const Request&, Response& res) {
                res.status = 404;
                res.set_content("Api description not found", "text/plain");
            });
        }else if (std::filesystem::exists("static/api/index.html"))
        {
            // swagger-ui files found
            svr.set_mount_point("/api/v1/", "static/api/");
        }
        else
        {
            // Only JSON definition found
            svr.Get("/api/v1/", [this](const Request&, Response& res) {
                res.status = 404;
                const std::string jsonDefinition = readFile("/static/api/eyecan-api-definition.json");
                res.set_content(jsonDefinition, "application/json");
            });
        }
    }

    // Init endpoints for knowledgebase, filters and datasets
    initKnowledgebaseEndpoints();
    initFilterEndpoints();
    initDatasetEndpoints();

    // Stop server endpoint
    svr.Get("/api/v1/stop", [&](const Request&, Response& res) {
        res.set_content("Server is shutting down", "text/plain");
        svr.stop();
    });
}

void EyeCANServer::initKnowledgebaseEndpoints(){
    svr.Post("/api/v1/knowledgebase", [this](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                const int status = articleHandler.create(request_json);

                // Send JSON response
                if (status != 201)
                {
                    res.status = status; // Error
                }else
                {
                    res.status = status; // Created successfully
                    res.set_content(request_json.dump(), "application/json");
                }
            } catch (json::parse_error&) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Put("/api/v1/knowledgebase", [this](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                const int status = articleHandler.edit(request_json, request_json["id"]);

                res.status = status;
            } catch (json::parse_error&) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
            } catch (json::parse_error&) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Delete("/api/v1/knowledgebase", [this](const Request& req, Response& res) {
        const std::string uuid = req.get_param_value("uuid");

        const int status = articleHandler.remove(uuid);

        res.status = status;
    });

    svr.Get("/api/v1/knowledgebase", [this](const Request& req, Response& res) {
        const std::string val = req.get_param_value("page");
        if (val.empty()) {
            res.status = 400;
            return;
        }
        int page = -1;
        try{
            page = std::stoi(val);
        }catch (const std::invalid_argument&) {
            res.status = 400;
            return;
        }
        json response;
        const int status = articleHandler.get(page,response);

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
            } catch (json::parse_error&) {
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
            } catch (json::parse_error&) {
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
        }catch (const std::invalid_argument&) {
            res.status = 400;
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
    svr.Post("/api/v1/data", [this](const Request& req, Response& res) {
        if (req.is_multipart_form_data()) {
            try {
                std::string title, description;
                std::string mf4_content, dbc_content;

                // Fetch each file separately

                for (const auto& file : req.files) {
                    if (file.first == "title") {
                        title = file.second.content;  // Extract text field
                    }
                    if (file.first == "description") {
                        description = file.second.content;  // Extract text field
                    }
                    if (file.first == "mf4") {
                        mf4_content = file.second.content;  // Extract file content
                    }
                    if (file.first == "dbc") {
                        dbc_content = file.second.content;  // Extract file content
                    }
                }

                if (title.empty() || description.empty() || mf4_content.empty() || dbc_content.empty()) {
                    res.status = 400; // Bad Request
                    res.set_content(R"({"error": "Missing required fields or files"})", "application/json");
                    return;
                }

                // Create JSON object from the parsed data
                json request_json = {
                    {"title", title},
                    {"description", description},
                    {"selectedSignalValues", json::array()},
                };

                // TODO fuse dataset here

                // Mock for testing
                request_json["signals"] = json::array();

                const int status = datasetHandler.create(request_json);

                // Send JSON response
                if (status != 201) {
                    res.status = status; // Error
                } else {
                    res.status = status; // Created successfully
                    res.set_content(request_json.dump(), "application/json");
                }
            } catch (const std::exception& e) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid multipart form data"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected multipart form data"})", "application/json");
        }
    });

    svr.Put("/api/v1/data", [this](const Request& req, Response& res) {
        if (req.get_header_value("Content-Type") == "application/json") {
            try {
                // Parse JSON request body
                json request_json = json::parse(req.body);

                const int status = datasetHandler.edit(request_json, request_json["id"]);

                res.status = status;
            } catch (json::parse_error&) {
                res.status = 400; // Bad Request
                res.set_content(R"({"error": "Invalid JSON"})", "application/json");
            }
        } else {
            res.status = 415; // Unsupported Media Type
            res.set_content(R"({"error": "Expected JSON format"})", "application/json");
        }
    });

    svr.Delete("/api/v1/data", [this](const Request& req, Response& res) {
        const std::string uuid = req.get_param_value("uuid");

        const int status = datasetHandler.remove(uuid);

        res.status = status;
    });

    svr.Get("/api/v1/data", [this](const Request& req, Response& res) {
        const std::string val = req.get_param_value("page");
        int page = -1;
        try{
            page = std::stoi(val);
        }catch (const std::invalid_argument&) {
            res.status = 400;
            return;
        }
        json response;
        const int status = datasetHandler.get(page,response);

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
