#ifndef _sloth_h
#define _sloth_h

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <unistd.h>

#define MAX_PLAYING_SOUND 10 //decided arbitrarily

struct point {float x,y;};

struct input
{
	char key[SDLK_LAST];
	char mousebuttons[8];
	struct point mouse;
	char quit;
};

struct sound_data
{
	Uint32 length; // length of our sample
	Uint8* buffer; // audio buffer
	SDL_AudioSpec spec;//try to guess :)
	Uint8 *currpos; // pointer to audio buffer to played
	Uint32 currlength; // remaining length of the sample we have to play
	Uint8 state; // 0:stop /  1: play / 2: pause 
	int volume;
};

struct audiop
{
	struct sound_data* s[MAX_PLAYING_SOUND];
	SDL_AudioSpec spec;
};

typedef struct point point, POINT;
typedef Uint32 COLOR, color;
typedef TTF_Font* font;
typedef SDL_Surface* texture;
typedef SDL_Rect rect;
typedef struct sound_data* sound;
typedef struct audiop audiop;

/**Init/Quit**/
void slothWindow(int width, int height, char* title, int fulls);
int shouldClose();
void slothRest();

/**Graphics**/
COLOR colorRgb(int r, int g, int b);
int	on_screen(int x, int y);
void drawPixel(POINT p, COLOR c);
void drawLine(POINT p1, POINT p2, COLOR color);
void drawRect(POINT p1, POINT p2, COLOR c);
void drawRectFill(POINT p1, POINT p2, COLOR c);
void drawCircle(point center, int radius, color c);
void drawCircleFill(point center, int radius, color c);
void drawTriangle(point p1, point p2, point p3, color c);
void drawTriangleFill(point p1, point p2, point p3, color c);
void fillScreen(color c);

/**Texture**/
texture loadBmp(char *path, color key);
void drawSprite(texture tex, point p);
void drawSpriteClip(texture tex, point p, rect clip);
void closeSprite(texture tex);

/**Text**/
font loadFont(char *path, int size);
void setFont(font f);
void writeText(point pos, char *text, color c);
void closeFont(font f);

/**Audio**/
sound loadWav(char *filename);
void playSound(sound s, int volume);
void pauseSound(sound s);
void pauseAllSound();
void stopSound(sound s);
void stopAllSound();
void closeWav(sound s);

/**Other**/
void wait(int ms);
float randf(void);
int	randi(int min, int max);

/**Input**/
void updateInput();
int getKey(int numkey);
int getMouseB(int numbutton);
int getMouseX();
int getMouseY();
void waitKey(int numkey);

/**Time**/
float getDt();
float getTime();

/**Render**/
void render();
void sync();

/**Mouse**/
#define m_left	SDL_BUTTON_LEFT
#define m_middle	SDL_BUTTON_MIDDLE
#define m_right	SDL_BUTTON_RIGHT
#define m_wheelup	SDL_BUTTON_WHEELUP
#define m_wheeldown	SDL_BUTTON_WHEELDOWN

/**Keyboard**/
#define k_0			SDLK_0
#define k_1			SDLK_1
#define k_2			SDLK_2
#define k_3			SDLK_3
#define k_4			SDLK_4
#define k_5			SDLK_5
#define k_6			SDLK_6
#define k_7			SDLK_7
#define k_8			SDLK_8
#define k_9			SDLK_9
#define k_A			SDLK_a
#define k_B			SDLK_b
#define k_C			SDLK_c
#define k_D			SDLK_d
#define k_E			SDLK_e
#define k_F			SDLK_f
#define k_G			SDLK_g
#define k_H			SDLK_h
#define k_I			SDLK_i
#define k_J			SDLK_j
#define k_K			SDLK_k
#define k_L			SDLK_l
#define k_M			SDLK_m
#define k_O			SDLK_o
#define k_P			SDLK_p
#define k_R			SDLK_r
#define k_S			SDLK_s
#define k_T			SDLK_t
#define k_U			SDLK_u
#define k_V			SDLK_v
#define k_W			SDLK_w
#define k_X			SDLK_x
#define k_Y			SDLK_y
#define k_Z			SDLK_z
#define k_backspace	SDLK_BACKSPACE
#define k_escape 	SDLK_ESCAPE
#define k_return 	SDLK_RETURN
#define k_space		SDLK_SPACE
#define k_f1		SDLK_F1
#define k_f2		SDLK_F2
#define k_f3		SDLK_F3
#define k_f4		SDLK_F4
#define k_f5		SDLK_F5
#define k_f6		SDLK_F6
#define k_f7		SDLK_F7
#define k_f8		SDLK_F8	
#define k_f9		SDLK_F9
#define k_f10		SDLK_F10
#define k_f11		SDLK_F11
#define k_kp0		SDLK_KP0
#define k_kp1		SDLK_KP1
#define k_kP2		SDLK_KP2
#define k_kp3		SDLK_KP3
#define k_kp4		SDLK_KP4
#define k_kp5		SDLK_KP5
#define k_kp6		SDLK_KP6
#define k_kp7		SDLK_KP7
#define k_kp8		SDLK_KP8
#define k_kp9		SDLK_KP9
#define k_up		SDLK_UP
#define k_down		SDLK_DOWN
#define k_left		SDLK_LEFT
#define k_right		SDLK_RIGHT
#define k_numlock	SDLK_NUMLOCK
#define k_capslock	SDLK_CAPSLOCK
#define k_scrollock	SDLK_SCROLLOCK
#define k_lshift	SDLK_LSHIFT
#define k_rshift	SDLK_RSHIFT
#define k_lctrl		SDLK_LCTRL
#define k_rctrl		SDLK_RCTRL
#define k_lalt		SDLK_LALT
#define k_ralt		SDLK_RALT

/**Color**/
#define aliceblue            0xF0F8FF
#define antiquewhite         0xFAEBD7
#define aqua                 0x00FFFF
#define aquamarine           0x7FFFD4
#define azure                0xF0FFFF
#define beige                0xF5F5DC
#define bisque               0xFFE4C4
#define black                0x000000
#define blanchedalmond       0xFFEBCD
#define blue                 0x0000FF
#define blueviolet           0x8A2BE2
#define brown                0xA52A2A
#define burlywood            0xDEB887
#define cadetblue            0x5F9EA0
#define chartreuse           0x7FFF00
#define chocolate            0xD2691E
#define coral                0xFF7F50
#define cornflowerblue       0x6495ED
#define cornsilk             0xFFF8DC
#define crimson              0xDC143C
#define cyan                 0x00FFFF
#define darkblue             0x00008B
#define darkcyan             0x008B8B
#define darkgoldenrod        0xB8860B
#define darkgray             0xA9A9A9
#define darkgreen            0x006400
#define darkkhaki            0xBDB76B
#define darkmagenta          0x8B008B
#define darkolivegreen       0x556B2F
#define darkorange           0xFF8C00
#define darkorchid           0x9932CC
#define darkred              0x8B0000
#define darksalmon           0xE9967A
#define darkseagreen         0x8FBC8F
#define darkslateblue        0x483D8B
#define darkslategray        0x2F4F4F
#define darkturquoise        0x00CED1
#define darkviolet           0x9400D3
#define deeppink             0xFF1493
#define deepskyblue          0x00BFFF
#define dimgray              0x696969
#define dodgerblue           0x1E90FF
#define firebrick            0xB22222
#define floralwhite          0xFFFAF0
#define forestgreen          0x228B22
#define fuchsia              0xFF00FF
#define gainsboro            0xDCDCDC
#define ghostwhite           0xF8F8FF
#define gold                 0xFFD700
#define goldenrod            0xDAA520
#define gray                 0x808080
#define green                0x008000
#define greenyellow          0xADFF2F
#define honeydew             0xF0FFF0
#define hotpink              0xFF69B4
#define indianred            0xCD5C5C
#define indigo               0x4B0082
#define ivory                0xFFFFF0
#define khaki                0xF0E68C
#define lavender             0xE6E6FA
#define lavenderblush        0xFFF0F5
#define lawngreen            0x7CFC00
#define lemonchiffon         0xFFFACD
#define lightblue            0xADD8E6
#define lightcoral           0xF08080
#define lightcyan            0xE0FFFF
#define lightgoldenrodyellow 0xFAFAD2
#define lightgreen           0x90EE90
#define lightgrey            0xD3D3D3
#define lightpink            0xFFB6C1
#define lightsalmon          0xFFA07A
#define lightseagreen        0x20B2AA
#define lightskyblue         0x87CEFA
#define lightslategray       0x778899
#define lightsteelblue       0xB0C4DE
#define lightyellow          0xFFFFE0
#define lime                 0x00FF00
#define limegreen            0x32CD32
#define linen                0xFAF0E6
#define magenta              0xFF00FF
#define maroon               0x800000
#define mediumaquamarine     0x66CDAA
#define mediumblue           0x0000CD
#define mediumorchid         0xBA55D3
#define mediumpurple         0x9370DB
#define mediumseagreen       0x3CB371
#define mediumslateblue      0x7B68EE
#define mediumspringgreen    0x00FA9A
#define mediumturquoise      0x48D1CC
#define mediumvioletred      0xC71585
#define midnightblue         0x191970
#define mintcream            0xF5FFFA
#define mistyrose            0xFFE4E1
#define moccasin             0xFFE4B5
#define navajowhite          0xFFDEAD
#define navy                 0x000080
#define oldlace              0xFDF5E6
#define olive                0x808000
#define olivedrab            0x6B8E23
#define orange               0xFFA500
#define orangered            0xFF4500
#define orchid               0xDA70D6
#define palegoldenrod        0xEEE8AA
#define palegreen            0x98FB98
#define paleturquoise        0xAFEEEE
#define palevioletred        0xDB7093
#define papayawhip           0xFFEFD5
#define peachpuff            0xFFDAB9
#define peru                 0xCD853F
#define pink                 0xFFC0CB
#define plum                 0xDDA0DD
#define powderblue           0xB0E0E6
#define purple               0x800080
#define red                  0xFF0000
#define rosybrown            0xBC8F8F
#define royalblue            0x4169E1
#define saddlebrown          0x8B4513
#define salmon               0xFA8072
#define sandybrown           0xF4A460
#define seagreen             0x2E8B57
#define seashell             0xFFF5EE
#define sienna               0xA0522D
#define silver               0xC0C0C0
#define skyblue              0x87CEEB
#define slateblue            0x6A5ACD
#define slategray            0x708090
#define snow                 0xFFFAFA
#define springgreen          0x00FF7F
#define steelblue            0x4682B4
#define tan                  0xD2B48C
#define teal                 0x008080
#define thistle              0xD8BFD8
#define tomato               0xFF6347
#define turquoise            0x40E0D0
#define violetlight          0xEE82EE
#define wheat                0xF5DEB3
#define white                0xFFFFFF
#define whitesmoke           0xF5F5F5
#define yellow               0xFFFF00
#define yellowgreen          0x9ACD32

#endif
