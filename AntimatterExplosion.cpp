#include "AntimatterExplosion.h"
#include "WorldManager.h"
#include "EventStep.h"

AntimatterExplosion::AntimatterExplosion()
{
    registerInterest(df::STEP_EVENT);
    // Link to "explosion" sprite.
    if (setSprite("antimatterexplosion") == 0)
        // Set live time as long as sprite length.
        time_to_live = getAnimation().getSprite()->getFrameCount();
    else
        time_to_live = 0;

    setType("AntimatterExplosion");

    setSolidness(df::SPECTRAL);

}

int AntimatterExplosion::eventHandler(const df::Event* p_e)
{
    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    // If get here, have ignored this event.
    return 0;
}

void AntimatterExplosion::step() {
    time_to_live--;
    if (time_to_live <= 0)
        WM.markForDelete(this);
}