//
// Created by ga78cat on 24.03.2021.
//

#ifndef ANDREIUTILS_CLASSES_BUFFER_H
#define ANDREIUTILS_CLASSES_BUFFER_H

#include <cstdint>

namespace AndreiUtils {
    class Buffer {
    public:
        explicit Buffer(uint64_t bufferSize = 0);

        ~Buffer();

        void reset();

        [[nodiscard]] Buffer *copy() const;

        void setData(char *data, unsigned int dataSize, int start = 0);

        void setData(const char *data, unsigned int dataSize, int start = 0);

        void setReferenceToData(char *data, unsigned int dataSize);

        void setConstReferenceToData(const char *data, unsigned int dataSize);

        void setChar(char data, int position);

        void setShort(short data, int position);

        void setInt(int data, int position);

        void setLongLong(long long data, int position);

        void setFloat(float data, int position);

        void setDouble(double data, int position);

        char getChar(int position);

        short getShort(int position);

        int getInt(int position);

        long long getLongLong(int position);

        float getFloat(int position);

        double getDouble(int position);

        [[nodiscard]] bool empty() const;

        char *&getBufferReference();

        [[nodiscard]] char *getBuffer();

        [[nodiscard]] const char *getConstBuffer();

        [[nodiscard]] uint64_t getBufferSize() const;

        [[nodiscard]] uint64_t getBufferContentSize() const;

        void setBufferContentSize(uint64_t _bufferContentSize);

    private:
        enum BufferType {
            BUFFER_LOCAL = 0,
            BUFFER_REFERENCE = 1,
            BUFFER_CONST_REFERENCE = 2,
        };

        void prepareBuffer(uint64_t desiredSize);

        void checkBufferContentSize(uint64_t size, bool modifySize);

        char *buffer, *referenceBuffer;
        const char *constReferenceBuffer;
        uint64_t bufferSize, bufferContentSize;
        BufferType bufferType;
    };
}

#endif //ANDREIUTILS_CLASSES_BUFFER_H
