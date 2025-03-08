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
                        MdfLibrary::MdfReader reader("../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4");
                        reader.ReadEverythingButData();
                        MdfLibrary::MdfHeader header = reader.GetHeader();

                        for (const auto& dataGroup : header.GetDataGroups()) {
                            reader.ReadData(dataGroup);
                            for (const auto& channelGroup : dataGroup.GetChannelGroups()) {
                                //std::cout << "ChannelGroup: " << channelGroup.GetName() << " " << channelGroup.GetIndex() << " " << channelGroup.GetRecordId() << " " << channelGroup.GetSourceInformation().GetDescription() << " end" << std::endl;
                                for (const auto& channel : channelGroup.GetChannels()) {
                                    if (channel.GetName() == "CAN_DataFrame.ID") {
                                        MdfLibrary::MdfChannelObserver observer(dataGroup, channelGroup, channel);
                                        std::cout << "Number of samples: " << channelGroup.GetNofSamples() << std::endl;

                                        for (size_t i = 0; i < channelGroup.GetNofSamples(); i++) {
                                            double canIdRaw = -1;
                                            observer.GetChannelValue(i, canIdRaw);

                                            // Convert to correct type
                                            auto n_canId = static_cast<uint32_t>(canIdRaw);
                                            std::cout << "Found CAN ID: " << n_canId << std::endl;
                                        }
                                    }
                                    if (channel.GetIndex() == message.Id()) {
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


                                        if (!valueFound) {
                                            std::cerr << "No values found for signal: " << signal.Name() << std::endl;
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