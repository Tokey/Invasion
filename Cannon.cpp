#include "Cannon.h"
#include "WorldManager.h"
#include "EventOut.h"

Cannon::Cannon(df::Vector hero_pos)
{
	// Link to "cannon" sprite. //TODO: MUST CHANGE
	setSprite("cannon");

	// Make the Bullets soft so can pass through Hero.
	setSolidness(df::SOFT);
	// Set other object properties.
	setType("Cannon");


	df::Vector p(hero_pos.getX(), hero_pos.getY() + 3);
	setPosition(p);

	// Bullets move 1 space each game loop.
  // The direction is set when the Hero fires.
	setSpeed(1);
}

int Cannon::eventHandler(const df::Event* p_e)
{
	if (p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	}

	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* p_collision_event = dynamic_cast <const df::EventCollision*> (p_e);
		hit(p_collision_event);
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// If Bullet moves outside world, mark self for deletion.
void Cannon::out() {
	WM.markForDelete(this);
}

// If Bullet hits Saucer, mark Saucer and Bullet for deletion.
void Cannon::hit(const df::EventCollision* p_collision_event) {
	if ((p_collision_event->getObject1()->getType() == "Saucer") ||
		(p_collision_event->getObject2()->getType() == "Saucer")) {
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
	}
}
