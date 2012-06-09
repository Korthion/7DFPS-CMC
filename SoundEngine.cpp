#include"SoundEngine.h"



void SoundEngine::playShotMG()
{
shot_sound_MG = engine->play2D(shotMG,false,false,false,true);
shot_sound_MG->setPlaybackSpeed(playSpeed);
}


void SoundEngine::playShotSniper()
{
shot_sound_sniper = engine->play2D(shotsniper,false,false,false,true);
shot_sound_sniper->setPlaybackSpeed(playSpeed);
}

void SoundEngine::playsniper_reload()
{
sniper_reload_sound = engine->play2D(sniper_reload,false,false,false,true);
sniper_reload_sound->setPlaybackSpeed(playSpeed);
}


void SoundEngine::play_mg_reload()
{
reload_mg_sound = engine->play2D(reload_mg,false,false,false,true);
reload_mg_sound->setPlaybackSpeed(playSpeed);
}
void SoundEngine::play_pistol_reload()
{
reload_pistol_sound = engine->play2D(reload_pistol,false,false,false,true);
reload_pistol_sound->setPlaybackSpeed(playSpeed);
}

void SoundEngine::playShotPistol()
{

int random = rand() % 2 + 1;

if (random==1)
	{
	shotsoundPistol = engine->play2D(shotPistol,false,false,false,true);
	shotsoundPistol->setPlaybackSpeed(playSpeed);
	}
else if (random==2)
	{
	shotsoundPistol2 = engine->play2D(shotPistol2,false,false,false,true);
	shotsoundPistol2->setPlaybackSpeed(playSpeed);
	}
}

void SoundEngine::playShotFade()
{
shot_fade_sound = engine2->play2D(shotfade,false,false,false,true);

if(playSpeed==1)
	{
	shot_fade_sound->setVolume(1.0);
	shot_fade_sound->setPlaybackSpeed(playSpeed);
	}

else 
	{
	shot_fade_sound->setVolume(5.5);
	shot_fade_sound->setPlaybackSpeed(0.7);
	}
}

void SoundEngine::playAmbient()
{
ambient->setDefaultVolume(0.3);

ambientS=engine->play2D(ambient,true,false,true,true);
ambientS->setPlaybackSpeed(playSpeed);
}

void SoundEngine::StopShotFade()
{
engine2->stopAllSounds();
}


void SoundEngine::playWalk()
{

walk_sound = engine->play2D(walk,true,false,true);

}

void SoundEngine::pause_Walk()
{
if(walk_sound->getIsPaused()==false)
	{
	walk_sound->setIsPaused(true);
	}
}

void SoundEngine::resume_Walk()
{
walk_sound->setPlaybackSpeed(playSpeed);
walk_sound->setIsPaused(false);
}

void SoundEngine::play_slow()
{
time_slow_sound = engine->play2D(time_slow,false,false,false,true);
time_slow_sound->setPlaybackSpeed(1);
}

void SoundEngine::play_resume()
{
time_resume_sound = engine->play2D(time_resume,false,false,false,true);
}

void SoundEngine::drop_engines()
{
 engine->drop(); 
 engine2->drop();
}