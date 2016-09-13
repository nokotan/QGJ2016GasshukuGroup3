#pragma once

enum STATE {
	EXIT = -1,
	TITLE = 0,
	GAME,
	RESULT,
	GAMEOVER,
};

STATE title();
STATE game();
STATE result();
STATE gameover();