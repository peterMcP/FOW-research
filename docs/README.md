# Fog of War research

I am [José Antonio Prieto](https://www.linkedin.com/in/japrietogarcia/), student of the
[Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). 
This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).

## Introduction
### A little "sight" to the history of Fog of War
The history says that the terminology of "Fog" of War comes from the battlefield, reference to uncertainty in war.
Later, board games and videogames adopted the term.
### But, what is Fog of War
In board games and videogames, Fog of War is used to hide concrete information to the player, thats it.
Hiding information is a very useful and powerfull way to improve the overall game experience, add strategy components, evolve game mechanics, incentive the player to uncover the game world and much more.
From hide enemy units or buildings, design special items that provides visibility or until your imagination can achieve to create. You are not limited on how the fog of war can be implemented. 

> But the technique of Fog of War is not the only one used to hide visual information to the player, at the end of this article you can read a quick look to other types of visibility techniques.

## Types of Fog of War
Before show you some videogames examples, we need to introduce the Fog of War flavors. Fog of War can be overalled on top of the piramid with two "flavors" or types, by one side we have the "chunky" and on the other the "smooth" fog of wars, i show you an examples:

> We talk more in deepth later, but here we go with a little introduction

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/chunky_fow.jpg" width="640">
</p>

The chunky one is the most easy to implement and understand of fog of war, we only track a "bool" array or a simple enumerator with the flags visible/not visible (we can have a shroud too, we will talk this later).

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/aoe2_conquerors_smooth_fog.jpg" width="640">
</p>

The smooth one is more tricky, but we have plenty of ways to implement, passing throught texture scaling and blurring, printing to a texture(erasing it for example), with tricky algorithms like [marching squares](https://en.wikipedia.org/wiki/Marching_squares), [tile neighbour aware method](https://web.archive.org/web/20170608082007/http://www.saltgames.com/article/awareTiles/), or the chosen one, with bitmap masking.

> This two flavor mostly are based on a tile-grid (doesn't matter if 3D or 2D graphics).

### Multiple combinations of mixing visibility layers

Most classic RTS games franchises like Age of Empires, Warcraft, StarCraft, Civilization (to say some) uses two types of "fog", by one side we have the **shroud**, this layer represents unexplored territory (we doesn't see nothing) represented mostly on pure black color, and the **fog** layer itself, with dark transparency color or custom sprites/graphics, this layer represents **explored territory** but **out** of **our sight range**. On the **fog** layer, we see the last **seen** (outdated data) updates from the enemy from when we have previously sight on this zone, and if the enemy build or moves its units under our fog layer, we doesn't see anything till we go to re-explore the zone.

> On most modern games we often only see the fog layer, without the shroud, making to a shroud a tasty old school look.

But since we are playing with visibility tricks, the combination and functions of mixing layers and visibilities can be many as we need or create.

### Quick visual look to examples of games that uses Fog of War
If we made a quick look from the beginning, we can see how very emblematic sagas of videogames are growth till today, and yes fog of war are a indispensable component that you may pass unobserved, till today.

#### Age of Empires

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/aoe2fog.jpg" width="640">
</p>

On Age of Empires we found two types of "fog", by one side we have the **shroud**, the black "background", and the fog, the explored but out of range areas with dark transparency.

#### StarCraft 2

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/sc2_wings.jpg" width="640">
</p>

On StarCraft2, like on Age of Empires we have the same layer configuration, but in this case with plus of interest, because some buildings can be moved out of previous place.

#### Leage Of Legends

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/League-of-Legends-gank.jpg" width="640">
</p>

Example of how the fog of war can be applied outside RTS games, on League of Legends (like Dota2), we only have one layer, hiding to the players the enemy information, but not the map (since the map never changes, seems to have no sense). Cool examples that uses a very strategic component, introducing the **wards**, objects that players can buy and if are well player offer a huge advantage to the team.

## Different approaches
### Quick look to different implementation methods
#### From tile-grid map to texture method (smooth fog)
##### League of Legends from Riot Games
Since we are talking about league of legends, i found a very interesting and cool [article](https://technology.riotgames.com/news/story-fog-and-war) from the developers from [Riot Games](https://www.riotgames.com/) of how they faced the migration to new re-design of the map and the adventure of the "jagged" edges of fog of war visibility.

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/sr2_fow_before_2.jpg" width="640">
</p>

At end, they coded a impressive simply well thinked upscaled method that "blurrs" one pixel. If you want to know more of how they did, for theory and more explanations go to its [article](https://technology.riotgames.com/news/story-fog-and-war) **highly recommended.**

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/lol_reworked_fog.jpg" width="640">
</p>

We can see that the result is awesome!, nice work Riot Games, and many thanks to share this type of information.

##### Iron Marines from Ironhide Game Studio

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_without_previous_ingame_noblur.png"  width="320">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_without_previous_ingame.png"  width="320">
</p>

Similar technique like explained above, another highly recommended lecture, composed by two articles explaining the evolution of the system [part1](https://blog.gemserk.com/2018/08/27/implementing-fog-of-war-for-rts-games-in-unity-1-2/) [part2](https://blog.gemserk.com/2018/11/20/implementing-fog-of-war-for-rts-games-in-unity-2-2/)

#### Print directly to texture
This technique is based to paint "unpaint" a alpha bitmap mask from a previously edited texture, checks needed pixel data from the "screen texture layer" and apply the pixel data pixel by pixel accessing it throught texture data.

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/Xkxt3.png"  width="253">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/yXIBG.png"  width="320">
</p>

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/jkxS5.png" width="580">
</p>

I recommend you take a look at this very interesting post at [StackOverFlow](https://stackoverflow.com/questions/13654753/sdl-drawing-negative-circles-fog-of-war=)

#### From tile map data, for chunky and smooth fog

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_1circle_radius3_no_smoothing.png" width="256">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_1circle_radius3.png" width="256">
</p>

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_2circles_radius3.png" width="412">
</p>

Probably the best articles that i found after a lot of diving. Very well explained and detailed. Divided in two parts, explaning both systems.

### Quick look to others 2D visibility alternatives

Apart from the technique what are focused this research, exists plenty of ways to implement other visibility solutions. From more simplistic to more complex, and merging with more knowledgemts of light physics, raycasting, etc. Occluding areas behind certain objects, walls, or any type of obstacle or mechanic. By the way the intention doesn't vary, because our intention is hide player information too, but in some cases this techniques are not used only to hide, also to calculate light sources affected areas for example.

A simple approach is to cast rays from the entity center, and obtain and fill the resultant triangles.

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/2dVisibility_2.JPG" width="320">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/2dVisibility_3.JPG" width="320">
</p>


You can found one very good example explaining this raycast technique [here](https://www.redblobgames.com/articles/visibility/):


## Selected approach

Till we use tiled map editor and all the University projects that we are running are based on a isometric tile space, i selected a similar idea that i previously found with Riot article, but mixing with the magic of bitmasking. It was a pleasant surprise to find and understand how this technique works and offer us a big potential and a simplistic code, a brilliant idea found at the last citated  Different Approaches. Probably the same underlaying technique used on games like Starcraft, Age Of Empires, Civilization, to say some, offering us this old school smooth finish.

> The amount of limit of smoothness is the artist hand design! You can shape what you want.

Here is how the final result is expected to look!:

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/final_result.gif" width="640">
</p>

### Core of smooth - overview

The core of this technique, is using bit masking, wich provides us a very smooth configuration "splitting" our fog tiles on a array of bits, used for assign every sprite change with perfect merging between visibility areas. Applying a precomputed "shape" masks bitwise ANDded to our fog data map (i explain this later).
You can find a full explanation on the [article](http://bobkoon.com/how-to-implement-a-fog-of-war-part-2-smooth/) itself **HIGHLY recommended lecture** . *Acknowledgments to its pertinent [autor](http://bobkoon.com/), many thanks to share this to the community.*

Fog data map is composed by an array of FOWTILE structures, looks like this:

```cpp
struct FOWTILE
{
	unsigned short m_bits_fog;
	unsigned short m_bits_shroud;
};
```
Since i want to implement shroud and fog, we store its bits states on separate variables.

For making a good smooth system, we need to "divide" each "pixel" or tile of our map or fog data map.

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_subpixel_example1a.png" width="96">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/fog_subpixel_example1b.png" width="380">
</p>


This image represents a North West Corner, as you can see, represented by the 0x5F hex value, or 95 on decimal base. This direction points directly to our table rects definition.

For this selected approach i select a 3x3 (9) bits "sub-pixel" divisions for each tile cell, but you can split this till you want (2x2, 4x4, etc), the most high, more definitions we need to do on our look up table of entries. (and more and more permutations of bits we can define to make a super smooth fog (and hire more graphic artists! ).

The look up table looks like this:

```cpp
// initialize table for get index of foggyTilesRects
	int i = 0;
	for (; i < NUM_FOW_ENTRIES; ++i)
	{
		fog_rects_table[i] = -1;
	}
	// index 0 are reserved to totally shrouded/fogged tile
	// full fog
	fog_rects_table[0x1FF] = 0;
	// big corners
	fog_rects_table[0x5F] = 1; // corner NW
	fog_rects_table[0x137] = 2; // corner NE
	fog_rects_table[0x1D9] = 3;// corner SW
	fog_rects_table[0x1F4] = 4; // corner SE
	// etc ...
	// more cases
	// ...
```
In this case, the values mapped to this table corresponds to an SDL_Rect array index wich stores the needed rects sections for each sprite case, looks like this:

```cpp
// foggy tiles rects
	for (int i = 0; i < MAX_FOW_GRAPHICS; ++i)
		foggyTilesRects[i] = { i * 64, 0, 64,64 };
```

Next, we need to define our "shape mask", thats it the shape wich we want to "substract" to the bits stored on each tile.
On order to make the code more readable and modificable/debuggable, store our bit definitions for each sub-pixel on a legible format:

```cpp
#define FOW_BIT_NW  (1 << 0)
#define FOW_BIT_N   (1 << 1)
#define FOW_BIT_NE  (1 << 2)
#define FOW_BIT_W   (1 << 3)
#define FOW_BIT_C   (1 << 4)
#define FOW_BIT_E   (1 << 5)
#define FOW_BIT_SW  (1 << 6)
#define FOW_BIT_S   (1 << 7)
#define FOW_BIT_SE  (1 << 8)
```
And combine them on new definitions to easily create pre-computed shape masks, like this: (remember: you can found the full project code on github page)

```cpp
#define fow_all         (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE | FOW_BIT_W | FOW_BIT_C | FOW_BIT_E |FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)
// ...
#define NUM_FOW_ENTRIES fow_all
// ...
// straights
#define fow_EEE         (FOW_BIT_SE | FOW_BIT_E | FOW_BIT_NE)
// ...
// corners
#define fow_CNE         (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N |FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
// ...
// joins
#define fow_JNE         (FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N)
// ...
```
If you don't remember or doesn't known how the bitwise operators works , you can found a great talk [here](https://stackoverflow.com/questions/141525/what-are-bitwise-shift-bit-shift-operators-and-how-do-they-work) **must see and understand how the application of bitmasking works** . [here](https://rambutan.readthedocs.io/projects/librambutan/en/master/lang/cpp/compoundbitwise.html) great documentation about compound Bitwise operators (`&=`, `|=`, `^=`). [here](https://en.wikipedia.org/wiki/Bitwise_operations_in_C#Left_shift_%3C%3C) another great article from wikipedia.

Next we have to define our precomputed shape mask, here is an example from a "circle shape mask" for radius 3.
```cpp
// circle radius 3
{
	fow_all, fow_all, fow_CNW, fow_NNN, fow_CNE, fow_all, fow_all,
	fow_all, fow_CNW, fow_JNW, fow_non, fow_JNE, fow_CNE, fow_all,
	fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_JNE, fow_CNE,
	fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE,
	fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_JSE, fow_CSE,
	fow_all, fow_CSW, fow_JSW, fow_non, fow_JSE, fow_CSE, fow_all,
	fow_all, fow_all, fow_CSW, fow_SSS, fow_CSE, fow_all, fow_all,
},
```

Finally we need to apply this mask to our fog data map (the array of FOWTILES structures previously mentioned). And this is where the magic happens thanks to compound operator &=, wich preservers the fog coverage of the map independent of the corresponding value of our mask radius array, and perfectly merges multiple areas of different visibility units without problems.

```cpp
void FowEmitter::ApplyMaskToMap(std::list<iPoint> tiles)
{
	// apply the mask
	// bitwise ANDded to fog data map

	const unsigned short *  mask = &App->fogOfWar->area_mask[radius - FOW_RADIUS_MIN][0];

	for (std::list<iPoint>::iterator iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		FOWTILE* tile = App->fogOfWar->GetFogTileAt((*iter));

		if (tile != nullptr)
		{
			tile->m_bits_fog &= *mask;
			tile->m_bits_shroud &= *mask;
		}

		mask++;
	}
	
}
```
### New dedicated module to manage Fog of War and Fog of War emitters

To manage all the entitites that provides visibility on our game, we need one new module that i named it "j1FowManager", and a one specific class that we need to put and update its position on any desired entity that we want named "FowEmitter". To create FowEmitters only we need to pass a one parameter, the radius. Radius are directly relationated with the "circle shape masks" what we have said before. For this implementation we only have two disponible radius of 3 and 4, but you can write the amount that you want, and remember, you are not constrained only to "circle" shapes, any shape that you want is a valid one, **but**, remember to get the same affected amount of tiles with the whatever function you are using to get the involved tiles for the "zone of sight".

The header of j1FowManager looks like this (apart of bit definitions and Fowtiles struct described before):

```cpp
class j1FowManager : public j1Module
{
	friend class FowEmitter;

public:
	j1FowManager();
	~j1FowManager();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	void CreateFogDataMap(const uint width, const uint height);
	FowEmitter* AddFogEmitter(uint radius);
	bool IsThisTileVisible(iPoint position) const;

public:
	FOWTILE* GetFogTileAt(iPoint position) const;
	bool CheckFogMapBoundaries(iPoint position) const;

private:
	void DebugMouseDrawTilePos() const;

private:
	SDL_Rect foggyTilesRects[MAX_FOW_GRAPHICS];
	SDL_Texture* smoothFogTex = nullptr;
	SDL_Texture* debugFogTex = nullptr;
	signed char fog_rects_table[NUM_FOW_ENTRIES + 1];
	bool debug = false;
	// stores data map size ---
	uint width;
	uint height;
	// ------------------------
	FOWTILE* fogDataMap = nullptr; // stores entire tilemap states for every tile
	std::list<FowEmitter*> currentEmitters; // stores all emitters on entities
```

And the simple class to manage the visibility spots of the desired entitites:

```cpp
// we have to add this emitter to any entity what we want
// fow emitter updates auto its visibility to fog data map
class FowEmitter
{
public:
	FowEmitter();
	FowEmitter(uint radius);
	~FowEmitter();

	bool Update(float dt);
	bool CleanUp();

public:
	void SetPos(iPoint pos);

private:
	std::list<iPoint> GetTilesAffected() const;
	void ApplyMaskToMap(std::list<iPoint> tileList);

public:
	bool to_delete = false;

private:
	uint radius;
	iPoint position; // on TILE values
};
```
And to make all this work, some other puntualizations are needed:

- We have a simplistic j1EntityFactory module that manages entitites behaviour, and a simple Entity class. If we want that specific entities provides us visibility, we need to declare a pointer to a new possible FowEmitter:

```cpp
enum class EntityType
{
	PLAYER,
	ENEMY,
	WARD
};

class Entity
{
public:
	Entity(EntityType type, iPoint position);
	virtual ~Entity();

	virtual bool Update(float dt);
	virtual	bool CleanUp();

public:
	bool Draw();

public:
	bool to_delete = false;
	EntityType type;
	iPoint position;
	SDL_Rect spriteRect;

	// TODO: add fog of war emitter
	// We need to add a fow emitter if we want that entities 
	// provides us visibility.
	FowEmitter* fogVisibilityEmitter = nullptr;

};
```
And on the interested player itself (or the entity that you want), we need to define it:

```cpp
Player::Player(iPoint position) : Entity(type, position)
{
	// Adds fow emitter
	fogVisibilityEmitter = App->fogOfWar->AddFogEmitter(3);

}
```
**Remember** to update the position of the emitter when you want:
```cpp
bool Player::Update(float dt)
{
	bool ret = true;
	// manages simple player input
	SimpleInput();

	// updates position of the emitter
	fogVisibilityEmitter->SetPos(position);

	// more functionality ...
	// ...
	return ret;
}
```
With all this put in place, we are ready to start with the implementation, but one last thing before that. If we want that enemies for example, doesn't draw if they are out of player range of sight. We need to tweak how they are drawn, this may vary dependent of your implementation, but in our case with a simple test we solve this. In our entities draw method we have:

```cpp
// TODO: check entity types of our interest to draw or not outside visibility zone
	if ((*iter)->type == EntityType::ENEMY)
	{
		if(App->fogOfWar->IsThisTileVisible((*iter)->position))
			(*iter)->Draw();
	}
	else
		(*iter)->Draw();
```

And we are ready to start with the TODO's!

## TODO's and Solutions
Quick proposals of very easy exercises to understand the versatility of this core system and the easiest implementation that we need.
### TODO1
In order to create a valid Fog data map filled with needed data to compute our fog, we need to create it first.

**Before TODO1** result:
<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/todo1_before.JPG" width="640">
</p>

**Expected** result:
<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/expected_results_1.JPG" width="640">
</p>

#### Solution:
```cpp
// TODO1: creates fog of war data, see the function on j1FowManager, 
// and pass the map data size on tile values
App->fogOfWar->CreateFogDataMap(App->map->data.columns, App->map->data.rows);
```
### TODO2
 We need to define our fogVisibilityEmitter in our player constructor since we need that players provides us a visibility.
 
 **Expected** result:
 <p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/expected_results_2.JPG" width="640">
</p>

#### Solution:
```cpp
// TODO2: Define fogVisibilityEmitter with radius 3 ---
// and set instantiation position
fogVisibilityEmitter = App->fogOfWar->AddFogEmitter(3);
fogVisibilityEmitter->SetPos(position);
```
### TODO3
If we want that visibility follow the player, we need to update its position.

Expected result: now visibility follows the player around.
<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/expected_results_3.JPG" width="640">
</p>

#### Solution
```cpp
// TODO3: update the position of the emitter to this position
fogVisibilityEmitter->SetPos(position);
```
### TODO4
Since we want a visibility mechanic, we need to filter what entities are drawn or not outside player sight radius.

**Before:**  Enemies are showing us its position under the fog! nerf them.
<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/todo4_before.JPG" width="640">
</p>

**Expected** result:
<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/expected_results_4.JPG" width="640">
</p>

#### Solution
```cpp
// TODO4: check entity types of our interest (ENEMY) to draw or not outside visibility zone
if ((*iter)->type == EntityType::ENEMY)
{
	if(App->fogOfWar->IsThisTileVisible((*iter)->position))
		(*iter)->Draw();
}
else
	(*iter)->Draw();
```

### TODO5 and last!
A lot of talk how much smooth is this fog, and its totally chunky. Yes, you need to do the last step inside j1FowManager, adapt the draw function to get the correctly mapped values to our foggyTilesRects array that stores the correct rects for any smoothed graphic.

**Expected** result:
<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/expected_results_5.JPG" width="640">
</p>


#### Solution
```cpp
// draw fog
if (frame_id_fog != -1)
{
	SDL_SetTextureAlphaMod(fogTex, 120);
				
	// TODO5 - adapt this draw function --------------------------------
	// you must to understand how the fog_rects_table mapping works ----
	App->render->Blit(fogTex, drawPos.x, drawPos.y, &foggyTilesRects[frame_id_fog]);
	// -----------------------------------------------------------------

}
// draw shroud on top
if (frame_id_shroud != -1)
{
	SDL_SetTextureAlphaMod(fogTex, 255);

	// TODO5 - adapt this draw function, same as above -----------------
	App->render->Blit(fogTex, drawPos.x, drawPos.y, &foggyTilesRects[frame_id_shroud]);
	// -----------------------------------------------------------------
}
```
Don't forget when you finish to test the debug functionality with F1 Key to help to understand how the system works.

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/debugF1.JPG" width="640">
</p>

And remember, you can instantiate more players and also wards!. Wards show you other ways to play with visibility mechanics, and don't forget, be creative!

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/image_more_players_wards.JPG" width="640">
</p>

## Improvements

 - The fog layer doesn't needs to be updated on every frame, add it to a Delayed Update or a X ticks only update.
 -  The visibility can be occluded throught certain types of tiles, or different terrain elevations, if your game play with heights this improvement is a must for you.
 - Make more table_rects permutations! if you want a super smooth or a very gradient smooth maybe you need more sprites defining different fog situations (remember sub-pixels).
 -  The dimensions of fogData Array and the tile fog size, doesn't need to be the same as your tilemap data, you can vary it if you want. Put a offseted fog to cover entire possible tilesets design that fill more of "diamond" tile shape.
 - If you have a huge map, maybe a some type of draw sorting are needed, we doesn't need to draw every fog tile. The same principle is used to cycle update.

## Acknowledgements and Webgraphy
[Riot Games Article of how they implemented new Fog Of War](https://technology.riotgames.com/news/story-fog-and-war)

[Fog of War - Wikipedia](https://en.wikipedia.org/wiki/Fog_of_war)

[Fog of War and 2D grid - StackOverflow](https://stackoverflow.com/questions/13936368/fog-of-war-and-2d-grid)

[How efficiently implement Fog of War in realtime- Post StackExchange gamedev](https://gamedev.stackexchange.com/questions/134040/how-do-i-efficiently-implement-fog-of-war-in-a-real-time-strategy-game)

[Iron Marines Fog of War implementation part1](https://blog.gemserk.com/2018/08/27/implementing-fog-of-war-for-rts-games-in-unity-1-2/)

[Iron Marines Fog of War implementation part2](https://blog.gemserk.com/2018/11/20/implementing-fog-of-war-for-rts-games-in-unity-2-2/)

[How to implement a Fog of War chunky System](http://bobkoon.com/how-to-implement-a-fog-of-war-part-1-chunky/)

[How to implement a Fog of War smooth System](http://bobkoon.com/how-to-implement-a-fog-of-war-part-2-smooth/)

[Efficient Fog Of War - GameDev.net](https://www.gamedev.net/forums/topic/530346-efficient-fog-of-war/)

[Implementing Fog Of War throught render to texture](http://damienclassen.blogspot.com/2014/02/implementing-fog-of-war.html)

[Implementing Fog Of War throught marching squares](https://startupfreakgame.com/2017/02/12/creating-fog-of-war-in-games/)

["Drawing negative circles" how to draw onto texture to simulate Fog of War - StackOverFlow](https://stackoverflow.com/questions/13654753/sdl-drawing-negative-circles-fog-of-war=)

[Neighbour Aware tile selection](https://web.archive.org/web/20170608082007/http://www.saltgames.com/article/awareTiles/)

[Extense list of how comercial games uses Fog of War](https://tvtropes.org/pmwiki/pmwiki.php/Main/FogOfWar)

[Fog of War on comercial games - list -](https://www.giantbomb.com/fog-of-war/3015-14/)


### Very interesting and useful articles
[2D visibility](https://www.redblobgames.com/articles/visibility/)

[Field of View](http://www.roguebasin.com/index.php?title=Field_of_Vision)

[Roguelike vision algorithms](http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html)

[Dynamic 2D shadow casting](http://archive.gamedev.net/archive/reference/programming/features/2dsoftshadow/page3.html)

[Gaussian Blurr with linear sampling](http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/)

[The HqX pixel art scaling algorithm](https://en.wikipedia.org/wiki/Hqx)

[HqX scaling filter more in deepth](http://blog.pkh.me/p/19-butchering-hqx-scaling-filters.html)

[Upscaling pixel art with extra smooth quality](http://johanneskopf.de/publications/pixelart/)

[Fast circle algorithm- Bresenhams, also "midpoint" circle - StackOverFlow](https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles)

[Bresenham's algorithm - wikipedia](https://es.wikipedia.org/wiki/Algoritmo_de_Bresenham)

[Implementing Fog of War on Unity](https://andrewhungblog.wordpress.com/2018/06/23/implementing-fog-of-war-in-unity/)

[Marching squares - wikipedia](https://en.wikipedia.org/wiki/Marching_squares)

[Fog of War on strategy games on general](http://keithburgun.net/fog-of-war-in-push-the-lane-and-strategy-games-generally/)

[Blurring on graphic cards without shaders](http://www.ulrichmierendorff.com/software/opengl_blur.html)

[SDL - Blurring with texture render target trick](https://discourse.libsdl.org/t/question-blur-filters-on-sdl-textures/21145)

