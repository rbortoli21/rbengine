//
// Created by bortoli on 05/03/25.
//

#include "ApplicationEntrypoint.h"
#include <iostream>

bool ApplicationEntrypoint::start() {
    std::cout << "Starting game" << std::endl;
    return true;
}

bool ApplicationEntrypoint::stop() {
    std::cout << "Game was stopped" << std::endl;
    return true;
}
