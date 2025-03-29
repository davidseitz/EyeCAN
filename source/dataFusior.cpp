//
// Created by eyecandevelopment on 25.02.25.
//
#include "dataFusior.h"

#include <fstream>
#include <dbcppp/Network.h>
#include <mdflibrary/MdfChannelObserver.h>
#include <mdflibrary/MdfReader.h>

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

void DataFusior::readChannelDataByCanId(uint32_t canId) {
    // Read DBC file to get signal information
    std::filesystem::path mdfPath = std::filesystem::path("../mf4Examples") / "GFR24-2024-08-14_19-09-12.mdf";
    std::ifstream dbcFile("../dbcExamples/Vehicle.dbc");
    if (dbcFile.is_open()) {
        auto network = dbcppp::INetwork::LoadDBCFromIs(dbcFile);
        if (network) {
            for (const auto& message : network->Messages()) {
                if (message.Id() == canId) {
                    for (const auto& signal : message.Signals()) {
                        nlohmann::ordered_json signalJson;
                        signalJson["signalname"] = signal.Name();
                        signalJson["signalunit"] = signal.Unit();
                        signalJson["signalvalues"] = nlohmann::ordered_json::array();
                        signalJson["messageID"] = message.Id();

                        // Read corresponding MDF data for this signal
                        MdfLibrary::MdfReader reader("../mf4Examples/GFR24-2024-08-14_19-09-12.mdf");
                        jsonData["name"] = mdfPath.filename().string();
                        reader.ReadEverythingButData();
                        MdfLibrary::MdfHeader header = reader.GetHeader();

                        for (const auto& dataGroup : header.GetDataGroups()) {
                            reader.ReadData(dataGroup);
                            for (const auto& channelGroup : dataGroup.GetChannelGroups()) {
                                //std::cout << "ChannelGroup: " << channelGroup.GetName() << " " << channelGroup.GetIndex() << " " << channelGroup.GetRecordId() << " " << channelGroup.GetSourceInformation().GetDescription() << " end" << std::endl;
                                for (const auto& channel : channelGroup.GetChannels()) {
                                    if (channel.GetName().find("time") != std::string::npos) {
                                        std::cout << "Time channel found: " << channel.GetName() << std::endl;
                                    }
                                    if (channel.GetName() == signal.Name()) {
                                        MdfLibrary::MdfChannelObserver observer(dataGroup, channelGroup, channel);
                                        std::vector<std::vector<double>> values;
                                        bool valueFound = false;

                                        for (size_t i = 0; i < channelGroup.GetNofSamples(); i++) {
                                            double channelValue, engValue;
                                            observer.GetChannelValue(i, channelValue);
                                            observer.GetEngValue(i, engValue);
                                            signalJson["signalvalues"].push_back({channelValue, engValue});
                                            valueFound = true;
                                        }
                                        if (valueFound == false) {
                                            std::cerr << "No values found" << std::endl;
                                        }
                                    }

                                    else {
                                    }
                                }

                            }
                        }
                        jsonData["signals"].push_back(signalJson);
                    }
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