#ifndef OOP_Building
#define OOP_Building
#include "virusStatus.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define B_NUMBER 20

class VirusStatus;
class Engine;
class Vaccine;
class AI;

struct Building_Information{
	Building_Information();
	Building_Information(int _total_pc, int _building_index) 
		: total_pc(_total_pc), building_index(_building_index)
	{
		uninfected_pc = total_pc;
		infected_pc = 0;
		dead_pc = 0;
		live_pc = total_pc;
	}
	int total_pc;			//�� pc��
	int uninfected_pc;		//���� ���� ���� pc��->total_pc-infected_pc�� ������Ʈ
	int infected_pc;		//���� pc��
	int dead_pc;			//�۵� �Ұ����� pc��
	int live_pc;			//�۵� ������ pc��
	int building_index;		//�� ��°�� �Ҵ�� building���� �˷��ִ� ����
};

class Building{
private:
	double Activity[4];						//On,Off-line Ȱ�� ���� 4���. ù ��°�� ������ �ſ� �ߵ�. �� ��°�� Online�� ���� ������ �ߵ�.
											//�� ��°�� Offline�� ���� ������ �ߵ�. �� ��°�� ������ �� �ȵ�.
	double Security[3];						//�����ǽ� �� �ܰ� ������. �� 3�����̸�, ù ��°�� �ǽ��� ���� ���� �������� �ǽ��� ���� ����.
											//�� �ܰ��� ���� �����ϸ�, ���� �ܰ��� ���� Ŭ���� pc���� �� ������.
	double Research_Ability;				//��ſ� �⿩�� �� �ִ� ����
	double Online_Use;						//ó������ 1������, �� ���� AI�� ���� �������� online�� ���� ���� ���� �������� ��������.
	double Offline_Use;						//ó������ 1������, �� ���� AI�� ���� �������� offline�� ���� ���� ���� �������� ��������.
	char name[21];							//building�� �̸�
	VirusStatus* vs;						//���̷���status ������
	double interact[2][B_NUMBER];			//building���� ��ȣ�ۿ��ϴ� pc���� control�� ����. ù ���� Online����̸�,
											//�ι�° ���� Offline����̴�. �� ���� 0�̵Ǹ�, �� �������� �̵��� �ȵȴ�.
	double infect_percent;					//������ percentage.
	double death_percent;					//�۵� �Ұ��� percentage
	int infect_level;						//���� ������ ��������
	int death_level;						//���� ������ ��������
	int coin;								//0�̸� ����x. coin����ŭ ���� �������� ��.
	Building* building_list[B_NUMBER];		//�ٸ� �������� ����Ű�� ������
	double Contribute_to_Vaccine;			//��ſ� ����ؼ� �����ִ� ��. �� �⿩���� ũ�� ��� ������ ������.
	Building_Information building_info;		//������ ����
public:
	Building(char* n, int index, int totalpc, double* act, double* sec, double RA, VirusStatus* _vs, double _interact[2][B_NUMBER])
		: building_info(totalpc,index)
	{
		for(int i=0; i<4; i++) Activity[i] = act[i];
		for(int i=0; i<3; i++) Security[i] = sec[i];
		Research_Ability = RA;
		Online_Use = 1;
		Offline_Use = 1;
		strcpy(name,n);
		vs = _vs;
		for(int i=0; i<B_NUMBER; i++) {interact[0][i] = _interact[0][i]; interact[1][i] = _interact[1][i];}
		infect_percent = 0;
		death_percent = 0;
		infect_level = 0;
		death_level = 0;
		coin = 0;
	}

													//get,set��	
	void getActivity(double* act){act = Activity;}
	void setActivity(int index, double act){Activity[index] = act;}

	void setBuildingList(Building** b){for(int i =0; i<B_NUMBER; i++) building_list[i] = b[i];}

	void getDeathPercent(double& dp){dp = death_percent;}

	void getIndex(int& id){id = building_info.building_index;}
	void getInfectPercent(double& ip){ip = infect_percent;}
	void getInformation(Building_Information* info);
	void getInteract(double** it){for(int i =0; i<2; i++) it[i] = interact[i];}
	void setInfectedpc(double ipc);
	void getInfectedpc(double& ipc);

	void setLivepc(double pc);
	void getLivepc(double& pc);

	void getName(char* _name){strcpy(_name,name);}

	void setOnlinUse(double use){Online_Use = use;}
	void getOnlineUse(double& use){use = Online_Use;}
	void setOfflineUse(double use){Offline_Use = use;}
	void getOfflineUse(double& use){use = Offline_Use;}

	void setSecurity(int index, double sec){Security[index] = sec;}
	void getSecurity(double* sec){sec = Security;}
	
	void setTotalpc(double pc);
	void getTotalpc(double& pc);
	
	
	

													//�Ʒ����� �ܺο��� ȣ�� �� method��
	void startedBuilding();
	//������ ���۵Ǵ� ����.
	void do_operation();
	//�� �Ͽ� ���� �����Լ����� ��� �������ش�. Engine���� call		
	void iscoinExist(int& coin_flag);
	//�������� ������ ����������� Ȯ���� �Լ�
	void getC2V(double& p);
	//�⿩��(p)�� ũ�� ��� ������ ������


													//�Ʒ����� �������ο��� ������ method��
	void infected_to_death();
	//������ pc�� �Ϻθ� ���δ�.	
	void virus_spread_in_a_building();
	//���� ���ο��� ���̷����� ������ ����. infected_pc���� dead_pc���� ����.
	void info_update();
	//������ ���̷����� ������ �� ������ ��ȭ�� update���ش�.
	void virus_spread_through_all_buildings();
	//�� �������� �ٸ� �������� ���� pc�̵��Լ�
};

#endif
