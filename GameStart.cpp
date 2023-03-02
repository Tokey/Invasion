#include "GameStart.h"
#include "Hero.h"
#include "Tank.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include <WorldManager.h>

df::ViewObject* hs_vo = nullptr;

void GameStart::start()
{

	// Pause start music.
	//p_music->pause();
	// Create hero.
	new Hero;

	// 5 RPM DEFAULT
	Tank* s1 = new Tank(1,5);
	Tank* s2 = new Tank(2,5);
	Tank* s3 = new Tank(3,5);

	// When game starts, become inactive.
	setActive(false);

	WM.markForDelete(hs_vo);
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

	int highscore;

	FILE* filePointer;

	filePointer = fopen("HighScore.txt", "r");
	if (filePointer == nullptr)
	{
		filePointer = fopen("HighScore.txt", "w");
		highscore = 0;
		fprintf(filePointer, "%d", highscore);
		fclose(filePointer);
	}
	else
	{
		fscanf(filePointer, "%d", &highscore);
	}

	hs_vo = new df::ViewObject; // Count of nukes.
	hs_vo->setLocation(df::BOTTOM_CENTER);
	hs_vo->setViewString("High Score");
	hs_vo->setValue(highscore);
	hs_vo->setColor(df::YELLOW);

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
