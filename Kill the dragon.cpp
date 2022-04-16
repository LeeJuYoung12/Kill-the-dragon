#define CRT_SECURE_NO_WARNINGS
#include<bangtal.h>
#include <iostream>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>


#define character_size_x 35 
#define character_size_y 60 
#define obj1_size_width 25 
#define obj1_size_hight 30
#define random_height 1500

SceneID scene1, scene2, scene3, scene4;//scene1 은 시작화면, scene2 은 인게임화면, scene3 은 엔딩화면, scene1 은 데스화면 
ObjectID start, end, restart, ch1, obj[6][10], gomain1, gomain2, heart, effect[9], goexplain, explain;
SoundID start_bgm, ingame_bgm, attack_bgm[7], death_bgm, ending_bgm, animation1_bgm, animation2_bgm[2], animation3_bgm[3]; //animation_bgm 들은 검격 애니메이션을 위한 것
TimerID timer1_t, movec_t, speed_t, sword_t[9], finish_t;

int speed = 8, random;
int cx = 500, cy = 50; //캐릭터에 대한 x, y좌표
int dx = 0;
int obstaclex = 21, swordx = 40;
int temporary;
int count1 = 0; //count1은 떨어지는 장애물들 개수 새는 용

bool check[7] = { 1,1,1,1,1,1,1 };
bool dropcheck[7] = { 1,1,1,1,1,1,1}; //dropcheck는 칼 떨어지는 것 버그 방지용
int soundendcheck = 0;
const char* name_bgm[7] = { "bgm/공격1.mp3", "bgm/공격2.mp3", "bgm/공격3.mp3", "bgm/공격4.mp3", "bgm/공격5.mp3", "bgm/공격6.mp3", "bgm/공격7.mp3" };


int objx[6][10] = { 0 }, objy[6][10]={0};
double objspeed = 5;
double objmoveblockx[6][10], objmoveblocky[6][10];

ObjectID obj1_1[8]; 
int swordnum=0 , obj1_x[15], obj1_y[15];
int obj1_speed = 5;
double obj1_moveblockx[15], obj1_moveblocky[15];

//show 포함 x 물체정의함수
ObjectID create1(const char* image, SceneID scene, int x, int y) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);

	return object;
}

//show 포함 물체정의함수
ObjectID create2(const char* image, SceneID scene, int x, int y) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	showObject(object);

	return object;
}

//처음 배치 초기화
void shuffle() {
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 10; i++) {
			objx[j][i] = { 0 }, objy[j][i] = { 0 };
		}
	}

	for (int i = 0; i < 15; i++) {
		obj1_x[i] = { 0 }, obj1_y[i] = { 0 };
	}

	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 10; i++) {
			random = rand() % random_height;
			objy[j][i] = 721 + random;
			objx[j][i] =  obstaclex;
			obstaclex += 21;
		}
	}

	for (int i = 0; i < 15; i++) {
		obj1_y[i] = 721;
		obj1_x[i] = swordx;
		swordx += 40;
	}

}

//장애물의 적당한 속도와 위치를 찾다보니 식이 복잡해졌습니다. 함수이름 뒤에 other을 붙인 것은 검 이동을 위한 함수
void  movetempo() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			 if (objx[j][i] >= 0 && objy[j][i] <= 720 && objx[j][i] <= 1280 && objy[j][i] >= 0) {
			}
			else {
				int num = rand();
				int k = (num % 20) * 30;
				int rndvaluey = (num % 10) * 30;
				double distance;
				double ratio;
				
				distance = sqrt((k + 250 - objx[j][i]) * (k + 250 - objx[j][i]) + (rndvaluey + 200 - objy[j][i]) * (rndvaluey + 200 - objy[j][i]));
				ratio = distance / objspeed;

				objmoveblocky[j][i] = (rndvaluey + 200 - objy[j][i]) / ratio;
			}
		}
	}
}

void movetempo_other(int counting) {
	

		if (obj1_x[counting] >= 0 && obj1_y[counting] <= 720 && obj1_x[counting] <= 1280 && obj1_y[counting] >= 0) {
		}
		else {

			int num = rand();
			int k = (num % 10) * 60;
			int rndvaluey = (num % 5) * 60;
			double distance;
			double ratio;

			distance = sqrt((k + 250 - obj1_x[counting]) * (k + 250 - obj1_x[counting]) + (rndvaluey + 200 - obj1_y[counting]) * (rndvaluey + 200 - obj1_y[counting]));
			ratio = distance / obj1_speed;

			obj1_moveblocky[counting] = (rndvaluey + 200 - obj1_y[counting]) / ratio;

		}
}



void move() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			objy[j][i] += objmoveblocky[j][i];
						
			if (objy[j][i] <= 50) {
				random = rand() % random_height;
				objy[j][i] = 721 + random;
				count1++;

			}
		}
	}
}

void move_other(int counting) {

		obj1_y[counting] += obj1_moveblocky[counting];

		if (obj1_y[counting] <= 50) {
			obj1_y[counting] = 721;
		}
}

void locate() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {

 			locateObject(obj[j][i], scene2, objx[j][i], objy[j][i]);
		}
	}
}

void locate_other(int counting) {

		locateObject(obj1_1[counting], scene2, obj1_x[counting], obj1_y[counting]);
}



void shufflex() {
	int randomnumber, temp;
	for (int i = 0; i < 15; i++) {
		randomnumber = rand() % 15;

		temp = obj1_x[i];
		obj1_x[i] = obj1_x[randomnumber];
		obj1_x[randomnumber] = temp;
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {


			int randomnumber1 = rand() % 6;
			int randomnumber2 = rand() % 10;


			temp = objx[i][j];
			objx[i][j] = objx[randomnumber1][randomnumber2];
			objx[randomnumber1][randomnumber2] = temp;
		}
	}
}

void death() {

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			if ((cx < (objx[j][i] + obj1_size_width)) && (cx + character_size_x > objx[j][i]) && (cy < (objy[j][i] + obj1_size_hight)) && (cy + character_size_y > objy[j][i])) {

				stopTimer(timer1_t);
				stopTimer(movec_t);
				stopTimer(speed_t);
				setObjectImage(heart, "image/heart1.png");
				for (int q = 0; q < 9; q++) {
					stopTimer(sword_t[q]);
					setTimer(sword_t[q], 0.01f);
					hideObject(effect[q]);
				}

				for (int w = 0; w < 7; w++) {
					stopSound(attack_bgm[w]);
				}
				count1 = 0;
				swordnum = 0;
				swordx = 40;
				enterScene(scene4);
				objspeed = 5;
				obj1_speed = 5;
				obstaclex = 21;

				cx = 500;
				cy = 50;
				soundendcheck = 0;
				for (int k = 0; k < 7; k++) {
					check[k] = true;
					dropcheck[k] = true;
				}

				shuffle();
				shufflex();
				locateObject(ch1, scene2, cx, cy);
				stopSound(ingame_bgm);
				playSound(death_bgm);
				for (int w = 0; w < 8; w++) {
					locateObject(obj1_1[w], scene2, obj1_x[w], obj1_y[w]);
				}
			}

		}
	}

}

void finish() {
	if (swordnum >= 7 && soundendcheck>=7) {
		stopSound(ingame_bgm);
		stopTimer(timer1_t);
		stopTimer(movec_t);
		stopTimer(speed_t);
		setObjectImage(heart, "image/heart1.png");
		for (int q = 0; q < 9; q++) {
			stopTimer(sword_t[q]);
			setTimer(sword_t[q], 0.01f);
			hideObject(effect[q]);
		}
		count1 = 0;
		swordx = 40;
		swordnum = 0;

		enterScene(scene3);
	
		objspeed = 5;
		obj1_speed = 5;
		obstaclex = 21;

		cx = 500;
		cy = 50;
		soundendcheck = 0;
		for (int k = 0; k < 7; k++) {
			check[k] = true;
			dropcheck[k] = true;
		}

		shuffle();
		shufflex();
		locateObject(ch1, scene2, cx, cy);
		for (int w = 0; w < 8; w++) {
			locateObject(obj1_1[w], scene2, obj1_x[w], obj1_y[w]);
		}
		playSound(ending_bgm);
	}
}

//검 획득
void getsword() {

		if ((cx < (obj1_x[1] + 10)) && (cx + character_size_x > obj1_x[1]) && (cy < (obj1_y[1] + 10)) && (cy + character_size_y > obj1_y[1])&&check[0]) {
			
			playSound(attack_bgm[0]);
			locateObject(obj1_1[1], scene2, obj1_x[1], 721);
			stopTimer(sword_t[1]);
			check[0] = false;
			swordnum++;
			showObject(effect[0]);
		}

		if ((cx < (obj1_x[2] + 10)) && (cx + character_size_x > obj1_x[2]) && (cy < (obj1_y[2] + 10)) && (cy + character_size_y > obj1_y[2]) && check[1]) {
			
			playSound(attack_bgm[1]);
			locateObject(obj1_1[2], scene2, obj1_x[2], 721);
			stopTimer(sword_t[2]);
			check[1] = false;
			swordnum++;
			showObject(effect[1]);
		}

		if ((cx < (obj1_x[3] + 10)) && (cx + character_size_x > obj1_x[3]) && (cy < (obj1_y[3] + 10)) && (cy + character_size_y > obj1_y[3]) && check[2]) {
			
			playSound(attack_bgm[2]);
			locateObject(obj1_1[3], scene2, obj1_x[3], 721);
			stopTimer(sword_t[3]);
			check[2] = false;
			swordnum++;
			showObject(effect[2]);
		}

		if ((cx < (obj1_x[4] + 10)) && (cx + character_size_x > obj1_x[4]) && (cy < (obj1_y[4] + 10)) && (cy + character_size_y > obj1_y[4]) && check[3]) {
			
			playSound(attack_bgm[3]);
			locateObject(obj1_1[4], scene2, obj1_x[4], 721);
			stopTimer(sword_t[4]);
			check[3] = false;
			swordnum++;
			playSound(animation1_bgm);
		}

		if ((cx < (obj1_x[5] + 10)) && (cx + character_size_x > obj1_x[5]) && (cy < (obj1_y[5] + 10)) && (cy + character_size_y > obj1_y[5]) && check[4]) {
			
			playSound(attack_bgm[4]);
			locateObject(obj1_1[5], scene2, obj1_x[5], 721);
			stopTimer(sword_t[5]);
			check[4] = false;
			swordnum++;
			playSound(animation2_bgm[0]);
		}

		if ((cx < (obj1_x[6] + 10)) && (cx + character_size_x > obj1_x[6]) && (cy < (obj1_y[6] + 10)) && (cy + character_size_y > obj1_y[6]) && check[5]) {
			
			playSound(attack_bgm[5]);
			locateObject(obj1_1[6], scene2, obj1_x[6], 721);
			stopTimer(sword_t[6]);
			check[5] = false;
			swordnum++;
			showObject(effect[0]);
		}

		if ((cx < (obj1_x[7] + 10)) && (cx + character_size_x > obj1_x[7]) && (cy < (obj1_y[7] + 10)) && (cy + character_size_y > obj1_y[7]) && check[6]) {
			
			playSound(attack_bgm[6]);
			locateObject(obj1_1[7], scene2, obj1_x[7], 721);
			stopTimer(sword_t[7]);
			check[6] = false;
			swordnum++;
			playSound(animation3_bgm[0]);
		}

		//드래곤 체력 변경
		if (swordnum == 1) {
			setObjectImage(heart, "image/heart2.png");
		}
		else if (swordnum == 2) {
			setObjectImage(heart, "image/heart3.png");
		}
		else if (swordnum == 3) {
			setObjectImage(heart, "image/heart4.png");
		}
		else if (swordnum == 4) {
			setObjectImage(heart, "image/heart5.png");
		}
		else if (swordnum == 5) {
			setObjectImage(heart, "image/heart6.png");
		}
		else if (swordnum == 6) {
			setObjectImage(heart, "image/heart7.png");
		}
		else if (swordnum == 7) {
			setObjectImage(heart, "image/heart8.png");
		}
}
// object들의 움직임, 검, 데스 등 게임관련 상황 업데이트
void update() {
	movetempo();
	move();
	locate();
	death();
	getsword();
	finish();
}

void soundCallback(SoundID sound) {
	if (sound == start_bgm) {
		playSound(start_bgm);
	}
	if (sound == ingame_bgm) {
		playSound(ingame_bgm);
	}

	if (sound == attack_bgm[0]) {
		soundendcheck++;
		hideObject(effect[0]);
	}

	if (sound == attack_bgm[1]) {
		soundendcheck++;
		hideObject(effect[1]);
	}

	if (sound == attack_bgm[2]) {
		soundendcheck++;
		hideObject(effect[2]);
	}

	if (sound == attack_bgm[3]) {
		soundendcheck++;
		hideObject(effect[3]);
	}

	if (sound == attack_bgm[4]) {
		soundendcheck++;
		hideObject(effect[4]);
		hideObject(effect[5]);
	}

	if (sound == attack_bgm[5]) {
		soundendcheck++;
		hideObject(effect[0]);
	}

	if (sound == attack_bgm[6]) {
		soundendcheck++;
		hideObject(effect[6]);
		hideObject(effect[7]);
		hideObject(effect[8]);
	}
	if (sound == ending_bgm) {
		playSound(ending_bgm);
	}
	if (sound == death_bgm) {
		playSound(death_bgm);
	}
	
	//첫 번째 애니메이션
	if (sound == animation1_bgm) {
		showObject(effect[3]);
	}

	//두 번째 애니메이션
	if (sound == animation2_bgm[0]) {
		showObject(effect[4]);
		playSound(animation2_bgm[1]);
	}

	if (sound == animation2_bgm[1]) {
		showObject(effect[5]);
	}

	//세 번째 애니메이션
	if (sound == animation3_bgm[0]) {
		showObject(effect[6]);
		playSound(animation3_bgm[1]);
	}

	if (sound == animation3_bgm[1]) {
		showObject(effect[7]);
		playSound(animation3_bgm[2]);
	}

	if (sound == animation3_bgm[2]) {
		showObject(effect[8]);
	}
}

void timerCallback(TimerID timer) {
	if (timer == timer1_t) {//장애물 움직이기
		setTimer(timer1_t, 0.01f);
		startTimer(timer1_t);
		update();
		
	}

	if (timer == movec_t) {//캐릭터 움직이기

		cx += dx;

		if (cx < -10 || cx > 1220 ) {
			cx -= dx;
		}

		locateObject(ch1, scene2, cx, cy);
		setTimer(movec_t, 0.01f);
		startTimer(movec_t);

	}

	if (timer == speed_t) { //장애물 속도 업
		if (count1 >= 500) {
		objspeed = 8.5;
		obj1_speed = 9;
		stopTimer(speed_t);
		}
		else if (count1>= 450) {
			objspeed = 8;
			obj1_speed= 8;
		}
		else if (count1 >= 350) {
			objspeed = 7;
			obj1_speed = 7;
		}
		else if (count1 >= 150) {
			objspeed = 6;
			obj1_speed = 6;
		}
		
		setTimer(speed_t, 0.1f);
		startTimer(speed_t);
	}

	if (timer == sword_t[0]) {//검에 대한 timer들의 start조절
		if (count1 >= 50 && dropcheck[0]) {
			startTimer(sword_t[1]);
			dropcheck[0] = false;
		}

		if (count1 >= 110 && dropcheck[1]) {
			startTimer(sword_t[2]);
			dropcheck[1] = false;
		}

		if (count1 >= 190 && dropcheck[2]) {
			startTimer(sword_t[3]);
			dropcheck[2] = false;
		}

		if (count1 >= 260 && dropcheck[3]) {
			startTimer(sword_t[4]);
			dropcheck[3] = false;
		}

		if (count1 >= 330 && dropcheck[4]) {
			startTimer(sword_t[5]);
			dropcheck[4] = false;
		}

		if (count1 >= 400 && dropcheck[5]) {
			startTimer(sword_t[6]);
			dropcheck[5] = false;
		}

		if (count1 >= 500 && dropcheck[6]) {
			startTimer(sword_t[7]);
			dropcheck[6] = false;
		}

		
		setTimer(sword_t[0], 0.01f);
		startTimer(sword_t[0]);
		
	}

	if (timer == sword_t[1]) {//첫번째 검 조절
		movetempo_other(1);
		move_other(1);
		locate_other(1);

		setTimer(sword_t[1], 0.01f);
		startTimer(sword_t[1]);

	}

	if (timer == sword_t[2]) {//두번째 검 조절
		movetempo_other(2);
		move_other(2);
		locate_other(2);

		setTimer(sword_t[2], 0.01f);
		startTimer(sword_t[2]);

	}

	if (timer == sword_t[3]) {//세번째 검 조절
		movetempo_other(3);
		move_other(3);
		locate_other(3);

		setTimer(sword_t[3], 0.01f);
		startTimer(sword_t[3]);

	}

	if (timer == sword_t[4]) {//네번째 검 조절
		movetempo_other(4);
		move_other(4);
		locate_other(4);

		setTimer(sword_t[4], 0.01f);
		startTimer(sword_t[4]);

	}

	if (timer == sword_t[5]) {//다섯번째 검 조절
		movetempo_other(5);
		move_other(5);
		locate_other(5);

		setTimer(sword_t[5], 0.01f);
		startTimer(sword_t[5]);

	}

	if (timer == sword_t[6]) {//여섯번째 검 조절
		movetempo_other(6);
		move_other(6);
		locate_other(6);

		setTimer(sword_t[6], 0.01f);
		startTimer(sword_t[6]);

	}

	if (timer == sword_t[7]) {//일곱번째 검 조절
		movetempo_other(7);
		move_other(7);
		locate_other(7);

		setTimer(sword_t[7], 0.01f);
		startTimer(sword_t[7]);

	}

}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == start) {

		swordnum = 0;
		locateObject(ch1, scene2, cx, cy);
		startTimer(movec_t);
		startTimer(timer1_t);
		startTimer(speed_t);
		startTimer(sword_t[0]);
		stopSound(start_bgm);
		playSound(ingame_bgm);
		enterScene(scene2);
	}
	
	else if (object == end) {
		endGame();
	}
	
	else if (object == gomain1) {
		enterScene(scene1);
		stopSound(ending_bgm);
		playSound(start_bgm);
	}
	else if (object == gomain2) {
		enterScene(scene1);
		stopSound(death_bgm);
		playSound(start_bgm);
	}
	else if (object == goexplain) {
		showObject(explain);
	}
	else if (object == explain) {
		hideObject(explain);
	}

}

void keyboardCallback(KeyCode code, KeyState state) {



	if (code == KeyCode::KEY_RIGHT_ARROW) {		// RIGHT
		dx += (state == KeyState::KEY_PRESSED ? speed : -speed);
		setObjectImage(ch1, "image/player1.png");
	}
	else if (code == KeyCode::KEY_LEFT_ARROW) {		// LEFT
		dx -= (state == KeyState::KEY_PRESSED ? speed : -speed);
		setObjectImage(ch1, "image/player2.png");
	}
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setKeyboardCallback(keyboardCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);
	
	scene1 = createScene("", "image/back1.png");
	scene2 = createScene("", "image/mainback.png");
	scene3 = createScene("", "image/clearback.png");
	scene4 = createScene("", "image/death.png");

	shuffle();
	shufflex();

	start = create2("image/start.png", scene1, 440, 250);
	end = create2("image/end.png", scene1, 440, 130);
	goexplain = create2("image/goexplain.png", scene1, 5, 5);
	explain = create1("image/explain.png", scene1, 370, 20);
	ch1 = create2("image/player1.png", scene2, cx, cy);
	heart = create2("image/heart1.png", scene2, 400, 650);
	gomain1 = create2("image/gomain.png", scene3, 900, 50);
	gomain2 = create2("image/gomain.png", scene4, 900, 50);	

	effect[0] = create1("image/1.png", scene2, 460, 310);
	effect[1] = create1("image/2.png", scene2, 480, 300);
	effect[2] = create1("image/3.png", scene2, 480, 300);
	effect[3] = create1("image/4.png", scene2, 480, 300);
	effect[4] = create1("image/5.png", scene2, 467, 300);
	effect[5] = create1("image/6.png", scene2, 483, 300);
	effect[6] = create1("image/7.png", scene2, 480, 300);
	effect[7] = create1("image/8.png", scene2, 480, 300);
	effect[8] = create1("image/9.png", scene2, 485, 300);

	scaleObject(start, 0.8f);
	scaleObject(end, 0.8f);
	scaleObject(ch1, 0.8f);
	scaleObject(heart, 0.6f);
	scaleObject(goexplain, 0.55f);
	scaleObject(explain, 1.7f);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			obj[j][i] = create2("image/fire1.png", scene2, objx[j][i], objy[j][i]);
			scaleObject(obj[j][i], 0.35f);
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 2; j <=3; j++) {
			setObjectImage(obj[j][i], "image/fire2.png");
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 4; j <= 5; j++) {
			setObjectImage(obj[j][i], "image/fire3.png");
		}
	}

	for (int i = 0; i < 8; i++) {
		obj1_1[i] = create2("image/sword.png", scene2, obj1_x[i], obj1_y[i]);
		scaleObject(obj1_1[i], 0.35f);
	}

	timer1_t = createTimer(0.01f);
	movec_t = createTimer(0.01f);
	speed_t = createTimer(0.01f);


	for (int i = 0; i < 9; i++) {
		sword_t[i] = createTimer(0.01f);
	}

	start_bgm = createSound("bgm/start.mp3");
	ingame_bgm = createSound("bgm/ingame.mp3");
	ending_bgm = createSound("bgm/ending.mp3");
	death_bgm = createSound("bgm/death.mp3");

	animation1_bgm = createSound("bgm/delay3.mp3");

	animation2_bgm[0] = createSound("bgm/delay1.mp3");
	animation2_bgm[1] = createSound("bgm/delay2.mp3");

	animation3_bgm[0] = createSound("bgm/delay3.mp3");
	animation3_bgm[1] = createSound("bgm/delay2.mp3");
	animation3_bgm[2] = createSound("bgm/delay2.mp3");

	for (int i = 0; i < 7; i++) {
		attack_bgm[i] = createSound(name_bgm[i]);
	}

	playSound(start_bgm);
	startGame(scene1);
}