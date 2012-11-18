/* Adds sound to system */
int wcsystem_process_sound(FILE *f,int code,int len)
{
	if(code == WCSYSTEM_CURSOR)
	{
		wcsys_snd_cursor = new sound(f);
		return 1;
	}
	if(code == WCSYSTEM_DECIDE)
	{
		wcsys_snd_choice = new sound(f);
		return 1;
	}
	if(code == WCSYSTEM_CANCEL)
	{
		wcsys_snd_cancel = new sound(f);
		return 1;
	}
	if(code == WCSYSTEM_BUZZER)
	{
		wcsys_snd_buzzer = new sound(f);
		return 1;
	}
	if(code == WCSYSTEM_BATTLE)
	{
		wcsys_snd_battle = new sound(f);
		return 1;
	}
	if(code == WCSYSTEM_DEFEATED)
	{
		wcsys_snd_defeated = new sound(f);
		return 1;
	}
	return 0;
}

/* Free all sound */
void wcsystem_free_sound()
{
	if(wcsys_snd_battle)
		delete wcsys_snd_battle;
	if(wcsys_snd_cursor)
		delete wcsys_snd_cursor;
	if(wcsys_snd_choice)
		delete wcsys_snd_choice;
	if(wcsys_snd_cancel)
		delete wcsys_snd_cancel;
	if(wcsys_snd_buzzer)
		delete wcsys_snd_buzzer;
	if(wcsys_snd_defeated)
		delete wcsys_snd_defeated;
}

/* Play system sound */
void wcsystem_play_sound(int id)
{
	if(id == WCSYSTEM_CURSOR)
	{
		if(wcsys_snd_cursor)
			wcsys_snd_cursor->play();
	}
	if(id == WCSYSTEM_DECIDE)
	{
		if(wcsys_snd_choice)
			wcsys_snd_choice->play();
	}
	if(id == WCSYSTEM_CANCEL)
	{
		if(wcsys_snd_cancel)
			wcsys_snd_cancel->play();
	}
	if(id == WCSYSTEM_BUZZER)
	{
		if(wcsys_snd_buzzer)
			wcsys_snd_buzzer->play();
	}
	if(id == WCSYSTEM_BATTLE)
	{
		if(wcsys_snd_battle)
			wcsys_snd_battle->play();
	}
	if(id == WCSYSTEM_DEFEATED)
	{
		if(wcsys_snd_defeated)
			wcsys_snd_defeated->play();
	}
}