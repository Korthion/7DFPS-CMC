#include"timed_functions.h"

void shotfade(int a)
{ 
    Sound.StopShotFade();
	Sound.playShotFade();
}

void firerate(int a)
{
    clearToShoot=true;
}

void accuracyfalloff(int a)
{
    if (accuracy>0)
		{
		  accuracy=accuracy-0.002;		  
		}
	if(recoil>0)
		{
		recoil=recoil-0.9;
		if (recoil <0.9)
			{recoil=0;}		
		}	
	glutTimerFunc(50, accuracyfalloff,0);
}

void walk(int a)
	{
	Sound.resume_Walk();
	}

void stopwalk(int a)
	{
	Sound.pause_Walk();
	}