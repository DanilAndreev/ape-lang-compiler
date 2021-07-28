//
// Created by danil on 28.07.21.
//

#include "ApeBinaryReader.h"
#include "Encoding.h"

ApeBinaryReader::ApeBinaryReader(std::istream *binary) {
    this->binary = binary;
}

void ApeBinaryReader::nextToken() {
    const ENCODING_TYPES type = static_cast<ENCODING_TYPES>(this->binary->get());
    void *buffer_meta[sizeof(std::int32_t)];
    this->binary->read(reinterpret_cast<char *>(buffer_meta), sizeof(std::int32_t));
    std::int32_t *meta = reinterpret_cast<std::int32_t *>(buffer_meta);
    void *buffer = nullptr;
    switch (type) {
        case ENCODING_TYPES::COMMAND:
            break;
        case ENCODING_TYPES::STRING:
            buffer = new char[*meta * sizeof(char)];
            this->binary->read(reinterpret_cast<char *>(buffer), static_cast<std::streamsize>(*meta * sizeof(char)));

        default:
            break;
    }
    delete buffer;
}
