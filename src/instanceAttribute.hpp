#ifndef _INSTANCE_ATTRIBUTE_HPP
#define _INSTANCE_ATTRIBUTE_HPP

struct InstanceAttribute
{
    unsigned int instanceIndex;
    unsigned int instanceSize;
    unsigned int attributeSize;
    unsigned int attributeOffset;

    InstanceAttribute(
        unsigned int instanceSize,
        unsigned int attributeSize,
        unsigned int attributeOffset
    ) :
        instanceSize(instanceSize),
        attributeSize(attributeSize),
        attributeOffset(attributeOffset)
    {}
};

#endif