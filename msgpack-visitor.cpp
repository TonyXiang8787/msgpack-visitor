// msgpack-visitor.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <msgpack.hpp>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string_view>
#include <nlohmann/json.hpp>
#include <exception>

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
        "inf",
        0.22e6
      ],
      [
        37,
        3,
        1,
        0,
        "-inf",
        "+inf"
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
          "-inf",
          "inf"
        ]
      ],
      "asym_load": [
        {
          "id": 31,
          "q_specified": [
            "inf",
            80.0,
            "+inf"
          ],
          "p_specified": [
            "-inf",
            75.0,
            "-inf"
          ]
        }
      ]
    }
  ]
}
)";

} // namespace

// default error visitor
struct DefaultErrorVisitor {
    bool visit_nil() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_boolean(bool /*v*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_positive_integer(uint64_t /*v*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_negative_integer(int64_t /*v*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_float32(float /*v*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_float64(double /*v*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_str(const char* /*v*/, uint32_t /*size*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_bin(const char* /*v*/, uint32_t /*size*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool visit_ext(const char* /*v*/, uint32_t /*size*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool start_array(uint32_t /*num_elements*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool start_array_item() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool end_array_item() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool end_array() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool start_map(uint32_t /*num_kv_pairs*/) {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool start_map_key() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool end_map_key() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool start_map_value() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool end_map_value() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    bool end_map() {
        throw std::exception{ "Unexpected data type!\n" };
    }
    void parse_error(size_t /*parsed_offset*/, size_t /*error_offset*/) {
        throw std::exception{ "Error in parsing!\n" };
    }
    void insufficient_bytes(size_t /*parsed_offset*/, size_t /*error_offset*/) {
        throw std::exception{ "Error in parsing!\n" };
    }
};


std::vector<char> create_msgpack(std::string_view json_string) {
    std::vector<char> res;
    auto const json = nlohmann::json::parse(json_string);
    nlohmann::json::to_msgpack(json, res);
    return res;
}


struct GlobalVisitor : DefaultErrorVisitor {
    uint32_t root_map_size{};
    bool start_map(uint32_t num_kv_pairs) {
        root_map_size = num_kv_pairs;
        return true;
    }
    bool start_map_key() {
        return false;
    }
};

struct RootVisitor : DefaultErrorVisitor {
    bool visit_str(const char* v, uint32_t size) {
        std::cout << std::string_view{ v, size } << '\n';
        return true;
    }
};


int main()
{
    std::cout << "Hello CMake." << std::endl;
    auto const msgpack_data = create_msgpack(json_single);
    char const* const data = msgpack_data.data();
    size_t const length = msgpack_data.size();
    size_t offset{};
    // parse global dict
    GlobalVisitor global_visitor{};
    msgpack::parse(data, length, offset, global_visitor);
    RootVisitor root_visitor{};
    msgpack::parse(data, length, offset, root_visitor);
    return 0;
}
