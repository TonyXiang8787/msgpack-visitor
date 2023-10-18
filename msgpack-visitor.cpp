// msgpack-visitor.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <msgpack.hpp>
#include <cstdint>
#include <cstddef>
#include <random>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;

#ifdef NDEBUG
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

struct CountVisitor : msgpack::null_visitor {
    uint32_t count{};
    double sum{};
    bool start_array_item() {
        ++count;
        sum += 0.5;
        return true;
    }
};

struct SumVisitor : msgpack::null_visitor {
    uint32_t count{};
    double sum{};
    bool start_array_item() {
        ++count;
        return true;
    }
    bool visit_float64(double v) {
        sum += v;
        return true;
    }
};

int main()
{
    std::cout << "Hello CMake." << std::endl;
    msgpack::sbuffer buffer;
    create_msgpack(buffer);
    CountVisitor count_visitor{};
    SumVisitor sum_visitor{};
    auto const t1 = Clock::now();
    msgpack::parse(buffer.data(), buffer.size(), count_visitor);
    auto const t2 = Clock::now();
    msgpack::parse(buffer.data(), buffer.size(), sum_visitor);
    auto const t3 = Clock::now();
    auto const count_visitor_time = Duration(t2 - t1);
    auto const sum_visitor_time = Duration(t3 - t2);
    std::cout << "Count visitor, count: " << count_visitor.count << ", sum: " << count_visitor.sum << ", duration: " << count_visitor_time.count() << std::endl;
    std::cout << "Sum visitor, count: " << sum_visitor.count << ", sum: " << sum_visitor.sum << ", duration: " << sum_visitor_time.count() << std::endl;

    return 0;
}
