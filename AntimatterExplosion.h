#include "Object.h"

class AntimatterExplosion : public df::Object {

private:
    int time_to_live;
    void step();

public:
    AntimatterExplosion();
    int eventHandler(const df::Event* p_e) override;
};
