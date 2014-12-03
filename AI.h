#ifndef OOP_AI
#define OOP_AI
#include "building.h"
#include "vaccine.h"
class AI{
private:
	Building* building_list[B_NUMBER];
	Vaccine* vc;
	VirusStatus* vs;
	bool infected_building[B_NUMBER];		//������ ������. ���� true�̸� ������ ���̴�.
	bool uninfected_building[B_NUMBER];		//�������� ���� ������. ���� true�̸� ������ ���̴�.
	bool dead_building[B_NUMBER];			//���� ������. ���� true�̸� ������ ���̴�.
	bool blocked_building[2][B_NUMBER];		//�� ������ ���������� �ſ� ���ϸ�, �� ������ Blocked ó���ȴ�. ó���� On/Off �� ���� ��찡 �ִ�.
											//���� On�� Blocked ó���Ǹ�, �� �������δ� Online�� ���� ������ �ȵǰ� 
											//�� �������� �ٸ� ������ �������� �ʴ´�. ���� true�̸� block�� ���̴�.

	double vac_max;							//vaccine�� max��
	double vac_point;						//vaccine�� ���� ��������
	double developing_speed;				//vaccine�� ���߼ӵ�
	int remain_time;						//vaccine�� ���ߵǱ���������ð� 
	double sum_total_pc;					//��� ������ pc������ ���Ѱ�
	double sum_infected_pc;					//��� ������ ������ pc������ ���Ѱ�
	double sum_uninfected_pc;				//��� ������ �������� ���� pc������ ���Ѱ�
	double sum_dead_pc;						//��� ������ ���� pc������ ���Ѱ�
	double sum_live_pc;						//��� ������ ����ִ� pc������ ���Ѱ�
	double vaccine_developing;				//����� ���ߵǴ� ����. �� ���ڸ� �Ѿ�� ����� �����ϱ� �����Ѵ�.
	double online_block[B_NUMBER];			//�� ������ online���� ������ �ִ��Ѱ�ġ. �� ���� �Ѿ�� �� �������δ� online�� ���� �̵��� ��������.
											//��, �� ���� Ŭ ���� ������ ���̷����� �ұ����� ��ó�� �Ѵ�.
	double online_blocking_control[4];		//online���� ������ ������ ������� ����
	double offline_block[B_NUMBER];			//�� ������ offline���� ������ �ִ��Ѱ�ġ. �� ���� �Ѿ�� �� �������δ� offline�� ���� �̵��� ��������.
											//��, �� ���� Ŭ ���� ������ ���̷����� �ұ����� ��ó�� �Ѵ�.
	double offline_blocking_control[4];		//offline���� ������ ������ ������� ����
public:
	AI(Building** _building_list){
		for(int i =0; i<B_NUMBER; i++){
			building_list[i] = _building_list[i];
			infected_building[i] = false;
			uninfected_building[i] = false;
			dead_building[i] = false;
			blocked_building[0][i] = false;
			blocked_building[1][i] = false;
		}
		
		sum_total_pc = 0;					//��� ������ pc������ ���Ѱ�
		sum_infected_pc = 0;				//��� ������ ������ pc������ ���Ѱ�
		sum_uninfected_pc = 0;				//��� ������ �������� ���� pc������ ���Ѱ�
		sum_dead_pc = 0;					//��� ������ ���� pc������ ���Ѱ�
		sum_live_pc = 0;					//��� ������ ����ִ� pc������ ���Ѱ�
											//�Ʒ��� blocking ���� ����.
		online_blocking_control[0] = 10;
		online_blocking_control[1] = 7;
		online_blocking_control[2] = 2;
		online_blocking_control[3] = 0;
		offline_blocking_control[0] = 10;
		offline_blocking_control[1] = 2;
		offline_blocking_control[2] = 7;
		offline_blocking_control[3] = 0;
	}
															//get method��

												
	void getVM_from_VirusStatus(){vs->getVacMax(vac_max);}			//virusstatus�κ��� vaccine�� max���� �����´�.
	void getVP_DS_from_Vaccine(){vc->getVaccine(vac_point,developing_speed);}			//vaccine���κ��� ���� vaccine���� �����´�.
	void getVM(double& vm){vm = vac_max;}
	void getVP(double& vp){vp = vac_point;}
	void getSum_total_pc(double& pc);
	void getSum_infected_pc(double& pc);
	void getSum_uninfected_pc(double& pc);
	void getSum_dead_pc(double& pc);
	void getSum_live_pc(double& pc);

															//������ method��
	void do_operation();					//�� �Ͽ� AI�� ���� �� �������� ������ ������ �������ִ� �۾��� �Ѵ�.
	void calVaccineUpgrade();				//����� ���ߵǷ��� �󸶳� �ɸ����� ����Ѵ�.
	void block_checking(Building* b);		//�� ������ ���� �� ���� ������ ���̷����� �ɰ����� ����� �ٸ� �������� ���� ������ control�Ѵ�.
	void initialize_pc();					//���� ���� ���� pc������ ���� �ʱ�ȭ�Ѵ�.
	void gather_info();						//��� building�� ������ ������.
	void setEach_Building_Information();	//�� �������� ������ ���� ���� �� �ı�����, block���θ� �������ش�.
	void building_called(int index, char* _name, double& _uninfected, double& _infected, double& _dead, bool& on_block, bool& off_block);
											//Engine���� Ư�� ������ ������ call�� ��� ����� ���� �����ϴ� method.
	void statistic_called(int& _total, int& _uninfected, int& _live, double& vm, double& vp, int& rt, bool* inf_b, bool* uninf_b, bool* de_b);
	void AI_calle();
											//��ü ������ ����ϱ� ���� �����ϴ� method.
};


#endif
