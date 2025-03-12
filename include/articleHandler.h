#ifndef ARTICLEHANDLER_H
#define ARTICLEHANDLER_H

#include <informationHandler.h>

class ArticleHandler : public InformationHandler {
public:
    ArticleHandler();
    ~ArticleHandler() override = default;
    int create(json& info) override;
    int get(const int page, json& response) override;
    std::string getEyeCANPath() const;
};

#endif //ARTICLEHANDLER_H
