#pragma once
namespace tw{
class Noncopyable{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable(const Noncopyable&&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;

};
}