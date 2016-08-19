#pragma comment(lib,"C:\ypspur\lib\libypspur-md_dll.lib")

#include "stdafx.h"

//#include <stdio.h>
//#include <unistd.h>
#include <math.h>
#include <ypspur.h>

#ifdef __WIN32
#	include <windows.h>
#endif

#define TREAD 590
#define PI 3.14159265359
#define TIRE 290

int main(void){

	char key = 0x20;

	double vel = 0.2;

	double ang = 90;

	double x, y, theta;

	// Windows環境で標準出力がバッファリングされないように設定
	setvbuf(stdout, 0, _IONBF, 0);

	// 初期化
	if (Spur_init() < 0)
	{
		fprintf(stderr, "ERROR : cannot open spur.\n");
		return -1;
	}


	Spur_set_vel(0.2);
	Spur_set_accel(1.0);
	Spur_set_angvel(PI / 2.0);
	Spur_set_angaccel(PI / 2.0);

	YP_set_wheel_vel(PI / 2.0, PI / 2.0);
	YP_set_wheel_accel(PI / 2.0,PI / 2.0);

	Spur_set_pos_GL(0, 0, 0);


	while (true){
		key = getchar();
		switch (key){
		case ' ':	//スペースで停止
			Spur_stop();
			break;
		case 0x48:  //↑で前進
			YP_wheel_vel(vel, vel);
			break;
		case 0x50:	//↓で後退
			YP_wheel_vel(-vel, -vel);
			break;
		case 0x4b:	//←で左回転
			YP_wheel_ang(-TREAD*(ang / 360) / TIRE, TREAD*(ang / 360) / TIRE);
			break;
		case 0x4d: //→で右回転
			YP_wheel_ang(TREAD*(ang / 360) / TIRE, -TREAD*(ang / 360) / TIRE);
			break;
		case 'v':	//速度変更
			printf("\033[2J");
			printf("速度変更\n");
			scanf("%f", &vel);
			break;
		case 'a':	//回転角度変更
			printf("\033[2J");
			printf("回転角度変更\n");
			scanf("%f", &ang);
			break;
		case 'o':	//原点リセット
			Spur_set_pos_GL(0, 0, 0);
			break;
		}

		Spur_get_pos_GL(&x, &y, &theta);

		printf("\033[2J");
		printf("矢印キーで方向制御\n");
		printf("スペースで停止\n");
		printf("v:速度変更\n");
		printf("a:回転角度変更\n");
		printf("o:原点リセット\n\n\n");
		printf("現在位置\n");
		printf("%f,%f,%f\n", x, y, theta);


	}

}