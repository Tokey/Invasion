#include "Moon.h"
#include "WorldManager.h"
#include "EventStep.h"

Moon::Moon()
{
    // Link to "explosion" sprite.
    setSprite("moon");

    setType("Moon");

    setSolidness(df::SPECTRAL);
}