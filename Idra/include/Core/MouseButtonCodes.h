#pragma once

#ifdef IDRA_WINDOW_GLFW
	// from glfw3.h
	#define IDRA_MOUSE_BUTTON_1         0
	#define IDRA_MOUSE_BUTTON_2         1
	#define IDRA_MOUSE_BUTTON_3         2
	#define IDRA_MOUSE_BUTTON_4         3
	#define IDRA_MOUSE_BUTTON_5         4
	#define IDRA_MOUSE_BUTTON_6         5
	#define IDRA_MOUSE_BUTTON_7         6
	#define IDRA_MOUSE_BUTTON_8         7
	#define IDRA_MOUSE_BUTTON_LAST      IDRA_MOUSE_BUTTON_8
	#define IDRA_MOUSE_BUTTON_LEFT      IDRA_MOUSE_BUTTON_1
	#define IDRA_MOUSE_BUTTON_RIGHT     IDRA_MOUSE_BUTTON_2
	#define IDRA_MOUSE_BUTTON_MIDDLE    IDRA_MOUSE_BUTTON_3
#endif // IDRA_WINDOW_GLFW