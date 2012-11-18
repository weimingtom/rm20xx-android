/* Command name finder */

/* The following is going to get ugly */

/* Returns true if this code's string is a resource reference */
int wccommand_has_resource(int code)
{
	if(code == 0x2D1E)
		return 1;
	if(code == 0x2792)
		return 1;
	if(code == 0x2B66)
		return 1;
	if(code == 0x2986)
		return 1;
	if(code == 0x2990)
		return 1;
	if(code == 0x2CF6)
		return 1;
	if(code == 0x2DC8)
		return 1;
	if(code == 0x29B8)
		return 1;
	return 0;
}

/* Returns true if the command ends the current branch */
int wccommand_is_end(int code)
{
	if(code == 0x4EAD)
		return 1;
	if(code == 0x55FB)
		return 1;
	return 0;
}

/* Get name from code */
char *wccommand_get_name(int code)
{
	if(code == 0x277E)
		return "Show Message";
	if(code == 0x4E8E)
		return "            ";
	if(code == 0x2C92)
		return "Wait";
	if(code == 0x2C42)
		return "Move";
	if(code == 0x2C4C)
		return "Proceed With Movement";
	if(code == 0x285A)
		return "Change Party";
	if(code == 0x27E2)
		return "Switch";
	if(code == 0x27EC)
		return "Variable";
	if(code == 0x279C)
		return "Show Choice";
	if(code == 0x4EAC)
		return "* choice";
	if(code == 0xA)
		return "* break";
	if(code == 0x4EAD)
		return "* end choice";
	if(code == 0x2EEA)
		return "* if";
	if(code == 0x55FA)
		return "* else";
	if(code == 0x55FB)
		return "* end if";
	if(code == 0x2FB2)
		return "* while";
	if(code == 0x2FBC)
		return "* break";
	if(code == 0x56C2)
		return "* end while";
	if(code == 0x2F4E)
		return "* label";
	if(code == 0x2F58)
		return "* goto";
	if(code == 0x2D1E)
		return "Play Sound";
	if(code == 0x2DE6)
		return "Swap Tile";
	if(code == 0x2792)
		return "Set Face";
	if(code == 0x2A3A)
		return "Teleport";
	if(code == 0x2788)
		return "Message Options";
	if(code == 0x2EB8)
		return "Enable Main Menu";
	if(code == 0x2E9A)
		return "Enable Saving";
	if(code == 0x2E86)
		return "Open Save Menu";
	if(code == 0x2EAE)
		return "Open Main Menu";
	if(code == 0x3020)
		return "Erase Event";
	if(code == 0x2B66)
		return "Show Picture";
	if(code == 0x2B7A)
		return "Erase Picture";
	if(code == 0x2B70)
		return "Move Picture";
	if(code == 0x2986)
		return "Change Sprite";
	if(code == 0x2990)
		return "Change Face";
	if(code == 0x302A)
		return "Call Event";
	if(code == 0x2850)
		return "Item";
	if(code == 0x2846)
		return "Money";
	if(code == 0x2B16)
		return "Tint Screen";
	if(code == 0x28B4)
		return "Level Up";
	if(code == 0x28BE)
		return "Change Stat";
	if(code == 0x28AA)
		return "Change EXP";
	if(code == 0x28DC)
		return "Change Health";
	if(code == 0x28E6)
		return "Change Mana";
	if(code == 0x2D5A)
		return "Key Input";
	if(code == 0x2BCA)
		return "Battle Animation";
	if(code == 0x2C38)
		return "Flash Event";
	if(code == 0x2B20)
		return "Flash Screen";
	if(code == 0x2CF6)
		return "Play Music";
	if(code == 0x2D00)
		return "Fade Music";
	if(code == 0x2D0A)
		return "Memorize Music";
	if(code == 0x2D14)
		return "Recall Music";
	if(code == 0x28D2)
		return "Change Equipment";
	if(code == 0x2B34)
		return "Pan Screen";
	if(code == 0x3016)
		return "Return";
	if(code == 0x28C8)
		return "Change Skills";
	if(code == 0x2972)
		return "Change Hero Name";
	if(code == 0x297C)
		return "Change Hero Title";
	if(code == 0x29D6)
		return "Start Combat";
	if(code == 0x2B02)
		return "Erase Screen";
	if(code == 0x2B0C)
		return "Show Screen";
	if(code == 0x2A9E)
		return "Get Terrain";
	if(code == 0x2AA8)
		return "Get Event";
	if(code == 0x2A6C)
		return "Place Event";
	if(code == 0x27F6)
		return "Timer";
	if(code == 0x3084)
		return "Game Over";
	if(code == 0x2DC8)
		return "Change BG";
	if(code == 0x29B8)
		return "Change System Graphic";
	if(code == 0x2A76)
		return "Swap Event Locations";
	/* Unknown code */
	return 0;
}

/* Is a comment? */
int wccommand_is_comment(int code)
{
	if(code == 0x307A)
		return 1;
	return 0;
}