#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define WIDTH 636
#define HEIGHT 480
#define SIZEMAX		100		// 定义蛇的最大长度
#define MAX_FOOD    100
#define MAX_SNAKE   6		// 生成蛇的最大数量
#define DEFAULT_AI_LENGHT 4 // 默认AI蛇的长度
#define SNAKELEN	12		// 蛇的大小和蛇每次移动的距离
#define SPEED		120		// 初始速度 
#define SPEED_BULLET 2		// 子弹速度 默认两倍蛇的速速

/****************************************/
enum DIR
{
	left, right, up, down
};

struct COOR	// 定义一个结构体表示坐标
{
	int x;
	int y;
};

struct SNAKE	// 定义结构体，来存储蛇的相关信息
{
	COOR crd[SIZEMAX];	// 保存蛇的每个结数的坐标，最多有100节
	int node;			// 保存蛇当前节数
	DIR dir;			// 当前蛇行进的方向
}snake;

struct SNAKE_AI	// 定义结构体，来存储蛇的相关信息
{
	COOR crd[SIZEMAX];	// 保存蛇的每个结数的坐标，最多有100节
	DIR dir;// 当前蛇行进的方向
	bool exist;
}aiSnake[MAX_SNAKE];

struct FOOD		// 定义结构体，来存储食物的相关信息
{
	COOR crd;	// 食物出现的坐标
	int flag;	// 食物的状态，1表示食物还没被吃，0表示食物被吃了
}food[MAX_FOOD];

struct Bullet {
	int x;
	int y;
	bool exist;
	DIR dir;
};

int speed = SPEED;		// 方便以后在程序中改变速度
Bullet bullet;			//子弹结构体
int foodi = 0;			// 食物的下标
int fraction = 0;       // 分数
int killCount = 0;			// 击杀数
int eatCount = 0;		// 吃的果实数

/***************************************/
void init();			 // 初始化游戏
void WelcomeScreen();	 // 开始界面
void setfoodcrd();		 // 设置食物要出现的坐标
void showfood();		 // 在相应的坐标画出食物
void snakemove();		 // 移动蛇，实际就是改变蛇各节的坐标
void judgeslod();		 // 判断蛇是否撞墙或者撞到自己
void judgesef();		 // 判断蛇是否吃到食物了
void showsnake();		 // 在屏幕上画出蛇
void changeskdir();		 // 接受用户输入，改变蛇的前进方向
void shoot();			 // 发射子弹
void drawBullet();       // 画子弹
void createAi(int count);// 生成ai蛇
void drawAi();			 // 画ai蛇
void aiSnakeMove();      // ai蛇移动
void scoreDisplay();     // 分数显示
void suspend();			 // 游戏暂停
void gameover();		 // 游戏结束 
/**************************************/

int main()
{
	initgraph(WIDTH + 250, HEIGHT);
	WelcomeScreen();
	init();
	while (1)
	{
		while (!_kbhit())
		{
			if (!food[foodi].flag)
			{
				setfoodcrd();
			}
			scoreDisplay();
			showfood();
			drawBullet();
			judgeslod();
			judgesef();
			aiSnakeMove();
			snakemove();
			Sleep(speed);
		}
		changeskdir();
	}
	return 0;
}

/***********************************/
void init()
{
	initgraph(WIDTH + 250, HEIGHT);
	srand(time(NULL));
	rectangle(WIDTH + SNAKELEN, 0, WIDTH + SNAKELEN + 1, HEIGHT);
	settextcolor(RED);
	settextstyle(25, 0, L"华文新魏"); //设置文字样式
	outtextxy(WIDTH + 20, 100, L"收集果实:");
	outtextxy(WIDTH + 20, 150, L"击败敌蛇:");
	outtextxy(WIDTH + 20, 200, L"总体得分:");
	food[foodi].flag = 0;
	snake.crd[0].x = 0 + SNAKELEN;
	snake.crd[0].y = 0;
	snake.crd[1].x = 0;
	snake.crd[1].y = 0;
	snake.node = 2;
	snake.dir = right;
	//初始化子弹
	bullet.exist = false;
	//初始化ai蛇
	for (int i = 0; i < MAX_SNAKE; i++) {
		aiSnake[i].exist = false;
	}
	createAi(1);
}

void WelcomeScreen()
{
	settextcolor(WHITE);//设置字体颜色为白色
	settextstyle(40, 0, L"华文新魏"); //设置文字样式
	outtextxy(350, 100, L"贪吃蛇");
	settextstyle(25, 0, L"华文新魏"); //设置文字样式
	outtextxy(310, 200, L"按任意键开始游戏");
	_getch();

}

void scoreDisplay()
{
	setfillcolor(BLACK);
	fillrectangle(WIDTH + 140, 90, WIDTH + 200, 210);
	WCHAR score[5];
	WCHAR kill[5];
	WCHAR eat[5];
	_stprintf(score, L"%d", fraction);
	_stprintf(kill, L"%d", killCount);
	_stprintf(eat, L"%d", eatCount);
	settextcolor(RED);
	settextstyle(25, 0, L"华文新魏"); //设置文字样式
	outtextxy(WIDTH + 150, 202, score);
	outtextxy(WIDTH + 150, 150, kill);
	outtextxy(WIDTH + 150, 100, eat);
	setfillcolor(BLACK);
}

void setfoodcrd()
{
	food[foodi].crd.x = rand() % (WIDTH / SNAKELEN) * SNAKELEN;
	food[foodi].crd.y = rand() % (HEIGHT / SNAKELEN) * SNAKELEN;
	food[foodi].flag = 1;
}

void showfood()
{
	setcolor(WHITE);
	setfillcolor(RED);
	for (int i = 0; i <= foodi; i++) {
		if (food[i].flag == 1)
			fillrectangle(food[i].crd.x, food[i].crd.y, food[i].crd.x + SNAKELEN, food[i].crd.y + SNAKELEN);
	}
}

void snakemove()
{
	int i; for (i = snake.node; i > 0; i--)
	{
		snake.crd[i].x = snake.crd[i - 1].x;
		snake.crd[i].y = snake.crd[i - 1].y;
	}
	switch (snake.dir)
	{
	case up:
		snake.crd[0].y -= SNAKELEN;
		break;
	case down:
		snake.crd[0].y += SNAKELEN;
		break;
	case left:
		snake.crd[0].x -= SNAKELEN;
		break;
	case right:
		snake.crd[0].x += SNAKELEN;
	} 
	showsnake();
}

void judgeslod()
{
	//撞墙
	if (snake.crd[0].x < 0 || snake.crd[0].x >= WIDTH ||
		snake.crd[0].y < 0 || snake.crd[0].y >= HEIGHT)
	{
		gameover();
	}
	//撞到自己
	for (int i = snake.node - 1; i > 0; i--)
	{
		if (snake.crd[0].x == snake.crd[i].x && snake.crd[0].y == snake.crd[i].y)
		{
			gameover();
		}
	}

	//撞到AI
	for (int i = 0; i < MAX_SNAKE; i++)
	{
		if (aiSnake[i].exist)
		{
			for (int j = 0; j < DEFAULT_AI_LENGHT; j++)
			{
				if (snake.crd[0].x == aiSnake[i].crd[j].x && snake.crd[0].y == aiSnake[i].crd[j].y)
					gameover();
			}
		}
	}
}

void judgesef()
{
	for (int i = 0; i <= foodi; i++) {
		if (snake.crd[0].x == food[i].crd.x && snake.crd[0].y == food[i].crd.y) {
			snake.node++;
			rectangle(food[i].crd.x, food[i].crd.y, food[i].crd.x + SNAKELEN, food[i].crd.y + SNAKELEN);
			food[i].flag = 0;
			eatCount ++;
			fraction += 5;
			return;
		}
	}
}

void showsnake()
{
	setcolor(BLUE);
	
	int i;
	for (i = snake.node - 1; i >= 0; i--)
	{
		setfillcolor(RGB(rand()%256,rand()%256,rand()%256));
		fillrectangle(snake.crd[i].x, snake.crd[i].y, snake.crd[i].x + SNAKELEN, snake.crd[i].y + SNAKELEN);
	}
	//清除经过的路线
	setfillcolor(BLACK);
	setcolor(BLACK);
	fillrectangle(snake.crd[snake.node].x, snake.crd[snake.node].y, snake.crd[snake.node].x + SNAKELEN, snake.crd[snake.node].y + SNAKELEN);
}

void changeskdir()
{
	char key;
	key = _getch();
	switch (key)
	{
	case 'w':
	case 'W':
		if (snake.dir != down)
		{
			snake.dir = up;
		}
		break;
	case 's':
	case 'S':
		if (snake.dir != up)
		{
			snake.dir = down;
		}
		break;
	case 'a':
	case 'A':
		if (snake.dir != right)
		{
			snake.dir = left;
		}
		break;
	case 'd':
	case 'D':
		if (snake.dir != left)
		{
			snake.dir = right;
		}
		break;
	case 'u':
		if (foodi < 99) {
			foodi++;
			setfoodcrd();
		}
		break;
	case 'i':
		if (foodi > 0) {
			setcolor(BLACK);
			setfillcolor(BLACK);
			fillrectangle(food[foodi].crd.x, food[foodi].crd.y, food[foodi].crd.x + SNAKELEN, food[foodi].crd.y + SNAKELEN);
			foodi--;
		}
		break;
	case 'j':
	case 'J':
		shoot();
		break;
	case ' ':
		suspend();
		break;
	}

}

void shoot() 
{
	if (!bullet.exist) 
	{
		//子弹的坐标为蛇头的坐标
		bullet.x = snake.crd[0].x;
		bullet.y = snake.crd[0].y;
		bullet.dir = snake.dir;
		//生成子弹
		bullet.exist = true;
	}
}

void drawBullet() 
{
	if (bullet.exist) {
		//清除经过的路线
		setfillcolor(BLACK);
		setcolor(BLACK);
		fillrectangle(bullet.x, bullet.y, bullet.x + SNAKELEN, bullet.y + SNAKELEN);
		setcolor(BLUE);
		setfillcolor(YELLOW);
		//判断子弹方向
		switch (bullet.dir)
		{
		case up:
			bullet.y -= SNAKELEN * SPEED_BULLET;
			break;
		case down:
			bullet.y += SNAKELEN * SPEED_BULLET;
			break;
		case left:
			bullet.x -= SNAKELEN * SPEED_BULLET;
			break;
		case right:
			bullet.x += SNAKELEN * SPEED_BULLET;
		}
	}
	//子弹运动
	if (bullet.exist)
	{
		fillrectangle(bullet.x, bullet.y, bullet.x + SNAKELEN, bullet.y + SNAKELEN);
	}
	//子弹越界判断
	if (bullet.x < 0 || bullet.x >= WIDTH || bullet.y < 0 || bullet.y >= HEIGHT)
	{
		setfillcolor(BLACK);
		setcolor(BLACK);
		fillrectangle(bullet.x, bullet.y, bullet.x + SNAKELEN, bullet.y + SNAKELEN);
		setcolor(WHITE);
		rectangle(WIDTH + SNAKELEN, 0, WIDTH + SNAKELEN + 1, HEIGHT);
		bullet.exist = false;
	}
		
	//子弹击中AI
	for (int i = 0; i < MAX_SNAKE; i++)
	{
		if (aiSnake[i].exist)
		{
			for (int j = 0; j < DEFAULT_AI_LENGHT; j++)
			{
				if (bullet.x == aiSnake[i].crd[j].x && bullet.y == aiSnake[i].crd[j].y)
				{
					setfillcolor(BLACK);
					setcolor(BLACK);
					for (int k = 0; k < DEFAULT_AI_LENGHT; k++)
					{
						fillrectangle(aiSnake[i].crd[k].x, aiSnake[i].crd[k].y, aiSnake[i].crd[k].x + SNAKELEN, aiSnake[i].crd[k].y + SNAKELEN);
					}				
					aiSnake[i].exist = false;
					bullet.exist = false;
					killCount++;
					fraction += 10;
					if (killCount < 2)
					{
						createAi(1);
					}
					else 
					{
						createAi(killCount / 2 + 1);
					}
					return;
				}
					
			}
		}
	}

}

void createAi(int count)
{
	for (int i = 0; i < MAX_SNAKE; i++) 
	{
		if (!aiSnake[i].exist)
		{
			int m = DEFAULT_AI_LENGHT * SNAKELEN * 2;
			//随机生成坐标		
			int x =  rand() % ((WIDTH - m)/ SNAKELEN) * SNAKELEN;
			int y = rand() % ((HEIGHT - m)/ SNAKELEN) * SNAKELEN;
			//生成的蛇为横向还是竖向
			int dir = rand() % 2;
			if (dir == 0) 
			{
				//横向生成
				aiSnake[i].crd[0].x = x;
				aiSnake[i].crd[0].y = y;
				for (int j = 1; j < DEFAULT_AI_LENGHT; j++)
				{
					aiSnake[i].crd[j].x = aiSnake[i].crd[j - 1].x + SNAKELEN;
					aiSnake[i].crd[j].y = aiSnake[i].crd[j - 1].y;
				}
				aiSnake[i].dir = left;
			}
			else
			{
				//竖向生成
				aiSnake[i].crd[0].x = x;
				aiSnake[i].crd[0].y = y;
				for (int j = 1; j < DEFAULT_AI_LENGHT; j++)
				{
					aiSnake[i].crd[j].x = aiSnake[i].crd[j - 1].x;
					aiSnake[i].crd[j].y = aiSnake[i].crd[j - 1].y + SNAKELEN;
				}
				aiSnake[i].dir = down;
			}
			aiSnake[i].exist = true;			
			
			if (i == count - 1)
				return;
		}
	}

}

void drawAi()
{
	for (int i = 0; i < MAX_SNAKE; i++) 
	{
		setcolor(BLUE);
		setfillcolor(GREEN);
		if (aiSnake[i].exist)
		{
			for (int j = DEFAULT_AI_LENGHT-1; j>=0; j--)
			{
				fillrectangle(aiSnake[i].crd[j].x, aiSnake[i].crd[j].y, aiSnake[i].crd[j].x + SNAKELEN, aiSnake[i].crd[j].y + SNAKELEN);
			}
		}
		//清除经过的路线
		setfillcolor(BLACK);
		setcolor(BLACK);
		fillrectangle(aiSnake[i].crd[DEFAULT_AI_LENGHT].x, aiSnake[i].crd[DEFAULT_AI_LENGHT].y, aiSnake[i].crd[DEFAULT_AI_LENGHT].x + SNAKELEN, aiSnake[i].crd[DEFAULT_AI_LENGHT].y + SNAKELEN);
	}
}

void aiSnakeMove()
{
	for (int i = 0; i < MAX_SNAKE; i++)
	{
		if (aiSnake[i].exist) {
			for (int j = DEFAULT_AI_LENGHT; j > 0; j--)
			{
				aiSnake[i].crd[j].x = aiSnake[i].crd[j-1].x;
				aiSnake[i].crd[j].y = aiSnake[i].crd[j-1].y;
			}
		}
		//撞墙情况
		if (aiSnake[i].dir == up || aiSnake[i].dir == down)
		{
			if (aiSnake[i].crd[0].y - SNAKELEN <= 0 || aiSnake[i].crd[0].y + SNAKELEN >= HEIGHT)
			{
				if (aiSnake[i].crd[0].x - SNAKELEN <= 0) {
					aiSnake[i].dir = right;
				}
				else if (aiSnake[i].crd[0].x + SNAKELEN >= WIDTH) {
					aiSnake[i].dir = left;
				}
				else {
					if (rand() % 2 == 0)
						aiSnake[i].dir = left;
					else
						aiSnake[i].dir = right;
				}			
			}
			else {
				if (rand() % 16 == 0) {
					if (aiSnake[i].crd[0].x - SNAKELEN <= 0) {
						aiSnake[i].dir = right;
					}
					else if (aiSnake[i].crd[0].x + SNAKELEN >= WIDTH) {
						aiSnake[i].dir = left;
					}
					else {
						if (rand() % 2 == 0)
							aiSnake[i].dir = left;
						else
							aiSnake[i].dir = right;
					}
				}
			}
			
		}
		else if(aiSnake[i].dir == left || aiSnake[i].dir == right)
		{
			if (aiSnake[i].crd[0].x - SNAKELEN <= 0 || aiSnake[i].crd[0].x + SNAKELEN >= WIDTH)
			{
				if (aiSnake[i].crd[0].y - SNAKELEN <= 0) {
					aiSnake[i].dir = down;
				}
				else if (aiSnake[i].crd[0].y + SNAKELEN >= HEIGHT) {
					aiSnake[i].dir = up;
				}
				else {
					if (rand() % 2 == 0)
						aiSnake[i].dir = up;
					else
						aiSnake[i].dir = down;
				}
			}
			else 
			{
				if(rand()%10 == 0)
				{
					if (aiSnake[i].crd[0].y - SNAKELEN <= 0) {
						aiSnake[i].dir = down;
					}
					else if (aiSnake[i].crd[0].y + SNAKELEN >= HEIGHT) {
						aiSnake[i].dir = up;
					}
					else {
						if (rand() % 2 == 0)
							aiSnake[i].dir = up;
						else
							aiSnake[i].dir = down;
					}
				}
			}
		}

		switch (aiSnake[i].dir)
		{
		case up:
			aiSnake[i].crd[0].y -= SNAKELEN;
			break;
		case down:
			aiSnake[i].crd[0].y += SNAKELEN;
			break;
		case left:
			aiSnake[i].crd[0].x -= SNAKELEN;
			break;
		case right:
			aiSnake[i].crd[0].x += SNAKELEN;
		}
		drawAi();
	}
}

void suspend()
{
	while (true)
	{
		if (_getch() == ' ')
			break;
	}
	Sleep(20);
}

void gameover()
{
	settextcolor(RED);//设置字体颜色为白色
	settextstyle(40, 0, L"华文新魏"); //设置文字样式
	outtextxy(280, 100, L"GAME OVER!");
	settextstyle(25, 0, L"华文新魏"); //设置文字样式
	outtextxy(300, 200, L"按任意键重新开始");
	_getch();
	foodi = 0;			// 食物的下标
	fraction = 0;       // 分数
	killCount = 0;			// 击杀数
	eatCount = 0;		// 吃的果实数
	main();
}
