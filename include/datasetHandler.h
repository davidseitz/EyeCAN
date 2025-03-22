#ifndef DATASETHANDLER_H
#define DATASETHANDLER_H

#include <informationHandler.h>

class DatasetHandler : public InformationHandler {
public:
    DatasetHandler();
    ~DatasetHandler() override = default;
    int create(json& info) override;
    int get(const int page, json& response) override;
    std::string getEyeCANPath() const;
};

#endif //DATASETHANDLER_H
