#ifndef TEMPERATURELIMITCHANGEBUFFER_H
#define TEMPERATURELIMITCHANGEBUFFER_H

namespace Ui {
class temperatureLimitChangeBuffer;
}

struct temperatureLimitChangeBuffer
{
    temperatureLimitChangeBuffer();
    void setBuffer(int value);
    int getBuffer();

 private:
    int buffer;

};

#endif // TEMPERATURELIMITCHANGEBUFFER_H
