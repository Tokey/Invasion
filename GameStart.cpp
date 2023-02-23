#include "GameStart.h"
#include "Hero.h"
#include "Tank.h"
#include "Points.h"
#include "GameManager.h"
#include "ResourceManager.h"

void GameStart::start()
{

	// Pause start music.
	p_music->pause();
	// Create hero.
	new Hero;

	// 5 RPM DEFAULT
	Tank* s1 = new Tank(1,5);
	Tank* s2 = new Tank(2,5);
	Tank* s3 = new Tank(3,5);


	// Setup heads-up display.
	new Points;		                     // Points display.
	df::ViewObject* p_vo = new df::ViewObject; // Count of nukes.
	p_vo->setLocation(df::TOP_LEFT);
	p_vo->setViewString("Nukes");
	p_vo->setValue(3);
	p_vo->setColor(df::YELLOW);

	// When game starts, become inactive.
	setActive(false);
}

GameStart::GameStart()
{
	setType("GameStart");

	// Link to "message" sprite.
	setSprite("gamestart");

	// Put in center of screen.
	setLocation(df::CENTER_CENTER);

	// Register for "keyboard" event.
	registerInterest(df::KEYBOARD_EVENT);
	// Play start music.
	p_music = RM.getMusic("start music");
	playMusic();
}



// Play start music.
void GameStart::playMusic() {
	if (p_music)
		p_music->play();
}

int GameStart::eventHandler(const df::Event* p_e)
{
	if (p_e->getType() == df::KEYBOARD_EVENT) {
		df::EventKeyboard* p_keyboard_event = (df::EventKeyboard*)p_e;
		switch (p_keyboard_event->getKey()) {
		case df::Keyboard::P: 			// play
			start();
			break;
		case df::Keyboard::Q:			// quit
			GM.setGameOver();
			break;
		default: // Key is not handled.
			break;
		}
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

int GameStart::draw()
{
	return df::Object::draw();
}
