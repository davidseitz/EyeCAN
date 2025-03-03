#include <fstream>
#include <dbcppp/Network.h>

#include "dbcppp/CApi.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <mdflibrary/MdfChannelObserver.h>
#include <mdflibrary/MdfReader.h>

// from uapi/linux/can.h
using canid_t = uint32_t;
struct can_frame
{
	canid_t    can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
	uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
	uint8_t    __pad;   /* padding */
	uint8_t    __res0;  /* reserved / padding */
	uint8_t    __res1;  /* reserved / padding */
	uint8_t    data[8];
};

void receive_frame_data(can_frame* frame)
{
    // receive meaningful data


    // Example frame
    // Set data for test_msg_mux0 from src/Test/Test.dbc
    // The selcted mux does have the format
    /* BO_ 1 test_msg_mux0: 8 VehicleBus
     *  [...]
     *  SG_ s3_0 m3 : 8|8@1+ (1,-40) [0|0] "C" Receiver
     *  SG_ s3_1 m3 : 16|11@1+ (0.5,1) [0|0] "C" Receiver
     *  SG_ s3_2 m3 : 27|10@1+ (1,-20) [0|0] "C" Receiver
     *  SG_ s3_3 m3 : 37|10@1+ (1,0) [0|0] "C" Receiver
     *  SG_ s3_4 m3 : 47|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_5 m3 : 48|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_6 m3 : 49|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_7 m3 : 50|11@1+ (0.125,-40) [0|0] "C" Receiver
     *  SG_ s3_8 m3 : 61|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_9 m3 : 62|1@1+ (1,0) [0|0] "" Receiver
     *  SG_ s3_10 m3 : 63|1@1+ (1,0) [0|0] "" Receiver
     *  [...]
     */
    frame->can_id = 1;
    *reinterpret_cast<uint64_t*>(frame->data) = 0;
    // set mux_switch_value to 3 (m3)
    frame->data[0] |= 3;
    // set value for signal s3_1 to 13
    // expected output after decoding and rawToPhys: s3_1 = 15 * 0.5 + 1 = 8.5
    frame->data[2] |= 15;
}

void printCANSignals(const std::string& dbcFilePath) {
    // Read the DBC file into a string
    std::ifstream file(dbcFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open DBC file." << std::endl;
        return;
    }
    std::string dbcContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Parse the DBC file
    std::istringstream dbcStream(dbcContent);
    auto network = dbcppp::INetwork::LoadDBCFromIs(dbcStream);
    if (!network) {
        std::cerr << "Error: Failed to parse DBC file." << std::endl;
        return;
    }

    // Iterate over all messages and print signals
    for (const auto& message : network->Messages()) {
        std::cout << "Message: " << message.Name() << " (ID: " << message.Id() << ")\n";
        for (const auto& signal : message.Signals()) {
            std::cout << "  Signal: " << signal.Name()
                      << ", Start Bit: " << signal.StartBit()
                      << ", Length: " << signal.BitSize()
                      << ", Factor: " << signal.Factor()
                      << ", Offset: " << signal.Offset()
                      << "\n";
        }
    }
}


int readMdf() {
    {
        std::cout << "Read" << std::endl;
        MdfLibrary::MdfReader Reader("../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4");
        Reader.ReadEverythingButData();
        auto Header = Reader.GetHeader();
        std::cout << "Author: " << Header.GetAuthor().c_str() << std::endl;
        std::cout << "Department: " << Header.GetDepartment() << std::endl;
        std::cout << "Description: " << Header.GetDescription() << std::endl;
        std::cout << "Project: " << Header.GetProject() << std::endl;
        std::cout << "StartTime: " << Header.GetStartTime() << std::endl;
        std::cout << "Subject: " << Header.GetSubject() << std::endl;

        auto Historys = Header.GetFileHistorys();
        std::cout << "History: " << Historys.size() << std::endl;
        for (const auto& Histroy : Historys) {
            std::cout << "Time: " << Histroy.GetTime() << std::endl;
            std::cout << "Description: " << Histroy.GetDescription() << std::endl;
            std::cout << "ToolName: " << Histroy.GetToolName() << std::endl;
            std::cout << "ToolVendor: " << Histroy.GetToolVendor() << std::endl;
            std::cout << "ToolVersion: " << Histroy.GetToolVersion() << std::endl;
            std::cout << "UserName: " << Histroy.GetUserName() << std::endl;
            std::cout << std::endl;
        }

        auto DataGroups = Header.GetDataGroups();
        std::cout << "DataGroups: " << DataGroups.size() << std::endl;
        for (const auto& DataGroup : DataGroups) {
            auto ChannelGroups = DataGroup.GetChannelGroups();
            std::cout << "ChannelGroups: " << ChannelGroups.size() << std::endl;
            for (const auto& ChannelGroup : ChannelGroups) {
                std::cout << "Name: " << ChannelGroup.GetName() << std::endl;
                std::cout << "Description: " << ChannelGroup.GetDescription()
                          << std::endl;

                auto SourceInformation = ChannelGroup.GetSourceInformation();
                std::cout << "SI Name: " << SourceInformation.GetName() << std::endl;
                std::cout << "SI Path: " << SourceInformation.GetPath() << std::endl;
                std::cout << "SI Description: " << SourceInformation.GetDescription()
                          << std::endl;

                std::cout << "Nof Samples: " << ChannelGroup.GetNofSamples()
                          << std::endl;

                auto Channels = ChannelGroup.GetChannels();
                std::cout << "Channels: " << Channels.size() << std::endl;
                std::vector<MdfLibrary::MdfChannelObserver> Observers;
                for (const auto& Channel : Channels) {
                    std::cout << "Name: " << Channel.GetName() << std::endl;
                    std::cout << "Description: " << Channel.GetDescription() << std::endl;
                    std::cout << "Type: " << static_cast<int>(Channel.GetType())
                              << std::endl;
                    std::cout << "Sync: " << static_cast<int>(Channel.GetSync())
                              << std::endl;
                    std::cout << "DataType: " << static_cast<int>(Channel.GetDataType())
                              << std::endl;
                    std::cout << "DataBytes: " << Channel.GetDataBytes() << std::endl;
                    std::cout << "Unit: " << Channel.GetUnit() << std::endl;
                    std::cout << std::endl;

                    Observers.emplace_back(
                        DataGroup, ChannelGroup, Channel);
                }

                Reader.ReadData(DataGroup);

                for (size_t i = 0; i < ChannelGroup.GetNofSamples(); i++) {
                    std::cout << "Sample: " << i << std::endl;
                    for (const auto& Observer : Observers) {
                        switch (Observer.GetChannel().GetDataType()) {
                            case MdfLibrary::ChannelDataType::CanOpenDate:
                            case MdfLibrary::ChannelDataType::CanOpenTime: {
                                uint64_t channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                std::cout << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::UnsignedIntegerLe:
                            case MdfLibrary::ChannelDataType::UnsignedIntegerBe: {
                                uint64_t channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                std::cout << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::SignedIntegerLe:
                            case MdfLibrary::ChannelDataType::SignedIntegerBe: {
                                int64_t channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                std::cout << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::FloatLe:
                            case MdfLibrary::ChannelDataType::FloatBe: {
                                double channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                std::cout << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::StringAscii:
                            case MdfLibrary::ChannelDataType::StringUTF8:
                            case MdfLibrary::ChannelDataType::StringUTF16Le:
                            case MdfLibrary::ChannelDataType::StringUTF16Be: {
                                std::string channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                std::cout << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::MimeStream:
                            case MdfLibrary::ChannelDataType::MimeSample:
                            case MdfLibrary::ChannelDataType::ByteArray: {
                                std::vector<uint8_t> channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                std::cout << "Channel: " << channel_value.size()
                                          << ", Eng: " << eng_value.size() << std::endl;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
