#pragma once

#include <map>


namespace libfreenect2
{

struct Frame {
public:
    enum Type {
        Color = 0x01,
        Ir = 0x02,
        Depth = 0x04
    };

    float height;
    float width;

    void* data;

public:
    void release() { delete data; }
};

using FrameMap = std::map<Frame::Type, Frame*>;

class FrameListener
{
public:
    bool waitForNewFrame(FrameMap frames, int milliseconds)
    {
        return true;
    }
    void release(FrameMap& frames) {}
};

class SyncMultiFrameListener : public FrameListener
{
public:
    SyncMultiFrameListener(int type_flag)
    {
    }
};

}  // namespace libfreenect2
