#ifndef _ARRAY_SLICE_HPP
#define _ARRAY_SLICE_HPP

// Array Slice
//
// An Array Slice provides a slice or window into another array-like data
// structure. It is intended to be transient, as a parameter or return-value.

template<typename T>
struct ArraySlice
{
    
    const T* data;
    const uint32_t count;

    ArraySlice(
        const T* data,
        const uint32_t count
    ) :
        data(data),
        count(count)
    {}

    T& operator[](uint32_t index)
    {
        return data[index];
    }

    const T& operator[](uint32_t index) const
    {
        return data[index];
    }

};

#endif