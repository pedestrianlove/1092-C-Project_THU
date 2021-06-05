#include <iostream>
#include <ncurses.h>

using namespace std;

class Sudoku {
	public:
	int wrong;
	bool warn = false;
	bool changed = false;
	int entries[9][9] = {{0, 3, 0, 0, 7, 0, 0, 8, 5},
	  		     {5, 9, 8, 0, 6, 2, 7, 0, 4},
			     {7, 0, 0, 0, 0, 0, 0, 0, 0},
			     {4, 2, 0, 9, 0, 0, 0, 6, 8},
			     {6, 0, 0, 2, 5, 4, 0, 9, 0},
			     {0, 0, 5, 0, 8, 0, 3, 0, 2},
			     {8, 7, 0, 0, 4, 0, 0, 2, 6},
			     {0, 0, 9, 7, 0, 0, 0, 0, 0},
			     {2, 0, 0, 8, 0, 0, 4, 7, 3}};
	Sudoku () {
		render ();
	}
	
	void render () {
		// clear screen
		initscr ();
		clear ();
		
		// setup color
		start_color();
		init_pair(1,3,6);                    //initializing the color pairs
		init_pair(2,7,5);
		init_pair(3,2,3);

		// layout
		printw ("\n+---+---+---+---+---+---+---+---+---+\n");
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (entries[i][j] != 0) {
					if (entries[i][j] < 10)
						printw ("| %d ", entries[i][j]);
					else{
						printw ("|");
						attron(COLOR_PAIR(1));
						printw (" %d ", entries[i][j] - 10);
						attroff(COLOR_PAIR(1));
					}
				}
				else {
					printw ("|   ");
					if (! cursor_set) {
						set_cursor (i, j);
						cursor_set = true;
					}
				}

				if (j == 8)
					printw ("|\n");
			}
			printw ("+---+---+---+---+---+---+---+---+---+\n");
		}
		printw ("Q: Quit\t\t Arrow Keys: Move\nS: Save game\t L: Load game\n");
		if (warn) {
			attron(COLOR_PAIR(1));
			printw ("'%d' can't be filled in here.\n", wrong);
			attroff(COLOR_PAIR(1));
			warn = false;
		}
		move_cursor ();
		refresh ();
	}

	void play () {
		bool isHalt = false;
		while (! isHalt) {
			bool found = false;
			int tmp_sol = entries[coord_y][coord_x];
			int tmp_y = coord_y, tmp_x = coord_x;
			int control = getch ();
			switch (control) {
				case 'q':
					isHalt = true;
					break;

				case 'd':
					while (! found) {
						if (++coord_x == 9) {
							coord_x = tmp_x;
							found = true;
						}
						else if (check_range (coord_y, coord_x)) {
							set_cursor (coord_y, coord_x);
							found = true;
						}
					}
					break;

				case 'w':
					while (! found) {
						if (--coord_y == -1) {
							coord_y = tmp_y;
							found = true;
						}
						else if (check_range (coord_y, coord_x)) {
							set_cursor (coord_y, coord_x);
							found = true;
						}
					}
					break;

				case 'a':
					while (! found) {
						if (--coord_x == -1) {
							coord_x = tmp_x;
							found = true;
						}
						else if (check_range (coord_y, coord_x)) {
							set_cursor (coord_y, coord_x);
							found = true;
						}
					}
					break;

				case 's':
					while (! found) {
						if (++coord_y == 9) {
							coord_x = tmp_x;
							found = true;
						}
						else if (check_range (coord_y, coord_x)) {
							set_cursor (coord_y, coord_x);
							found = true;
						}
					}
					break;

				default:
					if (control <= '9' && control >= '1') {
						entries[coord_y][coord_x] = 10 + control - '0';
						if (! is_valid ()) {
							warn = true;
							wrong = entries[coord_y][coord_x]; 
							entries[coord_y][coord_x] = tmp_sol;
						}
					}
					if (control == '0')
						entries[coord_y][coord_x] = 0;
					break;
			}
			render ();
		}
	}


	private:
	bool cursor_set = false;
	int coord_x, coord_y;
	int cursor_x, cursor_y;
	void set_cursor (int i, int j){ 
		coord_y = i, coord_x = j;
		cursor_x = 2 + j * 4;
		cursor_y = 2 + i * 2;
	}
	void move_cursor () {
		move (cursor_y, cursor_x);
	}
	bool is_valid () {
		return true;
	}
	bool check_range (int i, int j) {
		if (entries[i][j] == 0 || entries[i][j] > 10)
			return true;
		else
			return false;
	}
};

int main ()
{
	Sudoku* game = new Sudoku ();

	game -> play ();


	clear ();
	move (80, 0);
	refresh ();
}

