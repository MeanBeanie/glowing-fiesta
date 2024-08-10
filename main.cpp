#include <algorithm>
#include <cmath>
#include <iostream>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <utility>
#include <vector>

int main(){
	setlocale(LC_ALL, "");

	initscr();
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	if(w.ws_col-1 < 120 || w.ws_row-1 < 40){
		endwin();
		std::cerr << "Console Size Error: expected 120x40 or greater, instead got " << w.ws_col-1 << "x" << w.ws_row-1 << std::endl;
		return -1;
	}

  const char *brightnessChars = R"($@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.)";

  int scrWidth = 120;
  int scrHeight = 40;

	float fov = 3.14159f / 4.0f;
	const float depth = 32.0f;

	float angle = 0.0f;
	float playerX = 2.0f;
	float playerY = 2.0f;

	const int mapWidth = 32;
	const int mapHeight = 32;

	char map[mapHeight][mapWidth] = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', '-', '-', '-', '-', ' ', ' ', ' ', '-', '-', '-', '-', '-', '-', '-', '-', '-', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	};

	bool quit = false;

	while(!quit){
		erase();

		for(int i = 0; i < 32; i++){
			for(int j = 0; j < 64; j += 2){
				mvaddch(i, j+125, map[i][j/2]);
				if((int)playerX == j/2 && (int)playerY == i){
					mvaddch(i, j+125, 'P');
				}
			}
		}

		for(int x = 0; x < scrWidth; x++){
			float rayAngle = (angle - fov / 2.0f) + ((float)x / (float)scrWidth) * fov;

			float distToWall = 0.0f;
			bool hitWall = false;
			int wallType = -1;
			bool hitEdge = false;
			
			float eyeX = sinf(rayAngle);
			float eyeY = cosf(rayAngle);

			while(!hitWall && distToWall < depth){
				distToWall += 0.1f;

				int testX = (int)(playerX + eyeX * distToWall);
				int testY = (int)(playerY + eyeY * distToWall);

				if(testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight){
					hitWall = true;
					distToWall = depth;
				}
				else{
					if(map[testY][testX] == '#' || map[testY][testX] == '-'){
						wallType = map[testY][testX] == '#' ? 1 : 4;
						hitWall = true;	

						std::vector<std::pair<float, float>> p;

						for(int tx = 0; tx < 2; tx++){
							for(int ty = 0; ty < 2; ty++){
								float vy = (float)testY + ty - playerY;
								float vx = (float)testX + tx - playerX;

								float dist = sqrt(vx*vx + vy*vy);
								float dot = (eyeX * vx / dist) + (eyeY * vy / dist);
								
								p.push_back(std::make_pair(dist, dot));
							}
						}

						std::sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first;});

						float edgeLimit = 0.01f;
						if(distToWall < depth/2){
							if(acos(p.at(0).second) < edgeLimit){
								hitEdge = true;
							}
							if(acos(p.at(1).second) < edgeLimit){
								hitEdge = true;
							}
						}
					}
				}
			}

			int ceiling = (float)(scrHeight / 2.0) - scrHeight / ((float)distToWall);
			int flor = scrHeight - ceiling;

			for(int y = 0; y < scrHeight; y++){
				if(y < ceiling){
					attron(COLOR_PAIR(3));
					mvaddch(y, x, ' ' | A_REVERSE);
					attroff(COLOR_PAIR(3));
				}
				else if(y > ceiling && y <= flor){
					if(wallType < 0){
						mvaddch(y, x, ' ');
					}
					else {
						attron(COLOR_PAIR(wallType));
						if(hitEdge){
							mvaddch(y, x, ' ');
						}
						int brightness = (distToWall / depth) * 68;
						mvaddch(y, x, brightnessChars[brightness]);	
						attroff(COLOR_PAIR(wallType));
					}
				}
				else{
					attron(COLOR_PAIR(2));
					float b = 1.0f - (((float)y - scrHeight / 2.0f) / ((float)scrHeight / 2.0f));
					int brightness = b * 68;
					mvaddch(y, x, brightnessChars[brightness]);	
					attroff(COLOR_PAIR(2));
				}
			}
		}

		mvprintw(0, 0, "                                                                                                                        ");

		move(120,40);
		refresh();

		int input = getch();
		if(input == 'q'){
			quit = true;
		}
		else if(input == 'w'){
			playerX += sinf(angle) * 0.1f;
			playerY += cosf(angle) * 0.1f;
			if(map[(int)playerY][(int)playerX] == '#' || map[(int)playerY][(int)playerX] == '-'){
				playerX -= sinf(angle) * 0.1f;
				playerY -= cosf(angle) * 0.1f;
			}
		}
		else if(input == 's'){
			playerX -= sinf(angle) * 0.1f;
			playerY -= cosf(angle) * 0.1f;
			if(map[(int)playerY][(int)playerX] == '#' || map[(int)playerY][(int)playerX] == '-'){
				playerX += sinf(angle) * 0.1f;
				playerY += cosf(angle) * 0.1f;
			}
		}
		else if(input == 'd'){
			angle += 0.1;
		}
		else if(input == 'a'){
			angle -= 0.1f;
		}
	}
	
	endwin();
	return 0;
}
