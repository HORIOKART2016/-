#pragma comment(lib,"C:\ypspur\lib\libypspur-md_dll.lib")

#include "stdafx.h"

//#include <stdio.h>
//#include <unistd.h>
#include <math.h>
#include <ypspur.h>

#include <windows.h>


#define TREAD 590
#define PI 3.14159265359
#define TIRE 290

#define MODE 1

int main(void){

	char key = 0x20;

	double vel = 0.2;

	double ang = 90;

	double x, y, theta;

	// Windows���ŕW���o�͂��o�b�t�@�����O����Ȃ��悤�ɐݒ�
	setvbuf(stdout, 0, _IONBF, 0);

	// ������
	if (Spur_init() < 0)
	{
		fprintf(stderr, "ERROR : cannot open spur.\n");
		return -1;
	}


	Spur_set_vel(0.3);		//���x0.3m/sec
	Spur_set_accel(1.0);	//�����x�im/s/s�j
	Spur_set_angvel(1.5);	//�p���x�irad/s)
	Spur_set_angaccel(2.0);		//�p�����x�irad/s/s)

	YP_set_wheel_vel(PI / 2.0, PI / 2.0);
	YP_set_wheel_accel(PI / 2.0,PI / 2.0);

	Spur_set_pos_GL(0, 0, 0);

	if (MODE == 1){
		while (true){
			key = getchar();
			switch (key){
			case ' ':	//�X�y�[�X�Œ�~
				Spur_stop();
				break;
			case 'y':  //���őO�i
				//YP_wheel_vel(vel, vel);
				Spur_line_GL(0, 0, 0);
				break;
		/*	case 'j':	//���Ō��
				YP_wheel_vel(-vel, -vel);
				break;*/
			case 'g':	//���ō���]
				//YP_wheel_ang(-TREAD*(ang / 360) / TIRE, TREAD*(ang / 360) / TIRE);
				Spur_spin_GL(3.14 / 2);
				while (!Spur_near_ang_GL(ang*3.14 / 180, 0.1))
					Sleep(100);
				Spur_stop();
				break;
			case 'j': //���ŉE��]
				//YP_wheel_ang(TREAD*(ang / 360) / TIRE, -TREAD*(ang / 360) / TIRE);
				Spur_spin_GL(-3.14 / 2);
				while (!Spur_near_ang_GL(-ang*3.14 / 180, 0.1))
					Sleep(100);
				Spur_stop();
				break;
			case 'v':	//���x�ύX
				printf("\033[2J");
				printf("���x�ύX\n");
				scanf("%f", &vel);
				break;
			case 'a':	//��]�p�x�ύX
				printf("\033[2J");
				printf("��]�p�x�ύX\n");
				scanf("%f", &ang);
				break;
			case 'o':	//���_���Z�b�g
				Spur_set_pos_GL(0, 0, 0);
				break;
			case 'f':
				Spur_free();
				break;
			}

			Spur_get_pos_GL(&x, &y, &theta);

			printf("\033[2J");
			printf("���L�[�ŕ�������\n");
			printf("�X�y�[�X�Œ�~\n");
			printf("v:���x�ύX\n");
			printf("a:��]�p�x�ύX\n");
			printf("o:���_���Z�b�g\n\n\n");
			printf("���݈ʒu\n");
			printf("%f,%f,%f\n", x, y, theta);


		}
	}

	else {
		while (true){
			Spur_get_pos_GL(&x, &y, &theta);
			printf("���݈ʒu\n");
			printf("%f,%f,%f\n", x, y, theta);
			Sleep(1000);

		}
	}
}