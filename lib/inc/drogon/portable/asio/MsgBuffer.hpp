//
// Created by neo on 5/4/24.
//

#ifndef DROGON_MSGBUFFER_HPP
#define DROGON_MSGBUFFER_HPP

#include <drogon/portable/Common.hpp>

namespace drogon
{

static constexpr size_t kBufferDefaultLength{2048};
static constexpr char CRLF[]{"\r\n"};

class MsgBufferImpl;

class MsgBuffer
{
  public:
    explicit MsgBuffer(size_t len = kBufferDefaultLength);

    MsgBuffer(const MsgBuffer& msg);

    MsgBuffer(MsgBuffer&& other) noexcept
    {
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }

    MsgBuffer& operator=(MsgBuffer&& other) noexcept;

    virtual ~MsgBuffer();

    void append(const void* data, std::size_t len);

    void append(const MsgBuffer& buffer);

    void append(const std::string& str);

    void ensureWritableBytes(std::size_t len);

    char* beginWrite();

    [[nodiscard]] const char* peek() const;

    [[nodiscard]] uint8_t peekInt8() const
    {
        assert(readableBytes() >= 1);
        return *(static_cast<const uint8_t *>((void *)peek()));
    }

    void swap(MsgBuffer &buf) noexcept;

    /**
     * @brief Put new data to the beginning of the buffer.
     *
     * @param buf
     * @param len
     */
    void addInFront(const char *buf, size_t len);

    /**
     * @brief Put a byte value to the beginning of the buffer.
     *
     * @param b
     */
    void addInFrontInt8(const uint8_t b)
    {
        addInFront(static_cast<const char *>((void *)&b), 1);
    }

    /**
     * @brief Put a unsigned short value to the beginning of the buffer.
     *
     * @param s
     */
    void addInFrontInt16(uint16_t s);

    /**
     * @brief Put a unsigned int value to the beginning of the buffer.
     *
     * @param i
     */
    void addInFrontInt32(uint32_t i);

    /**
     * @brief Put a unsigned int64 value to the beginning of the buffer.
     *
     * @param l
     */
    void addInFrontInt64(uint64_t l);

    void hasWritten(std::size_t len);

    void unwrite(int len);

    [[nodiscard]] std::size_t readableBytes() const;

    [[nodiscard]] std::size_t writableBytes() const;

    const char* findCRLF();

    void retrieveUntil(const char* end);

    void retrieve(size_t len);

    void retrieveAll();

    const char& operator[](size_t offset) const;

    char& operator[](size_t offset);

    MsgBufferImpl* impl_;
};

}  // namespace drogon

#endif  // DROGON_MSGBUFFER_HPP
