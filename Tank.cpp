
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "EventOut.h"
#include "EventCollision.h"
#include "Explosion.h"
#include "TankCannon.h"
#include "Tank.h"
#include "EventNuke.h"
#include "Points.h"
#include "ViewObject.h"
#include "EventView.h"
#include "EventStep.h"
#include "Hero.h"


Tank::Tank(int id, float rof) {

	tankID = id;

	// Setup "saucer" sprite.
	setSprite("saucer");
	// Set object type.
	setType("Saucer");

	// Set speed in horizontal direction.
	setVelocity(df::Vector(-1, 0)); // 1 space left every 4 frames

	// Register interest in "nuke" event.
	registerInterest(NUKE_EVENT);
	registerInterest(df::STEP_EVENT);
	moveToStart();

	 sinVal = 0;
	 isInStartPos = false;
	 sinPol = true;
	 rateOfFire = rof; //Rounds per minute
	 weaponCooldown =  0;
}

Tank::~Tank()
{
	// Send "view" event with points to interested ViewObjects.
  // Add 10 points.
	df::EventView ev(POINTS_STRING, 10, true);
	WM.onEvent(&ev);
}

int Tank::eventHandler(const df::Event* p_e) {

	if (p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	}

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}
	
	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* p_collision_event = dynamic_cast <df::EventCollision const*> (p_e);
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

void Tank::out()  {
	if (getPosition().getX() >= 0 && getPosition().getY() >=0 && getPosition().getY() <= WM.getBoundary().getVertical())
		return;
	//moveToStart(); 
	// Spawn new Saucer to make the game get harder.
	//new Saucer(1);
}

void Tank::hit(const df::EventCollision* p_c) {
	// If Saucer on Saucer, ignore.
	if ((p_c->getObject1()->getType() == "Saucer") &&
		(p_c->getObject2()->getType() == "Saucer"))
		return;

	// If Bullet...
	if ((p_c->getObject1()->getType() == "Bullet") ||
		(p_c->getObject2()->getType() == "Bullet") || 
		(p_c->getObject1()->getType() == "Laser") ||
		(p_c->getObject2()->getType() == "Laser")) {

		// Create an explosion.
		Explosion* p_explosion = new Explosion;
		p_explosion->setPosition(this->getPosition());

		// Saucers appear stay around perpetually.
		//new Saucer(1);

		// Play "explode" sound.
		df::Sound* p_sound = RM.getSound("explode");
		if (p_sound)
			p_sound->play();
	}

	// If Hero, mark both objects for destruction.
	if (((p_c->getObject1()->getType()) == "Hero") ||
		((p_c->getObject2()->getType()) == "Hero")) {
		WM.markForDelete(p_c->getObject1());
		WM.markForDelete(p_c->getObject2());
	}
}

void Tank::moveToStart() {
	df::Vector temp_pos;

	float world_horiz = WM.getBoundary().getHorizontal();
	float world_vert = WM.getBoundary().getVertical();

	//// x is off right side of window
	//temp_pos.setX(world_horiz + rand() % (int)world_horiz + 3.0f);


	if (tankID == 1)
		temp_pos.setX(world_horiz + 5);
	else if (tankID == 2)
		temp_pos.setX(world_horiz + 20);
	else if (tankID == 3)
		temp_pos.setX(world_horiz + 35);


	// y is in vertical range
	temp_pos.setY(43); // Altitude of the tanks

	// If collision, move right slightly until empty space.
	df::ObjectList collision_list = WM.getCollisions(this, temp_pos);
	while (!collision_list.isEmpty()) {
		temp_pos.setX(temp_pos.getX() + 1);
		collision_list = WM.getCollisions(this, temp_pos);
	}

	WM.moveObject(this, temp_pos);
}

void Tank::step() {
	
	// To make the saucer oscillate at a random speed
	if (sinPol)
		sinVal += 0.013;
	else
		sinVal -= 0.013;

	if (sinVal > .17)
		sinPol = false;

	if(sinVal < -.17)
		sinPol = true;
	
	if (!isInStartPos)
	{
		if (tankID == 1 && this->getPosition().getX() <= 50)
			isInStartPos = true;
		else if (tankID == 2 && this->getPosition().getX() <= 75)
			isInStartPos = true;
		else if (tankID == 3 && this->getPosition().getX() <= 100)
			isInStartPos = true;
	}
	else
	{
		setVelocity(df::Vector(2.0f * sinVal * (rand()%5), 0.f));

		weaponCooldown -= 0.033; // Assuming this will alawys run at 30 fps
		if (weaponCooldown <= 0) {
			fireTankCannon();
			weaponCooldown = 60 / rateOfFire;
		}

	}
	
}

void Tank::fireTankCannon() {

	// Fire Bullet towards hero.
  // Compute normalized vector to position, then scale by speed (1).


	// Get the hero 
	Object* hero = nullptr;
	for (int i = 0; i < WM.getAllObjects().getCount(); i++)
	{
		Object* p_o = WM.getAllObjects()[i];
		if (p_o->getType() == "Hero")
		{
			hero = p_o;
		}
	}

	// If hero found
	if (hero != nullptr)
	{
		df::Vector v = df::Vector(hero->getPosition()) - getPosition();
		v.normalize();
		v.scale(1);
		TankCannon* p = new TankCannon(getPosition());
		p->setVelocity(v);

		// Play "fire" sound.
		df::Sound* p_sound = RM.getSound("fire");
		if (p_sound)
			p_sound->play();
	}
}