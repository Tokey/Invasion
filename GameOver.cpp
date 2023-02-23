#include "GameOver.h"
#include <EventStep.h>
#include <GameManager.h>
#include <WorldManager.h>
#include <Sound.h>
#include <ResourceManager.h>
#include "GameStart.h"

GameOver::GameOver()
{
    // Link to "message" sprite.
    if (setSprite("gameover") == 0)
        time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown();
    else
        time_to_live = 0;

    // Put in center of window.
    setLocation(df::CENTER_CENTER);

    // Register for step event.
    registerInterest(df::STEP_EVENT);

    // Play "game over" sound.
    df::Sound* p_sound = RM.getSound("game over");
    if (p_sound)
        p_sound->play();
}

// Count down to end of "message".
void GameOver::step() {
    time_to_live--;
    if (time_to_live <= 0)
        WM.markForDelete(this);
}

int GameOver::draw()
{
    return df::Object::draw();
}

// Handle event.
// Return 0 if ignored, else 1.
int GameOver::eventHandler(const df::Event* p_e) {

    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    // If get here, have ignored this event.
    return 0;
}

GameOver::~GameOver() {

    // Remove Saucers and ViewObjects, re-activate GameStart.
    df::ObjectList object_list = WM.getAllObjects(true);
    df::ObjectListIterator i(&object_list);
    for (i.first(); !i.isDone(); i.next()) {
        df::Object* p_o = i.currentObject();
        if (p_o->getType() == "Saucer" || p_o->getType() == "ViewObject")
            WM.markForDelete(p_o);

        if (p_o->getType() == "GameStart") {
            p_o->setActive(true);
            dynamic_cast <GameStart*> (p_o)->playMusic(); // Resume start music.
        }
    }

    
}
