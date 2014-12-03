#ifndef OOP_AI
#define OOP_AI
#include "building.h"
#include "vaccine.h"
class AI{
private:
	Building* building_list[B_NUMBER];
	Vaccine* vc;
	VirusStatus* vs;
	bool infected_building[B_NUMBER];		//감염된 빌딩들. 값이 true이면 감염된 것이다.
	bool uninfected_building[B_NUMBER];		//감염되지 않은 빌딩들. 값이 true이면 감염된 것이다.
	bool dead_building[B_NUMBER];			//죽은 빌딩들. 값이 true이면 감염된 것이다.
	bool blocked_building[2][B_NUMBER];		//한 빌딩이 감염정도가 매우 심하면, 그 빌딩은 Blocked 처리된다. 처리는 On/Off 두 가지 경우가 있다.
											//만약 On이 Blocked 처리되면, 그 빌딩으로는 Online을 통한 감염이 안되고 
											//그 빌딩에서 다른 곳으로 퍼지지도 않는다. 값이 true이면 block된 것이다.

	double vac_max;							//vaccine의 max값
	double vac_point;						//vaccine의 현재 개발정도
	double developing_speed;				//vaccine의 개발속도
	int remain_time;						//vaccine이 개발되기까지남은시간 
	double sum_total_pc;					//모든 빌딩의 pc수들을 합한값
	double sum_infected_pc;					//모든 빌딩의 감염된 pc수들을 합한값
	double sum_uninfected_pc;				//모든 빌딩의 감염되지 않은 pc수들을 합한값
	double sum_dead_pc;						//모든 빌딩의 죽은 pc수들을 합한값
	double sum_live_pc;						//모든 빌딩의 살아있는 pc수들을 합한값
	double vaccine_developing;				//백신이 개발되는 정도. 이 숫자를 넘어가면 백신을 개발하기 시작한다.
	double online_block[B_NUMBER];			//각 빌딩의 online으로 퍼지는 최대한계치. 이 값을 넘어가면 그 빌딩으로는 online을 통한 이동이 없어진다.
											//즉, 이 값이 클 수록 빌딩은 바이러스에 소극적인 대처를 한다.
	double online_blocking_control[4];		//online으로 퍼지는 정도를 계산해줄 변수
	double offline_block[B_NUMBER];			//각 빌딩의 offline으로 퍼지는 최대한계치. 이 값을 넘어가면 그 빌딩으로는 offline을 통한 이동이 없어진다.
											//즉, 이 값이 클 수록 빌딩은 바이러스에 소극적인 대처를 한다.
	double offline_blocking_control[4];		//offline으로 퍼지는 정도를 계산해줄 변수
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
		
		sum_total_pc = 0;					//모든 빌딩의 pc수들을 합한값
		sum_infected_pc = 0;				//모든 빌딩의 감염된 pc수들을 합한값
		sum_uninfected_pc = 0;				//모든 빌딩의 감염되지 않은 pc수들을 합한값
		sum_dead_pc = 0;					//모든 빌딩의 죽은 pc수들을 합한값
		sum_live_pc = 0;					//모든 빌딩의 살아있는 pc수들을 합한값
											//아래는 blocking 변수 설정.
		online_blocking_control[0] = 10;
		online_blocking_control[1] = 7;
		online_blocking_control[2] = 2;
		online_blocking_control[3] = 0;
		offline_blocking_control[0] = 10;
		offline_blocking_control[1] = 2;
		offline_blocking_control[2] = 7;
		offline_blocking_control[3] = 0;
	}
															//get method들

												
	void getVM_from_VirusStatus(){vs->getVacMax(vac_max);}			//virusstatus로부터 vaccine의 max값을 가져온다.
	void getVP_DS_from_Vaccine(){vc->getVaccine(vac_point,developing_speed);}			//vaccine으로부터 현재 vaccine값을 가져온다.
	void getVM(double& vm){vm = vac_max;}
	void getVP(double& vp){vp = vac_point;}
	void getSum_total_pc(double& pc);
	void getSum_infected_pc(double& pc);
	void getSum_uninfected_pc(double& pc);
	void getSum_dead_pc(double& pc);
	void getSum_live_pc(double& pc);

															//나머지 method들
	void do_operation();					//한 턴에 AI를 통해 각 빌딩들의 정보를 종합해 수정해주는 작업을 한다.
	void calVaccineUpgrade();				//백신이 개발되려면 얼마나 걸리는지 계산한다.
	void block_checking(Building* b);		//한 빌딩의 감염 및 죽음 정도와 바이러스의 심각성을 고려해 다른 빌딩과의 교류 정도를 control한다.
	void initialize_pc();					//다음 턴을 위해 pc정보의 합을 초기화한다.
	void gather_info();						//모든 building의 정보를 모은다.
	void setEach_Building_Information();	//각 빌딩들의 정보를 토대로 감염 및 파괴여부, block여부를 설정해준다.
	void building_called(int index, char* _name, double& _uninfected, double& _infected, double& _dead, bool& on_block, bool& off_block);
											//Engine에서 특정 빌딩의 정보를 call할 경우 출력을 위해 실행하는 method.
	void statistic_called(int& _total, int& _uninfected, int& _live, double& vm, double& vp, int& rt, bool* inf_b, bool* uninf_b, bool* de_b);
	void AI_calle();
											//전체 정보를 출력하기 위해 실행하는 method.
};


#endif
