# Lamer Intro sgi/sun
OpenGL 1.2 Lamer Intro *oldskool*

This initial version compiles on sgi IRIX 6.5.30 (used MIPSpro 7.4) and OpenIndiana 2021.10 (g++ 7) (Solaris 10 U8 SPARC tested as well).

Requires at least glut and libmikmod.

Bear in mind that playing a mod from the demo has a performance hit of nearly 17 FPS (instead of playing the mod via mikmod, xmms etc). Maybe threading (creating an individual process from within the app) via a pthread could mitigate that performance penalty.

Remember I call it Lamer Intro because the code and structure is probably the worst thing you may have seen as professional coder .. I am not :-) 

Enjoy the code and if you have an HP-UX or AIX machine I would love to see it running on those machines as well.

Feel free to extend, improve and suggest as for me this a learning process.

There are zillions of areas to improve (TGA loader is very pitty on the format of the TGA's to load), complex demo effect handling .. c and c++ mixed (watch out for reference passing).


