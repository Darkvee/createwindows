//Game states
#define MAIN_MENU	0
#define IN_GAME	1

//Used in main menu
#define PLAY 0
#define EXIT 1

/**************************
 * Includes
 * 
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include "../../../Code/Image.h"
#include "../../../Code/Font.h"
#include "../../../Code/Sound.h"

#include <sstream>
using namespace std;

/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);


/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "PONG";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "PONG", "PONG", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 800, 600,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
    DevIL_Init();
    BuildFont(hDC);

    //Load game Assets
	Image paddle("../../../Assets/paddle.tga");
	Image ball("../../../Assets/ball.tga");

	Init_Sound();
	Sound ball_paddle_hit("../../../Assets/paddle_hit.wav");
	Sound goal_hit("../../../Assets/goal.wav");
	Sound wall_hit("../../../Assets/wall_hit.wav");


    //get opengl ready to draw
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,800,0,600,0,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Number of collision from computer
    int number_of_collisions_with_computer = 0;

    //Collisions with computer
    bool Is_colliding_top_computer = false;
    bool Is_colliding_bottom_computer = false;
    bool Is_colliding_left_computer = false;
    bool Is_colliding_right_computer = false;

    //Number of collision from player
    int number_of_collisions_with_player = 0;

    //Collisions with player
    bool Is_colliding_top = false;
    bool Is_colliding_bottom = false;
    bool Is_colliding_left = false;
    bool Is_colliding_right = false;

    //computer position
    int computer_x = 700;
    int computer_y = 300;

    //player position
    int player_x = 100;
    int player_y = 300;

    //ball position
    int ball_x = 400;
    int ball_y = 0; 

    //ball direction
    int ball_direction_x = -1;
    int ball_direction_y = 1;

    //Speed of ball
    int ball_speed = 1;

    int player_score = 0;
    int computer_score = 0;
   
    //Game State
    int game_state = MAIN_MENU;
    LARGE_INTEGER previous_code_tick;
    float game_timer = 60;

    //User menu selection
    int user_menu_selection = 0;

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT);

		if(game_state == MAIN_MENU)
		{
			glPrint("PONG", 300, 525, 1,1,1);

			if(GetAsyncKeyState(VK_UP))
			{
				user_menu_selection = PLAY;
			}
			
			if(GetAsyncKeyState(VK_DOWN))
			{
				user_menu_selection = EXIT;
			}

			if(user_menu_selection == PLAY)
			{
				glPrint("PLAY", 300, 300, 1,1,1);

				if(GetAsyncKeyState(VK_SPACE))
				{
					game_state = IN_GAME; 
					QueryPerformanceCounter(&previous_code_tick);
					game_timer = 60;
					ball_speed = 1;
					player_score = 0;
					computer_score = 0;
				}
			}
			else
			{
				glPrint("PLAY", 300, 300, .5,.5,.5);
			}
			
			if(user_menu_selection == EXIT)
			{
				glPrint("EXIT", 310, 200, 1,1,1);
				if(GetAsyncKeyState(VK_SPACE))
				{
					bQuit = true;
				}
			}
			else
			{
				glPrint("EXIT", 310, 200, .5,.5,.5);
			}
		}
		if(game_state == IN_GAME)
		{ 
			LARGE_INTEGER cpuSpeed;
			LARGE_INTEGER current_code_tick;
			QueryPerformanceCounter(&current_code_tick);
			QueryPerformanceFrequency(&cpuSpeed);  

			float elapsed_time = current_code_tick.QuadPart - previous_code_tick.QuadPart;
			elapsed_time = elapsed_time /  cpuSpeed.QuadPart;
			game_timer -= elapsed_time;
			previous_code_tick = current_code_tick;

			if(game_timer < 0)
			{
				game_state = MAIN_MENU;
			}
			
			/* OpenGL animation code goes here */
		  
			stringstream stream;
			stream << player_score;

			//Print player score
			glPrint(stream.str().c_str(), 150, 525,1,1,1);
		
			//Print computer score
			stream.str("");
			stream << computer_score;
			glPrint(stream.str().c_str(), 600, 525,1,1,1); 

			stream.str("");
			stream << int(game_timer);
			glPrint(stream.str().c_str(), 350,525,1,1,1);
			
			// Is player hitting up key?
			if(GetAsyncKeyState(VK_UP))
			{
				player_y += 6;
			}  

			// Is player hitting down key?
			if(GetAsyncKeyState(VK_DOWN))
			{
				player_y -= 6;
			} 

			//Player
			//Wall paddle collision to stay inbounds for player
			if(player_y < 50)
			{
				player_y = 50;
			}
			else 
			if(player_y > 550)
			{
				player_y = 550;
			}

			//AI

			//Wall paddle collision to stay inbounds for computer
			if(computer_y < 50)
			{
				computer_y = 50;
			}
			else if(computer_y > 550)
			{
				computer_y = 550;
			}


			// AI Intelligence how the computer plays
				if(ball_x > 300)
				{
					if(ball_y > computer_y)
					{	
						computer_y += 6;		
					}

					if(ball_y < computer_y)
					{
						computer_y -= 6;
					}
				}
			
				if(ball_x < 300)
				{
					if(ball_y < 300)
					{
							
						computer_y += 1;
						if(computer_y > 500)
						{
							computer_y = 500;
						}
						
					}
					else if(ball_y > 300)
					{
						computer_y -= 1;
						if(computer_y < 100)
						{
							computer_y = 100;
						}
					}
				}
			

			// Ball movement
			ball_x += ball_direction_x * ball_speed;
			ball_y += ball_direction_y * ball_speed;

			if(ball_speed > 12)
				ball_speed = 7;

			//left edge
			if(ball_x > (player_x-20) )
			{
				if(Is_colliding_left == false)
				{
					number_of_collisions_with_player += 1;
					//Collision with left edge
					if(number_of_collisions_with_player == 4)
					{
						
						//Flip  x-ball direction
						ball_direction_x *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_left = true;
				
			}
			else
			{
				if(Is_colliding_left == true)	
				{
					number_of_collisions_with_player -= 1;
				}
				Is_colliding_left = false;
			}
			//right edge
			if(ball_x < (player_x+20) )
			{
				if(Is_colliding_right == false)
				{
					number_of_collisions_with_player += 1;
					//Collision with right edge
					if(number_of_collisions_with_player == 4)
					{
						
						//Flip  x-ball direction
						ball_direction_x *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_right = true;
			}
			else
			{
				if(Is_colliding_right == true)	
				{
					number_of_collisions_with_player -= 1;
				}
				Is_colliding_right = false;
			}
			//top edge
			if(ball_y < (player_y+40) )
			{
				if(Is_colliding_top == false)
				{
					number_of_collisions_with_player += 1;
					//Collision with top edge
					if(number_of_collisions_with_player == 4)
					{
						
						//Flip  y-ball direction
						ball_direction_y *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_top = true;
			}
			else
			{
				if(Is_colliding_top == true)	
				{
					number_of_collisions_with_player -= 1;
				}
				Is_colliding_top = false;
			}
			//bottom edge
			if(ball_y > (player_y-40)  )
			{
				if(Is_colliding_bottom == false)
				{
					number_of_collisions_with_player += 1;
					//Collision with bottom edge
					if(number_of_collisions_with_player == 4)
					{
						
						//Flip  y-ball direction
						ball_direction_y *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_bottom = true; 
			}
			else
			{
				if(Is_colliding_bottom == true)	
				{
					number_of_collisions_with_player -= 1;
				}
				Is_colliding_bottom = false;
			}


			// Collision between ball and computer
			//left edge
			if(ball_x > (computer_x-20) )
			{
				if(Is_colliding_left_computer == false)
				{
					number_of_collisions_with_computer += 1;
					//Collision with left edge
					if(number_of_collisions_with_computer == 4)
					{
						
						//Flip  x-ball direction
						ball_direction_x *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_left_computer = true;
				
			
			}
			else
			{
				if(Is_colliding_left_computer == true)	
				{
					number_of_collisions_with_computer -= 1;
				}
				Is_colliding_left_computer = false;
			}
			//right edge
			if(ball_x < (computer_x+20) )
			{
				if(Is_colliding_right_computer == false)
				{
					number_of_collisions_with_computer += 1;
					//Collision with right edge
					if(number_of_collisions_with_computer == 4)
					{
						
						//Flip  x-ball direction
						ball_direction_x *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_right_computer = true;
			}
			else
			{
				if(Is_colliding_right_computer == true)	
				{
					number_of_collisions_with_computer -= 1;
				}
				Is_colliding_right_computer = false;
			}
			//top edge
			if(ball_y < (computer_y+40) )
			{
				if(Is_colliding_top_computer == false)
				{
					number_of_collisions_with_computer += 1;
					//Collision with top edge
					if(number_of_collisions_with_computer == 4)
					{
						
						//Flip  y-ball direction
						ball_direction_y *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_top_computer = true;
			}
			else
			{
				if(Is_colliding_top_computer == true)	
				{
					number_of_collisions_with_computer -= 1;
				}
				Is_colliding_top_computer = false;
			}
			//bottom edge
			if(ball_y > (computer_y-40)  )
			{
				if(Is_colliding_bottom_computer == false)
				{
					number_of_collisions_with_computer += 1;
					//Collision with bottom edge
					if(number_of_collisions_with_computer == 4)
					{
						
						//Flip  y-ball direction
						ball_direction_y *= -1;
						ball_speed += 1;
						ball_paddle_hit.Play();
					}
				}
				Is_colliding_bottom_computer = true; 
			}
			else
			{
				if(Is_colliding_bottom_computer == true)	
				{
					number_of_collisions_with_computer -= 1;
				}
				Is_colliding_bottom_computer = false;
			}

			//Collision between left edge and ball
			if(ball_x < 0)
			{
				goal_hit.Play();  
				ball_direction_x = ball_direction_x *-1; 
				computer_score += 1;

				ball_x = 400;
				ball_y = 300;
				ball_speed = 1;
			}
			//Collision between top edge and ball
			if(ball_y > 600)
			{
				wall_hit.Play();
				ball_direction_y = ball_direction_y *-1;
			
			}
			//Collision between right edge and ball
			if(ball_x > 800)
			{
				goal_hit.Play(); 
				ball_direction_x = ball_direction_x *-1;
				player_score += 1;

				ball_x = 400;
				ball_y = 300;
				ball_speed = 1;
			}
			//Collision between bottom edge and ball
			if(ball_y < 0)
			{
				wall_hit.Play();
				ball_direction_y = ball_direction_y*-1;
			}

			//player
			paddle.DrawImage(player_x,player_y,-1);

			//computer
			paddle.DrawImage(computer_x,computer_y,-1);

			//ball
			ball.DrawImage(ball_x,ball_y,-1);

			
		}

		SwapBuffers(hDC); 
		Sleep(10);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}






