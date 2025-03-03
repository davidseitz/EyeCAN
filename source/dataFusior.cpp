//
// Created by eyecandevelopment on 25.02.25.
//
#include "dataFusior.h"

#include <fstream>
#include <dbcppp/Network.h>
#include <mdflibrary/MdfChannelObserver.h>
#include <mdflibrary/MdfReader.h>

DataFusior::DataFusior() {
    // Constructor
    #ifdef LINUX
    if (std::filesystem::exists(~/.local/EyeCAN/fusedData.json)) {
        std::ifstream file(~/.local/EyeCAN/fusedData.json);
        m_fusedData = nlohmann::ordered_json::parse(file);
        file.close();

    }else {
        m_fusedData = nlohmann::ordered_json();
    }
    #elif defined(WINDOWS)
    #elif defined(MAC)
    #endif


}

void DataFusior::changeDbcData() {
    nlohmann::ordered_json jsonData;
    jsonData["name"] = "FrontendBackendTemplate";
    jsonData["version"] = "1.0.0";
    jsonData["description"] = "Template for Frontend and Backend";
    jsonData["signals"] = nlohmann::ordered_json::array();

    // Read DBC file to get signal information
    std::ifstream dbcFile("../dbcExamples/Vehicle.dbc");
    if (dbcFile.is_open()) {
        auto network = dbcppp::INetwork::LoadDBCFromIs(dbcFile);
        if (network) {
            for (const auto& message : network->Messages()) {
                for (const auto& signal : message.Signals()) {
                    nlohmann::ordered_json signalJson;
                    signalJson["signalname"] = signal.Name();
                    signalJson["signalunit"] = signal.Unit();
                    signalJson["signalvalues"] = nlohmann::ordered_json::array();

                    // Read corresponding MDF data for this signal
                    // Note: You'll need to modify this part based on your actual MDF file structure
                    MdfLibrary::MdfReader reader("../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4");
                    reader.ReadEverythingButData();
                    MdfLibrary::MdfHeader header = reader.GetHeader();

                    for (const auto& dataGroup : header.GetDataGroups()) {
                        reader.ReadData(dataGroup);
                        for (const auto& channelGroup : dataGroup.GetChannelGroups()) {
                            for (const auto& channel : channelGroup.GetChannels()) {
                                if (channel.GetName() == signal.Name()) {
                                    MdfLibrary::MdfChannelObserver observer(dataGroup, channelGroup, channel);
                                    std::vector<std::vector<double>> values;

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
    }

    // Save the JSON data to disk
    +
























































        std::filesystem::create_directories(std::filesystem::path("~/.local/EyeCAN"));
    std::ofstream outFile("~/.local/EyeCAN/fusedData.json");
    if (outFile.is_open()) {
        outFile << jsonData.dump(2);
        outFile.close();
        m_fusedData = jsonData;
    }
}
