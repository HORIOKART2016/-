#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <ypspur.h>

#ifdef __WIN32
#	include <windows.h>
#endif

#define TREAD 590
#define PI 3.14159265359
#define TIRE 290

void main(void){

	char key=0x20;
	
	double vel=0.2;

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
	

	Spur_set_vel(0.2);
	Spur_set_accel(1.0);
	Spur_set_angvel(M_PI / 2.0);
	Spur_set_angaccel(M_PI / 2.0);

	YP_set_wheel_vel(M_PI / 2.0);
	YP_set_wheel_accel(M_PI / 2.0);

	Spur_set_pos_GL(0, 0, 0);


	while (true){
		key = getchar();
		switch (key){
		case ' ':	//�X�y�[�X�Œ�~
			Spur_stop();
			break;
		case 0x48:  //���őO�i
			yp_wheel_vel(vel, vel);
			break;
		case 0x50:	//���Ō��
			yp_wheel_vel(-vel, -vel);
			break;
		case 0x4b:	//���ō���]
			yp_wheel_ang(-TREAD*(ang / 360) / TIRE, TREAD*(ang / 360) / TIRE);
			break;
		case 0x4d: //���ŉE��]
			yp_wheel_ang(TREAD*(ang / 360) / TIRE, -TREAD*(ang / 360) / TIRE);
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