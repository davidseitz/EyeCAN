/**
#include <eyecanserver.h>
#include "dataFusior.h"

int main() {
    DataFusior fusior;
    for (int i = 0; i < 1000; i++ ){
        fusior.readChannelDataByCanId(i);
        }
    return 0;
}
*/

#include <mdflibrary/MdfReader.h>
#include <mdflibrary/MdfChannelObserver.h>
#include <iostream>
#include <vector>
#include <cmath> // for std::abs
#include <memory>

int main() {
    MdfLibrary::MdfReader reader("../mf4Examples/GFR24-2024-08-14_19-09-12.mdf");
    reader.ReadEverythingButData();
    MdfLibrary::MdfHeader header = reader.GetHeader();

    std::string targetChannelName = "Your_Channel_Name"; // Channel to search for
    double targetValue = 123.45;                         // Value to search
    double tolerance = 0.01;                             // Tolerance for approximate match

    for (const auto& dataGroup : header.GetDataGroups()) {
        reader.ReadData(dataGroup); // Read data payload for each group

        for (const auto& channelGroup : dataGroup.GetChannelGroups()) {
            std::shared_ptr<MdfLibrary::MdfChannel> timeChannel = nullptr;
            std::shared_ptr<MdfLibrary::MdfChannel> targetChannel = nullptr;
            channelGroup.GetMasterChannel(timeChannel); // Get time channel
            // 1. Find time channel and target channel within the group
            for (const auto& channel : channelGroup.GetChannels()) {

                if (channel.GetName() == targetChannelName) {
                    targetChannel = channel;
                }
            }

            // If we found both channels
            if (timeChannel && targetChannel) {
                // 2. Read time data
                auto timeReader = timeChannel->GetChannelReader();
                std::vector<double> timestamps;
                while (timeReader->Read()) {
                    timestamps.push_back(timeReader->Value<double>());
                }

                // 3. Read target channel data
                auto valueReader = targetChannel->GetChannelReader();
                std::vector<double> samples;
                while (valueReader->Read()) {
                    samples.push_back(valueReader->Value<double>());
                }

                // 4. Make sure both vectors are the same length
                if (timestamps.size() != samples.size()) {
                    std::cerr << "Mismatch between timestamps and samples count!\n";
                    continue; // Skip to next channel group
                }

                // 5. Find a specific index (example: index 42)
                size_t targetIndex = 42; // Example index
                if (targetIndex < samples.size()) {
                    std::cout << "At index " << targetIndex
                              << ": Sample = " << samples[targetIndex]
                              << ", Timestamp = " << timestamps[targetIndex] << " s\n";
                } else {
                    std::cerr << "Index out of bounds.\n";
                }

                // 6. Search for target value within tolerance
                bool found = false;
                for (size_t i = 0; i < samples.size(); ++i) {
                    if (std::abs(samples[i] - targetValue) <= tolerance) {
                        std::cout << "Found value ~" << targetValue
                                  << " at index " << i
                                  << ", Timestamp = " << timestamps[i] << " s\n";
                        found = true;
                        break; // stop after first match
                    }
                }
                if (!found) {
                    std::cout << "Value near " << targetValue << " not found in channel '" << targetChannelName << "'.\n";
                }

                // Optional: break out of loops if you only care about first match
                // return 0;
            }
        }
    }

    return 0;
}
