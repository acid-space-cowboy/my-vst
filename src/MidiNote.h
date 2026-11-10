

class MidiNote {
public:
    int note = 0;
    float freq = 0.0;
    float phase = 0.0;
    int startPos = 0;
    int endPos = 0;
    bool active = false;
    bool deferredInactive = false;
    void reset() {
        note = 0;
        freq = 0.0;
        phase = 0.0;
        startPos = 0;
        endPos = 0;
        active = false;
        deferredInactive = false;
    }
};