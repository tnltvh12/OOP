#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>



#pragma warning(disable : 4996)


class Player {
	int pos;
	char face[100];
	char attack_face[100];
	int nattack;//attack 받고 있는지 확인
	
public:
	int died;//죽인 숫자 카운트
	int player_hp;
	Player(int pos = 20, const char* face = "^___^",const char* attack_face= ">_____<") :pos(pos), player_hp(2),nattack(0),died(0) {

		strcpy(this->face, face);

	}


	void move(int inc) { pos += inc; }//값에 따라 이동 
	int getPosition() const { return pos; }//포지션 받는다
	void checkRange(int screen_size)//범위 체크
	{
		if (pos < 0) pos = 0;
		if (pos >= screen_size - 1) pos = screen_size - 1;
	}
	void draw(char* screen, int screen_size) {//draw에서 바로 check range를 해준다.

		checkRange(screen_size);
		strncpy(screen + pos, face, strlen(face));

	}
	void update(){
		if (nattack == 0) return;
		nattack--;
		if (nattack == 0) {

			strcpy(face, "^___^");
		}
	}
	void on_attacked(){
		nattack = 15;//15프레임지나면 hp가 2씩 줄어든다
		player_hp -= 2;
		strcpy(face, attack_face);
	}
	
};


class Enemy {
	int pos;
	char face[100];
	int frame;//enemy 움직임 시간제어
	int hp;
public:

	
	int active_enemy;//활동하고 있는지 확인
	Enemy(int pos = rand() % 50 + 50, const char* face = "-____-") : pos(pos), hp(10), active_enemy(1),frame(60) {

		strcpy(this->face, face);


	}
	void move(int inc) { pos += inc; }
	int getPosition() const { return pos; }
	void checkRange(int screen_size)
	{
		if (pos < 0) pos = 0;
		if (pos >= screen_size - 1) pos = screen_size - 1;

	}
	void draw(char* screen, int screen_size) {//draw에서 바로 check range를 해준다.

		checkRange(screen_size);

		strncpy(screen + pos, face, strlen(face));


	}
	void update(const Player* player) {

		if (hp == 0) {
			pos = -10;
			active_enemy = 0;
			return;
		}

		else if (hp > 0) {
			
			active_enemy = 1;
			if (frame == 0)//60프레임이 지나면
			{
				move(-1);//움직여주고
				wait();//다시 frame값을 초기화
				return;
			}
			frame--;
			
			
		}
			// 0.5초에 한 칸 씩 이동


    }

	
	void OnHit() {
		hp--;

	}
	void wait() {//60프레임당 1씩 움직임
		frame = 60;
	
	}
	
};

class Bullet {

	char shape = '-';

public:
	bool fire = false;
	int pos = -1;
	void first_move(int player_pos) {//fire를 on 해주고 초기 pos값을 준다.
		pos = player_pos; 
		fire = true;
	}
	void move(int inc) { pos += inc; }
	int getPosition() { return pos; }
	void checkRange(int screen_size)
	{
		
		if (pos >= screen_size) { pos = -1; }
	}
	void draw(char* screen,int screen_size) {//draw에서 바로 check range를 해준다.
		
		screen[pos] = shape;
		checkRange(screen_size);
	}
	void update(const Player *player, const Enemy *enemy) {

		if (!player || !enemy)return;
		if (player->getPosition() < enemy->getPosition())
		{
			move(1);

		}
		else {
			move(-1);

		}
	}


};

int main()

{
	
	
	srand((unsigned)time(NULL));
	const int screen_size = 119;
	char screen[screen_size + 1 + 1];

	Player player;
	Enemy enemy[5];

	Bullet pBullet[10];


	clock_t start_t= clock();
	//게임 시작 시간체크
	clock_t oldtime = clock();//깜빡임 처리 변수
	while (true)
	{
		memset(screen, ' ', screen_size);



		if (_kbhit()) {

			char key = _getch();
			if (key == 27) break;
			if (key == -32)
			{
				key = _getch();
			}
			switch (key)
			{

			case'a':
				player.move(-1);
				break;
			case 'd':
				player.move(1);
				break;


			case ' ':

				for (int i = 0; i < 10; i++) {//배열을 확인해서 비어있는 bullet 찾는다.
					if (pBullet[i].fire == false) {
						
						pBullet[i].first_move(player.getPosition());

					}
					break;
				}

			}


		}
		player.draw(screen, screen_size);//screen의 값을 받아서 그림

		for (int i = 0; i < 5; i++) {//enemy draw
			enemy[i].draw(screen, screen_size);

		}
		
		
		
		for (int i = 0; i < 10; i++) {//움직이는 bullet 체크
			if (pBullet[i].fire == false) {
				break;

			}
			else if (pBullet[i].fire == true) {
				pBullet[i].draw(screen,screen_size);
				


			}
		}
		
		for (int i = 0; i < 10; i++) {
			if (pBullet[i].fire) {//bullet이 fire 중일 때

				for (int j = 0; j < 5; j++) {

					if (enemy[j].active_enemy == 1) {//활동중인 enemy 찾아서


						if (pBullet[i].getPosition() == enemy[j].getPosition()) {//총알이 enemy에 닿으면


							enemy[j].OnHit();
							pBullet[i].pos = -1;

							pBullet[i].fire = false;//사용이 끝난 총알 초기화 시켜줌
							break;

						}



						else {


							pBullet[i].update(&player, &enemy[j]);//클래스 변수는 크기가 크니까 레퍼런스로 넘겨줌


							if (player.getPosition() < enemy[j].getPosition()) { pBullet[i].move(1); }
							else { pBullet[i].move(-1); }


						}

					}

				}
			}
		}
		if ((player.getPosition() + 5) >= enemy->getPosition()) {//player의 enemy와의 충돌체크
			player.on_attacked();
		
		}

		player.update();//player의 damage를 업데이트함
		

		for (int k = 0; k < 5; k++) {

			enemy[k].update(&player);

		}

		if (player.player_hp == 0) {//player의 수명이 다하면 게임을 종료한다.
			break;
		}
		screen[screen_size] = '\r';
		screen[screen_size + 1] = 0;
		printf_s("%s", screen);

		while (1) {//초당 30번출력
		
			clock_t curtime = clock();
			if (curtime - oldtime > 33)
			{
				oldtime = curtime;
				break;
			}
		}
	}
	clock_t end_t = clock();
	
	int game_time = (end_t - start_t);
	//게임 결과 print
	printf("player가 죽었습니다. 죽인 몬스터 숫자:%d, 걸린시간: %d ",player.died,game_time);
	return 0;
}
