// msgpack-visitor.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <msgpack.hpp>
#include <cstdint>
#include <cstddef>
#include <random>


#ifdef _NDEBUG
constexpr uint32_t N = 10'000'000;
#else
constexpr uint32_t N = 1'000;
#endif

void create_msgpack(msgpack::sbuffer& buffer) {
    msgpack::packer<msgpack::sbuffer> packer(buffer);
    packer.pack_array(N);
    std::mt19937 gen(0);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (uint32_t i = 0; i != N; ++i) {
        packer.pack_double(dis(gen));
    }
}

int main()
{
    std::cout << "Hello CMake." << std::endl;
    msgpack::sbuffer buffer;
    create_msgpack(buffer);
    return 0;
}
