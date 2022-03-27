#include <bangtal.h>
#include <time.h>
#include <stdlib.h>

SceneID scene;
ObjectID startButton, game_board[9], init_board[9];
const char* board_image[9] = {
	"Images/animation1.png",
	"Images/animation2.png",
	"Images/animation3.png",
	"Images/animation4.png",
	"Images/animation5.png",
	"Images/animation6.png",
	"Images/animation7.png",
	"Images/animation8.png",
	"Images/animation9.png",

};

const int board_x[9] = { 0, 0, 0, 426, 426, 426, 852, 852, 852};
const int board_y[9] = { 400, 200, 0, 400, 200, 0, 400, 200, 0 };

int blank;
TimerID timer;
int mixCount;


int board_index(ObjectID object) {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] == object) return i;

	}
	return -1;
}

void board_move(int index) {
	ObjectID t = game_board[blank];
	game_board[blank] = game_board[index];
	game_board[index] = t;

	
	locateObject(game_board[blank], scene, board_x[blank], board_y[blank]);
	locateObject(game_board[index], scene, board_x[index], board_y[index]);

	blank = index;
}


bool movable(int index) {
	if (blank < 0 or blank > 8) return false;
	if (blank % 3 != 0 and blank - 1 == index) return true;
	if (blank % 3 != 2 and blank + 1 == index) return true;
	if (blank / 3 != 0 and blank - 3 == index) return true;
	if (blank / 3 != 2 and blank + 3 == index) return true;

	return false;
}

void board_mix() {

	int index;

	do {
		switch (rand() % 3) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank + 3; break;
		case 3: index = blank + 3; break;
		}
	} while (!movable(index));
	board_move(index);
}

bool completed() {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] != init_board[i]) return false;
	}
	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == startButton) {
		hideObject(startButton);
		mixCount = 10;
		setTimer(timer, 1.f);
		startTimer(timer);
	}
	else {
		int index = board_index(object);
		if (movable(index)) {
			board_move(index);

			if (completed()) {
				showMessage("Completed!!");
				
			}
		}
	}
}



void timerCallback(TimerID timer) {
	mixCount--;

	if (mixCount > 0) {


		board_mix();

		setTimer(timer, 1.f);
		startTimer(timer);
	}
}


int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);



	scene = createScene("포뇨 맞추기", "Images/white.png");

	for (int i = 0; i< 9; i++) {
		game_board[i] = createObject(board_image[i]);
		init_board[i] = game_board[i];
		locateObject(game_board[i], scene, board_x[i], board_y[i]);
		showObject(game_board[i]);

	}

	blank = 8;

	hideObject(game_board[blank]);


	startButton = createObject("Images/start.png");
	locateObject(startButton, scene, 640, 30);
	showObject(startButton);

	
	

	timer = createTimer(1.f);
	

	startGame(scene);


	return 0;
}