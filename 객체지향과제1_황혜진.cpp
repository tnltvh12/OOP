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
	int nattack;//attack �ް� �ִ��� Ȯ��
	
public:
	int died;//���� ���� ī��Ʈ
	int player_hp;
	Player(int pos = 20, const char* face = "^___^",const char* attack_face= ">_____<") :pos(pos), player_hp(2),nattack(0),died(0) {

		strcpy(this->face, face);

	}


	void move(int inc) { pos += inc; }//���� ���� �̵� 
	int getPosition() const { return pos; }//������ �޴´�
	void checkRange(int screen_size)//���� üũ
	{
		if (pos < 0) pos = 0;
		if (pos >= screen_size - 1) pos = screen_size - 1;
	}
	void draw(char* screen, int screen_size) {//draw���� �ٷ� check range�� ���ش�.

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
		nattack = 15;//15������������ hp�� 2�� �پ���
		player_hp -= 2;
		strcpy(face, attack_face);
	}
	
};


class Enemy {
	int pos;
	char face[100];
	int frame;//enemy ������ �ð�����
	int hp;
public:

	
	int active_enemy;//Ȱ���ϰ� �ִ��� Ȯ��
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
	void draw(char* screen, int screen_size) {//draw���� �ٷ� check range�� ���ش�.

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
			if (frame == 0)//60�������� ������
			{
				move(-1);//�������ְ�
				wait();//�ٽ� frame���� �ʱ�ȭ
				return;
			}
			frame--;
			
			
		}
			// 0.5�ʿ� �� ĭ �� �̵�


    }

	
	void OnHit() {
		hp--;

	}
	void wait() {//60�����Ӵ� 1�� ������
		frame = 60;
	
	}
	
};

class Bullet {

	char shape = '-';

public:
	bool fire = false;
	int pos = -1;
	void first_move(int player_pos) {//fire�� on ���ְ� �ʱ� pos���� �ش�.
		pos = player_pos; 
		fire = true;
	}
	void move(int inc) { pos += inc; }
	int getPosition() { return pos; }
	void checkRange(int screen_size)
	{
		
		if (pos >= screen_size) { pos = -1; }
	}
	void draw(char* screen,int screen_size) {//draw���� �ٷ� check range�� ���ش�.
		
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
	//���� ���� �ð�üũ
	clock_t oldtime = clock();//������ ó�� ����
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

				for (int i = 0; i < 10; i++) {//�迭�� Ȯ���ؼ� ����ִ� bullet ã�´�.
					if (pBullet[i].fire == false) {
						
						pBullet[i].first_move(player.getPosition());

					}
					break;
				}

			}


		}
		player.draw(screen, screen_size);//screen�� ���� �޾Ƽ� �׸�

		for (int i = 0; i < 5; i++) {//enemy draw
			enemy[i].draw(screen, screen_size);

		}
		
		
		
		for (int i = 0; i < 10; i++) {//�����̴� bullet üũ
			if (pBullet[i].fire == false) {
				break;

			}
			else if (pBullet[i].fire == true) {
				pBullet[i].draw(screen,screen_size);
				


			}
		}
		
		for (int i = 0; i < 10; i++) {
			if (pBullet[i].fire) {//bullet�� fire ���� ��

				for (int j = 0; j < 5; j++) {

					if (enemy[j].active_enemy == 1) {//Ȱ������ enemy ã�Ƽ�


						if (pBullet[i].getPosition() == enemy[j].getPosition()) {//�Ѿ��� enemy�� ������


							enemy[j].OnHit();
							pBullet[i].pos = -1;

							pBullet[i].fire = false;//����� ���� �Ѿ� �ʱ�ȭ ������
							break;

						}



						else {


							pBullet[i].update(&player, &enemy[j]);//Ŭ���� ������ ũ�Ⱑ ũ�ϱ� ���۷����� �Ѱ���


							if (player.getPosition() < enemy[j].getPosition()) { pBullet[i].move(1); }
							else { pBullet[i].move(-1); }


						}

					}

				}
			}
		}
		if ((player.getPosition() + 5) >= enemy->getPosition()) {//player�� enemy���� �浹üũ
			player.on_attacked();
		
		}

		player.update();//player�� damage�� ������Ʈ��
		

		for (int k = 0; k < 5; k++) {

			enemy[k].update(&player);

		}

		if (player.player_hp == 0) {//player�� ������ ���ϸ� ������ �����Ѵ�.
			break;
		}
		screen[screen_size] = '\r';
		screen[screen_size + 1] = 0;
		printf_s("%s", screen);

		while (1) {//�ʴ� 30�����
		
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
	//���� ��� print
	printf("player�� �׾����ϴ�. ���� ���� ����:%d, �ɸ��ð�: %d ",player.died,game_time);
	return 0;
}
