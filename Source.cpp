#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <fstream>

// ct thiet ke boardWidth de vua khit xe voi toc do cua 3 level
//BCNN(x,y,z), chon x, =>> bW=BCNN*sotuchon+ 2(bien)+ maxcar - 1 

using namespace std;

#define consoleWidth 85
#define consoleHeight 25
#define boardWidth 66 // cho 3 speed 1, 3,5
#define boardHeight 25 
#define carSize 5
#define MaxCar 10


HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

BOOL WINAPI CloseHandle(
	_In_ HANDLE hObject
	);

BOOL WINAPI SetConsoleTextAttribute(
	_In_ HANDLE hConsoleOutput,
	_In_ WORD   wAttributes
	);

enum TrangThai{ turnleft, turnright };
enum Choose{ openfile, playing, yes, no };
//Choose luachon = playing;
enum life{ song, chet };
enum status{ pause, play };

struct toado
{
	int x, y;
};

struct Car
{
	char s[5];
	int y, x;
	TrangThai tt;
};

struct Human
{
	int x;
	int y;
	char hinhdang[5];
};

//danh_sach_bien_toan_cuc
Human human;
Human buff[4];
int dem = -1;
int level;
int Diem = 0;
Choose luachon = playing;
life mang = song;
char TenFileLuu[40];
char TenFileMo[40];
Car ford[MaxCar];
int deflag = 10;
status tus = play;

void FixConsoleWindow() 
{  
HWND consoleWindow = GetConsoleWindow(); 
LONG style = GetWindowLong(consoleWindow, GWL_STYLE); 
style = style & ~(WS_MAXIMIZEBOX)& ~(WS_THICKFRAME); 
SetWindowLong(consoleWindow, GWL_STYLE, style); 
}

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void removeCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)
{
	GotoXY(x, y); //cout << '==';
	//SetConsoleTextAttribute(consoleHandle, (19 << 4) | 12);
	for (int i = 1; i <= width; i++){
		cout << 'X';
	}
	cout << 'X';
	GotoXY(x, height + y);  cout << 'X';
	for (int i = 1; i < width; i++)cout << 'X';
	cout << 'X';
	for (int i = y + 1; i < height + y; i++)
	{
		GotoXY(x, i); cout << 'X';
		GotoXY(x + width, i); cout << 'X';
	}
	GotoXY(3, height + 3); cout << "-[Game made by 1712828 - Huynh Thanh Khai Tran]-";
	GotoXY(80, 20); cout << "  [][][]  [W]:len";
	GotoXY(80, 21); cout << "[A]:trai___[S]:xuong__[D]::Phai";
	GotoXY(80, 22); cout << "[E]:thoat__[L]:Luugame";
	GotoXY(80, 23);	cout << "[T]:Loadlaigame__[P]:pause game";
	GotoXY(curPosX, curPosY);
}
void Reset(int x, int y)
{

	for (int d = 0; d < y; d++)
	for (int r = 0; r < x; r++)
	{
		GotoXY(r, d);
		cout << " ";
	}
}
void ThongBao();
void ThongBao()
{
	GotoXY(40, 15);
	for (int d = 14; d < 21; d++)
	for (int r = 40; r < 81; r++)
	{
		if ((r == 40) || (d == 14) || (r == 80) || (d == 20)){
			GotoXY(r, d);
			cout << "*";
		}
		else { cout << " "; }
	}
	GotoXY(41, 16);
	cout << " Ban muon choi tiep ko ?";
	GotoXY(42, 17);
	cout << "-----Nhan Y de choi lai tu dau";
	GotoXY(42, 18);
	cout << "-----Nhan N de thoat game";
	GotoXY(42, 19);
	cout << "-----Nhan T de load lai menu";
}
void KhoiTaoHuman()
{
	if (luachon != yes){
		human.x = 5 + rand() % 20;
		human.y = boardHeight - 1;
	}
	strcpy(human.hinhdang, "Y");
}


void KhoiTaoXe()
{
 
	int sp;
	switch (level)
	{
	case 1: {sp = 1; break; };
	case 2: {sp = 3; break; };
	case 3: {sp = 5; break; };
	}
	int SoXe = MaxCar;
	for (int i = 0; i < SoXe; i++)
	{
		if (luachon != yes){
			ford[i].tt = turnright;
			ford[i].y = 3 + rand() % (boardHeight - 5);
			ford[i].x = 1 + ((sp))* (rand() % 8);
		}
		strcpy(ford[i].s, "=0=0=");
	}
}

void MotXeChay();
void MotXeChay(Car &oto, int &lev)
{
	
	int sp;
	switch (lev)
	{
	case 1: {sp = 1; break; };
	case 2: {sp = 3; break; };
	case 3: {sp = 5; break; };
	}
	GotoXY(oto.x, oto.y);
	Sleep(3);
	if (oto.x + carSize > (boardWidth)-1)
	{
		oto.tt = turnleft;
	}
	else if (oto.x - sp < 1)
	{
		oto.tt = turnright;
	}
	if (oto.tt == turnright) {

		if (level != 3) oto.x += sp;
		else if (level == 3)
		{
			if (deflag > 100)   oto.x += sp;
			deflag--;
			if (deflag == 0) deflag = 140;
		}
		GotoXY(oto.x, oto.y);
		cout << oto.s;
		GotoXY(oto.x - sp, oto.y);
		for (int id = 0; id < sp; id++)
		{
			cout << "_";
		}

	}
	if (oto.tt == turnleft) {
		oto.x -= sp;
		GotoXY(oto.x, oto.y);
		cout << oto.s;
		GotoXY(oto.x + carSize, oto.y);
		for (int id = 0; id < sp; id++)
		{
			cout << "_";
		}
	}
}


void Xechay()
{

	int i = 0;
	while (1)
	{
		MotXeChay(ford[i], level);
		i++;
		if (i >= MaxCar) break;
		MotXeChay(ford[i], level);

	}
}
void DieuKhien();
void HienThiHuman()
{
	luachon = playing;
	GotoXY(human.x, human.y);
	cout << (human.hinhdang);
	Diem = 0;
	GotoXY(human.x, human.y);
	for (int dem = 0; dem < MaxCar; dem++)
	{
		for (int j = 0; j < dem; j++)
		if ((human.x == buff[j].x) && (human.y == 1))
		{
			ThongBao();
			GotoXY(42, 15);
			cout << "game over :: tong chet nguoi khac \n \n";
			GotoXY(42, 16);
			//cout << buff[j].x << buff[j].y;
			GotoXY(70, 30);
			mang = chet;
			return;

		}
		if (human.y == ford[dem].y){
			if ((human.x >= ford[dem].x) && (human.x <= ford[dem].x + carSize))
			{
				ThongBao();
				GotoXY(42, 15);
				cout << "game over :: bi xe tong \n \n";
				GotoXY(70, 30);
				mang = chet;
				return;
			}
			//break;
		}


		else if (human.y < ford[dem].y)
		{
			Diem++;
		}

	}
	GotoXY(100, 1);
	cout << "Diem:       ";
	GotoXY(107, 1);
	cout << Diem;
	Sleep(2);
}
void thongbao_saveGame()
{
	GotoXY(40, 15);
	for (int d = 14; d < 21; d++)
	for (int r = 40; r < 81; r++)
	{
		if ((r == 40) || (d == 14) || (r == 80) || (d == 20)){
			GotoXY(r, d);
			cout << "*";
		}
		else { cout << " "; }
	}
	GotoXY(43, 15);
	cout << "Luu tap ten duoi tin: ";
	GotoXY(43, 16);
	cin >> TenFileLuu;

}

void saveGame()
{
	thongbao_saveGame();
	ofstream fileOut(TenFileLuu);
	if (fileOut.fail())
	{
		GotoXY(43, 17);  cout << " khong the tao file. exit ";
		exit(0);
	}
	fileOut << human.x << " ";
	fileOut << human.y << endl;
	for (int tam = 0; tam < 4; tam++)
	{
		fileOut << buff[tam].x << " ";
		buff[tam].x = 0;
		fileOut << buff[tam].y << " " << endl;
		buff[tam].y = 0;
	}
	int buffxe = 0;
	for (int dxe = 0; dxe < MaxCar; dxe++)
	{
		fileOut << ford[dxe].x << " ";
		fileOut << ford[dxe].y << " ";
		if (ford[dxe].tt == turnleft)
		{
			buffxe = 1;
			fileOut << buffxe << endl;
		}
		else if (ford[dxe].tt == turnright)
		{
			buffxe = 2;
			fileOut << buffxe << endl;
		}
	}
	fileOut << dem << endl;
	fileOut << level << endl;
	fileOut << Diem << endl;
	int buffmang = 1;
	if (mang == song)
		fileOut << buffmang << endl;
	else if (mang == chet)
	{
		buffmang = 0;
		fileOut << buffmang << endl;
	}
	// fileOut << TenFile << endl;
	GotoXY(43, 17);
	cout << "OK, Da luu";
	GotoXY(43, 18); cout << "chuong trinh se thoat trong giay lat";
	GotoXY(3, 26);
	cout << "nhan phim bat ky de thoat: ";
	exit(0);
}

void thongbao_loadGame()
{
	GotoXY(40, 15);
	for (int d = 14; d < 21; d++)
	for (int r = 40; r < 81; r++)
	{
		if ((r == 40) || (d == 14) || (r == 80) || (d == 20)){
			GotoXY(r, d);
			cout << "*";
		}
		else { cout << " "; }
	}
	GotoXY(41, 16);
	cout << " Ban muon load game cu ko? ";
	GotoXY(42, 17);
	cout << "----- Nhan C de load file da luu";
	GotoXY(42, 18);
	cout << "----- Nhan K de load game moi";
	GotoXY(42, 19);
	cout << "------Nhan E de thoat game";

	int key = _getch();
	while ((key != 'C') && (key != 'c') && (key != 'K') && (key != 'k') && (key != 'E') && (key != 'e'))
	{
		key = _getch();
	}
	if ((key == 'C') || (key == 'c'))	{
		luachon = yes;
		strcpy(human.hinhdang, "Y");
	}
	else if ((key == 'K') || (key == 'k')) {
		luachon = no;
	}
	else if ((key == 'E') || (key == 'e'))
	{
		GotoXY(42, 21);
		exit(0);
	}


}

void loadGame()
{
	system("cls");
	GotoXY(50, 10);
	cout << " GAME ROAD CROSSING ";
	GotoXY(50, 11);
	cout << " ******[MENU]******* ";
	GotoXY(45, 22);
	cout << " ******[DO AN LOP THUC HANH]******* ";
	GotoXY(45, 23);
	cout << " MADE BY 1712828 - HUYNH THANH KHAI TRAN ";
	thongbao_loadGame();
	if (luachon == no) {
		system("cls"); return;
	}
	GotoXY(40, 15);
	for (int d = 14; d < 21; d++)
	for (int r = 40; r < 81; r++)
	{
		if ((r == 40) || (d == 14) || (r == 80) || (d == 20)){
			GotoXY(r, d);
			cout << "*";
		}
		else { cout << " "; }
	}
	GotoXY(43, 15);
	cout << "Nhap ten tap tin dang xxx.txt: ";
	GotoXY(43, 16);
	cin >> TenFileMo;
	ifstream fileIn(TenFileMo);
	if (fileIn.fail())
	{
		GotoXY(43, 17);  cout << " khong the mo file:::> exit ";
		exit(0);
	}
	int buffxe = 0;
	while (!fileIn.eof())
	{
		fileIn >> human.x;
		fileIn >> human.y;
		for (int tmp = 0; tmp < 4; tmp++)
		{
			fileIn >> buff[tmp].x;
			fileIn >> buff[tmp].y;
		}

		for (int dxe = 0; dxe < MaxCar; dxe++)
		{
			fileIn >> ford[dxe].x;

			fileIn >> ford[dxe].y;

			fileIn >> buffxe;
			if (buffxe == 1) { ford[dxe].tt == turnleft; }
			else if (buffxe == 2)
			{
				ford[dxe].tt = turnright;
			}
		}
		GotoXY(80, 4);
		fileIn >> dem;
		fileIn >> level;
		fileIn >> Diem;
		int buffmang;
		fileIn >> buffmang;
		if (buffmang == 0) mang = chet;
		else if (buffmang == 1) mang = song;
	}
	system("cls");
}

void DieuKhien()
{
	if (kbhit())
	{
		int key = _getch();
		GotoXY(human.x, human.y);
		cout << " ";
		if ((key == 'A') || (key == 'a') && (human.x > 1))
			human.x--;
		else if (((key == 'D') || (key == 'd')) && (human.x) < boardWidth - 1)
			human.x++;
		else if (((key == 'W') || (key == 'w')) && (human.y > 1))
			human.y--;
		else if (((key == 'S') || (key == 's')) && (human.y < boardHeight - 1))
			human.y++;
		else if ((key == 'L') || (key == 'l'))
			saveGame();
		else if ((key == 'T') || (key == 't'))
			loadGame();
		else if ((key == 'E') || (key == 'e'))
			exit(0);
		else if ((key == 'P') || (key == 'p'))
			tus = pause;
	}
	HienThiHuman();
}
void runLevel();
void runLevel()
{
	if (luachon != yes) level = 1;
	while (level < 4)
	{
		mang = song;
		//if(luachon!=yes) level = 1;
		dem++;
		if (luachon != yes){
			buff[dem].x = human.x;
			buff[dem].y = human.y;
		}
		GotoXY(0, 0);
		Reset(boardWidth, boardHeight);
		for (int index = 1; index <= dem; index++)
		{
			GotoXY(buff[index].x, buff[index].y);
			cout << "Y";
		}
		GotoXY(80, 3);
		KhoiTaoHuman();
		KhoiTaoXe();
		DrawBoard(0, 0, boardWidth, boardHeight);
		Diem = 0;
		GotoXY(70, 1); cout << "Trang thai: level " << level;
		while (1)
		{

			HienThiHuman();
			if (mang == chet){
				GotoXY(human.x, human.y);
				for (int ef = 0; ef <= 5; ef++)
				{
					GotoXY(human.x, human.y);
					cout << "   ";
					Sleep(100);
					GotoXY(human.x, human.y);
					cout << "die";
					Sleep(100);
					GotoXY(human.x, human.y);
					cout << "   ";
				}
				int key = _getch();
				while ((key != 'Y') && (key != 'n') && (key != 'y') && (key != 'N') && (key != 'T') && (key != 't'))
				{
					key = _getch();
				}
				if ((key == 'Y') || (key == 'y'))	{
					for (int tp = 0; tp < 4; tp++)
					{
						buff[tp].x = 0;
						buff[tp].y = 0;
					}
					system("cls");
				}
				else if ((key == 'N') || (key == 'n')) {
					GotoXY(3, 26);
					cout << "nhan phim bat ky de thoat: ";
					exit(0);
				}
				else if ((key == 'T') || (key == 't')) {
					loadGame();
				}
				level = 0;
				dem = -1;
				break;
			}
			DieuKhien();
			while (tus == pause)
			{
				GotoXY(82, 26); cout << "|||||| PAUSE |||||| Nhan P de tiep tuc";
				int khoa = _getch();
				while ((khoa != 'P') && (khoa != 'p')){
					khoa = _getch();
				}
				if ((khoa == 'P') || (khoa == 'p'))	{
					tus = play;
					GotoXY(82, 26); cout << "                       ";
				}
			}
			Xechay();
			if ((Diem == 10) && (human.y == 1))
			{
				if (level < 3){
					GotoXY(70, 9);
					cout << "chuc mung hoan thanh level " << level;
					Sleep(500);
					GotoXY(70, 9);
					cout << "dang chuyen sang level " << level + 1 << "        ";
					Sleep(500);
					GotoXY(70, 9);
					cout << "                                             ";
					//GotoXY(70, 3); cout << "level up";
					break;
				}
				else if (level == 3)
				{
					GotoXY(70, 9);
					cout << "chuc mung hoan thanh level " << level;
					Sleep(500);
					GotoXY(70, 9);
					cout << "**** Ban da hoan thanh tat ca level *** ";
					GotoXY(70, 10);
					break;
				}
			}
		}
		level++;
	}
}

int main()
{
	srand(time(NULL));
	FixConsoleWindow();
	resizeConsole(1200, 600);
	removeCursor();
	//SetConsoleTextAttribute(consoleHandle, ( 17 << 4) | 12);
	loadGame();
	DrawBoard(0, 0, boardWidth, boardHeight);
	runLevel();
	return 0;
}