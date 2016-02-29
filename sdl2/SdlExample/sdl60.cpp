
#include "sdl_example.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
static SDL_Window* gWindow = NULL;

//The window renderer
static SDL_Renderer* gRenderer1 = NULL;
static SDL_Renderer* gRenderer2 = NULL;

//Current displayed texture
static SDL_Texture* gTexture = NULL;


//Starts up SDL and creates window
static bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer1 = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            gRenderer2 = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );

            if (gRenderer2 == NULL) {
                printf( "Dual Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
                return false;
            }

            if( gRenderer1 == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer1, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

//Loads media
static bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Nothing to load
    return success;
}

//Frees media and shuts down SDL
static void close()
{
    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    //Destroy window    
    SDL_DestroyRenderer( gRenderer1 );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer1 = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int sdl60Main()
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer1, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer1 );

                //Render red filled quad
                SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
                SDL_SetRenderDrawColor( gRenderer1, 0xFF, 0x00, 0x00, 0xFF );        
                SDL_RenderFillRect( gRenderer1, &fillRect );

                //Render green outlined quad
                SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
                SDL_SetRenderDrawColor( gRenderer1, 0x00, 0xFF, 0x00, 0xFF );        
                SDL_RenderDrawRect( gRenderer1, &outlineRect );

                //Draw blue horizontal line
                SDL_SetRenderDrawColor( gRenderer1, 0x00, 0x00, 0xFF, 0xFF );        
                SDL_RenderDrawLine( gRenderer1, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

                //Draw vertical line of yellow dots
                SDL_SetRenderDrawColor( gRenderer1, 0xFF, 0xFF, 0x00, 0xFF );
                for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
                {
                    SDL_RenderDrawPoint( gRenderer1, SCREEN_WIDTH / 2, i );
                }

                //Update screen
                SDL_RenderPresent( gRenderer1 );
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}

