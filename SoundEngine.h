#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace irrklang;
using namespace std;


class SoundEngine
{
  public: 
	SoundEngine():
		shotMG(engine->addSoundSourceFromFile("shot.mp3")),	
		shotPistol(engine->addSoundSourceFromFile("shotPistol.mp3")),	
		shotPistol2(engine->addSoundSourceFromFile("shotPistol2.mp3")),	
		shotsniper(engine->addSoundSourceFromFile("sniper.mp3")),	
		sniper_reload(engine->addSoundSourceFromFile("sniperload.mp3")),	
		shotfade(engine2->addSoundSourceFromFile("shotfade.mp3")),
		ambient(engine->addSoundSourceFromFile("bkg.mp3")),
		walk(engine->addSoundSourceFromFile("walk.mp3")),
		time_slow(engine->addSoundSourceFromFile("time_stop.mp3")),
		time_resume(engine->addSoundSourceFromFile("time_start.mp3")),
		reload_mg(engine->addSoundSourceFromFile("reload_mg.mp3")),
		reload_pistol(engine->addSoundSourceFromFile("reload_pistol.mp3")),
		engine(createIrrKlangDevice()),
		playSpeed(1.0),
		engine2(createIrrKlangDevice())
		  {}

	void playShotMG();
	void playShotPistol();
	void playShotFade();
	void playAmbient();
	void StopShotFade();
	void stopAmbient();
	void playWalk();
	void pause_Walk();
	void resume_Walk();
	void play_slow();
	void play_resume();
	void test();
	void playShotSniper();
	void playsniper_reload();
	void play_mg_reload();
	void play_pistol_reload();
	void drop_engines();

	ISoundEngine* engine;   
	ISoundEngine* engine2;
	float playSpeed;



private:
	
	ISoundSource* shotMG;
	ISoundSource* shotPistol;
	ISoundSource* shotPistol2;
	ISoundSource* walk;
	ISoundSource* shotfade;
	ISoundSource* ambient;
	ISoundSource* shotsniper;
	ISoundSource* sniper_reload;
	ISoundSource* time_slow;
	ISoundSource* time_resume;
	ISoundSource* reload_pistol;
	ISoundSource* reload_mg;
	ISound* shot_sound_MG;
	ISound* shotsoundPistol;
	ISound* shotsoundPistol2;
	ISound* walk_sound;
	ISound* ambientS;
	ISound* shot_sound_sniper;
	ISound* sniper_reload_sound;
	ISound* shot_fade_sound;	
	ISound* time_slow_sound;
	ISound* time_resume_sound;
	ISound* reload_mg_sound;
	ISound* reload_pistol_sound;

};