// msgpack-visitor.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <msgpack.hpp>
#include <cstdint>
#include <cstddef>


#ifdef _NDEBUG
constexpr uint32_t N = 10'000'000;
#else
constexpr uint32_t N = 1'000;
#endif


int main()
{
    std::cout << "Hello CMake." << std::endl;
    return 0;
}
