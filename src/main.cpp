#include <iostream>

#include "sample/ApplicationEntrypoint.h"

int main() {
    ApplicationEntrypoint applicationEntryPoint;
    return rbengine::Application::getInstance()->run();
}
