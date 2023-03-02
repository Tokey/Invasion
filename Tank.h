#include "Object.h"
#include "EventCollision.h"

class Tank : public df::Object {

    int eventHandler(const df::Event* p_e) override;
    void out();
    void step();
    void fireTankCannon();
    void moveToStart();
    void hit(const df::EventCollision* p_c);

public:
    Tank(int id, float rof);
    ~Tank();

    float sinVal;
    bool sinPol;

    int tankID;
    bool movementPolarity;
    bool isInStartPos;

    float rateOfFire;
    float weaponCooldown;

    bool isLocked;
    bool isTankAlive;
};

