
#define _CRT_SECURE_NO_WARNINGS

#include <ypspur.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>

#define PI 3.14159265359

#define _CRT_SECURE_NO_WARNINGS

//�����Ƀg���N�L�^���Ă݂�
FILE *trq;
const char *torq_record = "TorqRecord.csv";

//�����Ɏ��Ԍv�����Ă݂�
LARGE_INTEGER freq;
LARGE_INTEGER start, now;

#define MAX_VEL 3500		//�ō����x�ݒ�@[m/h]
#define MAX_ACC 2000		//�����x�̐ݒ�  [m/h.s]

#define TIRE_R 0.2900   //�^�C���a[m]
double vel = 2.0;			//���x�̎w��[km/h]


int initSpur(void){
	// Windows���ŕW���o�͂��o�b�t�@�����O����Ȃ��悤�ɐݒ�
	setvbuf(stdout, 0, _IONBF, 0);

	// ������
	if (Spur_init() < 0)
	{
		fprintf(stderr, "ERROR : cannot open spur.\n");
		return -1;
	}

	std::cout << "Spur initialized\n\n";

	/*Spur_set_vel(vel / 3600);		//���x0.3m/sec
	Spur_set_accel(acc / 3600);	//�����x�im/s/s�j
	Spur_set_angvel(90 * PI / 180);	//�p���x�irad/s)
	Spur_set_angaccel(180 * PI / 180);		//�p�����x�irad/s/s)
	*/

	Spur_set_vel(0.3);		//���x0.3m/sec
	Spur_set_accel(1.0);	//�����x�im/s/s�j
	Spur_set_angvel(0.5);	//�p���x�irad/s)
	Spur_set_angaccel(2.0);		//�p�����x�irad/s/s)

	YP_set_wheel_vel((1000 * MAX_VEL / 3600) / (TIRE_R / 2), (1000 * MAX_VEL / 3600) / (TIRE_R / 2));

	YP_set_wheel_accel(1.5, 1.5);


	return 0;
}


int initialize(){


	//�o�b�N�O���E���h�ŃR���g���[�����N�����Ă���
	if (system("start ../../MainSystem/HORIOKART_MainControlSystem/Debug/MS_Controller.exe")){
		std::cout << "controller open error....\n";
	}
	else{ std::cout << "cotroller Open\n\n"; }






	//YPSpur�Ƃ̒ʐM���J�n����
	if (initSpur())
	{
		return 1;
	}



	//�g���N�L�^�p�̃t�@�C���I�[�v��
	trq = fopen(torq_record, "w");
	fprintf(trq, "TimeStamp,right_torque,left_torque,x,y,th,xLC,y_LC,th_LC\n");




	return 0;



}

void RecordTorq(){

	double R_torq, L_torq, x, y, th;
	double x_LC, y_LC, th_LC;

	YP_get_wheel_torque(&R_torq, &L_torq);
	Spur_get_pos_GL(&x, &y, &th);


	QueryPerformanceCounter(&now);

	fprintf(trq, "%lf,%lf,%lf,%lf,%lf,%lf\n", (((double)now.QuadPart - (double)start.QuadPart) / (double)freq.QuadPart),R_torq, L_torq, x, y, th);

	printf("%lf,%lf,%lf\n", x, y, th);

}




void run_straight(double length){

	double x, y, th;

	Spur_set_pos_GL(0.0, 0.0, 0.0);

	Spur_line_GL(length, 0.0, 0.0);

	while (!Spur_over_line_GL(length, 0.0, 0.0)){
		RecordTorq();
		Sleep(100);
	}
	Spur_stop();
	
	Spur_get_pos_GL(&x, &y, &th);
	printf("Run end\n");
	printf("%lf,%lf,%lf\n", x, y, th);

}


void Rotation_con(int rot){

	double x, y, th;
	double r, l;
	double ang_vel;
	double count;
	ang_vel = (800 * vel / 3600) / (TIRE_R / 2);
	
	Spur_set_pos_GL(0.0, 0.0, 0.0);

	YP_wheel_vel(ang_vel, ang_vel);

	while (1){
		
		Spur_get_pos_GL(&x, &y, &th);
		count = abs(th / (PI * 2));
		if (count > ((double)rot-0.2)){
			break;
		}
		RecordTorq();
		YP_get_wheel_vel(&r, &l);
		printf("wheel vel: %lf,%lf\n", r, l);
		Sleep(100);
	}
	
	Spur_spin_GL(0.0);

	Spur_get_pos_GL(&x, &y, &th);

	while (!Spur_near_ang_GL(0.0, 0.001))
	{
		Sleep(10);

	}
	Spur_stop();
	printf("Run end\n");
	printf("%lf,%lf,%lf\n", x, y, th);
	printf("��]���F%lf\n", th / (2 * PI));



}

void kaihi(void){
	double x, y, th;
	double saki, kaihi, goal;
	int sayuu;

	printf("������ŉ�����邩�F");
	scanf("%lf", &saki);

	printf("\n�E:1 ��:2�@�F");
	scanf("%d", &sayuu);

	printf("�������ɉ�����邩");
	scanf("%lf", &kaihi);

	do{
		printf("������܂ōs����(%lfm�ȏ�)", saki);
		scanf("%lf", &goal);
	} while (goal > saki);

	Spur_set_pos_GL(0.0, 0.0, 0.0);

	Spur_line_GL(goal, 0.0, 0.0);

	while (Spur_over_line_GL(saki, 0.0, 0.0)){
		RecordTorq();
		Sleep(10);
	}
	if (sayuu == 1)
		Spur_line_GL(goal, -kaihi, 0.0);
	else if (sayuu==2)
		Spur_line_GL(goal, kaihi, 0.0);

	while (Spur_over_line_GL(goal, 0.0, 0.0)){
		RecordTorq();
		Sleep(10);
	}

	Spur_stop();

	Spur_get_pos_GL(&x, &y, &th);
	printf("Run end\n");
	printf("%lf,%lf,%lf\n", x, y, th);


}


int main(void)
{
	int ret;
	int mode;

	QueryPerformanceFrequency(&freq);

	ret = initialize();

	if (ret != 0){
		return 1;
	}

	printf("1:���i\n2:��]\n3:�������\n\n");
	scanf("%d", &mode);

	if (mode == 1){
		double length;
		printf("���s��������́F");
		scanf("%lf", &length);

		run_straight(length);
		}
	else if (mode == 2){
			int Rotation;
			printf("��]������́F");
			scanf("%d", &Rotation);

			Rotation_con(Rotation);
		}

	else if (mode == 3){
		kaihi();
	}
	
		
	


	return 0;
}

