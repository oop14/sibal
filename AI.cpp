#include "AI.h"
												//get��
void AI::getSum_total_pc(double& pc){
	pc = sum_total_pc;
}

void AI::getSum_infected_pc(double& pc){
	pc = sum_infected_pc;
}

void AI::getSum_uninfected_pc(double& pc){
	pc = sum_uninfected_pc;
}

void AI::getSum_dead_pc(double& pc){
	pc = sum_dead_pc;
}

void AI::getSum_live_pc(double& pc){
	pc = sum_live_pc;
}

												//get�� ��
void AI::initialize_pc(){
	sum_total_pc = 0;
	sum_infected_pc = 0;
	sum_uninfected_pc = 0;
	sum_dead_pc = 0;
	sum_live_pc = 0;
}

void AI::calVaccineUpgrade(){
	remain_time = (vac_max - vac_point) / developing_speed;
}

void AI::do_operation(){
	initialize_pc();							//������ ���� ���� �ʱ�ȭ
	gather_info();								//�� �������� ���� ����
	calVaccineUpgrade();						//��� ���߿� �����ð� ���
	setEach_Building_Information();				//�� �������� ���� ��� �� setting.
}

void AI::gather_info(){
	Building_Information temp_info;
	initialize_pc();
	for(int i=0; i<B_NUMBER; i++){
		building_list[i]->getInformation(&temp_info);
		sum_total_pc += temp_info.total_pc;	
		sum_infected_pc += temp_info.infected_pc;
		sum_uninfected_pc += temp_info.uninfected_pc;
		sum_dead_pc += temp_info.dead_pc;
		sum_live_pc += temp_info.live_pc;
	}
	getVM_from_VirusStatus();
	getVP_DS_from_Vaccine();
}

void AI::setEach_Building_Information(){
	Building_Information temp_info;	
	
	for(int i=0; i<B_NUMBER; i++){
		building_list[i]->getInformation(&temp_info);
		if(temp_info.infected_pc == 0) {uninfected_building[i] = true; infected_building[i] = false;}
		else if(temp_info.infected_pc > 0) {infected_building[i] = true; uninfected_building[i] = false;}
		if(temp_info.dead_pc == 0) dead_building[i] = false;
		else if(temp_info.dead_pc > 0) dead_building[i] = true;
												//�����ȵ� building��� ������ building��� ���� building�� ó���Ϸ�
		block_checking(building_list[i]);		
												//������ ���� �� ���� ������ ���̷����� �ɰ����� ���� ��� ���߽��� ���ε� �Ǵ��ϸ� 
												//�� ������ ���� ������ ���̰ų� �Ѵ�.
	}
}

void AI::block_checking(Building* b){
	double building_serious = 0;				//������ ���� �� ���� ������ ����, �� ������ �ٸ� �������� ��ȣ�ۿ��� control���ش�.
												//���� �������� �̻����� �ɰ�������, �� ������ ���� ������ ��� ��� �� ������ ���ο����� ���� �� ������ ������ �Ѵ�.
	double building_infected_percent = 0;		//������ ������
	double building_death_percent = 0;			//������ ������
	double virus_serious = 0;					//���̷��� �ɰ���
	double* building_interact[2];				//���� ���õ� ������ �ٸ� ��������� ��ȣ�ۿ뼺
	double* temp_interact[2];					//�ٸ� �������� ��ȣ�ۿ뼺�� �ӽ÷� ������ ����
	int building_index;							//���� ���õ� building�� index�� �����´�.
	b->getInfectPercent(building_infected_percent);
	b->getDeathPercent(building_death_percent);
	b->getInteract(building_interact);
	vs->getSerious(virus_serious);
	b->getIndex(building_index);

	building_serious = building_infected_percent + (building_death_percent + virus_serious)*5;
	//�ɰ��� ���

	if(building_serious >= online_block[building_index]){
		for(int i=0; i<B_NUMBER; i++){
			building_interact[0][i] = 0;
			if(i == building_index) continue;										//���� �����ϰ� �������� �Ѿ��.
			building_list[i]->getInteract(temp_interact);
			temp_interact[0][building_index] = 0;
		}
		blocked_building[0][building_index] = true;
	}
	if(building_serious >= offline_block[building_index]){
		for(int i=0; i<B_NUMBER; i++){
			building_interact[1][i] = 0;
			if(i == building_index) continue;										//���� �����ϰ� �������� �Ѿ��.
			building_list[i]->getInteract(temp_interact);
			temp_interact[1][building_index] = 0;
		}
		blocked_building[1][building_index] = true;
	}

	if(150 < building_serious && building_serious < 180)/*�ɰ����� 1*/{
		for(int i=0; i<B_NUMBER; i++){
			building_interact[0][i] = building_interact[0][i]-1;						//����������� �ٸ� �������� Online������ ������ ������ �����.
			building_interact[1][i] = building_interact[1][i]-1;						//����������� �ٸ� �������� Offline������ ������ ������ �����.
			if(i == building_index) continue;										//���� �����ϰ� �������� �Ѿ��.
			building_list[i]->getInteract(temp_interact);
			temp_interact[0][building_index] = temp_interact[0][building_index]-1;	//�ٸ� �������� ���� �������� Online�� ���� ������ ������ �����.
			temp_interact[1][building_index] = temp_interact[1][building_index]-1;	//�ٸ� �������� ���� �������� Offline�� ���� ������ ������ �����.
		}
	}
	if(180 < building_serious && building_serious < 220)/*�ɰ����� 2*/{
		for(int i=0; i<B_NUMBER; i++){
			building_interact[0][i] = building_interact[0][i]-2;						//����������� �ٸ� �������� Online������ ������ ������ �����.
			building_interact[1][i] = building_interact[1][i]-2;						//����������� �ٸ� �������� Offline������ ������ ������ �����.
			if(i == building_index) continue;										//���� �����ϰ� �������� �Ѿ��.
			building_list[i]->getInteract(temp_interact);
			temp_interact[0][building_index] = temp_interact[0][building_index]-2;	//�ٸ� �������� ���� �������� Online�� ���� ������ ������ �����.
			temp_interact[1][building_index] = temp_interact[1][building_index]-2;	//�ٸ� �������� ���� �������� Offline�� ���� ������ ������ �����.
		}
	}

										/*...���� �����Ϸ�. ���� ��ȣ�ۿ뼺�� 0���� �۾��� ���� 0���� �ٽ� setting...*/
										/*���� ��� 0���� ������ block�� ���̳� ���������Ƿ�, block����*/

	int building_to_another_control[2][B_NUMBER] = {0};								//���� �������� �ٸ� �������� ���� ��ȣ�ۿ뼺�� ��� 0���� ������ �Ǵ����ٺ���
	int another_to_building_control[2][B_NUMBER] = {0};								//�ٸ� �������� ���� �������� ���� ��ȣ�ۿ뼺�� ��� 0���� ������ �Ǵ����ٺ���	
	for(int i=0; i<B_NUMBER; i++){
		if(building_interact[0][i] <= 0){ building_interact[0][i] = 0; building_to_another_control[0][i] = 1; }
		//���� ���� �� ���� �������� �ٸ� �������� Online�� ���� ��ȣ�ۿ뼺�� 0���� �۾����� 0���� set���ش�.
		if(building_interact[1][i] <= 0){ building_interact[1][i] = 0; building_to_another_control[1][i] = 1; }
		//���� ���� �� ���� �������� �ٸ� �������� Offline�� ���� ��ȣ�ۿ뼺�� 0���� �۾����� 0���� set���ش�.
		if(i == building_index) continue;
		building_list[i]->getInteract(temp_interact);
		if(temp_interact[0][building_index] <= 0){ temp_interact[0][building_index] = 0; another_to_building_control[0][i] = 1; }
		//���� ���� �� �ٸ� ����i���� ���� �������� Online�� ���� ��ȣ�ۿ뼺�� 0���� �۾����� 0���� set���ش�.
		if(temp_interact[1][building_index] <= 0){ temp_interact[1][building_index] = 0; another_to_building_control[1][i] = 1; }
		//���� ���� �� �ٸ� ����i���� ���� �������� Offline�� ���� ��ȣ�ۿ뼺�� 0���� �۾����� 0���� set���ش�.
	}

	//���� ��� 0���� ������, block����.(�Ʒ��� �� �۾�)
	int sum_building_to_another[2] = {0};
	int sum_another_to_building[2] = {0};
	for(int i=0; i<B_NUMBER; i++){
		sum_building_to_another[0] += building_to_another_control[0][i];
		sum_building_to_another[1] += building_to_another_control[1][i];
		sum_another_to_building[0] += another_to_building_control[0][i];
		sum_another_to_building[1] += another_to_building_control[1][i];
	}
	if(sum_building_to_another[0] == B_NUMBER && sum_another_to_building[0] == B_NUMBER) blocked_building[0][building_index] = true;
	if(sum_building_to_another[1] == B_NUMBER && sum_another_to_building[1] == B_NUMBER) blocked_building[1][building_index] = true;
}

void AI::building_called(int index, char* _name, double& _uninfected, double& _infected, double& _dead, bool& on_block, bool& off_block){
	Building_Information temp_info;
	building_list[index]->getInformation(&temp_info);
	building_list[index]->getName(_name);		//�̸� ����	
	_uninfected = temp_info.uninfected_pc;		//�������� ���� pc����
	_infected = temp_info.infected_pc;			//������ pc����
	_dead = temp_info.dead_pc;					//���� pc����
	on_block = blocked_building[0][index];		//online�� block���� ����
	off_block = blocked_building[1][index];		//offline�� block���� ����
}

void AI::statistic_called(int& _total, int& _uninfected, int& _live, double& vm, double& vp, int& rt, bool* inf_b, bool* uninf_b, bool* de_b){
	_total = sum_total_pc;
	_uninfected = sum_uninfected_pc;
	_live = sum_live_pc;
	vm = vac_max;
	vp = vac_point;
	rt = remain_time;
	inf_b = infected_building;
	uninf_b = uninfected_building;
	de_b = dead_building;
}