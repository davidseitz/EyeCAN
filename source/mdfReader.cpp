

#include "mdfReader.h"

//#include <mdflibrary/MdfChannelObserver.h>
//#include <mdflibrary/MdfReader.h>
#include <iostream>
#include <fstream>
#include <string>

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

/**
int readMdf() {
    {
        std::ofstream outputFile("output.txt");
        outputFile << "Read" << std::endl;
        MdfLibrary::MdfReader Reader("../mf4Examples/Testing_Ehingen_19d_2019-05-04_14-35-43.mf4");
        Reader.ReadEverythingButData();
        auto Header = Reader.GetHeader();
        outputFile << "Author: " << Header.GetAuthor().c_str() << std::endl;
        outputFile << "Department: " << Header.GetDepartment() << std::endl;
        outputFile << "Description: " << Header.GetDescription() << std::endl;
        outputFile << "Project: " << Header.GetProject() << std::endl;
        outputFile << "StartTime: " << Header.GetStartTime() << std::endl;
        outputFile << "Subject: " << Header.GetSubject() << std::endl;

        auto Historys = Header.GetFileHistorys();
        outputFile << "History: " << Historys.size() << std::endl;
        for (const auto& Histroy : Historys) {
            outputFile << "Time: " << Histroy.GetTime() << std::endl;
            outputFile << "Description: " << Histroy.GetDescription() << std::endl;
            outputFile << "ToolName: " << Histroy.GetToolName() << std::endl;
            outputFile << "ToolVendor: " << Histroy.GetToolVendor() << std::endl;
            outputFile << "ToolVersion: " << Histroy.GetToolVersion() << std::endl;
            outputFile << "UserName: " << Histroy.GetUserName() << std::endl;
            outputFile << std::endl;
        }

        auto DataGroups = Header.GetDataGroups();
        outputFile << "DataGroups: " << DataGroups.size() << std::endl;
        for (const auto& DataGroup : DataGroups) {
            auto ChannelGroups = DataGroup.GetChannelGroups();
            outputFile << "ChannelGroups: " << ChannelGroups.size() << std::endl;
            for (const auto& ChannelGroup : ChannelGroups) {
                outputFile << "Name: " << ChannelGroup.GetName() << std::endl;
                outputFile << "Description: " << ChannelGroup.GetDescription()
                          << std::endl;

                auto SourceInformation = ChannelGroup.GetSourceInformation();
                outputFile << "SI Name: " << SourceInformation.GetName() << std::endl;
                outputFile << "SI Path: " << SourceInformation.GetPath() << std::endl;
                outputFile << "SI Description: " << SourceInformation.GetDescription()
                          << std::endl;

                outputFile << "Nof Samples: " << ChannelGroup.GetNofSamples()
                          << std::endl;

                auto Channels = ChannelGroup.GetChannels();
                outputFile << "Channels: " << Channels.size() << std::endl;
                std::vector<MdfLibrary::MdfChannelObserver> Observers;
                for (const auto& Channel : Channels) {
                    outputFile << "Name: " << Channel.GetName() << std::endl;
                    outputFile << "Description: " << Channel.GetDescription() << std::endl;
                    outputFile << "Type: " << static_cast<int>(Channel.GetType())
                              << std::endl;
                    outputFile << "Sync: " << static_cast<int>(Channel.GetSync())
                              << std::endl;
                    outputFile << "DataType: " << static_cast<int>(Channel.GetDataType())
                              << std::endl;
                    outputFile << "DataBytes: " << Channel.GetDataBytes() << std::endl;
                    outputFile << "Unit: " << Channel.GetUnit() << std::endl;
                    outputFile << std::endl;

                    Observers.emplace_back(
                        DataGroup, ChannelGroup, Channel);
                }

                Reader.ReadData(DataGroup);

                for (size_t i = 0; i < ChannelGroup.GetNofSamples(); i++) {
                    outputFile << "Sample: " << i << std::endl;
                    for (const auto& Observer : Observers) {
                        switch (Observer.GetChannel().GetDataType()) {
                            case MdfLibrary::ChannelDataType::CanOpenDate:
                            case MdfLibrary::ChannelDataType::CanOpenTime: {
                                uint64_t channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                outputFile << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::UnsignedIntegerLe:
                            case MdfLibrary::ChannelDataType::UnsignedIntegerBe: {
                                uint64_t channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                outputFile << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::SignedIntegerLe:
                            case MdfLibrary::ChannelDataType::SignedIntegerBe: {
                                int64_t channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                outputFile << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::FloatLe:
                            case MdfLibrary::ChannelDataType::FloatBe: {
                                double channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                outputFile << "Channel: " << channel_value
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
                                outputFile << "Channel: " << channel_value
                                          << ", Eng: " << eng_value << std::endl;
                                break;
                            }
                            case MdfLibrary::ChannelDataType::MimeStream:
                            case MdfLibrary::ChannelDataType::MimeSample:
                            case MdfLibrary::ChannelDataType::ByteArray: {
                                std::vector<uint8_t> channel_value, eng_value;
                                Observer.GetChannelValue(i, channel_value);
                                Observer.GetEngValue(i, eng_value);
                                outputFile << "Channel: " << channel_value.size()
                                          << ", Eng: " << eng_value.size() << std::endl;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    outputFile << std::endl;
                }
                outputFile << std::endl;
            }
            outputFile << std::endl;
        }
        outputFile.close();
    }

    return 0;
}
**/