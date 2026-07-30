#ifndef _CHUNKED_ARRAY_HPP
#define _CHUNKED_ARRAY_HPP

#include "fixedPackedArray.hpp"
#include "arraySlice.hpp"

// Chunked Array
//
// A Chunked Array is a Fixed Packed Array that is "chunked," or spatially divided
// into cubical, axis-aligned cells. The contents of the array are organized to
// group components in the same chunk together. The details of the chunking
// system are captured in a ChunkGrid, which the ChunkedArray points to.

struct ChunkGrid;

template<typename T>
struct ChunkedArray
{

    const FixedPackedArray<Chunk>* const chunks;
    FixedPackedArray<T> array;

    void init(
        const FixedPackedArray<Chunk>* const chunks,
        const uint32_t& capacity
    ) :
        chunks(chunks)
    {
        array.allocate(capacity);
    }

    void destroy(void)
    {
        array.deallocate();
    }

    ArraySlice<T> operator[](uint32_t index) const
    {
        return ArraySlice<T>(
            &array[chunks[index].start],
            chunks[index].count
        );
    }

};

#endif