#include "sloth.h"

/****Global variables****/
SDL_Surface* screen = NULL;
TTF_Font *m_font = NULL;
audiop mplayer;
int H;							//Global Height
Uint32 start = 0;
Uint32 currtime = 0;
float deltaTime = 0.016;		//perfect delta -> ~ 60 fps
struct input in;


/****Init/Quit****/
void initAudio();
void quitAudio();

void slothWindow(int width, int height, char* title, int fulls)
{
	H = height;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	initAudio();
	screen = SDL_SetVideoMode(width,height,32,fulls ? SDL_FULLSCREEN : SDL_SWSURFACE);
	if(screen == NULL) return ;
	SDL_WM_SetCaption(title,NULL);
	memset(&in,0,sizeof(in));
}

void slothRest()
{
	SDL_FreeSurface(screen);
	TTF_CloseFont( m_font );
	TTF_Quit();
	quitAudio();
	SDL_Quit();
}

int shouldClose()
{
	return in.quit;
}

/*******Graphics*******/
COLOR colorRgb(int r, int g, int b) { return ((r%256)<<16) + ((g%256)<<8) + (b%256); }

int	on_screen(int x, int y)
{
	return (x < 0 || x >= screen->w || y < 0 || y >= screen->h) ? 0: 1;
}

void render_pixel(int x, int y, COLOR c)
{
	if (on_screen(x, y))
		*((COLOR *)screen->pixels + ((screen->h - y - 1) * screen->w + x)) = c;
}

void drawPixel(POINT p, COLOR c)
{
	render_pixel((int)p.x,(int)p.y,c);
}

void drawLine(POINT p1, POINT p2, COLOR color)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;
	float a,b,ii,jj;
	
	if (p1.x < p2.x) {xmin=(int)p1.x; xmax=(int)p2.x;} else{xmin=(int)p2.x; xmax=(int)p1.x;}
	if (p1.y < p2.y) {ymin=(int)p1.y; ymax=(int)p2.y;} else{ymin=(int)p2.y; ymax=(int)p1.y;}
	
	if (xmin==xmax) for (j=ymin;j<=ymax;j++) render_pixel(xmin,j,color);
	if (ymin==ymax) for (i=xmin;i<=xmax;i++) render_pixel(i,ymin,color);
	
	if ((xmax-xmin >= ymax-ymin) && (ymax-ymin>0))
	{
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (i=xmin;i<=xmax;i++)
		{
			jj = a*i+b;
			j = jj;
			if (((jj-j) > 0.5) && (j < screen->h-1)) j++;
			render_pixel(i,j,color);
		}
	}
	
	if ((ymax-ymin > xmax-xmin) && (xmax-xmin>0))
	{
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (j=ymin;j<=ymax;j++)
		{
			ii = (j-b)/a;
			i = ii;
			if (((ii-i) > 0.5) && (i < screen->w-1)) i++;
			render_pixel(i,j,color);
		}
	}
}

void drawRect(POINT p1, POINT p2, COLOR c)
{
	POINT min, max;
	int cxy;
	 
	if (p1.x < p2.x) {min.x = p1.x; max.x = p2.x;} else {min.x = p2.x; max.x = p1.x;}
	if (p1.y < p2.y) {min.y = p1.y; max.y = p2.y;} else {min.y = p2.y; max.y = p1.y;}
	for (cxy = min.x; cxy <= max.x; cxy++)
	{
		render_pixel(cxy, min.y, c);
		render_pixel(cxy, max.y, c);
	}
	for (cxy = min.y; cxy <= max.y; cxy++)
	{
		render_pixel(min.x, cxy, c);
		render_pixel(max.x, cxy, c);
	}
}

void drawRectFill(POINT p1, POINT p2, COLOR color)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;
	
	if (p1.x < p2.x) {xmin=(int)p1.x; xmax=(int)p2.x;} else{xmin=(int)p2.x; xmax=(int)p1.x;}
	if (p1.y < p2.y) {ymin=(int)p1.y; ymax=(int)p2.y;} else{ymin=(int)p2.y; ymax=(int)p1.y;}
	
	for (i=xmin;i<=xmax;i++) for (j=ymin;j<=ymax;j++) render_pixel(i,j,color);
	
}

//Andre's Circle Algorythm
void drawCircle(point center, int radius, color c)
{
	int x_center = (int)center.x; int y_center = (int)center.y;
	int x = 0; int y = radius; int d = y - 1;
	
	while(y>=x)
	{ 
        render_pixel( x_center + x , y_center + y, c);
        render_pixel( x_center + y , y_center + x, c);
        render_pixel( x_center - x , y_center + y, c);
        render_pixel( x_center - y , y_center + x, c);
        render_pixel( x_center + x , y_center - y, c);
        render_pixel( x_center + y , y_center - x, c);
        render_pixel( x_center - x , y_center - y, c);
        render_pixel( x_center - y , y_center - x, c);
		if(d >= 2*x){ 
			d = d-2*x-1;
			x = x+1;
		}
		else if(d < 2*(radius-y)){
			d = d+2*y-1;
			y = y-1;
		}
		else{ 
			d = d+2*(y-x-1);
			y = y-1;
			x = x+1;
		}
	}
}

void drawCircleFill(point center, int radius, color c)
{
	point min, max, p;
	float dx, dy;
	int x, y;
	
	min.x = center.x - radius; max.x = center.x + radius;
	min.y = center.y - radius; max.y = center.y + radius;
	for (x = min.x ; x <= max.x ; x++)
	{
		dx = x - center.x;
		for (y = min.y; y <= max.y; y++)
		{
			dy = y - center.y;
			if ((float)(dx * dx + dy * dy <= radius * radius))
			{
				p.x = x; p.y = y;
				drawPixel(p, c);
			}
		}
	}
}

void drawTriangle(point p1, point p2, point p3, color c)
{
	drawLine(p1, p2, c);
	drawLine(p2, p3, c);
	drawLine(p3, p1, c);
}

void fillScreen(color c)
{
	SDL_FillRect(screen, NULL, c);
}

/**Text**/
font loadFont(char *path, int size)
{
	return TTF_OpenFont( path, size);
}

void closeFont(font f)
{
	TTF_CloseFont(f);
}

void setFont(font f)
{
	m_font = f;
}

void writeText(point pos, char *text, color c)
{
	SDL_Rect p;SDL_Surface* text_surf = NULL;
	p.x = pos.x;p.y = H - pos.y;
	SDL_Color SDLc;
	SDL_GetRGB(c, screen->format, &(SDLc.r), &(SDLc.g), &(SDLc.b));
	text_surf = TTF_RenderText_Blended(m_font, text, SDLc);
	SDL_BlitSurface(text_surf, NULL, screen, &p);
	SDL_FreeSurface(text_surf);
}

/**Texture**/
texture loadBmp(char *path, color key)
{	
	texture loadedImg = SDL_LoadBMP(path);
	texture optiImg;
	if(loadedImg != NULL)
	{
		optiImg = SDL_DisplayFormat(loadedImg);
		if(key < 0)
			SDL_SetColorKey(optiImg, SDL_SRCCOLORKEY, key);
	}
	SDL_FreeSurface(loadedImg);
	return optiImg;
}

void drawSprite(texture tex, point p)
{
	SDL_Rect pos;
	pos.x = p.x; pos.y = H - (p.y + tex->h);
	SDL_BlitSurface(tex, NULL, screen, &pos);
}

void drawSpriteClip(texture tex, point p, rect clip)
{
    SDL_Rect pos,rclip;
	
	pos.x = p.x; pos.y = H - (p.y + clip.h);
	
	rclip.x = clip.x;
	rclip.y = tex->h - (clip.y + clip.h);
	rclip.w = clip.w;
	rclip.h = clip.h;
	
	SDL_BlitSurface( tex, &rclip, screen , &pos );
}

void closeSprite(texture tex)
{
	SDL_FreeSurface(tex);
}

/**Audio**/
void audioMix(void *userdata, Uint8 *stream, int len) {
	
	int i;
	for(i = 0;i<MAX_PLAYING_SOUND; i++)
	{
		if(mplayer.s[i] != NULL && mplayer.s[i]->state == 1)
		{
			len = ( len > mplayer.s[i]->currlength ? mplayer.s[i]->currlength : len );
	
			SDL_MixAudio(stream, mplayer.s[i]->currpos, len, mplayer.s[i]->volume);
	
			mplayer.s[i]->currpos += len;
			mplayer.s[i]->currlength -= len;
		}
	}	
}

void initAudio()
{
	mplayer.spec.freq = 44000;
    mplayer.spec.format = AUDIO_S16SYS;
    mplayer.spec.channels = 2;    /* 1 = mono, 2 = stereo */
    mplayer.spec.samples = 1024;  /* Good low-latency value for callback */
    mplayer.spec.callback = audioMix;
    mplayer.spec.userdata = NULL;
	
	int i;
	for(i=0;i<MAX_PLAYING_SOUND;i++)
	{
		mplayer.s[i] = NULL;
	}

	if ( SDL_OpenAudio(&mplayer.spec, NULL) < 0 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
}

sound loadWav(char *filename)
{
	sound s = (sound)malloc(sizeof(struct sound_data));
	if( SDL_LoadWAV(filename, &s->spec, &s->buffer, &s->length) != NULL ){
		s->currlength = s->length;
		s->currpos = s->buffer;
		s->state = 0;
		s->volume = 0;
	}
	return s;
}

void playSound(sound s, int volume)
{
	int i = 0;
	int find = 0;
	if(s->state != 1)
	{
		while(!find && i<MAX_PLAYING_SOUND)//find a available place in the playing sound array "s"
		{	
			if(mplayer.s[i] == NULL || mplayer.s[i]->state != 1)
			{
				if(mplayer.s[i] != s) mplayer.s[i] = s;
				find = 1;
			}
			i++;
		}
		s->state = 1;
		s->volume = volume;
		SDL_PauseAudio(0);
	}
	if(s->state == 1 && s->currlength < 0)
	{
		stopSound(s);
	}
}

void pauseSound(sound s)
{
	s->state = 2;
}

void pauseAllSound()
{
	int i;
	for(i = 0;i < MAX_PLAYING_SOUND; i++)
	{
		if(mplayer.s[i] != NULL)
			pauseSound(mplayer.s[i]);
	}
}

void stopSound(sound s)
{
	s->state = 0;
	s->currpos = s->buffer;
	s->currlength = s->length;
}

void stopAllSound()
{
	int i;
	for(i = 0;i < MAX_PLAYING_SOUND; i++)
	{
		if(mplayer.s[i] != NULL)
			stopSound(mplayer.s[i]);
	}
}

void closeWav(sound s)
{
	SDL_FreeWAV(s->buffer);
}

void quitAudio()
{
	SDL_PauseAudio(1);
	SDL_CloseAudio();
	int i;
	for(i=0;i<MAX_PLAYING_SOUND;i++)
	{
		mplayer.s[i] = NULL;
	}
}

/**Other**/
void wait(int ms)
{
	SDL_Delay(ms);
}

float randf(void)
{
	static int init_alea = 1;
	if (init_alea) srand(getpid());
	init_alea = 0;
	return (float)rand() / (float)RAND_MAX;
}

int	randi(int min, int max)
{
	int randv(int n) { return (int)(n * randf()); }
	int myabs(int n) { return n<0 ? -n : n; }
	
	if (min == max) { return min; }
	if (min > max) { int A = min; min = max; max = A; }
	if (min == 0) {	return randv(max + 1); }
	else if (min > 0) { return randv(max - min + 1) + min; }
	else if (min < 0 && max == 0) { return -randv(myabs(min) + 1); }
	else
	{
		int A = randv(max + myabs(min) + 1);
		return (A > max) ? -(A - max) : A;
	}
}

/**Input**/
void updateInput()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				in.key[event.key.keysym.sym] = 1;
				break;
			case SDL_KEYUP:
				in.key[event.key.keysym.sym] = 0;
				break;
			case SDL_MOUSEMOTION:
				in.mouse.x = event.motion.x;
				in.mouse.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				in.mousebuttons[event.button.button] = 1;
				break;
			case SDL_MOUSEBUTTONUP: //Pour les WHEELUP et WHEELDOWN marche pas trop
					in.mousebuttons[event.button.button] = 0;
				break;
			case SDL_QUIT:
				in.quit = 1;
				break;
			default:
				break;
		}
	}	
}

int getKey(int numkey){ return in.key[numkey]; }

int getMouseX(){ return in.mouse.x; }
int getMouseY(){ return H - in.mouse.y; }
int getMouseB(int numbutton){ return in.mousebuttons[numbutton]; }

void waitKey(int numkey)
{
	while(!shouldClose() && !getKey(k_escape) && !getKey(numkey))
		updateInput();
}

/**Time**/
float getDt()
{
	return deltaTime;
}

float getTime()
{
	return SDL_GetTicks() / 1000.f;
}

/****Render****/
void render()
{
	SDL_Flip(screen);
	currtime = SDL_GetTicks();
	deltaTime = (currtime - start) / 1000.f;
		if(deltaTime < 0.0001)
			deltaTime = 0.0001;
	start = SDL_GetTicks();
	updateInput();
	
}

void sync()
{
	SDL_Flip(screen);
}
