#include <iostream>
#include "SDL/SDL.h"
#include <stdlib.h> // for exit()

using namespace std;


int main( int argc, char* argv[] )
{
    cout << "Initializing SDL..." << endl;

    // Start SDL
    if( (SDL_Init( SDL_INIT_VIDEO ) == -1) )
    {
    	cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
    	exit(-1);
    }
    // SDL_Init( SDL_INIT_EVERYTHING );
    // SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_CDROM | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE | SDL_INIT_EVENTTHREAD );
    /*
     *
     * SDL_INIT_NOPARACHUTE == Prevents SDL from catching fatal signals
     * SDL_INIT_EVENTTHREAD == Runs the event manager in a separate thread
     *
     * SDL_Init returns -1 on error and 0 on success
     *
     * It is also possible at every time in the program to initialize uninitialized subsystems via SDL_InitSubSystem:
     * // Initialize the joystick subsystem
     * SDL_InitSubSystem(SDL_INIT_JOYSTICK);
     *
     * ... do something with joystick...
     *
     * and then quit some via SDL_QuitSubSystem:
     * SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
     *
     * ATTENTION about SDL_INIT_EVENTTHREAD on Win32
     * ----------------------------------------------
     * Well don't ask me why but initializing the event thread leads to SDL to crash on windows.
     * The problem is that when you leave it away on any X11 system it may, depending on the version, not produce any events.
     * The only real solution is to use conditional compiling. At least there is the WIN32 macro defined.
     * #ifdef WIN32
     * SDL_Init(SDL_INIT_VIDEO);
     * #else
     * SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTTHREAD);
     * #endif
     *
     */

    cout << "SDL initialized." << endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Get the current video hardware information
    const SDL_VideoInfo* p_VideoInfo = SDL_GetVideoInfo();

    // define the pointer to SDL_PixelFormat
    const SDL_PixelFormat* p_PixelFormat = NULL;

    // Print out some information
    if (p_VideoInfo != NULL)
    {
    	cout << endl;
    	cout << "Is it possible to create hardware surfaces? " << p_VideoInfo->hw_available << endl;
    	cout << "Is there a window manager available? " << p_VideoInfo->wm_available << endl;
    	cout << "Are hardware to hardware blits accelerated? " << p_VideoInfo->blit_hw << endl;
    	cout << "Are hardware to hardware colorkey blits accelerated? " << p_VideoInfo->blit_hw_CC << endl;
    	cout << "Are hardware to hardware alpha blits accelerated? " << p_VideoInfo->blit_hw_A << endl;
    	cout << "Are software to hardware blits accelerated? " << p_VideoInfo->blit_sw << endl;
    	cout << "Are software to hardware colorkey blits accelerated? " << p_VideoInfo->blit_sw_CC << endl;
    	cout << "Are software to hardware alpha blits accelerated? " << p_VideoInfo->blit_sw_A << endl;
    	cout << "Are color fills accelerated? " << p_VideoInfo->blit_fill << endl;
    	cout << "Total amount of video memory in Kilobytes: " << p_VideoInfo->video_mem << endl;// Total amount of video memory should be accessed only if hw_available == 1, otherwise it is equal to 0
    	cout << "Current video resolution is " << p_VideoInfo->current_w << "x" << p_VideoInfo->current_h << " pixels" << endl;
    	cout << endl;
    	cout << "Pixel format of current video device is:" << endl;
    	cout << "----------------------------------------" << endl;
    	p_PixelFormat = p_VideoInfo->vfmt;
    	cout << "Palette: " << p_PixelFormat->palette << endl;// Pointer to the palette, or NULL if the BitsPerPixel > 8
    	cout << "Bits Per Pixel: " << (int)p_PixelFormat->BitsPerPixel << endl;// BitsPerPixel: the number of bits used to represent each pixel in a surface. Usually 8, 16, 24 or 32. (1 to 7 are not allowed when creating a surface or open a video mode)
    	cout << "Bytes Per Pixel: " << (int)p_PixelFormat->BytesPerPixel << endl;// BytesPerPixel: The number of bytes used to represent each pixel in a surface. Usually one to four.
    	cout << "[RGBA]mask: " << (int)p_PixelFormat->Rmask << " " << p_PixelFormat->Gmask << " " << p_PixelFormat->Bmask << " " << p_PixelFormat->Amask;// Binary mask used to retrieve individual color values
    	cout << endl;
    }
    else
    {
    	cerr << "Get the current video hardware information failed: " << SDL_GetError() << endl;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << endl;

    SDL_Rect** modes;

    // Get available fullscreen/hardware modes
    modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);

    // Check if there are any modes available
    if (modes == (SDL_Rect**)0) {
        cerr << "No modes available!" << endl;
        exit(-1);
    }

    // Check if our resolution is restricted
    if (modes == (SDL_Rect**)-1) {
        cout << "All resolutions available." << endl;
    } else {
        // Print valid modes
        cout << "Available Modes:" << endl;
        for (int i=0; modes[i]; ++i)
          cout << "\t" << modes[i]->w << " x " << modes[i]->h << endl;
    }
    cout << endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
     * If you want to control the position on the screen when creating a windowed surface,
     * you may do so by setting the environment variables SDL_VIDEO_CENTERED=center or SDL_VIDEO_WINDOW_POS=x,y.
     */
    SDL_putenv("SDL_VIDEO_CENTERED=center"); //center the game Window
    //SDL_WM_SetCaption(szTitle, NULL);
    SDL_WM_SetCaption("Nick Pepper's SDL OpenGL test window", NULL); //sets the Window Title
    //SDL_WM_SetIcon(imgIcon, NULL);    //sets the Window Icon

////////////////////////////////////////////////////////////////////////////////////////////////////////

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );//Size of the framebuffer red component, in bits.
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );//Size of the framebuffer green component, in bits.
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );//Size of the framebuffer blue component, in bits.
    //SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 5 );//Size of the framebuffer alpha component, in bits.
    //SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 16 );//Size of the framebuffer, in bits.
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );//Enable or disable double buffering (0 or 1).
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );//Size of the depth buffer, in bits.

    /*
     * Some have found that enabling OpenGL attributes like SDL_GL_STENCIL_SIZE (the stencil buffer size)
     * before the video mode has been set causes the application to simply ignore those attributes,
     * while enabling attributes after the video mode has been set works fine.
     */
    //SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 16 );//Size of the stencil buffer, in bits.

    //SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 5 );//Size of the accumulation buffer red component, in bits.
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 5 );//Size of the accumulation buffer green component, in bits.
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 5 );//Size of the accumulation buffer blue component, in bits.
    //SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 5 );//Size of the accumulation buffer alpha component, in bits.
    //SDL_GL_SetAttribute( SDL_GL_STEREO, 1 );//Enable or disable stereo (left and right) buffers (0 or 1).
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 ); // available since SDL 1.2.10; guarantee hardware acceleration

    /*
     * In Windows, setting the video mode resets the current OpenGL context.
     * You must execute again the OpenGL initialization code (set the clear color or the shade model, or reload textures, for example)
     * after calling SDL_SetVideoMode.
     * In Linux, however, it works fine, and the initialization code only needs to be executed after the first call to SDL_SetVideoMode
     * (although there is no harm in executing the initialization code after each call to SDL_SetVideoMode, for example for a multiplatform application).
     */

    SDL_Surface* screen = SDL_SetVideoMode( 800, 600, 32, SDL_OPENGL );

    if ( screen == NULL ) {
        cerr << "Couldn't set GL mode: " << SDL_GetError() << endl;
        SDL_Quit();
        exit(-1);
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////

	const int BUFFER_SIZE = 256;
    char myBuffer[BUFFER_SIZE];

    // Print out some information about the video surface
    if (screen != NULL) {
        cout << "The current video surface width is " << screen->w << endl;
        cout << "The current video surface height is " << screen->h << endl;
        cout << "The current video surface bits per pixel is " << (int)screen->format->BitsPerPixel << endl;
    } else {
        cerr << "Video initialization failed: " << SDL_GetError() << endl;
    }

    // Obtain the video driver name
    if (SDL_VideoDriverName(myBuffer, BUFFER_SIZE) != NULL) {
    	cout << "The video driver name is " << myBuffer << endl;
    } else {
    	cerr << "Failed to obtain the video driver name." << endl;
    }
    cout << endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Pause
    cout << "...waiting for 2 seconds..." << endl;
    SDL_Delay( 2000 );

    cout << "Quitting SDL..." << endl;

    /* Shutdown all subsystems */
    SDL_Quit();

    cout << "Successfully done." << endl;

    exit(0);
}
