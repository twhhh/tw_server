#pragma once
#include <memory>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
namespace tw{
//默认ByteArray存储为大端结构， 系统为小端存储
//因此写入要先转大端结构，读出要转小端结构
//从外面看就相当于都在用小端，忽略了大小端转换了细节
class ByteArray{
public:
    using ptr = std::shared_ptr<ByteArray>;
    struct Node{
        Node(size_t s);
        Node();
        ~Node();
        char* ptr;
        Node* next;
        size_t size;
    };
    ByteArray(size_t base_size = 4096);
    ~ByteArray();
    //无zigzag序列化写
    void writeFint8(int8_t value);
    void writeFuin8(uint8_t value);
    void writeFint16(int16_t value);
    void writeFuint16(uint16_t value);
    void writeFint32(int32_t value);
    void writeFuint32(uint32_t value);
    void writeFint64(int64_t value);
    void writeFuint64(uint64_t value);
    //zigzag序列化写
    void writeInt32(int32_t value);
    void wirteUint32(uint32_t value);
    void writeInt64(uint64_t value);
    void writeUint64(uint64_t value);
    //写浮点数
    void writeFloat(float value);
    void writeDouble(double value);
    //写string
    void writeStringF16(const std::string& value);
    void writeStringF32(const std::string& value);
    void writeStringF64(const std::string& value);
    void writeStringVint(const std::string& value);
    void writeStringWithoutLength(const std::string& value);
    //读写对应
    int8_t readFint8();
    uint8_t readFuint8();
    int16_t readFint16();
    uint16_t readFuint16();
    int32_t readFint32();
    uint32_t readFuint32();
    int64_t readFint64();
    uint64_t readFuint64();
    //
    int32_t readInt32();
    uint32_t readUint32();
    int64_t readInt64();
    uint64_t readUint64();
    //
    float readFloat();
    double readDouble();
    //
    std::string readStringF16();
    std::string readStringF32();
    std::string readStringF64();
    std::string readStringVint();

    bool writeToFile(const std::string& name) const;
    bool readFromFile(const std::string& name);

    std::string toString() const;
    std::string toHexString() const;

    uint64_t getReadBuffers(std::vector<iovec>& buffers, uint64_t len = ~0ull) const;
    uint64_t getReadBuffers(std::vector<iovec>& buffers, uint64_t len, uint64_t position) const;
    uint64_t getWriteBuffers(std::vector<iovec>& buffers, uint64_t len);
    void clear();
    void write(const void* buf, size_t size);
    void read(void* buf, size_t size);
    void read(void* buf, size_t size, size_t position) const;
    size_t getBaseSize() const {return m_baseSize;}
    size_t getPosition() const {return m_position;}
    void setPosition(size_t v);
    size_t getReadSize() const {return m_size - m_position;}
    size_t getSize() const {return m_size;}
private:
    void addCapacity(size_t moreSize);
    size_t getRemainCapacity() const {return m_capacity - m_position;}
    uint64_t ntohll(uint64_t s);
    uint64_t htonll(uint64_t s);
private:
    size_t m_baseSize;
    size_t m_position;
    size_t m_capacity;
    size_t m_size;
    Node* m_root;
    Node* m_cur;
};
}