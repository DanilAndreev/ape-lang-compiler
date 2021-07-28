//
// Created by danil on 28.07.21.
//

#ifndef APE_LANG_COMPILER_APEBINARYREADER_H
#define APE_LANG_COMPILER_APEBINARYREADER_H

#include <istream>

class ApeBinaryReader {
protected:
    std::istream* binary;
public:
    explicit ApeBinaryReader(std::istream* binary);
    ApeBinaryReader(const ApeBinaryReader& reader) = default;
    ~ApeBinaryReader() = default;
public:
    void nextToken();
};


#endif //APE_LANG_COMPILER_APEBINARYREADER_H
