#include "Background.h"
#include "WorldManager.h"
#include "EventStep.h"

Background::Background()
{
    // Link to "explosion" sprite.
    setSprite("background");
        
    setType("Background");

    setSolidness(df::SPECTRAL);
}