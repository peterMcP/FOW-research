# Fog of War research

I am [José Antonio Prieto](https://www.linkedin.com/in/japrietogarcia/), student of the
[Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). 
This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).

## About the project

On this research of theory and code, i tried to compact the most relevant information that i found after a lot of internet diving.
I explain the most used approachs of Fog Of War on some succesfull videogames, why uses them and how implement the selected approach capable of:

-  Smooth Fog of War.
-  Put players with providing visibility sight based on radius.
-  Put example "Wards" that provides visibility on larger area with life time.
-  Capable of hide desired game entities outside player visibility sight range.
-  Manage multiple spots of visibility without uggly hacks and workarounds.

### Expected final result:

<p align="center">
<img src="https://raw.githubusercontent.com/peterMcP/FOW-research/master/docs/images/final_result.gif" width="640">
</p>

You can found the full article on this research [website](https://petermcp.github.io/FOW-research/)

## Installing

-  Download executable demo from [here](https://github.com/peterMcP/FOW-research/releases)
-  Unzip the downloaded file.
-  Execute the **.exe** inside the uncompressed folder.

## Controls

- **Mouse left click:** Put new players on desired position
- **Mouse right click:** Put wards that provides you a larger visibility zone on desired position.
- **Keyboard W,A,S,D:**: Moves instantiated player/s.
- **Keyboard UP,DOWN,LEFT,RIGHT:** Moves camera around.

### Debug keys

- **F1:** View debug fog tile draw

## Build with

-  [Visual Studio](https://visualstudio.microsoft.com/es/) 2017 - IDE
-  [SDL](https://www.libsdl.org/) - Development library
-  [STL](https://es.wikipedia.org/wiki/Standard_Template_Library) - standard c++ development library
-  [Tiled Map Editor](https://www.mapeditor.org/) - tile map editor

## Author

[**José Antonio Prieto**](https://www.linkedin.com/in/jos%C3%A9-antonio-prieto-garcia-b378b0184/)

-  Github Account: [peterMcP](https://github.com/peterMcP)

## License

This project is licensed under the MIT license, see the [LICENSE.md](https://github.com/peterMcP/FOW-research/blob/master/LICENSE) file for more details.

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

[Blurring on graphic carts without shaders](http://www.ulrichmierendorff.com/software/opengl_blur.html)

[SDL - Blurring with texture render target trick](https://discourse.libsdl.org/t/question-blur-filters-on-sdl-textures/21145)
