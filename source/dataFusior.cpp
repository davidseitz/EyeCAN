//
// Created by eyecandevelopment on 25.02.25.
//
#include "dataFusior.h"

#include <fstream>
#include <dbcppp/Network.h>
#include <mdflibrary/MdfChannelObserver.h>
#include <mdflibrary/MdfReader.h>
#include <codecvt>
#include <locale>

std::ofstream outFile;
nlohmann::ordered_json jsonData;

DataFusior::DataFusior() {
    // Constructor

    jsonData["name"] = "FrontendBackendTemplate";
    jsonData["version"] = "1.0.0";
    jsonData["description"] = "Template for Frontend and Backend";
    jsonData["signals"] = nlohmann::ordered_json::array();

    #ifdef __linux__
    char *homePath = std::getenv("HOME");
    std::filesystem::path dirPath = std::filesystem::path(homePath) / ".local" / "EyeCAN";
    std::filesystem::path filePath = dirPath / "fusedData.json";
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
    outFile.open(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    #elif defined(WINDOWS)
    // Windows-specific file path handling
    #elif defined(MAC)
    // macOS-specific file path handling
    #endif
}

// Function to check if a string is valid UTF-8
bool isValidUTF8(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    try {
        std::wstring wideStr = converter.from_bytes(str);
    } catch (const std::range_error&) {
        return false;
    }
    return true;
}

// Function to sanitize a string by removing invalid UTF-8 characters
std::string sanitizeUTF8(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideStr;
    try {
        wideStr = converter.from_bytes(str);
    } catch (const std::range_error&) {
        wideStr = L"";
    }
    return converter.to_bytes(wideStr);
}

void DataFusior::readAllData(const std::string dbcFilepath, const std::string mdfFilepath) {
    // Read DBC file to get signal information
    std::ifstream dbcFile(dbcFilepath);
    if (dbcFile.is_open()) {
        auto network = dbcppp::INetwork::LoadDBCFromIs(dbcFile);
        if (network) {
            for (const auto& message : network->Messages()) {
                for (const auto& signal : message.Signals()) {
                    std::string signalName = signal.Name();
                    std::string signalUnit = signal.Unit();

                    // Sanitize strings before adding to JSON
                    if (!isValidUTF8(signalName)) {
                        signalName = sanitizeUTF8(signalName);
                    }
                    if (!isValidUTF8(signalUnit)) {
                        signalUnit = sanitizeUTF8(signalUnit);
                    }

                    nlohmann::ordered_json signalJson;
                    signalJson["signalname"] = signalName;
                    signalJson["signalunit"] = signalUnit;
                    signalJson["signalvalues"] = nlohmann::ordered_json::array();
                    signalJson["messageID"] = message.Id();

                    // Read corresponding MDF data for this signal
                    MdfLibrary::MdfReader reader(mdfFilepath.c_str());
                    reader.ReadEverythingButData();
                    MdfLibrary::MdfHeader header = reader.GetHeader();

                    for (const auto& dataGroup : header.GetDataGroups()) {
                        reader.ReadData(dataGroup);
                        for (const auto& channelGroup : dataGroup.GetChannelGroups()) {
                            for (const auto& channel : channelGroup.GetChannels()) {
                                if (channel.GetName() == signal.Name()) {
                                    MdfLibrary::MdfChannelObserver observer(dataGroup, channelGroup, channel);
                                    for (size_t i = 0; i < channelGroup.GetNofSamples(); i++) {
                                        double channelValue, engValue;
                                        observer.GetChannelValue(i, channelValue);
                                        observer.GetEngValue(i, engValue);
                                        signalJson["signalvalues"].push_back({channelValue, engValue});
                                    }
                                }
                            }
                        }
                    }
                    jsonData["signals"].push_back(signalJson);
                }
            }
        }
        dbcFile.close();
    } else {
        std::cerr << "Failed to open DBC file" << std::endl;
    }
}

nlohmann::ordered_json DataFusior::getFusedData() {
    return m_fusedData;
}

DataFusior::~DataFusior() {
    // Destructor
    if (outFile.is_open()) {
        outFile << jsonData.dump(2);
        outFile.close();
        m_fusedData = jsonData;
    } else {
        std::cerr << "Outfile is not open" << std::endl;
    }
}