
enum class NoteState {
    Attack,
    Decay,
    Sustain,
    Release
};

class MidiNote {
public:
    int note = 0;
    float freq = 0.0;
    float phase = 0.0;
    bool active = false;
    float amp = 0.0;
    NoteState state = NoteState::Attack;
    void setAll(int n, float f, float p, bool ac, float am, NoteState s) {
        note = n;
        freq = f;
        phase = p;
        active = ac;
        amp = am;
        state = s;
    }
    void reset() {
        note = 0;
        freq = 0.0;
        phase = 0.0;
        amp = 0.0;
        state = NoteState::Attack;
        active = false;
    }
};