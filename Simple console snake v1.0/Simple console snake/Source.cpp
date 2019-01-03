// v1.0
// it works!
// better put it in class so it won't be that much arguments as it is now

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <utility>
#include <Windows.h>
#include <conio.h>
#include <ctime>

// Controls
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27

using namespace std;

// Declarations


// Enum - direction that snake faces
enum direction {
	UP, DOWN, LEFT, RIGHT
};

// Constants
const unsigned int sleepTime = 250; // time  between iterations of the cycle
// Building chars
// To look for another symbol try to use alt+X where x is a number (for example alt+219=█) and define as "(char)"+X+"u"
const char snakeBody = 254u; // snake body (default ■ (alt+254))
const char snakeFood = 'O'; // food that snake should eat to grow
const char fieldWallHorizontalUpper = 220u; // horizontal walls (default ▄ (alt+220))
const char fieldWallHorizontalBottom = 223u; // horizontal walls (default ▀ (alt+223))
const char fieldWallVertical = 219u; // vertical walls (default █ (alt+219))
const unsigned int fieldSizeHorizontal = 5; // Field size (horizontal)
const unsigned int fieldSizeVertical = 6; // Field size (vertical), better - even number, because otherwise snake most likely will be caught into the dead end
const unsigned int fieldIndentVertical = 3; // Indent between the upper edge of the window and the gaming field
const unsigned int fieldIndentHorizontalCount = 6; // Indent between the left edge of the window and the gaming field
// Definition of fieldIndentHorizontal in string format (so the program won't calculate it every time it draws it)
const string fieldIndentHorizontal(fieldIndentHorizontalCount, ' ');

// Functions
bool isSnakeHeadingTail(pair <int, int> snakeHead, pair <int, int> snakeTail, direction snakeDirection);
void renderGameField(vector < vector <char> > &field);
void clearScreen();
void putSnakeInTheField(vector < vector <char> > &field, vector < pair<int, int> > &snake);
void getInput(direction &snakeDirection, bool &isRunning);
bool checkFood(direction snakeDirection, vector < pair<int, int> > &snake, vector < vector <char> > &field, bool &isFoodInField);
bool moveSnake(direction snakeDirection, vector < pair<int, int> > &snake, vector < vector <char> > &field, bool isFoodEaten);
void putFoodInField(vector < vector <char> > &field);
bool updateField(vector < vector <char> > &field, vector < pair<int, int> > &snake, direction snakeDirection, bool &isFoodInField);
void game();
void startScreen();
void deathScreen(int score);
void winScreen(int score);
void endScreen();
void clearField(vector < vector <char> > &field);


bool isSnakeHeadingTail(pair <int, int> snakeHead, pair <int, int> snakeTail, direction snakeDirection) {
	switch (snakeDirection) {
	case UP:
		if (snakeHead.first - 1 == snakeTail.first && snakeHead.second == snakeTail.second) {
			return true;
		}
		else {
			return false;
		}
	case DOWN:
		if (snakeHead.first + 1 == snakeTail.first && snakeHead.second == snakeTail.second) {
			return true;
		}
		else {
			return false;
		}
	case LEFT:
		if (snakeHead.first == snakeTail.first && snakeHead.second - 1 == snakeTail.second) {
			return true;
		}
		else {
			return false;
		}
	case RIGHT:
		if (snakeHead.first == snakeTail.first && snakeHead.second + 1 == snakeTail.second) {
			return true;
		}
		else {
			return false;
		}
	}
}

/*
**
NEEDS IMPROVEMENT
**
Renders game field

Parameters:
field - array of characters (field)
*/
void renderGameField(vector < vector <char> > &field) {
	int i, j; // iterators
	// Vertical indent
	for (i = 0; i <= fieldIndentVertical; ++i) {
		cout << endl;
	}
	// Upper wall
	cout << fieldIndentHorizontal;
	for (i = 0; i < fieldSizeHorizontal+2; ++i) { // fieldSizeHorizontal+2 because there are 2 vertical walls
		cout << fieldWallHorizontalUpper;
	}
	cout << endl;
	// Vertical walls
	for (i = 0; i < fieldSizeVertical; ++i) {
		cout << fieldIndentHorizontal; // Indent
		cout << fieldWallVertical; // Wall
		for (j = 0; j < fieldSizeHorizontal; ++j) {
			cout << field[i][j];
		}
		cout << fieldWallVertical;
		cout << endl;
	}
	// Bottom wall
	cout << fieldIndentHorizontal;
	for (i = 0; i < fieldSizeHorizontal + 2; ++i) { // fieldSizeHorizontal+2 because there are 2 vertical walls
		cout << fieldWallHorizontalBottom;
	}
	cout << endl;
}

void clearScreen() {
	system("cls");
}

void putSnakeInTheField(vector < vector <char> > &field, vector < pair<int, int> > &snake) {
	for (auto now : snake) {
		field[now.first][now.second] = snakeBody;
 	}
}


// Gets snake direction according to user input
void getInput(direction &snakeDirection, bool &isRunning) {
	int key;
	if (_kbhit()) {
		key = _getch();
		/*
		because if it's arrow (extended ASCII), you need to read the key code twice:
		1st would be 0 or 224 (it tells us that pressed key is from extended ASCII table),
		2nd would be the key code itself
		*/
		if (key == 0 || key == 224) { // because if it's arrow (extended ASCII), you need to read the key code twice: 1st would be 0 or 224, 2nd would be the key code itself
			key = _getch();
		}
		switch (key) {
		case KEY_UP:
			if (snakeDirection != DOWN) {
				snakeDirection = UP;
			}
			break;
		case KEY_LEFT:
			if (snakeDirection != RIGHT) {
				snakeDirection = LEFT;
			}
			break;
		case KEY_DOWN:
			if (snakeDirection != UP) {
				snakeDirection = DOWN;
			}
			break;
		case KEY_RIGHT:
			if (snakeDirection != LEFT) {
				snakeDirection = RIGHT;
			}
			break;
		case KEY_ESC:
			// closes the game
			isRunning = false;
			break;
		}
	}
}

// If there's food in front of the snake, returns true, else false;
bool checkFood(direction snakeDirection, vector < pair<int, int> > &snake, vector < vector <char> > &field, bool &isFoodInField) {
	switch (snakeDirection) {
	case UP:
		if (snake[0].first - 1 >= 0) {
			if (field[snake[0].first - 1][snake[0].second] == snakeFood) {
				isFoodInField = false;
				return true;
			}
		}
		break;
	case DOWN:
		if (snake[0].first + 1 < fieldSizeVertical) {
			if (field[snake[0].first + 1][snake[0].second] == snakeFood) {
				isFoodInField = false;
				return true;
			}
		}
		break;
	case RIGHT:
		if (snake[0].second + 1 < fieldSizeHorizontal) {
			if (field[snake[0].first][snake[0].second + 1] == snakeFood) {
				isFoodInField = false;
				return true;
			}
		}
		break;
	case LEFT:
		if (snake[0].second - 1 >= 0) {
			if (field[snake[0].first][snake[0].second - 1] == snakeFood) {
				isFoodInField = false;
				return true;
			}
		}
		break;
	}
	return false;
}

// if succesful, return true, otherwise false (death), also lengthens snake if it have eaten food
bool moveSnake(direction snakeDirection, vector < pair<int, int> > &snake, vector < vector <char> > &field, bool isFoodEaten) {
	// move itself
	int i;
	if (isFoodEaten) {
		snake.push_back(pair<int, int>(snake[snake.size() - 1]));
	}
	switch (snakeDirection) {
	case UP:
		// checking if (the next cell in the field) && (not a snake body), but if it's the end of the snake, it's ok, because the cell will become free next step
		if (snake[0].first - 1 >= 0 && (field[snake[0].first - 1][snake[0].second] != snakeBody || isSnakeHeadingTail(snake[0], snake[snake.size() - 1], snakeDirection))) {
			if (snake.size() > 1) {
				// moving whole snake except it's head forward
				for (i = snake.size() - 1; i > 0; --i) {
					snake[i].first = snake[i - 1].first;
					snake[i].second = snake[i - 1].second;
				}
			}
			// moving head (or snake with length 1)
			snake[0].first--;
			return true;
		}
		else {
			return false;
		}
		break;
	case DOWN:
		if (snake[0].first + 1 < fieldSizeVertical && (field[snake[0].first + 1][snake[0].second] != snakeBody || isSnakeHeadingTail(snake[0], snake[snake.size() - 1], snakeDirection))) {
			if (snake.size() > 1) {
				// moving whole snake except its head forward
				for (i = snake.size() - 1; i > 0; --i) {
					snake[i].first = snake[i - 1].first;
					snake[i].second = snake[i - 1].second;
				}
			}
			// moving head (or snake with length 1)
			snake[0].first++;
			return true;
		}
		else {
			return false;
		}
		break;
	case RIGHT:
		if (snake[0].second + 1 < fieldSizeHorizontal && (field[snake[0].first][snake[0].second + 1] != snakeBody || isSnakeHeadingTail(snake[0], snake[snake.size() - 1], snakeDirection))) {
			if (snake.size() > 1) {
				// moving whole snake except it's head forward
				for (i = snake.size() - 1; i > 0; --i) {
					snake[i].first = snake[i - 1].first;
					snake[i].second = snake[i - 1].second;
				}
			}
			// moving head (or snake with length 1)
			snake[0].second++;
			return true;
		}
		else {
			return false;
		}
		break;
	case LEFT:
		if (snake[0].second - 1 >= 0 && (field[snake[0].first][snake[0].second - 1] != snakeBody || isSnakeHeadingTail(snake[0], snake[snake.size() - 1], snakeDirection))) {
			if (snake.size() > 1) {
				// moving whole snake except it's head forward
				for (i = snake.size() - 1; i > 0; --i) {
					snake[i].first = snake[i - 1].first;
					snake[i].second = snake[i - 1].second;
				}
			}
			// moving head (or snake with length 1)
			snake[0].second--;
			return true;
		}
		else {
			return false;
		}
		break;

	}
}

// puts food in the free cell of the field
void putFoodInField(vector < vector <char> > &field) {
	srand(time(NULL));
	bool isEmptyCell = false;
	int x_rand, y_rand;
	while (!isEmptyCell) {
		x_rand = rand() % fieldSizeHorizontal;
		y_rand = rand() % fieldSizeVertical;
		if (field[y_rand][x_rand] == ' ') {
			isEmptyCell = true;
			field[y_rand][x_rand] = snakeFood;
		}
	}
}

// makes a "step" in game
bool updateField(vector < vector <char> > &field, vector < pair<int, int> > &snake, direction snakeDirection, bool &isFoodInField) {
	if (!isFoodInField) {
		putFoodInField(field);
		isFoodInField = true;
	}
	bool isFoodEaten = checkFood(snakeDirection, snake, field, isFoodInField);
	if (moveSnake(snakeDirection, snake, field, isFoodEaten)) { // if move was succesful
		clearField(field); 
		putSnakeInTheField(field, snake); // rewrite snake into the array
		return true;
	}
	else {
		return false;
	}
}

// the game itself
void game() {
	// Welcome screen
	startScreen();


	/*
	 * Declarations and definitions
	 */

	// Field array definition
	vector < vector <char> > field(fieldSizeVertical, vector <char>(fieldSizeHorizontal, ' '));

	/* Snake array definition
	pair - coords of snake's part of body
	first - y, second x
	for example field[y][x] - place in field array of the part

	first pair - head of the snake
	last pair - end of the snake's tail
	*/
	vector < pair<int, int> > snake;
	// Snake centred at the beginning
	snake.push_back(pair<int, int>((int)fieldSizeVertical / 2, (int)fieldSizeHorizontal / 2));

	// if it's true - game will begin again after death
	bool isRunning = true;
	// if it's true - there is food in the gaming field, otherwise program will put it into this
	bool isFoodInField = false;

	// Direction that snake faces, default - RIGHT
	direction snakeDirection = RIGHT;


	// render for the first time so the pleyer can see the beginning;
	renderGameField(field);

	while (isRunning) {
		Sleep(sleepTime);
		getInput(snakeDirection, isRunning);
		bool isSuccessfulUpd = updateField(field, snake, snakeDirection, isFoodInField);
		// if snakes fills whole field, win
		if (snake.size() >= fieldSizeHorizontal * fieldSizeVertical) {
			winScreen(snake.size());
			return;
		}
		if (!isSuccessfulUpd) {
			deathScreen(snake.size());
			return;
		}
		renderGameField(field);
	}
}

// screen showed at the beginning
void startScreen() {
	// fill later
}

// screen showed after death (score - snake size)
void deathScreen(int score) {
	cout << "YOU LOST | SCORE : " << score << "." << endl;
	// fill later
}

// YOU WON screen
void winScreen(int score) {
	cout << "YOU WON | SCORE : " << score << "." << endl;
	// fill later
}

// end screen lol
void endScreen() {
	// fill later
}


// clears field
void clearField(vector < vector <char> > &field) {
	system("cls");
	int i, j;
	for (i = 0; i < fieldSizeVertical; ++i) {
		for (j = 0; j < fieldSizeHorizontal; ++j) {
			if (field[i][j] != snakeFood) {
				field[i][j] = ' ';
			}
		}
	}
}


// Main function
int main() {
	game();
	return 0;
}