#include "TankCannon.h"
#include "WorldManager.h"
#include "EventOut.h"
#include "EventNuke.h"
#include "Explosion.h"
#include <Sound.h>
#include <ResourceManager.h>
#include <utility.h>
#include <DisplayManager.h>

TankCannon::TankCannon(df::Vector hero_pos)
{
	// Link to "tankCannon" sprite.
	setSprite("tankCannon");		// TODO, MUST CHANGE THIS

	// Make the Bullets soft so can pass through Hero.
	setSolidness(df::SOFT);
	// Set other object properties.
	setType("TankCannon");


	df::Vector p(hero_pos.getX(), hero_pos.getY() - 3);
	setPosition(p);

	// Bullets move 1 space each game loop.
  // The direction is set when the Hero fires.
	setSpeed(.6);

	registerInterest(NUKE_EVENT);
}

int TankCannon::eventHandler(const df::Event* p_e)
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

	if (p_e->getType() == NUKE_EVENT) {
		// Create an explosion.
		Explosion* p_explosion = new Explosion;
		p_explosion->setPosition(this->getPosition());
		WM.markForDelete(this);
		//new Saucer(1);
	}

	// If get here, have ignored this event.
	return 0;
}

// If Bullet moves outside world, mark self for deletion.
void TankCannon::out() {
	WM.markForDelete(this);
}

// If Bullet hits Saucer, mark Saucer and Bullet for deletion.
void TankCannon::hit(const df::EventCollision* p_collision_event) {
	if ((p_collision_event->getObject1()->getType() == "Hero") ||
		(p_collision_event->getObject2()->getType() == "Hero")) {

		df::addParticles(df::SPARKS, getPosition(), 2, df::CYAN);

		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());

		df::Sound* p_sound = RM.getSound("ammocollision");
		if (p_sound)
			p_sound->play();
		DM.shake(5, 5, 2);
	}

	if ((p_collision_event->getObject1()->getType() == "Bullet") ||
		(p_collision_event->getObject2()->getType() == "Bullet")) {

		df::addParticles(df::SPARKS, getPosition(), 2, df::CYAN);

		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());

		df::Sound* p_sound = RM.getSound("ammocollision");
		if (p_sound)
			p_sound->play();
		DM.shake(5, 5, 2);
	}

	if ((p_collision_event->getObject1()->getType() == "Laser") ||
		(p_collision_event->getObject2()->getType() == "Laser")) {

		df::addParticles(df::SPARKS, getPosition(), 2, df::CYAN);

		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());

		df::Sound* p_sound = RM.getSound("ammocollision");
		if (p_sound)
			p_sound->play();
		DM.shake(5, 5, 2);
	}
	if ((p_collision_event->getObject1()->getType() == "Cannon") ||
		(p_collision_event->getObject2()->getType() == "Cannon")) {

		df::addParticles(df::SPARKS, getPosition(), 2, df::CYAN);

		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());

		df::Sound* p_sound = RM.getSound("ammocollision");
		if (p_sound)
			p_sound->play();
		DM.shake(5, 5, 2);
	}
}