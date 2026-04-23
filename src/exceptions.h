#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

// i know i should only write the class skeleton here but im not sure which parts are the skeleton,

class BadARModelParameter : public std::runtime_error {
public:
    BadARModelParameter(const std::string& msg)
    : std::runtime_error(msg) {};
};

#endif