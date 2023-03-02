
#include "HomingMissile.h"
#include "WorldManager.h"
#include "EventOut.h"
#include "EventStep.h"
#include "Tank.h"

HomingMissile::HomingMissile(df::Vector hero_pos)
{
	// Link to "bullet" sprite.
	setSprite("bullet");

	// Make the Bullets soft so can pass through Hero.
	setSolidness(df::SOFT);
	// Set other object properties.
	setType("Bullet");

	registerInterest(df::STEP_EVENT);
	df::Vector p(hero_pos.getX() + 3, hero_pos.getY());
	setPosition(p);

	// Bullets move 1 space each game loop.
  // The direction is set when the Hero fires.
	setSpeed(1);

	targetLocked = false;
}

int HomingMissile::eventHandler(const df::Event* p_e)
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

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;

	}

	// If get here, have ignored this event.
	return 0;
}

// If Bullet moves outside world, mark self for deletion.
void HomingMissile::out() {

	if(lockedObject->getPosition().getX() >= 0 && lockedObject->getPosition().getY() >= 0)
		lockedObject->setSprite("saucer");
	WM.markForDelete(this);
}

// If Bullet hits Saucer, mark Saucer and Bullet for deletion.
void HomingMissile::hit(const df::EventCollision* p_collision_event) {
	if ((p_collision_event->getObject1()->getType() == "Saucer") ||
		(p_collision_event->getObject2()->getType() == "Saucer")) {
		lockedObject->setSprite("saucer");
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
	}
}

void HomingMissile::step() {

	if (targetLocked) // Checks if bullet has locked on or not
	{
		if (lockedObject->getPosition().getX() <= 0 && lockedObject->getPosition().getY() <= 0)
		{
			targetLocked = false; // If the locked object gets destroyed, then a new target will be assigned

			while (!saucerArray.empty())
				saucerArray.pop_back();
		}
		
		df::Vector v = lockedObject->getPosition() - getPosition();
		v.normalize();
		v.scale(1);
		this->setVelocity(v);
		return;
	}

	// Getting list of all saucers
	for (int i = 0; i < WM.getAllObjects().getCount(); i++)
	{
		Object* p_o = WM.getAllObjects()[i];
		if (p_o->getType() == "Saucer")
		{
			saucerArray.push_back(p_o);
		}
	}

	// Randomly locking on to a saucer
	int idx = rand() % saucerArray.size();
	Object* p_o = saucerArray[idx];
	lockedObject = p_o;
	Tank* lt = dynamic_cast <Tank*> (lockedObject);
	lt->isLocked = true;
	targetLocked = true;
	df::Vector v = p_o->getPosition() - getPosition();
	v.normalize();
	v.scale(1);
	this->setVelocity(v);

}


