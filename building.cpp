#include "building.h"
void Building::getInformation(Building_Information* info){
	*info = building_info;
}
void Building::setTotalpc(double pc){
	building_info.total_pc = pc;
}
void Building::getTotalpc(double& pc){
	pc = building_info.total_pc;
}
void Building::setLivepc(double pc){
	building_info.live_pc = pc;
}
void Building::getLivepc(double& pc){
	pc = building_info.live_pc;
}
void Building::setInfectedpc(double ipc){
	building_info.infected_pc = ipc;
}
void Building::getInfectedpc(double& ipc){
	ipc = building_info.infected_pc;
}
														//�⺻���� get,set��

														//�Ʒ����� �ܺο��� ȣ��Ǵ� method��
void Building::getInformation(Building_Information* info){
	*info = building_info;
}

void Building::startedBuilding(){
	building_info.infected_pc+=1;
}

void Building::do_operation(){
	infected_to_death();
	virus_spread_in_a_building();
	virus_spread_through_all_buildings();
}

void Building::iscoinExist(int& coin_flag){
	coin_flag = coin; 
}

void Building::getC2V(double& p){ 
	double v_serious=0;	//���̷����� �ɰ���
	vs->getSerious(v_serious);
	Contribute_to_Vaccine = Research_Ability*(infect_percent + death_percent + v_serious);
	if(death_percent == 100) Contribute_to_Vaccine = 0;
	p += Contribute_to_Vaccine;
}

void Building::info_update(){
	building_info.total_pc = building_info.live_pc + building_info.dead_pc;
	building_info.uninfected_pc = building_info.live_pc - building_info.infected_pc;
	infect_percent =(double) building_info.infected_pc / building_info.live_pc * 100;	//������ percentage
	death_percent =(double) building_info.dead_pc / building_info.total_pc * 100;		//���� percentage
}

														//�Ʒ����� building���ο����� ȣ��Ǵ� method��

void Building::virus_spread_in_a_building(){
	double act[4];
	vs->getInfect(act);	//���̷����� ������
	double online_infecting;
	double offline_infecting;
	online_infecting = Online_Use*(Activity[0]*act[0] + Activity[1]*act[1] + Activity[2]*act[2] + Activity[3]*act[3]);
	//Online �������� Online ��뷮�� pc��� ������ ���ؼ� ���.
	offline_infecting = Offline_Use*(Activity[0]*act[0] + Activity[1]*act[1] + Activity[2]*act[2] + Activity[3]*act[3]);
	//Offline �������� Online ��뷮�� pc��� ������ ���ؼ� ���.
	building_info.infected_pc += (building_info.uninfected_pc * infect_percent * (online_infecting + offline_infecting));
	//�����Ǵ� ���� �������� �ʴ� pc�����ٰ� ���� ������ ������, �׸��� on/off �������� ���ؼ� ����Ѵ�.
	//��� ��
	info_update();
	//���� ������Ʈ
	if((infect_percent+death_percent) >= 3 && infect_level == 0) {infect_level = 1; coin = 1;}
	else if((infect_percent+death_percent) >= 7 && infect_level == 1) {infect_level = 2; coin = 2;}
	//....
	else{ coin = 0;}
	//percent�� ��������� level�� ���ġ �ʴٸ� coin�� ���� 0�������ش�.
	
	//������ ���� ���� ������Ʈ
	if(death_percent >= 3 && death_level == 0) { death_level = 1; coin = 5;}
	if(death_percent >= 7 && death_level == 1) { death_level = 2; coin = 10;}
	else{ coin = 0;}
	//������ ���� ���� ������Ʈ
}

void Building::virus_spread_through_all_buildings(){
	srand(time(NULL));
	double going_live_pc;							//�̵��ϴ� �� pc��
	double going_infected_pc = 0;					//�̵��ϴ� ������ pc��
	double livepc_of_to;							//to�� �� pc��
	double infectedpc_of_to;						//to�� ������ pc��
	double pc=1,act1=2,act2=1,act3=2,act4=1;		//�뷱�� ������.
	int check[B_NUMBER] = {0};						//���Ƿ� �������� ������ pc�� �̵��Ѵ�. 1�̸� �̹� �̵��� �ߴٴ� ���̴�.
	int random_index;								//���Ƿ� �̵��� ������ index. �Ź��ٲ��.
	int random_going;								//������ pc �ϳ��� �̵��� Ȯ������� ���� ����
	double probability_of_going;					//������ pc �ϳ��� �̵��� Ȯ��
	for(int i=0; i<B_NUMBER; i++){
		if(i==building_info.building_index) continue;	//�ڱ��ڽ��̸� �پ�Ѵ´�.
		do{
			random_index = rand() % B_NUMBER;
		}while(check[random_index] != 0);			//�̹� �̵��� �����̸� �ٽ� ����ش�.
		check[random_index] = 1;					//����� ������ ǥ�����ش�.
		going_live_pc = (interact[0][random_index] + interact[1][random_index]) * building_info.live_pc;
		//�ٸ� �������� ��ȣ�ۿ� ������ ���� ���޵Ǵ� pc���� ��������.
		for(int i=0; i<going_live_pc; i++){
			random_going = rand() % building_info.live_pc;
			probability_of_going = (double) random_going / building_info.live_pc;
			if(probability_of_going <= infect_percent) going_infected_pc++;
		}
		//���� ������ �������� Ȯ����ŭ ������ pc�� �̵��Ѵ�.
		//���� ��� 20%�� �����Ǿ� ������, �� pc�� �Ѿ �� �� pc�� ������ pc�� Ȯ���� 20%�� ���̴�.
		this->setInfectedpc(this->building_info.infected_pc - going_infected_pc);	//������ �ִ� �������� ������ pc�� ����������.
		this->setLivepc(this->building_info.live_pc - going_live_pc);				//������ �ִ� �������� ��ü pc(�����Ȱ�+�ȵȰ�)�� ����������.
		info_update();
		//from�� �̵� �� ��ȭ�� total pc���� update���ش�.
		building_list[random_index]->getInfectedpc(infectedpc_of_to);				//to ������ ������ pc���� �޾ƿ´�.
		building_list[random_index]->setInfectedpc(infectedpc_of_to + going_infected_pc);
		building_list[random_index]->getLivepc(livepc_of_to);						//to������ pc���� �޾ƿ´�.
		building_list[random_index]->setLivepc(livepc_of_to + going_live_pc);		//to�� �������� ���� pc�����ٰ� this���� ����������ŭ pc�� �߰����ش�.
		//to�� �̵� �� ��ȭ�� total pc���� update���ش�.
		building_list[random_index]->info_update();
	}	
}

void Building::infected_to_death(){
	double act[3];
	double kill=0;	//���̴� ����
	vs->getDestroy(act);
	for(int i=0; i<3; i++){kill += act[i] * Security[i];}
	building_info.dead_pc = kill*building_info.infected_pc;
	info_update();
}



