
#pragma once
#include<string>

namespace HoneyBadger
{
// why using string you say?
// well we might implement our own string class or change to utf8 compatible strings
// in case we do that I don't want to change all my code but just replace this one thing here
using string = std::string;
}
