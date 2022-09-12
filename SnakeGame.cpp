#include <iostream>
#include <conio.h>
#include <time.h>
#include <fstream>
#include <Windows.h>

using namespace std;

struct position {
	int ri;
	int ci;
};

struct snake {
	char sym;
	char hsym;
	int direction;
	int score;
	int size;
	position* Ps;
	int LKey, RKey, UKey, DKey;

};

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}


void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}


void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void SnakeInnit(snake& S, int Row, int Col, position& fpos, char& fsym) {
	S.score = 0;
	S.direction = UP;
	S.size = 3;
	S.sym = -37;
	S.hsym = 1;
	S.Ps = new position[S.size + 1];
	for (int i = 0; i < S.size; i++) {
		S.Ps[i].ri = (Row) / 2;
		S.Ps[i].ci = (Col / 4) - i;
	}
	S.UKey = 'w', S.DKey = 's', S.RKey = 'd', S.LKey = 'a';
}

void SnakeInnit2(snake& S, int Row, int Col, position& fpos, char& fsym) {
	S.score = 0;
	S.direction = UP;
	S.size = 3;
	S.sym = -37;
	S.hsym = 4;
	S.Ps = new position[S.size + 1];
	for (int i = 0; i < S.size; i++) {
		S.Ps[i].ri = Row / 2;
		S.Ps[i].ci = ((3*Col) / 4) - i;
	}
	S.UKey = 'i', S.DKey = 'k', S.RKey = 'l', S.LKey = 'j';
}

void Innit(snake *&S, int Row, int Col, position& fpos, char& fsym, int &hs) {
	SnakeInnit(S[0], Row, Col, fpos, fsym);
	SnakeInnit2(S[1], Row, Col, fpos, fsym);
	fpos.ri = 10;
	fpos.ci = 10;
	fsym = '*';
	ifstream Rdr("score.txt");
	Rdr >> hs;
}

void EraseSnake(snake *&S, int n) {
	for (int x = 0; x < n; x++) {
		gotoRowCol(S[x].Ps[0].ri, S[x].Ps[0].ci);
		for (int i = 0; i < S[x].size; i++) {
			gotoRowCol(S[x].Ps[i].ri, S[x].Ps[i].ci);
			cout << " ";
		}
	}
}

void DisplaySnake(snake *&S, int n) {
	for (int x = 0; x < n; x++) {
		gotoRowCol(S[x].Ps[0].ri, S[x].Ps[0].ci);
		cout << S[x].hsym;
		for (int i = 1; i < S[x].size; i++) {
			gotoRowCol(S[x].Ps[i].ri, S[x].Ps[i].ci);
			cout << S[x].sym;
		}
	}
}

void FoodDisplay(position fpos, char fsym) {
	gotoRowCol(fpos.ri, fpos.ci);
	cout << fsym;
}

bool FoodCapture(snake *&S, position fpos, int &which, int n) {
	if (n > 1) {
		if (S[0].Ps[0].ri == fpos.ri && S[0].Ps[0].ci == fpos.ci) {
			which = 1;
			return true;
		}
		if (S[1].Ps[0].ri == fpos.ri && S[1].Ps[0].ci == fpos.ci) {
			which = 2;
			return true;
		}
	}
	else {
		if (S[0].Ps[0].ri == fpos.ri && S[0].Ps[0].ci == fpos.ci) {
			which = 1;
			return true;
		}
	}
	return false;
}

void SnakeExpand(snake& S) {
	position* HA = new position[S.size + 1];
	for (int i = 0; i < S.size; i++)
		HA[i + 1] = S.Ps[i];
	HA[0] = HA[1];
	delete[] S.Ps;
	S.Ps = HA;
	S.size++;
}
void ChangeDirection(char ch, snake *&S, int n) {
	for (int i = 0; i < n; i++) {
		if (ch == S[i].LKey && S[i].direction != RIGHT)
			S[i].direction = LEFT;
		if (ch == S[i].RKey && S[i].direction != LEFT)
			S[i].direction = RIGHT;
		if (ch == S[i].UKey && S[i].direction != DOWN)
			S[i].direction = UP;
		if (ch == S[i].DKey && S[i].direction != UP)
			S[i].direction = DOWN;
	}
}

void MoveSnake(snake *&S, int Rows, int Cols) {
	for (int x = 0; x < 2; x++) {
		for (int i = S[x].size - 1; i - 1 >= 0; i--) {
			S[x].Ps[i] = S[x].Ps[i - 1];
		}
		switch (S[x].direction) {
		case UP:
			S[x].Ps[0].ri--;
			/*if (S.Ps[0].ri == 1)
				S.Ps[0].ri = Rows - 1;*/
			break;
		case DOWN:
			S[x].Ps[0].ri++;
			/*if (S.Ps[0].ri == Rows-1)
				S.Ps[0].ri = 1;*/
			break;
		case RIGHT:
			S[x].Ps[0].ci++;
			/*if (S.Ps[0].ci == Cols-1)
				S.Ps[0].ci = 1;*/
			break;
		case LEFT:
			S[x].Ps[0].ci--;
			/*if (S.Ps[0].ci == 1)
				S.Ps[0].ci = Cols - 1;*/
			break;
		}
	}
}

bool existsOnSnake(snake *&S, int ri, int ci) {
	for (int x = 0; x < 2; x++) {
		for (int i = 0; i < S[x].size; i++) {
			if (ri == S[x].Ps[i].ri && ci == S[x].Ps[i].ci)
				return true;
		}
	}
	return false;
}

void RegenerateFood(position& fpos, snake *&S, int Rows, int Cols) {
	int ri = 0, ci = 0;
	do {
		srand(time(0));
		ri = rand() % Rows;
		ci = rand() % Cols;
	} while (existsOnSnake(S, ri, ci));
	fpos.ri = ri;
	fpos.ci = ci;

}

void displayScore(snake*& S, int n) {
	int r = 12, c = 110;
	for (int i = 0; i < n; i++) {
		gotoRowCol(r, c);
		cout << "SNAKE#" << i + 1 << " SCORE: " << S[i].score;
		r += 2; c += 2;
	}
}


void displayTime(int tE) {
	gotoRowCol(16, 110);
	cout << "TIME: " << time(NULL) - tE;
}

void displayHighScore(int &hs) {
	gotoRowCol(18, 110);
	cout << "HIGH SCORE: " << hs;

}

void newHighScore(int score) {
	ofstream Rdr("score.txt");
	Rdr << score;
	Rdr.close();
}

bool snakeKill(snake *&S, int Rows, int Cols, int n) {
	for (int x = 0; x < n; x++) {
		for (int i = 2; i < S[x].size; i++) {
			if (S[x].Ps[0].ri == S[x].Ps[i].ri && S[x].Ps[0].ci == S[x].Ps[i].ci)
				return true;
		}
		if (S[x].Ps[0].ri == 0)
			return true;
		if (S[x].Ps[0].ri == Rows)
			return true;
		if (S[x].Ps[0].ci == 0)
			return true;
		if (S[x].Ps[0].ci == Cols)
			return true;
	}
	for (int i = 2; i < S[0].size; i++) {
		if (S[0].Ps[0].ri == S[1].Ps[i].ri && S[0].Ps[0].ci == S[1].Ps[i].ci)
			return true;
	}
	for (int i = 2; i < S[1].size; i++) {
		if (S[1].Ps[0].ri == S[0].Ps[i].ri && S[1].Ps[0].ci == S[0].Ps[i].ci)
			return true;
	}

	return false;
}

void drawBoundariesL1(int Rows, int Cols) {
	for (int ir1 = 0; ir1 <= Cols; ir1++) {
		gotoRowCol(0, ir1);
		cout << (char)-37;
	}
	for (int ir2 = 0; ir2 <= Rows; ir2++) {
		gotoRowCol(ir2, Cols);
		cout << (char)-37;
	}
	for (int ic1 = 0; ic1 <= Rows; ic1++) {
		gotoRowCol(ic1, 0);
		cout << (char)-37;
	}
	for (int ic2 = 0; ic2 <= Cols; ic2++) {
		gotoRowCol(Rows, ic2);
		cout << (char)-37;
	}
}
void printBanner() {
	cout << endl << endl << endl << endl << endl;
	cout << "\t\#####                                   #####                       \n";
	cout << "\t#      #    #    #    #   #  #######   # # ##    #    #    # #####\n";
	cout << "\t#      ##   #   #  #  #   #  #         #        #  #  ##  ## #    \n";
	cout << "\t#####  # #  #  #    # ####   #####     #  #### #    # # ## # ##### \n";
	cout << "\t    #  #  # #  ###### #  #   #         #     # ###### #    # #      \n";
	cout << "\t    #  #    #  #    # #   #  #         #     # #    # #    # #       \n";
	cout << "\t#####  #    #  #    # #    # ######     #####  #    # #    # ######\n";
}

void printStart(int br, int bc) {
	int rdim, cdim;
	cdim = 15; rdim = 5;
	char sym = -37;
	for (int r = 0; r < rdim; r++)
	{
		for (int c = 0; c < cdim; c++)
		{
			gotoRowCol(br + r, bc + c);
			cout << sym;
		}
	}
}

void printGameOver() {
	system("cls");
	cout << endl << endl << endl << endl << endl << endl << endl;
	cout << "\t\t\t\t#####                           #######                      \n";
	cout << "\t\t\t\t#     #   ##   #    # ######    #     # #    # ###### #####  \n";
	cout << "\t\t\t\t#        #  #  ##  ## #         #     # #    # #      #    #\n";
	cout << "\t\t\t\t#  #### #    # # ## # #####     #     # #    # #####  #    # \n";
	cout << "\t\t\t\t#     # ###### #    # #         #     # #    # #      #####  \n";
	cout << "\t\t\t\t#     # #    # #    # #         #     #  #  #  #      #   #  \n";
	cout << "\t\t\t\t #####  #    # #    # ######    #######   ##   ###### #    # \n";
}

void winOne() {
	system("cls");
	cout << endl << endl;
cout << "#######                  #     #                 \n";
cout << "#     # #    # ######    #  #  # # #    #  ####  \n";
cout << "#     # ##   # #         #  #  # # ##   # #      \n";
cout << "#     # # #  # #####     #  #  # # # #  #  ####  \n";
cout << "#     # #  # # #         #  #  # # #  # #      # \n";
cout << "#     # #   ## #         #  #  # # #   ## #    # \n";
cout << "####### #    # ######     ## ##  # #    #  ####  \n";


}

void winTwo() {
	system("cls");
	cout << endl << endl;
cout << "#######                  #     #                 \n";
cout << "   #    #    #  ####     #  #  # # #    #  ####  \n";
cout << "   #    #    # #    #    #  #  # # ##   # #      \n";
cout << "   #    #    # #    #    #  #  # # # #  #  ####  \n";
cout << "   #    # ## # #    #    #  #  # # #  # #      # \n";
cout << "   #    ##  ## #    #    #  #  # # #   ## #    # \n";
cout << "   #    #    #  ####      ## ##  # #    #  ####  \n";
}

void getChoice(int& choice) {
	system("cls");
	cout << "\n\n\n\n\t\t\t\t Single Player/2-Player?(1/2): ";
	cin >> choice;
}

int level1(int snums) {
	system("cls");
	snake *S = new snake[snums];
	position fpos;
	int hs = 0;
	int which = 0;
	char fsym;
	char ch;
	int Rows = 70, Cols = 100;
	ifstream Rdr("score.txt");
	Rdr >> hs;
	Rdr.close();
	drawBoundariesL1(Rows, Cols);
	Innit(S, Rows, Cols, fpos, fsym, hs);
	FoodDisplay(fpos, fsym);

	int tS = time(NULL);
	while (true) {
		displayScore(S, snums);
		displayTime(tS);
		displayHighScore(hs);
		DisplaySnake(S, snums);
		if (_kbhit()) {
			ch = _getch();
			ChangeDirection(ch, S, snums);
		}
		EraseSnake(S, snums);
		MoveSnake(S, Rows, Cols);
		DisplaySnake(S, snums);
		Sleep(100);
		if (FoodCapture(S, fpos, which, snums)) {
			S[which-1].score++;
			SnakeExpand(S[which-1]);
			RegenerateFood(fpos, S, Rows, Cols);
			FoodDisplay(fpos, fsym);
			which = 0;
		}
		if (snakeKill(S, Rows, Cols, snums)) {
			for (int i = 0; i < 2; i++) {
				if (S[i].score > hs)
					newHighScore(hs);
			}
			if (snums > 1) {
				if (S[0].score > S[1].score)
					return 1;
				else if (S[1].score > S[0].score)
					return 2;
			}
			return -1;
		}
	}


	return _getch();
}

int main() {
	int choice = 0;
	getChoice(choice);
	printBanner();
	printStart(18, 35);
	cout << endl << endl << endl;
	gotoRowCol(20, 40);
	cout << "START!";
	int r = 0, c = 0, x = 0;
	while (true) {
		getRowColbyLeftClick(r, c);
		if ((r >= 18 && c >= 35) && (r < 23 && c < 50))
			x = level1(choice);
		if (x == -1) {
			printGameOver();
			break;
		}
		if (x == 1) {
			winOne();
			break;
		}
		if (x == 2) {
			winTwo();
			break;
		}
	}
	_getch();
}