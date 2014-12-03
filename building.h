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
	int total_pc;			//총 pc수
	int uninfected_pc;		//감염 되지 않은 pc수->total_pc-infected_pc로 업데이트
	int infected_pc;		//감염 pc수
	int dead_pc;			//작동 불가능한 pc수
	int live_pc;			//작동 가능한 pc수
	int building_index;		//몇 번째로 할당된 building인지 알려주는 변수
};

class Building{
private:
	double Activity[4];						//On,Off-line 활동 정도 4경우. 첫 번째는 감염이 매우 잘됨. 두 번째는 Online에 의한 감염이 잘됨.
											//세 번째는 Offline에 의한 감염이 잘됨. 네 번째는 감염이 잘 안됨.
	double Security[3];						//보안의식 세 단계 상중하. 총 3가지이며, 첫 번째가 의식이 제일 높고 마지막이 의식이 제일 높음.
											//세 단계의 합은 일정하며, 높은 단계의 값이 클수록 pc들이 잘 안죽음.
	double Research_Ability;				//백신에 기여할 수 있는 정도
	double Online_Use;						//처음에는 1이지만, 이 값이 AI에 의해 낮아지면 online을 통한 내부 국가 전염성이 낮아진다.
	double Offline_Use;						//처음에는 1이지만, 이 값이 AI에 의해 낮아지면 offline을 통한 내부 국가 전염성이 낮아진다.
	char name[21];							//building의 이름
	VirusStatus* vs;						//바이러스status 포인터
	double interact[2][B_NUMBER];			//building끼리 상호작용하는 pc수를 control할 변수. 첫 행은 Online경로이며,
											//두번째 행은 Offline경로이다. 이 값이 0이되면, 그 국가로의 이동은 안된다.
	double infect_percent;					//감염된 percentage.
	double death_percent;					//작동 불가능 percentage
	int infect_level;						//현재 빌딩의 감염레벨
	int death_level;						//현재 빌딩의 죽음레벨
	int coin;								//0이면 생성x. coin값만큼 돈을 엔진에게 줌.
	Building* building_list[B_NUMBER];		//다른 빌딩들을 가리키는 포인터
	double Contribute_to_Vaccine;			//백신에 계산해서 전해주는 값. 이 기여도가 크면 백신 개발이 빨라짐.
	Building_Information building_info;		//빌딩의 정보
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

													//get,set들	
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
	
	
	

													//아래부터 외부에서 호출 될 method들
	void startedBuilding();
	//오염이 시작되는 빌딩.
	void do_operation();
	//한 턴에 빌딩 내부함수들을 모두 진행해준다. Engine에서 call		
	void iscoinExist(int& coin_flag);
	//엔진에서 코인이 얻어졌는지를 확인할 함수
	void getC2V(double& p);
	//기여도(p)가 크면 백신 개발이 빨라짐


													//아래부터 빌딩내부에서 동작할 method들
	void infected_to_death();
	//감염된 pc중 일부를 죽인다.	
	void virus_spread_in_a_building();
	//빌딩 내부에서 바이러스가 퍼지는 정도. infected_pc수와 dead_pc수가 변함.
	void info_update();
	//빌딩에 바이러스가 퍼지고 난 다음의 변화를 update해준다.
	void virus_spread_through_all_buildings();
	//한 빌딩에서 다른 빌딩으로 가는 pc이동함수
};

#endif
