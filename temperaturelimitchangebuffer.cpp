#include "temperaturelimitchangebuffer.h"

temperatureLimitChangeBuffer::temperatureLimitChangeBuffer()
{
    buffer = 0;
}

void temperatureLimitChangeBuffer::setBuffer(int value)
{
    buffer = value;
}

int temperatureLimitChangeBuffer::getBuffer()
{
    return buffer;
}

