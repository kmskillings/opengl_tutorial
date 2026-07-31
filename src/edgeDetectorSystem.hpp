#ifndef _EDGE_DETECTOR_SYSTEM_HPP
#define _EDGE_DETECTOR_SYSTEM_HPP

#include "doubleBuffer.hpp"
#include "fixedPackedArray.hpp"

#include <algorithm>

template<typename T>
class EdgeDetectorSystem
{

private:

public:

    void detectEdges(
        const DoubleBuffer<FixedPackedArray<T>>& array,
        FixedPackedArray<T>& appeared,
        FixedPackedArray<T>& disappeared
    ) const
    {
        uint32_t nowCursor = 0;
        uint32_t lastCursor = 0;
        appeared.clear();
        disappeared.clear();
        while (
            nowCursor < array.now().count &&
            lastCursor < array.last().count
        )
        {
            if (array.now()[nowCursor] > array.last()[lastCursor])
            {
                // If the now item is greater, that means last has items that
                // now doesn't, meaning those items disappeared.
                disappeared.push(array.last()[lastCursor]);
                lastCursor += 1;
            }
            else if (array.now()[nowCursor] < array.last()[lastCursor])
            {
                // If the last item is greater, that means now has items that
                // last doesn't, meaning those items appeared.
                appeared.push(array.now()[nowCursor]);
                nowCursor += 1;
            }
            else
            {
                // The items matched, meaning both last and now have it.
                lastCursor += 1;
                nowCursor += 1;
            }
        }

        // After the while loop terminates, one array may have items left to
        // consider. If that array is now, that means all those items appeared.
        // If that array is last, that means all those items disappeared.
        if (nowCursor < array.now().count)
        {
            for (; nowCursor < array.now().count; nowCursor++)
            {
                appeared.push(array.now()[nowCursor]);
            }
        }
        if (lastCursor < array.last().count)
        {
            for (; lastCursor < array.last().count; lastCursor++)
            {
                disappeared.push(array.last()[lastCursor]);
            }
        }
    }

};

#endif