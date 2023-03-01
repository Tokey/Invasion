#include "ForceField.h"
#include "WorldManager.h"
#include "EventOut.h"
#include <DisplayManager.h>
#include "AntimatterExplosion.h"
#include <Sound.h>
#include <ResourceManager.h>

ForceField::ForceField(df::Vector hero_pos)
{
	// Link to "cannon" sprite. //TODO: MUST CHANGE
	setSprite("forcefield");

	// Make the Bullets soft so can pass through Hero.
	setSolidness(df::SOFT);
	// Set other object properties.
	setType("ForceField");


	df::Vector p(hero_pos.getX(), hero_pos.getY() + 3);
	setPosition(p);

	// Bullets move 1 space each game loop.
  // The direction is set when the Hero fires.
	setSpeed(1);
}

int ForceField::eventHandler(const df::Event* p_e)
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
void ForceField::out() {
	WM.markForDelete(this);
}

// If Bullet hits Saucer, mark Saucer and Bullet for deletion.
void ForceField::hit(const df::EventCollision* p_collision_event) {
	if ((p_collision_event->getObject1()->getType() == "TankCannon")){
		WM.markForDelete(p_collision_event->getObject1());
		DM.shake(30, 20, 2);
		AntimatterExplosion* p_explosion = new AntimatterExplosion;
		p_explosion->setPosition(p_collision_event->getObject1()->getPosition());

		df::Sound* p_sound = RM.getSound("antimatterExplosion");
		if (p_sound)
			p_sound->play();
	}
	else if ((p_collision_event->getObject2()->getType() == "TankCannon")) {
		WM.markForDelete(p_collision_event->getObject2());
		DM.shake(30, 20, 2);
		AntimatterExplosion* p_explosion = new AntimatterExplosion;
		p_explosion->setPosition(p_collision_event->getObject2()->getPosition());

		df::Sound* p_sound = RM.getSound("antimatterExplosion");
		if (p_sound)
			p_sound->play();
	}
}