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
														//기본적인 get,set끝

														//아래부터 외부에서 호출되는 method들
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
	double v_serious=0;	//바이러스의 심각성
	vs->getSerious(v_serious);
	Contribute_to_Vaccine = Research_Ability*(infect_percent + death_percent + v_serious);
	if(death_percent == 100) Contribute_to_Vaccine = 0;
	p += Contribute_to_Vaccine;
}

void Building::info_update(){
	building_info.total_pc = building_info.live_pc + building_info.dead_pc;
	building_info.uninfected_pc = building_info.live_pc - building_info.infected_pc;
	infect_percent =(double) building_info.infected_pc / building_info.live_pc * 100;	//감염된 percentage
	death_percent =(double) building_info.dead_pc / building_info.total_pc * 100;		//죽은 percentage
}

														//아래부터 building내부에서만 호출되는 method들

void Building::virus_spread_in_a_building(){
	double act[4];
	vs->getInfect(act);	//바이러스의 전염성
	double online_infecting;
	double offline_infecting;
	online_infecting = Online_Use*(Activity[0]*act[0] + Activity[1]*act[1] + Activity[2]*act[2] + Activity[3]*act[3]);
	//Online 감염량은 Online 사용량과 pc사용 정도와 곱해서 계산.
	offline_infecting = Offline_Use*(Activity[0]*act[0] + Activity[1]*act[1] + Activity[2]*act[2] + Activity[3]*act[3]);
	//Offline 감염량은 Online 사용량과 pc사용 정도와 곱해서 계산.
	building_info.infected_pc += (building_info.uninfected_pc * infect_percent * (online_infecting + offline_infecting));
	//감염되는 양은 감염되지 않는 pc수에다가 현재 빌딩의 감염률, 그리고 on/off 감염량을 곱해서 계산한다.
	//계산 끝
	info_update();
	//정보 업데이트
	if((infect_percent+death_percent) >= 3 && infect_level == 0) {infect_level = 1; coin = 1;}
	else if((infect_percent+death_percent) >= 7 && infect_level == 1) {infect_level = 2; coin = 2;}
	//....
	else{ coin = 0;}
	//percent는 충분하지만 level이 충분치 않다면 coin은 값은 0으로해준다.
	
	//감염에 의한 코인 업데이트
	if(death_percent >= 3 && death_level == 0) { death_level = 1; coin = 5;}
	if(death_percent >= 7 && death_level == 1) { death_level = 2; coin = 10;}
	else{ coin = 0;}
	//죽음에 의한 코인 업데이트
}

void Building::virus_spread_through_all_buildings(){
	srand(time(NULL));
	double going_live_pc;							//이동하는 총 pc수
	double going_infected_pc = 0;					//이동하는 감염된 pc수
	double livepc_of_to;							//to의 총 pc수
	double infectedpc_of_to;						//to의 감염된 pc수
	double pc=1,act1=2,act2=1,act3=2,act4=1;		//밸런스 조정용.
	int check[B_NUMBER] = {0};						//임의로 빌딩들을 가리켜 pc가 이동한다. 1이면 이미 이동을 했다는 뜻이다.
	int random_index;								//임의로 이동할 빌딩의 index. 매번바뀐다.
	int random_going;								//감염된 pc 하나가 이동할 확률계산을 위한 변수
	double probability_of_going;					//감염된 pc 하나가 이동할 확률
	for(int i=0; i<B_NUMBER; i++){
		if(i==building_info.building_index) continue;	//자기자신이면 뛰어넘는다.
		do{
			random_index = rand() % B_NUMBER;
		}while(check[random_index] != 0);			//이미 이동한 빌딩이면 다시 골라준다.
		check[random_index] = 1;					//골라준 빌딩은 표시해준다.
		going_live_pc = (interact[0][random_index] + interact[1][random_index]) * building_info.live_pc;
		//다른 빌딩과의 상호작용 정도에 의해 전달되는 pc수가 정해진다.
		for(int i=0; i<going_live_pc; i++){
			random_going = rand() % building_info.live_pc;
			probability_of_going = (double) random_going / building_info.live_pc;
			if(probability_of_going <= infect_percent) going_infected_pc++;
		}
		//현재 빌딩의 감염률의 확률만큼 감염된 pc가 이동한다.
		//예를 들어 20%가 감염되어 있으면, 한 pc가 넘어갈 때 그 pc가 감염된 pc일 확률이 20%인 것이다.
		this->setInfectedpc(this->building_info.infected_pc - going_infected_pc);	//기존에 있던 빌딩에서 감염된 pc가 빠져나간다.
		this->setLivepc(this->building_info.live_pc - going_live_pc);				//기존에 있던 빌딩에서 전체 pc(감염된것+안된것)가 빠져나간다.
		info_update();
		//from의 이동 후 변화한 total pc수를 update해준다.
		building_list[random_index]->getInfectedpc(infectedpc_of_to);				//to 빌딩의 감염된 pc수를 받아온다.
		building_list[random_index]->setInfectedpc(infectedpc_of_to + going_infected_pc);
		building_list[random_index]->getLivepc(livepc_of_to);						//to빌딩의 pc수를 받아온다.
		building_list[random_index]->setLivepc(livepc_of_to + going_live_pc);		//to의 빌딩에는 기존 pc수에다가 this에서 빠져나간만큼 pc를 추가해준다.
		//to의 이동 후 변화한 total pc수를 update해준다.
		building_list[random_index]->info_update();
	}	
}

void Building::infected_to_death(){
	double act[3];
	double kill=0;	//죽이는 정도
	vs->getDestroy(act);
	for(int i=0; i<3; i++){kill += act[i] * Security[i];}
	building_info.dead_pc = kill*building_info.infected_pc;
	info_update();
}



