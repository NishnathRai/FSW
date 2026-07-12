enum class ACTION_to_BLINK
{
    NOWAY,
    SETUP,
    LOOP,
    UNKNOWN = 255
};


class Blink{
public:
    Blink() = default;
    static Blink& get_blink_instence();
    bool blink( ACTION_to_BLINK action );
};
