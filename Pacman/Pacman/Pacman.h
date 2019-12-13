#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

//This will be the amount of munchies
#define MUNCHIECOUNT 50

//This will be the amount of ghosts
#define GHOSTCOUNT 1

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Player // Data to represent the player
{
	bool dead;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	int score;
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
};

struct Enemy
{
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	int currentFrameTime;
	int frameCount;
	int frameTime;
	bool rKeyDown;
	int direction;
};

struct Menu
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool paused;
	bool pKeyDown;
	bool start;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int currentFrameTime;
	int frameCount;
	int frameTime;
	int direction;
	float speed;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState* state, Input::Keys start);
	void CheckViewportCollision();
	
	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(Enemy*, int elapsedtime);
	void UpdateCherry(int elapsedtime);
	void checkGhostCollisions();
	void updateInky(MovingEnemy*, int elapsedTime);
	void updateBlinky(MovingEnemy*, int elapsedTime);
	void updatePinky(MovingEnemy*, int elapsedTime);
	void updateClyde(MovingEnemy*, int elapsedTime);
	void checkMunchieCollisopns();

	//data to reresent sound
	SoundEffect* _pop;
	SoundEffect* _waka;

	//Data to reresent pacman
	Player* _pacman;
	const float _cPacmanSpeed; 
	const int _cFrameTime;

	// Data to represent Munchie
	Enemy* _munchies[MUNCHIECOUNT];
	
	// Data to represent Cherry
	Enemy* _cherry;
	
	// Data to represent Ghost
	MovingEnemy* _inky[GHOSTCOUNT];
	MovingEnemy* _blinky[GHOSTCOUNT];
	MovingEnemy* _pinky[GHOSTCOUNT];
	MovingEnemy* _clyde[GHOSTCOUNT];

	//Data for Menu
	Menu* _menu;

	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};