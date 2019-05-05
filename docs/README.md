# Fog of War research

I am [José Antonio Prieto](https://www.linkedin.com/in/jos%C3%A9-antonio-prieto-garcia-b378b0184/), student of the
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

Image

The chunky one is the most easy to implement and understand of fog of war, we only track a "bool" array or a simple enumerator with the flags visible/not visible (we can have a shroud too, we will talk this later).

Image

The smooth one is more tricky, but we have plenty of ways to implement, passing throught texture scaling and blurring, printing to a texture(erasing it for example), with tricky algorithms like [marching squares](https://en.wikipedia.org/wiki/Marching_squares), [tile neighbour aware method](https://web.archive.org/web/20170608082007/http://www.saltgames.com/article/awareTiles/), or the chosen one, with bitmap masking.

> This two flavor mostly are based on a tile-grid (doesn't matter if 3D or 2D graphics).

### Multiple combinations of mixing visibility layers

Most classic RTS games franchises like Age of Empires, Warcraft, StarCraft, Civilization (to say some) uses two types of "fog", by one side we have the **shroud**, this layer represents unexplored territory (we doesn't see nothing) represented mostly on pure black color, and the **fog** layer itself, with dark transparency color or custom sprites/graphics, this layer represents **explored territory** but **out** of **our sight range**. On the **fog** layer, we see the last **seen** updates from the enemy from when we have previously sight on this zone, and if the enemy build or moves its units under our fog layer, we doesn't see anything till we go to re-explore the zone.

> On most modern games we often only see the fog layer, without the shroud, making to a shroud a tasty old school look.

### Quick visual look to examples of games that uses Fog of War
If we made a quick look from the beginning, we can see how very emblematic sagas of videogames are growth till today, and yes fog of war are a indispensable component that you may pass unobserved, till today.

#### Age of Empires

Image

On Age of Empires we found two types of "fog", by one side we have the **shroud**, the black "background", and the fog, the explored but out of range areas with dark transparency.

#### StarCraft 2

Image

On StarCraft2, like on Age of Empires we have the same layer configuration, but in this case with plus of interest, because some buildings can be moved out of previous place.

#### Leage Of Legends

Image

Example of how the fog of war can be applied outside RTS games, on League of Legends (like Dota2), we only have one layer, hiding to the players the enemy information, but not the map (since the map never changes, seems to have no sense). Cool examples that uses a very strategic component, introducing the **wards**, objects that players can buy and if are well player offer a huge advantage to the team.

## Different approaches
### Quick look to different implementation methods


