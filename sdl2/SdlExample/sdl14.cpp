
#include "sdl_example.h"

// http://lazyfoo.net/tutorials/SDL/14_animated_sprites_and_vsync/index.php
// Copy Example 11.

namespace SDL14ExampleNamespace {
    //Texture wrapper class
    class LTexture
    {
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = NULL );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
    };

    //Screen dimension constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    //Walking animation
    const int WALKING_ANIMATION_FRAMES = 4;
    SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
    LTexture gSpriteSheetTexture;

    LTexture::LTexture()
    {
        //Initialize
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    LTexture::~LTexture()
    {
        //Deallocate
        free();
    }

    bool LTexture::loadFromFile( std::string path )
    {
        //Get rid of preexisting texture
        free();

        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == NULL )
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        }
        else
        {
            //Color key image
            SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
            if( newTexture == NULL )
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else
            {
                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }

        //Return success
        mTexture = newTexture;
        return mTexture != NULL;
    }

    void LTexture::free()
    {
        //Free texture if it exists
        if( mTexture != NULL )
        {
            SDL_DestroyTexture( mTexture );
            mTexture = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }

    void LTexture::render( int x, int y, SDL_Rect* clip )
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { x, y, mWidth, mHeight };

        //Set clip rendering dimensions
        if( clip != NULL )
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
    }

    int LTexture::getWidth()
    {
        return mWidth;
    }

    int LTexture::getHeight()
    {
        return mHeight;
    }

    //Starts up SDL and creates window
    bool init()
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
                //Create vsynced renderer for window
                gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

                // For this (and future tutorials), we want to use Vertical Sync.
                // VSync allows the rendering to update at the same time as when your monitor updates during vertical refresh.
                // For this tutorial it will make sure the animation doesn't run too fast.
                // Most monitors run at about 60 frames per second and that's the assumption we're making here.
                // If you have a different monitor refresh rate, that would explain why the animation is running too fast or slow.

                if( gRenderer == NULL )
                {
                    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                    success = false;
                }
                else
                {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor( gRenderer, 0, 0xFF, 0xFF, 0xFF );

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

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !gSpriteSheetTexture.loadFromFile( "foo_animation.png" ) )
        {
            printf( "Failed to load walking animation texture!\n" );
            success = false;
        }
        else
        {
            //Set sprite clips
            gSpriteClips[ 0 ].x =   0;
            gSpriteClips[ 0 ].y =   0;
            gSpriteClips[ 0 ].w =  64;
            gSpriteClips[ 0 ].h = 205;

            gSpriteClips[ 1 ].x =  64;
            gSpriteClips[ 1 ].y =   0;
            gSpriteClips[ 1 ].w =  64;
            gSpriteClips[ 1 ].h = 205;

            gSpriteClips[ 2 ].x = 128;
            gSpriteClips[ 2 ].y =   0;
            gSpriteClips[ 2 ].w =  64;
            gSpriteClips[ 2 ].h = 205;

            gSpriteClips[ 3 ].x = 196;
            gSpriteClips[ 3 ].y =   0;
            gSpriteClips[ 3 ].w =  64;
            gSpriteClips[ 3 ].h = 205;
        }

        return success;
    }

    void close()
    {
        //Free loaded images
        gSpriteSheetTexture.free();

        //Destroy window    
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }
}

int sdl14Main()
{
    using namespace SDL14ExampleNamespace;

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

            //Current animation frame
            int frame = 0;

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

                SDL_RenderClear(gRenderer);

                //Render current frame
                SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
                gSpriteSheetTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2, ( SCREEN_HEIGHT - currentClip->h ) / 2, currentClip );

                //Update screen
                SDL_RenderPresent( gRenderer );

                //Go to next frame
                ++frame;

                //Cycle animation
                if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                {
                    frame = 0;
                }
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}

