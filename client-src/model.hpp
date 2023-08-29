#pragma once

#include "../common-src/common_lib.hpp"
#include <boost/array.hpp>
#include <fmt/format.h>
#include <stdexcept>

struct Model {
    std::vector<Message> messages;
};
