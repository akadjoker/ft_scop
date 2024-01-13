Subject
II.1
 - It does not hurt to feel good.
 - Once in a while, it feels good to feel good. So we are going to create a small app to feel great.
 - This project is also organized for you to use a bit of elbow grease. So we have a few restrictions in place.


**What you need to do: **
Your goal is to create a small program that will show a 3D object conceived with a modelization program like Blender. 
The 3D object is stored in a .obj file. You will be at least in charge of parsing to obtain the requested rendering.
In a window, your 3D object will be displayed in perspective (which means that what is far must be smaller), rotate on itself around its main symmetrical axis (middle of the object basically...).  
By using various colors, it must be possible to distinguish the various sides. 

The object can be moved on three axis, in both directions.
Finally, a texture must be applicable simply on the object when we press a dedicated key, and the same key allows us to go back to the different colors. 
A soft transition between the two is requested.

The technical constraints are as follows: 
 -  You’re free to use any langages (C / C++ / Rust preferred ) 
 -  You’re free to choose between OpenGL, Vulkan, Metal and the MinilibX 
 -  Have a classic Makefile (everything you usually put in there). 
 -  You can use external libraries (other than OpenGL, Vulkan or Metal) ONLY to manage the windows and the events. 
 -  No libraries are allowed to load the 3D object, nor to make your matrixes or to loadthe shaders. 


As this is a program to auto-congratulate ourselves, it is crucial that you can present during defense at least the 42 logo given as resources, turning around its central axis (careful, not around one of its corners), with some shades of gray on the sides and a texture of poneys, kitten or unicorn your choice.

Bonus
Here are a few ideas of bonuses:

- **Correct Management of Ambiguous .obj Files:**
  - Handle ambiguous, concave, and non-coplanar .obj files correctly.
  - Render the original version of the "teapot" file with strange border effects correctly.

- **Improved Texture Application:**
  - Implement a more subtle application of the texture.
  - Ensure that the texture cannot be stretched on any side.

- **Additional Bonuses:**
  - Implement other bonuses not specified here.
  - These may include performance optimizations, visual enhancements, animations, etc.


# Some screenshots
![anim](images/anim_0.gif)
![1](images/vlcsnap-2024-01-07-20h37m46s401.png)
![2](images/vlcsnap-2024-01-07-20h37m55s462.png)
![3](images/vlcsnap-2024-01-07-20h38m08s901.png)
![4](images/vlcsnap-2024-01-07-20h38m23s761.png)


