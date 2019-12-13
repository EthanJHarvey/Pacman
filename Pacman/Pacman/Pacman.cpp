#include "Pacman.h"

#include <sstream>
#include <time.h>

#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cFrameTime(250)
{
	int i;

	srand(time(NULL));

	// Data to represent all ghosts
	_inky[0] = new MovingEnemy();
	_inky[0]->direction = 0;
	_inky[0]->speed = 0.2f;

	_blinky[0] = new MovingEnemy();
	_blinky[0]->direction = 0;
	_blinky[0]->speed = 0.2f;

	_pinky[0] = new MovingEnemy();
	_pinky[0]->direction = 0;
	_pinky[0]->speed = 0.2f;

	_clyde[0] = new MovingEnemy();
	_clyde[0]->direction = 0;
	_clyde[0]->speed = 0.2f;

	// Data to represent all Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameTime = rand() % 500 + 50;
	}
	_munchies[0]->frameCount = 0;

	// Data to represent Cherry
	_cherry = new Enemy();
	_cherry->frameCount = 0;
	_cherry->rKeyDown = false;

	//Data to represent sound
	_pop = new SoundEffect();
	_waka = new SoundEffect();

	// Data to represent Pacman
	_pacman = new Player();
	_pacman->dead = false;
	_pacman->direction = 0;
	_pacman->frame = 0;
	_pacman->currentFrameTime = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->score = 0;

	//Data for Menu
	_menu = new Menu();
	_menu->paused = false;
	_menu->pKeyDown = false;
	_menu->start = true;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _munchies[0]->texture;

	int nCount = 0;
	for (nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount];
		//Order of operations is important, array deleted last
	}

	delete _cherry->texture;
	delete _cherry->sourceRect;
	delete _cherry->position;

	delete _pop;
	delete _waka;

	delete _inky[0]->texture;
	delete _inky[0]->sourceRect;
	delete _inky[0]->position;

	delete _blinky[0]->texture;
	delete _blinky[0]->sourceRect;
	delete _blinky[0]->position;

	delete _pinky[0]->texture;
	delete _pinky[0]->sourceRect;
	delete _pinky[0]->position;

	delete _clyde[0]->texture;
	delete _clyde[0]->sourceRect;
	delete _clyde[0]->position;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Ghosts
	_inky[0]->texture = new Texture2D();
	_inky[0]->texture->Load("Textures/Inky.png", false);
	_inky[0]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_inky[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	_blinky[0]->texture = new Texture2D();
	_blinky[0]->texture->Load("Textures/Blinky.png", false);
	_blinky[0]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_blinky[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	_pinky[0]->texture = new Texture2D();
	_pinky[0]->texture->Load("Textures/Pinky.png", false);
	_pinky[0]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_pinky[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	_clyde[0]->texture = new Texture2D();
	_clyde[0]->texture->Load("Textures/Clyde.png", false);
	_clyde[0]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_clyde[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	// Load Munchies
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);
	int i;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->texture->Load("Textures/Munchie.png", true);
		_munchies[i]->sourceRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}
	//Load cherry
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/PacmanCherry.png", true);
	_cherry->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));

	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0);

	//Load Sounds
	_pop->Load("Sounds/pop.wav");
	_waka->Load("Sounds/waka.wav");

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;

	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 4;
		Audio::Play(_waka);
	}
	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 2;
		Audio::Play(_waka);
	}
	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 3;
		Audio::Play(_waka);
	}
	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 1;
		Audio::Play(_waka);
	}

	mouseState->LeftButton;
	//Checks if the mouse button is pressed 
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_cherry->position->X = mouseState->X;
		_cherry->position->Y = mouseState->Y;
	}

	// Speed Multiplier
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
		_pacman->speedMultiplier = 2.0f; //Apply multiplier
	else
		_pacman->speedMultiplier = 1.0f; //Reset multiplier

	//Checks if the R key is pressed 
	if (state->IsKeyDown(Input::Keys::R) && !_cherry->rKeyDown && _cherry->position->X != 100)
	{
		_cherry->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_cherry->rKeyDown = true;
	}
	if (state->IsKeyUp(Input::Keys::R)) // Makes it so cherry only moves once per R press
		_cherry->rKeyDown = false;
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pausedKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_menu->pKeyDown)
	{
		_menu->pKeyDown = true;
		_menu->paused = !_menu->paused;
	}
	if (state->IsKeyUp(Input::Keys::P))
		_menu->pKeyDown = false;
}

void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys start)
{
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_menu->start = false;
	}
}

void Pacman::CheckViewportCollision()
{
	//checks if Pacman is trying to disappear
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth()) //1024 is the game width
	{
		// Pacman hit right wall
		_pacman->position->X = 0;
	}
	if (_pacman->position->X < 0)
	{
		// Pacman hit left wall - reset his position
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}
	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight()) //It uses the set height whitch was 768
	{
		// Pacman hits the wall - reset his position
		_pacman->position->Y = 0;
	}
	if (_pacman->position->Y + _pacman->sourceRect->Height < 0)
	{
		// Pacman hits the top - reset his position
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->currentFrameTime += elapsedTime; // when value gets higher than elapsed time the animation will move on.

	if (_pacman->currentFrameTime > _cFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}

	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction; // changes the way pacman is faceing
}

void Pacman::UpdateMunchie(Enemy*, int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->currentFrameTime += elapsedTime;

		if (_munchies[i]->currentFrameTime > _munchies[i]->frameTime)
		{
			_munchies[i]->frameCount++;

			if (_munchies[i]->frameCount >= 2)
				_munchies[i]->frameCount = 0;

			_munchies[i]->currentFrameTime = 0;
		}
		_munchies[i]->sourceRect->X = _munchies[i]->sourceRect->Width * _munchies[i]->frameCount;
	}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->currentFrameTime += elapsedTime;

	if (_cherry->currentFrameTime > _cherry->frameTime)
	{
		_cherry->frameCount++;

		if (_cherry->frameCount >= 2)
			_cherry->frameCount = 0;

		_cherry->currentFrameTime = 0;
	}
	_cherry->sourceRect->X = _cherry->sourceRect->Width * _cherry->frameCount; //updates the cherrys animation
}

void Pacman::updateInky(MovingEnemy* ghost, int elapsedTime)
{

	ghost->currentFrameTime += elapsedTime; // when value gets higher than elapsed time the animation will move on.

	if (ghost->currentFrameTime > _cFrameTime)
	{
		ghost->frameTime++;

		if (ghost->frameTime >= 2)
			ghost->frameTime = 0;

		ghost->currentFrameTime = 0;
	}
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->direction;
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction;

	if (ghost->direction == 0) //Moves Right
		ghost->position->X += ghost->speed * elapsedTime;
	else if (ghost->direction == 1) //Moves left
		ghost->position->X -= ghost->speed * elapsedTime;

	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits Right edge
		ghost->direction = 1; //Change direction
	else if (ghost->position->X <= 0) // Hits left edge
		ghost->direction = 0; //Change direction
}

void Pacman::updateBlinky(MovingEnemy* ghost, int elapsedTime)
{

	ghost->currentFrameTime += elapsedTime; // when value gets higher than elapsed time the animation will move on.

	if (ghost->currentFrameTime > _cFrameTime)
	{
		ghost->frameTime++;

		if (ghost->frameTime >= 2)
			ghost->frameTime = 0;

		ghost->currentFrameTime = 0;
	}
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->direction;
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction;

	if (ghost->direction == 0) //Moves Right
		ghost->position->X += ghost->speed * elapsedTime;
	else if (ghost->direction == 1) //Moves left
		ghost->position->X -= ghost->speed * elapsedTime;

	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits Right edge
		ghost->direction = 1; //Change direction
	else if (ghost->position->X <= 0) // Hits left edge
		ghost->direction = 0; //Change direction
}

void Pacman::updatePinky(MovingEnemy* ghost, int elapsedTime)
{

	ghost->currentFrameTime += elapsedTime; // when value gets higher than elapsed time the animation will move on.

	if (ghost->currentFrameTime > _cFrameTime)
	{
		ghost->frameTime++;

		if (ghost->frameTime >= 2)
			ghost->frameTime = 0;

		ghost->currentFrameTime = 0;
	}
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->direction;
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction;

	if (ghost->direction == 0) //Moves Right
		ghost->position->X += ghost->speed * elapsedTime;
	else if (ghost->direction == 1) //Moves left
		ghost->position->X -= ghost->speed * elapsedTime;

	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits Right edge
		ghost->direction = 1; //Change direction
	else if (ghost->position->X <= 0) // Hits left edge
		ghost->direction = 0; //Change direction
}

void Pacman::updateClyde(MovingEnemy* ghost, int elapsedTime)
{

	ghost->currentFrameTime += elapsedTime; // when value gets higher than elapsed time the animation will move on.

	if (ghost->currentFrameTime > _cFrameTime)
	{
		ghost->frameTime++;

		if (ghost->frameTime >= 2)
			ghost->frameTime = 0;

		ghost->currentFrameTime = 0;
	}
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->direction;
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction;

	if (ghost->direction == 0) //Moves Right
		ghost->position->X += ghost->speed * elapsedTime;
	else if (ghost->direction == 1) //Moves left
		ghost->position->X -= ghost->speed * elapsedTime;

	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits Right edge
		ghost->direction = 1; //Change direction
	else if (ghost->position->X <= 0) // Hits left edge
		ghost->direction = 0; //Change direction
}
void Pacman::checkGhostCollisions()
{
	// local Variables
	int i = 0;
	int l = 0;
	int j = 0;
	int k = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Populate variable with Ghost data
		bottom2 = _inky[i]->position->Y + _inky[i]->sourceRect->Height;
		left2 = _inky[i]->position->X;
		right2 = _inky[i]->position->X + _inky[i]->sourceRect->Width;
		top2 = _inky[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
	for (l = 0; l < GHOSTCOUNT; l++)
	{
		bottom2 = _blinky[l]->position->Y + _blinky[l]->sourceRect->Height;
		left2 = _blinky[l]->position->X;
		right2 = _blinky[l]->position->X + _blinky[l]->sourceRect->Width;
		top2 = _blinky[l]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			l = GHOSTCOUNT;
		}
	}
	for (j = 0; j < GHOSTCOUNT; j++)
	{
		bottom2 = _clyde[j]->position->Y + _clyde[j]->sourceRect->Height;
		left2 = _clyde[j]->position->X;
		right2 = _clyde[j]->position->X + _clyde[j]->sourceRect->Width;
		top2 = _clyde[j]->position->Y;
		
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			j = GHOSTCOUNT;
		}
	}
	for (k = 0; k < GHOSTCOUNT; k++)
	{
		bottom2 = _pinky[k]->position->Y + _pinky[k]->sourceRect->Height;
		left2 = _pinky[k]->position->X;
		right2 = _pinky[k]->position->X + _pinky[k]->sourceRect->Width;
		top2 = _pinky[k]->position->Y;
	}
	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
	{
		_pacman->dead = true;
		k = GHOSTCOUNT;
	}
}

void Pacman::checkMunchieCollisopns()
{
	// local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		bottom2 = _munchies[i]->position->Y + _munchies[i]->sourceRect->Height;
		left2 = _munchies[i]->position->X;
		right2 = _munchies[i]->position->X + _munchies[i]->sourceRect->Width;
		top2 = _munchies[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_munchies[i]->position->X = -100;
			_munchies[i]->position->Y = -100;
			_pacman->score += 10;
			Audio::Play(_pop);
			i = MUNCHIECOUNT;
		}
	}

	bottom2 = _cherry->position->Y + _cherry->sourceRect->Height;
	left2 = _cherry->position->X;
	right2 = _cherry->position->X + _cherry->sourceRect->Width;
	top2 = _cherry->position->Y;

	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
	{
		_cherry->position->X = -100;
		_cherry->position->Y = -100;
		_pacman->score += 100;
		Audio::Play(_pop);
		i = MUNCHIECOUNT;
	}
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (_menu->start == true)
	{
		CheckStart(keyboardState, Input::Keys::SPACE);
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_menu->paused)
		{
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(_munchies[i], elapsedTime);
			}
			Input(elapsedTime, keyboardState, mouseState);
			UpdatePacman(elapsedTime);
			UpdateCherry(elapsedTime);
			CheckViewportCollision();
			updateInky(_inky[0], elapsedTime);
			updateBlinky(_blinky[0], elapsedTime);
			updatePinky(_pinky[0], elapsedTime);
			updateClyde(_clyde[0], elapsedTime);
			checkGhostCollisions();
			checkMunchieCollisopns();
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;
	stream << "\nPlayer score:" << _pacman->score;

	SpriteBatch::BeginDraw(); // Starts Drawing  

	// Draws Pacman
	if (!_pacman->dead)
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect);

	// Draws Ghosts
	SpriteBatch::Draw(_inky[0]->texture, _inky[0]->position, _inky[0]->sourceRect);
	SpriteBatch::Draw(_blinky[0]->texture, _blinky[0]->position, _blinky[0]->sourceRect);
	SpriteBatch::Draw(_pinky[0]->texture, _pinky[0]->position, _pinky[0]->sourceRect);
	SpriteBatch::Draw(_clyde[0]->texture, _clyde[0]->position, _clyde[0]->sourceRect);

	for (int i = 0; i < MUNCHIECOUNT; i++)//Draws all munchies
	{
		SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
	}

	SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect);//Draws a cherry

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);


	if (_menu->paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Red);
	}

	if (_menu->start)
	{
		std::stringstream menuStream;
		menuStream << "PACMAN" << endl << "PRESS SPACE TO PLAY";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Yellow);
	}

	if (_pacman->dead == true)
	{
		std::stringstream menuStream;
		menuStream << "GAMEOVER" << endl << "SCORE:" << _pacman->score << endl; //<< "TO PLAY A PRESS SPACE";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Yellow);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}