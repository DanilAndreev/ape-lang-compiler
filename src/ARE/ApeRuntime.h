//
// Created by danil on 28.07.21.
//

#ifndef APE_LANG_COMPILER_APERUNTIME_H
#define APE_LANG_COMPILER_APERUNTIME_H

#include <istream>

class ApeRuntime {
protected:
    std::istream* program;
public:
    ApeRuntime(std::istream* program);
    ~ApeRuntime();
public:
    void execute();
};


#endif //APE_LANG_COMPILER_APERUNTIME_H
