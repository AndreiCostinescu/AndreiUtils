//
// Created by ga78cat on 24.03.2021.
//

#include <andrei_utils/classes/Buffer.h>
#include <andrei_utils/NetworkData.h>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

Buffer::Buffer(uint64_t bufferSize) :
        buffer(nullptr), bufferSize(0), bufferContentSize(0), referenceBuffer(nullptr), constReferenceBuffer(nullptr),
        bufferType(BufferType::BUFFER_LOCAL) {
    this->prepareBuffer(bufferSize);
}

Buffer::~Buffer() {
    this->reset();
}

void Buffer::reset() {
    delete[] this->buffer;
    this->buffer = nullptr;
    this->bufferSize = 0;
    this->bufferContentSize = 0;
    this->referenceBuffer = nullptr;
    this->constReferenceBuffer = nullptr;
    this->bufferType = BufferType::BUFFER_LOCAL;
}

Buffer *Buffer::copy() const {
    auto *copy = new Buffer(this->bufferSize);

    assert (copy->bufferSize == this->bufferSize);
    memcpy(copy->buffer, this->buffer, this->bufferSize);

    copy->bufferContentSize = this->bufferContentSize;

    copy->referenceBuffer = this->referenceBuffer;
    copy->constReferenceBuffer = this->constReferenceBuffer;
    copy->bufferType = this->bufferType;

    return copy;
}

void Buffer::setData(char *data, unsigned int dataSize, int start) {
    this->setData((const char *) data, dataSize, start);
}

void Buffer::setData(const char *data, unsigned int dataSize, int start) {
    this->setBufferContentSize(dataSize + start);
    memcpy(this->buffer + start, data, dataSize);
    this->bufferType = BufferType::BUFFER_LOCAL;
}

void Buffer::setReferenceToData(char *data, unsigned int dataSize) {
    this->referenceBuffer = data;
    this->bufferContentSize = dataSize;
    this->bufferType = BufferType::BUFFER_REFERENCE;
}

void Buffer::setConstReferenceToData(const char *data, unsigned int dataSize) {
    this->constReferenceBuffer = data;
    this->bufferContentSize = dataSize;
    this->bufferType = BufferType::BUFFER_CONST_REFERENCE;
}

void Buffer::setChar(char data, int position) {
    this->checkBufferContentSize(position + sizeof(char), true);
    this->buffer[position] = data;
    this->bufferType = BufferType::BUFFER_LOCAL;
}

void Buffer::setShort(short data, int position) {
    this->checkBufferContentSize(position + sizeof(short), true);
    NetworkData::shortToNetworkBytes(this->buffer, position, data);
    this->bufferType = BufferType::BUFFER_LOCAL;
}

void Buffer::setInt(int data, int position) {
    this->checkBufferContentSize(position + sizeof(int), true);
    NetworkData::intToNetworkBytes(this->buffer, position, data);
    this->bufferType = BufferType::BUFFER_LOCAL;
}

void Buffer::setLongLong(long long data, int position) {
    this->checkBufferContentSize(position + sizeof(long long), true);
    NetworkData::longLongToNetworkBytes(this->buffer, position, data);
    this->bufferType = BufferType::BUFFER_LOCAL;
}

void Buffer::setFloat(float data, int position) {
    this->checkBufferContentSize(position + sizeof(float), true);
    NetworkData::floatToNetworkBytes(this->buffer, position, data);
    this->bufferType = BufferType::BUFFER_LOCAL;
}

void Buffer::setDouble(double data, int position) {
    this->checkBufferContentSize(position + sizeof(double), true);
    NetworkData::doubleToNetworkBytes(this->buffer, position, data);
    this->bufferType = BufferType::BUFFER_LOCAL;
}

char Buffer::getChar(int position) {
    this->checkBufferContentSize(position + sizeof(char), false);
    return this->buffer[position];
}

short Buffer::getShort(int position) {
    this->checkBufferContentSize(position + sizeof(short), false);
    return NetworkData::networkBytesToShort(this->buffer, position);
}

int Buffer::getInt(int position) {
    this->checkBufferContentSize(position + sizeof(int), false);
    return NetworkData::networkBytesToInt(this->buffer, position);
}

long long Buffer::getLongLong(int position) {
    this->checkBufferContentSize(position + sizeof(long long), false);
    return NetworkData::networkBytesToLongLong(this->buffer, position);
}

float Buffer::getFloat(int position) {
    this->checkBufferContentSize(position + sizeof(float), false);
    return NetworkData::networkBytesToFloat(this->buffer, position);
}

double Buffer::getDouble(int position) {
    this->checkBufferContentSize(position + sizeof(double), false);
    return NetworkData::networkBytesToDouble(this->buffer, position);
}

bool Buffer::empty() const {
    return this->bufferContentSize == 0;
}

char *&Buffer::getBufferReference() {
    switch (this->bufferType) {
        case BUFFER_LOCAL: {
            return this->buffer;
        }
        case BUFFER_REFERENCE: {
            this->bufferType = BufferType::BUFFER_LOCAL;
            return this->referenceBuffer;
        }
        case BUFFER_CONST_REFERENCE: {
            // this->bufferType = BufferType::BUFFER_LOCAL;
            throw runtime_error("The const reference buffer is a const buffer, call getConstBuffer()");
        }
        default: {
            throw runtime_error("Unknown buffer type...");
        }
    }
}

char *Buffer::getBuffer() {
    char *result;
    switch (this->bufferType) {
        case BUFFER_LOCAL: {
            result = this->buffer;
            break;
        }
        case BUFFER_REFERENCE: {
            result = this->referenceBuffer;
            break;
        }
        case BUFFER_CONST_REFERENCE: {
            throw runtime_error("The const reference buffer is a const buffer, call getConstBuffer()");
        }
    }
    this->bufferType = BufferType::BUFFER_LOCAL;
    return result;
}

const char *Buffer::getConstBuffer() {
    const char *result;
    switch (this->bufferType) {
        case BUFFER_LOCAL: {
            result = this->buffer;
            break;
        }
        case BUFFER_REFERENCE: {
            result = this->referenceBuffer;
            break;
        }
        case BUFFER_CONST_REFERENCE: {
            result = this->constReferenceBuffer;
            break;
        }
    }
    this->bufferType = BufferType::BUFFER_LOCAL;
    return result;
}

uint64_t Buffer::getBufferSize() const {
    return this->bufferSize;
}

uint64_t Buffer::getBufferContentSize() const {
    return this->bufferContentSize;
}

void Buffer::setBufferContentSize(uint64_t _bufferContentSize) {
    this->prepareBuffer(_bufferContentSize);
    this->bufferContentSize = _bufferContentSize;
}

void Buffer::prepareBuffer(uint64_t desiredSize) {
    if (this->bufferSize < desiredSize) {
        char *oldBuffer = this->buffer;
        uint64_t oldSize = this->bufferSize;

        // cout << "Initialize new buffer!" << endl;
        this->buffer = new char[desiredSize];
        this->bufferSize = desiredSize;
        // cout << "Initialized new buffer (" << bufferLength << ")!" << endl;

        if (oldSize > 0) {
            memcpy(this->buffer, oldBuffer, oldSize * sizeof(*(this->buffer)));
        }
        memset(this->buffer + oldSize, 0, (this->bufferSize - oldSize) * sizeof(*(this->buffer)));

        delete[] oldBuffer;
    }
}

void Buffer::checkBufferContentSize(uint64_t size, bool modifySize) {
    if (size > this->bufferContentSize) {
        if (modifySize) {
            this->prepareBuffer(size);
            this->bufferContentSize = size;
        } else {
            cerr << "Size of " << size << " is greater than the current buffer content size "
                 << this->bufferContentSize << "! The next assertion will fail..." << endl;
        }
    }
    if (size > this->bufferContentSize) {
        cout << "Requested size = " << size << " vs. this->bufferContentSize = " << this->bufferContentSize << endl;
        assert (!modifySize);
    }
    assert (size <= this->bufferContentSize);
}
