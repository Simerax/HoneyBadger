#pragma once
#include <memory>

namespace HoneyBadger
{

template <typename T>
using Ref = std::shared_ptr<T>;
}
