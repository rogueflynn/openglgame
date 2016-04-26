# openglgame

This is our project for our graphics class.  The project is intended to be a game but as 
of right now, we are still unsure of what type of game that it will be. This version of the project 
currently doesn't do much.  It's just a a square that you can move and another square that is static.
I'm going to try to add collision detection tomorrow.  The aim is to do a 2D game with 3D textures.
We are using vertex and fragment shaders to load the graphics in the GPU.  

3/7/2016 - Version 0
Two different color blocks.  One moves and the other remains static.

3/8/2016 - Version 0.01
Added collision detection using the aabb algorithm

3/9/2016 - Version 0.015
Added collision detection with multiple objects. Can collide with enemy and tiles.
Decoupled much of the programming code.

3/20/2016 - Version 0.02
Took out shader code and immediate mode code.  Added in ship graphic.

4/26/2016 - Version 0.025
Updated render() in main.cpp to handle switching between multiple scenes using
the int 'Current_Game_Scene_Enum' and a Scene_Changer enum structure.
Current scenes setup in this version include main-menu, scrolling-intro-text, and level-1.
SceneTransition.h added for fade effect on any screne rendered.
Background.h updated with multiple draw fucntions for different scenes.
