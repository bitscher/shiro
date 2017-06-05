#pragma once

#include <cstdint>

constexpr unsigned int BIT_MASK_N(unsigned int n) { return (1 << n); }

template<class numtype>
constexpr numtype AND_BIT_N(numtype value, unsigned int n) { return static_cast<numtype>(value & static_cast<numtype>(BIT_MASK_N(n))); }

template<class numtype>
constexpr numtype SET_BIT_N(numtype value, unsigned int n) { return static_cast<numtype>(value | static_cast<numtype>(BIT_MASK_N(n))); }

template<class numtype>
constexpr numtype CLEAR_BIT_N(numtype value, unsigned int n) { return static_cast<numtype>(value & ~(static_cast<numtype>(BIT_MASK_N(n)))); }

