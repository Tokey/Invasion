#include "ViewObject.h"

class GameOver : public df::ViewObject {
    int time_to_live;

public:
    GameOver();
    ~GameOver();
    int eventHandler(const df::Event* p_e);
    void step();
    int draw() override;

};

