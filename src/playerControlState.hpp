#ifndef _PLAYER_CONTROL_STATE_HPP
#define _PLAYER_CONTROL_STATE_HPP

static void normalizeAxis(int8_t& axis)
{
    if (axis >= 1)
    {
        axis = 1;
    }
    else if (axis <= -1)
    {
        axis = -1;
    }
    else
    {
        axis = 0;
    }
}

struct PlayerControlState
{
    int8_t forward;
    int8_t up;
    int8_t right;

    bool cursorInitialized;
    double mouseXLast;
    double mouseYLast;
    double mouseXNow;
    double mouseYNow;

    PlayerControlState(void) :
        forward(0),
        up(0),
        right(0),
        cursorInitialized(false),
        mouseXLast(0.0),
        mouseYLast(0.0),
        mouseXNow(0.0),
        mouseYNow(0.0)
    {}

    void normalize(void)
    {
        normalizeAxis(forward);
        normalizeAxis(up);
        normalizeAxis(right);
    }
};

#endif