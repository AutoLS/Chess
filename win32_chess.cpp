#include <AutoEngine.h>
#include "chess.cpp"

int wmain()
{
	if(AE_InitLib(AE_LIB_SDL_FULL) == 0)
	{
		printf("Init lib successfully!\n");
		AE_Graphics Graphics = {};
		
		if(AE_InitGraphics(&Graphics, "Chess", V2(800, 800), 0) == 0)
		{
			printf("Init graphics successfully!\n");
			
			AE_Core Core = AE_InitCore(&Graphics);
			
			bool Running = true;
			
			chess_game Chess = InitChess(&Core);
			
			while(Running)
			{
				if(AE_HandleEvents(&Core))
				{
					Running = false;
				}
				
				AE_RenderClear(&Graphics, Color(49, 46, 43));
				//Update
				PlayerMove(&Core.Input, &Chess);
				if(IsKingDead(&Chess.White) || IsKingDead(&Chess.Black))
				{
					InitPlayers(&Chess);
					Chess.State = STATE_PLAYER_WHITE;
				}
				
				AE_LockFPS(&Core.Clock, 144);
				//Render
				
				DrawBoard(&Graphics, &Chess);
				
				AE_RenderShow(&Graphics);
				
				AE_UpdateClock(&Core.Clock);
				//printf("FPS: %.1f\n", AE_GetFPS(&Core.Clock));
			}
		}
		else
		{
			AE_PrintLastError();
		}
	}
	else
	{
		AE_PrintLastError();
	}
	return 0;
}