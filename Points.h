#include "ViewObject.h"

#define POINTS_STRING "Points"

class Points : public df::ViewObject {

public:
    int eventHandler(const df::Event* p_e) override;
    Points();
};
