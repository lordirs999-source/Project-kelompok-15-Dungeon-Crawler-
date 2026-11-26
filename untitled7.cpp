#include <ncurses.h>
#include <string>
using namespace std;

// ---------- DATA MAP (array string, '#' = dinding, '.' = jalan) ----------
const int TINGGI = 13;
const int LEBAR  = 31;

string peta[TINGGI] = {
    "###############################",
    "#...............#.....#.#.....#",
    "###...#########.###.#.#.#.#.#.#",
    "#...#.#.......#.....#.#...#.#.#",
    "#.###.#.###.#.#######.#####.###",
    "#.#...#.#.#.#.......#.....#...#",
    "#.#.#.#.#.#.#...###.#####.#.#.#",
    "#.#.#.#...#.#.......#.....#.#.#",
    "#.#.#.#.###.#########.#####.#.#",
    "#.#.#.#.#...#.........#.....#.#",
    "#.###.#.#.###.##############..#",
    "#.....#.#.....................#",
    "###############################"
};

// ---------- POSISI KARAKTER ----------
int posX = 1;   // kolom
int posY = 1;   // baris

// ---------- FUNCTION: menggambar map ke layar ----------
void gambarPeta() {
    for (int y = 0; y < TINGGI; y++) {
        for (int x = 0; x < LEBAR; x++) {
            if (peta[y][x] == '#') {
                attron(COLOR_PAIR(1));
                mvaddch(y + 2, x + 2, '#');
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvaddch(y + 2, x + 2, '.');
                attroff(COLOR_PAIR(2));
            }
        }
    }
}

// ---------- FUNCTION: menggambar karakter ----------
void gambarPemain() {
    attron(COLOR_PAIR(3) | A_BOLD);
    mvaddch(posY + 2, posX + 2, '@');
    attroff(COLOR_PAIR(3) | A_BOLD);
}

// ---------- FUNCTION: cek apakah posisi masih di dalam area map ----------
// (Tabrakan dengan dinding dinonaktifkan untuk laporan awal,
//  pemain hanya dibatasi agar tidak keluar dari area map)
bool dalamBatas(int y, int x) {
    return (y >= 0 && y < TINGGI && x >= 0 && x < LEBAR);
}

// ---------- FUNCTION: gerakkan karakter ----------
void gerak(int dy, int dx) {
    int yBaru = posY + dy;
    int xBaru = posX + dx;
    if (dalamBatas(yBaru, xBaru)) {
        posY = yBaru;
        posX = xBaru;
    }
}

int main() {
    // --- inisialisasi ncurses ---
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);              // sembunyikan kursor

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE,  COLOR_BLACK);   // dinding
        init_pair(2, COLOR_GREEN,  COLOR_BLACK);   // jalan
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);   // pemain
    }

    int ch;
    bool jalan = true;

    while (jalan) {
        clear();
        mvprintw(0, 2, "=== DUNGEON CRAWLER (Laporan 1) ===");
        gambarPeta();
        gambarPemain();
        mvprintw(TINGGI + 3, 2, "Posisi  : x=%d  y=%d", posX, posY);
        mvprintw(TINGGI + 4, 2, "Kontrol : Panah / WASD untuk jalan, Q untuk keluar");
        refresh();

        ch = getch();

        switch (ch) {
            case KEY_UP:
            case 'w':
                gerak(-1, 0);
                break;
            case KEY_DOWN:
            case 's':
                gerak(1, 0);
                break;
            case KEY_LEFT:
            case 'a':
                gerak(0, -1);
                break;
            case KEY_RIGHT:
            case 'd':
                gerak(0, 1);
                break;
            case 'q':
            case 'Q':
                jalan = false;
                break;
        }
    }

    endwin();
    return 0;
}
