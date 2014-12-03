#include "AI.h"
												//get들
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

												//get들 끝
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
	initialize_pc();							//빌딩들 통합 정보 초기화
	gather_info();								//각 빌딩들의 정보 통합
	calVaccineUpgrade();						//백신 개발에 남은시간 계산
	setEach_Building_Information();				//각 빌딩들의 정보 계산 후 setting.
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
												//감염안된 building들과 감염된 building들과 죽은 building들 처리완료
		block_checking(building_list[i]);		
												//빌딩의 감염 및 죽음 정도와 바이러스의 심각성에 따라 백신 개발시작 여부도 판단하며 
												//그 빌딩에 대한 교류도 줄이거나 한다.
	}
}

void AI::block_checking(Building* b){
	double building_serious = 0;				//빌딩의 감염 및 죽음 정도를 보고, 그 빌딩과 다른 빌딩과의 상호작용을 control해준다.
												//만약 일정수준 이상으로 심각해지면, 그 빌딩에 대한 교류는 모두 끊어서 그 빌딩은 내부에서만 감염 및 죽음이 퍼지게 한다.
	double building_infected_percent = 0;		//빌딩의 감염률
	double building_death_percent = 0;			//빌딩의 죽음률
	double virus_serious = 0;					//바이러스 심각성
	double* building_interact[2];				//현재 선택된 빌딩의 다른 빌딩들과의 상호작용성
	double* temp_interact[2];					//다른 빌딩들의 상호작용성을 임시로 저장할 변수
	int building_index;							//현재 선택된 building의 index를 가져온다.
	b->getInfectPercent(building_infected_percent);
	b->getDeathPercent(building_death_percent);
	b->getInteract(building_interact);
	vs->getSerious(virus_serious);
	b->getIndex(building_index);

	building_serious = building_infected_percent + (building_death_percent + virus_serious)*5;
	//심각성 계산

	if(building_serious >= online_block[building_index]){
		for(int i=0; i<B_NUMBER; i++){
			building_interact[0][i] = 0;
			if(i == building_index) continue;										//현재 빌딩하고 같아지면 넘어간다.
			building_list[i]->getInteract(temp_interact);
			temp_interact[0][building_index] = 0;
		}
		blocked_building[0][building_index] = true;
	}
	if(building_serious >= offline_block[building_index]){
		for(int i=0; i<B_NUMBER; i++){
			building_interact[1][i] = 0;
			if(i == building_index) continue;										//현재 빌딩하고 같아지면 넘어간다.
			building_list[i]->getInteract(temp_interact);
			temp_interact[1][building_index] = 0;
		}
		blocked_building[1][building_index] = true;
	}

	if(150 < building_serious && building_serious < 180)/*심각정도 1*/{
		for(int i=0; i<B_NUMBER; i++){
			building_interact[0][i] = building_interact[0][i]-1;						//현재빌딩에서 다른 빌딩으로 Online을통해 퍼지는 정도를 낮춘다.
			building_interact[1][i] = building_interact[1][i]-1;						//현재빌딩에서 다른 빌딩으로 Offline을통해 퍼지는 정도를 낮춘다.
			if(i == building_index) continue;										//현재 빌딩하고 같아지면 넘어간다.
			building_list[i]->getInteract(temp_interact);
			temp_interact[0][building_index] = temp_interact[0][building_index]-1;	//다른 빌딩에서 현재 빌딩으로 Online을 통해 퍼지는 정도를 낮춘다.
			temp_interact[1][building_index] = temp_interact[1][building_index]-1;	//다른 빌딩에서 현재 빌딩으로 Offline을 통해 퍼지는 정도를 낮춘다.
		}
	}
	if(180 < building_serious && building_serious < 220)/*심각정도 2*/{
		for(int i=0; i<B_NUMBER; i++){
			building_interact[0][i] = building_interact[0][i]-2;						//현재빌딩에서 다른 빌딩으로 Online을통해 퍼지는 정도를 낮춘다.
			building_interact[1][i] = building_interact[1][i]-2;						//현재빌딩에서 다른 빌딩으로 Offline을통해 퍼지는 정도를 낮춘다.
			if(i == building_index) continue;										//현재 빌딩하고 같아지면 넘어간다.
			building_list[i]->getInteract(temp_interact);
			temp_interact[0][building_index] = temp_interact[0][building_index]-2;	//다른 빌딩에서 현재 빌딩으로 Online을 통해 퍼지는 정도를 낮춘다.
			temp_interact[1][building_index] = temp_interact[1][building_index]-2;	//다른 빌딩에서 현재 빌딩으로 Offline을 통해 퍼지는 정도를 낮춘다.
		}
	}

										/*...레벨 조정완료. 이제 상호작용성이 0보다 작아진 경우는 0으로 다시 setting...*/
										/*만약 모두 0보다 작으면 block된 것이나 마찬가지므로, block해줌*/

	int building_to_another_control[2][B_NUMBER] = {0};								//현재 빌딩에서 다른 빌딩으로 가는 상호작용성이 모두 0보다 작은지 판단해줄변수
	int another_to_building_control[2][B_NUMBER] = {0};								//다른 빌딩에서 현재 빌딩으로 가는 상호작용성이 모두 0보다 작은지 판단해줄변수	
	for(int i=0; i<B_NUMBER; i++){
		if(building_interact[0][i] <= 0){ building_interact[0][i] = 0; building_to_another_control[0][i] = 1; }
		//레벨 조정 후 현재 빌딩에서 다른 빌딩으로 Online을 통한 상호작용성이 0보다 작아지면 0으로 set해준다.
		if(building_interact[1][i] <= 0){ building_interact[1][i] = 0; building_to_another_control[1][i] = 1; }
		//레벨 조정 후 현재 빌딩에서 다른 빌딩으로 Offline을 통한 상호작용성이 0보다 작아지면 0으로 set해준다.
		if(i == building_index) continue;
		building_list[i]->getInteract(temp_interact);
		if(temp_interact[0][building_index] <= 0){ temp_interact[0][building_index] = 0; another_to_building_control[0][i] = 1; }
		//레벨 조정 후 다른 빌딩i에서 현재 빌딩으로 Online을 통한 상호작용성이 0보다 작아지면 0으로 set해준다.
		if(temp_interact[1][building_index] <= 0){ temp_interact[1][building_index] = 0; another_to_building_control[1][i] = 1; }
		//레벨 조정 후 다른 빌딩i에서 현재 빌딩으로 Offline을 통한 상호작용성이 0보다 작아지면 0으로 set해준다.
	}

	//만약 모두 0보다 작으면, block해줌.(아래는 그 작업)
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
	building_list[index]->getName(_name);		//이름 전달	
	_uninfected = temp_info.uninfected_pc;		//감염되지 않은 pc전달
	_infected = temp_info.infected_pc;			//감염된 pc전달
	_dead = temp_info.dead_pc;					//죽은 pc전달
	on_block = blocked_building[0][index];		//online의 block여부 전달
	off_block = blocked_building[1][index];		//offline의 block여부 전달
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