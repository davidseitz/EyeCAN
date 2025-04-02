//
// Created by eyecandevelopment on 25.02.25.
//

#include "dbcReader.h"


void dbcReader::printCANSignals(const std::string& dbcFilePath) {
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
                      << ", Min: " << signal.Minimum()
                      << ", Max: " << signal.Maximum()
                      << ", Unit: " << signal.Unit()
                      << ", Comment: " << signal.Comment()
                      << "\n";
        }
    }
}
