#include "Reticle.h"
#include "WorldManager.h"
#include "DisplayManager.h"

Reticle::Reticle() {
    setSolidness(df::SPECTRAL);
    setAltitude(df::MAX_ALTITUDE); // Make Reticle in foreground.
    registerInterest(df::MSE_EVENT);
}

int Reticle::eventHandler(const df::Event* p_e) {

    if (p_e->getType() == df::MSE_EVENT) {
        const df::EventMouse* p_mouse_event =
            dynamic_cast <const df::EventMouse*> (p_e);
        if (p_mouse_event->getMouseAction() == df::MOVED) {
            // Change location to new mouse position.
            setPosition(p_mouse_event->getMousePosition());
            return 1;
        }
    }

    // If get here, have ignored this event.
    return 0;
}

// Draw reticle on window.
int Reticle::draw() {
    return DM.drawString(getPosition(), RETICLE_CHAR, df::CENTER_JUSTIFIED, df::YELLOW);
}