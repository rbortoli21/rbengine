//
// Created by bortoli on 05/03/25.
//

#ifndef APPLICATIONPROTOCOL_H
#define APPLICATIONPROTOCOL_H


class ApplicationInterface {
public:
    virtual ~ApplicationInterface() = default;

    virtual bool start() = 0;

    virtual bool stop() = 0;
};


#endif //APPLICATIONPROTOCOL_H
