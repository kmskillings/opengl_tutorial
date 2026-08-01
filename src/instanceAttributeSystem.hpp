#ifndef _INSTANCE_ATTRIBUTE_SYSTEM_HPP
#define _INSTANCE_ATTRIBUTE_SYSTEM_HPP

template<typename T>
struct FixedPackedArray;

struct InstanceAttribute;

class InstanceAttributeSystem
{

private:

public:

    void updateAttributes(
        unsigned int vao,
        unsigned int vbo,
        const FixedPackedArray<InstanceAttribute>& attributes,
        const FixedPackedArray<unsigned int>& instanceIndexes,
        const FixedPackedArray<unsigned char>& data
    ) const;

};

#endif