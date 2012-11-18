/* Set title */
void wcsystem_set_title_music(music *m)
{
	if(wcsys_song_title)
		delete wcsys_song_title;
	wcsys_song_title = m;
	wclog_write(wcrpg_get_log(),"Title BGM is now ");
	wclog_write(wcrpg_get_log(),m->get_file());
	wclog_enter(wcrpg_get_log());
}

/* Play title */
void wcsystem_play_title_music()
{
	wcsound_music_play(wcsys_song_title);
}

/* Set battle */
void wcsystem_set_battle_music(music *m)
{
	if(wcsys_song_battle)
		delete wcsys_song_battle;
	wcsys_song_battle = m;
	wclog_write(wcrpg_get_log(),"Battle BGM is now ");
	wclog_write(wcrpg_get_log(),m->get_file());
	wclog_enter(wcrpg_get_log());
}

/* Play battle */
void wcsystem_play_battle_music()
{
	wcsound_music_play(wcsys_song_battle);
}

/* Set victory */
void wcsystem_set_victory_music(music *m)
{
	if(wcsys_song_victory)
		delete wcsys_song_victory;
	wcsys_song_victory = m;
	wclog_write(wcrpg_get_log(),"Victory BGM is now ");
	wclog_write(wcrpg_get_log(),m->get_file());
	wclog_enter(wcrpg_get_log());
}

/* Play victory */
void wcsystem_play_victory_music()
{
	wcsound_music_play(wcsys_song_victory);
}

/* Set gameover */
void wcsystem_set_gameover_music(music *m)
{
	if(wcsys_song_gameover)
		delete wcsys_song_gameover;
	wcsys_song_gameover = m;
	wclog_write(wcrpg_get_log(),"Game Over BGM is now ");
	wclog_write(wcrpg_get_log(),m->get_file());
	wclog_enter(wcrpg_get_log());
}

/* Play gameover */
void wcsystem_play_gameover_music()
{
	wcsound_music_play(wcsys_song_gameover);
}