#include "j1App.h"
#include "player.h"
#include "j1Input.h"
#include "SDL/include/SDL_scancode.h"

#include "p2Log.h"

Player::Player(iPoint position) : Entity(type, position)
{
	// TODO2: Define fogVisibilityEmitter with radius 3 ---
	// and set instantiation position
	

	// ----------------------------------------------------
}

Player::~Player() {}


bool Player::Update(float dt)
{
	bool ret = true;

	SimpleInput();

	// updates position of the emitter
	if (fogVisibilityEmitter != nullptr)
	{
		// TODO3: update the position of the emitter to this position ---
		
		// --------------------------------------------------------------
	}

	return ret;
}

void Player::SimpleInput()
{
	int speed = 3;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
	}
}