//
// Created by bortoli on 05/03/25.
//

#ifndef APPLICATIONENTRYPOINT_H
#define APPLICATIONENTRYPOINT_H
#include "../core/Application.h"


class ApplicationEntrypoint final : rbengine::Application {
public:
    ApplicationEntrypoint();

    ~ApplicationEntrypoint() override;

    bool start() override;

    bool stop() override;
};


#endif //APPLICATIONENTRYPOINT_H
