#include "BitArray.h"
#include <iostream>
#include <string>

using namespace std;

BitArray::BitArray()
{
    bit_count = 0;
    array_size = 0;
    data = nullptr;
}

BitArray::~BitArray()
{
    delete[] data;
}

BitArray::BitArray(int num_bits, unsigned long value)
{
    if (num_bits <= 0)
    {
        throw invalid_argument("num_bits cannot be <= 0");
    }

    bit_count = num_bits;
    const int BITS_PER_ULONG = sizeof(unsigned long) * 8;
    array_size = (num_bits + BITS_PER_ULONG - 1) / BITS_PER_ULONG;

    data = new unsigned long[array_size];

    for (int i = 0; i < array_size; ++i) {
        data[i] = value;
    }
    if (array_size > 0) {
        int bits_in_last = num_bits % BITS_PER_ULONG;
        if (bits_in_last == 0) {
            bits_in_last = BITS_PER_ULONG;
        }
        if (bits_in_last < BITS_PER_ULONG) {
            unsigned long mask = (1UL << bits_in_last) - 1;
            data[array_size - 1] &= mask;
        }
    }
}

BitArray::BitArray(const BitArray& b)
{
    bit_count = b.bit_count;
    array_size = b.array_size;
    if (b.array_size > 0)
    {
        data = new unsigned long[array_size];
        memcpy(data, b.data, array_size * sizeof(unsigned long));
    }
    else
    {
        data = nullptr;
    }
}

void BitArray::swap(BitArray& b) noexcept
{
    if (this == &b) {
        return;
    }

    std::swap(data, b.data);
    std::swap(bit_count, b.bit_count);
    std::swap(array_size, b.array_size);
}

BitArray& BitArray::operator=(const BitArray& b)
{
    if (this != &b)
    {
        BitArray temp(b);
        swap(temp);
    }
    return *this;
}

void BitArray::resize(int num_bits, bool value)
{
    if (num_bits <= 0)
    {
        throw invalid_argument("num_bits cannot be <= 0");
    }

    BitArray new_array(num_bits, value ? ~0UL : 0UL);

    int bits_to_copy = std::min(bit_count, num_bits);
    for (int i = 0; i < bits_to_copy; ++i) {
        if ((*this)[i]) {
            new_array.set(i);
        } else {
            new_array.reset(i);
        }
    }
    swap(new_array);
}

void BitArray::clear()
{
    delete[] data;
    data = nullptr;
    bit_count = 0;
    array_size = 0;
}

void BitArray::push_back(bool bit)
{
    if (bit_count == array_size * sizeof(unsigned long) * 8)
    {
        int new_array_size = array_size == 0 ? 1 : array_size * 2;
        unsigned long* new_data = new unsigned long[new_array_size];
        for (int i = 0; i < array_size; ++i)
        {
            new_data[i] = data[i];
        }
        for (int i = array_size; i < new_array_size; ++i)
        {
            new_data[i] = 0;
        }
        delete[] data;
        data = new_data;
        array_size = new_array_size;
    }

    if (bit)
    {
        data[bit_count / (sizeof(unsigned long) * 8)] |= (1UL << (bit_count % (sizeof(unsigned long) * 8)));
    }
    else
    {
        data[bit_count / (sizeof(unsigned long) * 8)] &= ~(1UL << (bit_count % (sizeof(unsigned long) * 8)));
    }
    ++bit_count;
}

BitArray& BitArray::operator&=(const BitArray& b)
{
    if (bit_count != b.bit_count)
    {
        throw invalid_argument("BitArrays must be of the same size for AND operation");
    }

    for (int i = 0; i < array_size; ++i)
    {
        data[i] &= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b)
{
    if (bit_count != b.bit_count)
    {
        throw invalid_argument("BitArrays must be of the same size for OR operation");
    }

    for (int i = 0; i < array_size; ++i)
    {
        data[i] |= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b)
{
    if (bit_count != b.bit_count)
    {
        throw invalid_argument("BitArrays must be of the same size for XOR operation");
    }

    for (int i = 0; i < array_size; ++i)
    {
        data[i] ^= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n)
{
    if (n < 0)
    {
        throw invalid_argument("Shift amount cannot be negative");
    }

    if (array_size == 0)
    {
        throw std::invalid_argument("BitArray must be non-zero");
    }
    if (data == nullptr)
    {
        throw invalid_argument("BitArrays must be of the same size for shift operation");
    }
    if (n >= bit_count)
    {
        reset();
        return *this;
    }

    int whole_longs = n / (sizeof(unsigned long) * 8);
    int bits = n % (sizeof(unsigned long) * 8);

    if (whole_longs > 0)
    {
        for (int i = array_size - 1; i >= whole_longs; --i)
        {
            data[i] = data[i - whole_longs];
        }
        for (int i = 0; i < whole_longs; ++i)
        {
            data[i] = 0;
        }
    }

    if (bits > 0)
    {
        for (int i = array_size - 1; i > 0; --i)
        {
            data[i] = (data[i] << bits) | (data[i - 1] >> (sizeof(unsigned long) * 8 - bits));
        }
        data[0] <<= bits;
    }

    int bits_in_last = bit_count % (sizeof(unsigned long) * 8);
    if (bits_in_last == 0)
    {
        bits_in_last = sizeof(unsigned long) * 8;
    }
    unsigned long mask = (1UL << bits_in_last) - 1;
    data[array_size - 1] &= mask;

    return *this;
}

BitArray& BitArray::operator>>=(int n)
{
    if (n < 0)
    {
        throw invalid_argument("Shift amount cannot be negative");
    }

    if (array_size == 0)
    {
        throw std::invalid_argument("BitArray must be non-zero");
    }
    if (data == nullptr)
    {
        throw invalid_argument("BitArrays must be of the same size for shift operation");
    }
    if (n >= bit_count)
    {
        reset();
        return *this;
    }

    int whole_longs = n / (sizeof(unsigned long) * 8);
    int bits = n % (sizeof(unsigned long) * 8);

    if (whole_longs > 0)
    {
        for (int i = 0; i < array_size - whole_longs; ++i)
        {
            data[i] = data[i + whole_longs];
        }
        for (int i = array_size - whole_longs; i < array_size; ++i)
        {
            data[i] = 0;
        }
    }

    if (bits > 0)
    {
        for (int i = 0; i < array_size - 1; ++i)
        {
            data[i] = (data[i] >> bits) | (data[i + 1] << (sizeof(unsigned long) * 8 - bits));
        }
        data[array_size - 1] >>= bits;
    }

    int bits_in_last = bit_count % (sizeof(unsigned long) * 8);
    if (bits_in_last == 0)
    {
        bits_in_last = sizeof(unsigned long) * 8;
    }
    unsigned long mask = (1UL << bits_in_last) - 1;
    data[array_size - 1] &= mask;

    return *this;
}

BitArray BitArray::operator<<(int n) const
{
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const
{
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val)
{
    if (n < 0 || n >= bit_count) {
        throw out_of_range("index out of range");
    }

    const int BITS_PER_ULONG = sizeof(unsigned long) * 8;
    int array_index = n / BITS_PER_ULONG;
    int bit_index = n % BITS_PER_ULONG;

    if (val) {
        data[array_index] |= (1UL << bit_index);
    } else {
        data[array_index] &= ~(1UL << bit_index);
    }
    return *this;
}

BitArray& BitArray::set()
{
    for (int i = 0; i < array_size; ++i) {
        data[i] = ~0UL;
    }

    if (array_size > 0) {
        int bits_in_last = bit_count % (sizeof(unsigned long) * 8);
        if (bits_in_last == 0) {
            bits_in_last = sizeof(unsigned long) * 8;
        }
        if (bits_in_last < sizeof(unsigned long) * 8) {
            unsigned long mask = (1UL << bits_in_last) - 1;
            data[array_size - 1] &= mask;
        }
    }
    return *this;
}


BitArray& BitArray::reset(int n)
{
    if (n < 0 || n >= bit_count) {
        throw out_of_range("index out of range");
    }

    const int BITS_PER_ULONG = sizeof(unsigned long) * 8;
    int array_index = n / BITS_PER_ULONG;
    int bit_index = n % BITS_PER_ULONG;

    data[array_index] &= ~(1UL << bit_index);
    return *this;
}

BitArray& BitArray::reset()
{
    if (array_size == 0)
    {
        throw std::invalid_argument("BitArray must be non-zero");
    }
    for (int i = 0; i < array_size; ++i) {
        data[i] = 0;
    }
    return *this;
}

bool BitArray::any() const
{
    if (array_size == 0)
    {
        throw std::invalid_argument("BitArray must be non-zero");
    }

    for (int i = 0; i < array_size; ++i) {
        if (data[i] != 0) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const
{
    if (array_size == 0)
    {
        throw std::invalid_argument("BitArray must be non-zero");
    }

    if (bit_count == 0) {
        return true;
    }

    for (int i = 0; i < array_size; ++i) {
        if (data[i] != 0) {
            return false;
        }
    }
    return true;
}

BitArray BitArray::operator~() const
{
    if (array_size == 0)
    {
        throw std::invalid_argument("BitArray must be non-zero");
    }

    if (bit_count == 0)
    {
        throw invalid_argument("BitArray must have at least one bit");
    }

    BitArray result(*this);

    for (int i = 0; i < result.array_size; ++i)
    {
        result.data[i] = ~result.data[i];
    }

    if (result.array_size > 0) {
        int bits_in_last = result.bit_count % (sizeof(unsigned long) * 8);
        if (bits_in_last == 0) {
            bits_in_last = sizeof(unsigned long) * 8;
        }
        if (bits_in_last < sizeof(unsigned long) * 8) {
            unsigned long mask = (1UL << bits_in_last) - 1;
            result.data[result.array_size - 1] &= mask;
        }
    }

    return result;
}

int BitArray::count() const
{
    if (bit_count == 0)
    {
        return 0;
    }

    int count = 0;

    for (int i = 0; i < array_size; ++i) {
        unsigned long value = data[i];

        while (value != 0) {
            count += (value & 1);
            value >>= 1;
        }
    }

    return count;
}

bool BitArray::operator[](int i) const
{
    if (i < 0 || i >= bit_count) {
        throw out_of_range("index out of range");
    }

    const int BITS_PER_ULONG = sizeof(unsigned long) * 8;
    int array_index = i / BITS_PER_ULONG;
    int bit_index = i % BITS_PER_ULONG;

    return (data[array_index] >> bit_index) & 1;
}

int BitArray::size() const
{
    return bit_count;
}

bool BitArray::empty() const
{
    if (bit_count == 0)
    {
        return true;
    }

    return false;
}

std::string BitArray::to_string() const
{
    std::string result;
    result.reserve(bit_count);

    for (int i = bit_count - 1; i >= 0; --i)
    {
        if ((*this)[i]) {
            result += '1';
        } else {
            result += '0';
        }
    }

    return result;
}

bool operator==(const BitArray& a, const BitArray& b)
{
    if (a.size() != b.size())
    {
        return false;
    }

    if (a.empty())
    {
        return true;
    }

    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }

    return true;
}

bool operator!=(const BitArray& a, const BitArray& b)
{
    if (a.size() != b.size())
    {
        return true;
    }

    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
        {
            return true;
        }
    }

    return false;
}

BitArray operator&(const BitArray& a, const BitArray& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("BitArrays must be of same size");
    }

    BitArray tmp(a.size());

    for (int i = 0; i < a.size(); ++i)
    {
        tmp.set(i, a[i] & b[i]);
    }

    return tmp;
}

BitArray operator|(const BitArray& a, const BitArray& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("BitArrays must be of same size");
    }

    BitArray tmp(a.size());

    for (int i = 0; i < a.size(); ++i)
    {
        tmp.set(i, a[i] | b[i]);
    }

    return tmp;
}

BitArray operator^(const BitArray& a, const BitArray& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("BitArrays must be of same size");
    }

    BitArray tmp(a.size());

    for (int i = 0; i < a.size(); ++i)
    {
        tmp.set(i, a[i] ^ b[i]);
    }

    return tmp;
}
