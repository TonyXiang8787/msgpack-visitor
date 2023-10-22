// msgpack-visitor.cpp : Defines the entry point for the application.
//

#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <msgpack.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <vector>

// single data
namespace {
constexpr std::string_view json_single = R"(
{
  "version": "1.0",
  "type": "input",
  "is_batch": false,
  "attributes": {
    "node": [
      "id",
      "u_rated"
    ],
    "sym_load": [
      "id",
      "node",
      "status",
      "type",
      "p_specified",
      "q_specified"
    ],
    "source": [
      "id",
      "node",
      "status",
      "u_ref",
      "sk"
    ]
  },
  "data": {
    "node": [
      [
        1,
        10.5e3
      ],
      [
        2,
        10.5e3
      ],
      [
        3,
        10.5e3
      ]
    ],
    "line": [
      {
        "id": 4,
        "from_node": 1,
        "to_node": 2,
        "from_status": 1,
        "to_status": 1,
        "r1": 0.11,
        "x1": 0.12,
        "c1": 4e-05,
        "tan1": 0.1,
        "i_n": 500.0
      },
      {
        "id": 5,
        "from_node": 2,
        "to_node": 3,
        "from_status": 1,
        "to_status": 1,
        "r1": 0.15,
        "x1": 0.16,
        "c1": 5e-05,
        "tan1": 0.12,
        "i_n": 550.0
      }
    ],
    "source": [
      [
        15,
        1,
        1,
        1.03,
        1e20
      ],
      [
        16,
        1,
        1,
        1.04,
        null
      ],
      {
        "id": 17,
        "node": 1,
        "status": 1,
        "u_ref": 1.03,
        "sk": 1e10,
        "rx_ratio": 0.2
      }
    ],
    "sym_load": [
      [
        7,
        2,
        1,
        0,
        1.01e6,
        0.21e6
      ],
      [
        8,
        3,
        1,
        0,
        1.02e6,
        0.22e6
      ],
      [
        36,
        3,
        1,
        0,
        3,
        0.22e6
      ],
      [
        37,
        3,
        1,
        0,
        5,
        7
      ]
    ]
  }
}
)";

} // namespace

// batch data
namespace {
constexpr std::string_view json_batch = R"(
{
  "version": "1.0",
  "type": "update",
  "is_batch": true,
  "attributes": {
    "sym_load": [
      "id",
      "p_specified",
      "q_specified"
    ],
    "asym_load": [
      "id",
      "p_specified"
    ]
  },
  "data": [
    {
      "sym_load": [
        [
          7,
          20.0,
          50.0
        ]
      ],
      "asym_load": [
        [
          9,
          [
            100.0,
            null,
            200.0
          ]
        ]
      ]
    },
    {
      "asym_load": [
        [
          9,
          null
        ]
      ]
    },
    {
      "sym_load": [
        [
          7,
          null,
          10.0
        ],
        {
          "id": 8,
          "status": 0
        }
      ],
      "asym_load": [
        {
          "id": 9,
          "q_specified": [
            70.0,
            80.0,
            90.0
          ]
        }
      ]
    },
    {
      "sym_load": [
        [
          37,
          4,
          5
        ]
      ],
      "asym_load": [
        {
          "id": 31,
          "q_specified": [
            2,
            80.0,
            3
          ],
          "p_specified": [
            4,
            75.0,
            1
          ]
        }
      ]
    }
  ]
}
)";

} // namespace

class SerializationError : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

// default null visitor with parse error
struct DefaultNullVisitor : msgpack::null_visitor {
    void parse_error(size_t /*parsed_offset*/, size_t /*error_offset*/) {
        throw SerializationError{"Error in parsing!\n"};
    }
    void insufficient_bytes(size_t /*parsed_offset*/, size_t /*error_offset*/) {
        throw SerializationError{"Error in parsing!\n"};
    }
};

// default error visitor
template <class T> struct DefaultErrorVisitor : DefaultNullVisitor {
    static constexpr std::string_view static_err_msg = "Unexpected data type !\n";

    bool visit_nil() { return throw_error(); }
    bool visit_boolean(bool /*v*/) { return throw_error(); }
    bool visit_positive_integer(uint64_t /*v*/) { return throw_error(); }
    bool visit_negative_integer(int64_t /*v*/) { return throw_error(); }
    bool visit_float32(float /*v*/) { return throw_error(); }
    bool visit_float64(double /*v*/) { return throw_error(); }
    bool visit_str(const char* /*v*/, uint32_t /*size*/) { return throw_error(); }
    bool visit_bin(const char* /*v*/, uint32_t /*size*/) { return throw_error(); }
    bool visit_ext(const char* /*v*/, uint32_t /*size*/) { return throw_error(); }
    bool start_array(uint32_t /*num_elements*/) { return throw_error(); }
    bool start_array_item() { return throw_error(); }
    bool end_array_item() { return throw_error(); }
    bool end_array() { return throw_error(); }
    bool start_map(uint32_t /*num_kv_pairs*/) { return throw_error(); }
    bool start_map_key() { return throw_error(); }
    bool end_map_key() { return throw_error(); }
    bool start_map_value() { return throw_error(); }
    bool end_map_value() { return throw_error(); }
    bool end_map() { return throw_error(); }

    bool throw_error() { throw SerializationError{(static_cast<T&>(*this)).get_err_msg()}; }

    std::string get_err_msg() { return std::string{T::static_err_msg}; }
};

std::vector<char> create_msgpack(std::string_view json_string) {
    std::vector<char> res;
    auto const json = nlohmann::json::parse(json_string);
    nlohmann::json::to_msgpack(json, res);
    return res;
}

struct MapSizeVisitor : DefaultErrorVisitor<MapSizeVisitor> {
    uint32_t map_size{};
    bool start_map(uint32_t num_kv_pairs) {
        map_size = num_kv_pairs;
        return true;
    }
    bool start_map_key() { return false; }
    bool end_map() { return false; }
};

struct MapKeyVisitor : DefaultErrorVisitor<MapKeyVisitor> {
    std::string_view str{};
    bool visit_str(const char* v, uint32_t size) {
        str = {v, size};
        return true;
    }
};

std::string_view get_key(char const* data, size_t length, size_t& offset) {
    MapKeyVisitor visitor{};
    msgpack::parse(data, length, offset, visitor);
    return visitor.str;
}

void create_big_map(msgpack::sbuffer& buffer) {
    msgpack::packer<msgpack::sbuffer> packer{buffer};
    packer.pack_map(2000);
    for (int i = 0; i < 4000; ++i) {
        packer.pack(5.0);
    }
}

int main() {
    std::cout << "Hello CMake." << std::endl;
    auto const msgpack_data = create_msgpack(json_single);
    char const* const data = msgpack_data.data();
    size_t const length = msgpack_data.size();
    size_t offset{};
    // parse global dict
    MapSizeVisitor global_visitor{};
    msgpack::parse(data, length, offset, global_visitor);
    std::cout << get_key(data, length, offset) << '\n';

    // parse big array
    msgpack::sbuffer buffer;
    create_big_map(buffer);
    global_visitor = {};
    offset = 0;
    msgpack::parse(buffer.data(), buffer.size(), offset, global_visitor);
    std::cout << global_visitor.map_size << '\n';
    std::cout << offset << '\n';

    return 0;
}
