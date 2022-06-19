#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

const int MINE_START_GOLD = 2000;
const int MINERS_DEFAULT = 2;
const int BASE_START_CURRENT_GOLD = 0;
const int BASE_START_SUM_GOLD = 0;
const int BASE_START_FARM_COUNT = 0;
const int FARM_COST = 50;
const int FARM_MAX_COUNT = 20;
const int MINER_CARRY_CAPACITY = 10;
const int FARM_BUILD_TIME = 2;
const int WORKER_TRAVEL_TIME = 1;


typedef struct Base{
	int currentGold;
	int sumGold;
	int farmCount;
} Base;

typedef struct Miner{
	int currentGold;
	int number;
} Miner;

typedef struct Mine{
	int currentGold;
} Mine;

typedef struct WorkData{
	Miner* miner;
	Base* base;
	Mine* mine;
} WorkData;

int getMiners(int argc, char** argv);
Base* initialBase();
Mine* initialMine();
Miner* initialMiner(int number);
void initializeLock();
void* work(void *data);
void* buildFarm(void *data);

pthread_mutex_t lock;

int main(int argc, char **argv){
	int minersCount = getMiners(argc, &argv[1]);
	Mine* mine = initialMine();
	Base* base = initialBase();
	initializeLock();


	int i;
	pthread_t* tids = (pthread_t*)malloc(sizeof(pthread_t) * minersCount);
	pthread_t farmBuilderThread;
	pthread_create(&farmBuilderThread, NULL, &buildFarm, base);
	for(i = 0; i < minersCount; i++){
		
		Miner* miner = initialMiner(i);
		WorkData* data = (WorkData*)malloc(sizeof(WorkData));
		data->miner = miner;
		data->mine = mine;
		data->base = base;

		long result = pthread_create(&tids[i], NULL, &work, data);
		if(result != 0)
			printf("error:");
		
	}

	for(i = 0; i < minersCount; i++){
		pthread_join(tids[i], NULL);
	}
	pthread_join(farmBuilderThread, NULL);
	pthread_mutex_destroy(&lock);

	printf("Gold in mine: %d Collected gold: %d Player gold: %d Farms: %d\n", mine->currentGold, base->sumGold, base->currentGold, base->farmCount);

	return 0;
}

Miner* initialMiner(int number){
	Miner* miner = (Miner*)malloc(sizeof(Miner));
	miner->currentGold = 0;
	miner->number = number;
	return miner;
}

void initializeLock(){
	if(pthread_mutex_init(&lock, NULL) != 0)
		printf("Mutex init failed!\n");
}

void* work(void *data){
	
	WorkData* workData = (WorkData*)data;
	
	while(workData->mine->currentGold > 0){
		int goldForWorker = MINER_CARRY_CAPACITY;

		pthread_mutex_lock(&lock);
		printf("Worker %d entered mine 1 \n", workData->miner->number);
		if(workData->mine->currentGold <= 0){
			pthread_mutex_unlock(&lock);
			break;
		}
		if(workData->mine->currentGold <= 10 && workData->mine->currentGold > 0){
			goldForWorker = workData->mine->currentGold;
		}

		workData->mine->currentGold -= goldForWorker;
		workData->miner->currentGold += goldForWorker;

		pthread_mutex_unlock(&lock);
		sleep(WORKER_TRAVEL_TIME);
		printf("Worker %d depositing \n", workData->miner->number);
		workData->base->currentGold += workData->miner->currentGold;
		workData->base->sumGold += workData->miner->currentGold;
		workData->miner->currentGold = 0;
	}

	printf("Worker %d exiting\n", workData->miner->number);
}

int getMiners(int argc, char** argv){
	int minersCount = MINERS_DEFAULT;
	if(argc >= 2){
		minersCount = atoi(argv[0]);
	}

	return minersCount;
}

Mine* initialMine(){
	Mine* mine = (Mine*)malloc(sizeof(Mine));
	mine->currentGold = MINE_START_GOLD;
	return mine;
}

Base* initialBase(){
	Base* base = (Base*)malloc(sizeof(Base));
	base->currentGold = BASE_START_CURRENT_GOLD;
	base->sumGold = BASE_START_SUM_GOLD;
	base->farmCount = BASE_START_FARM_COUNT;

	return base;
}

void* buildFarm(void *data){
	Base* base = (Base*)data;

	while(base->farmCount < FARM_MAX_COUNT){
		if(base->currentGold >= FARM_COST){
			pthread_mutex_lock(&lock);

			base->currentGold -= FARM_COST;

			pthread_mutex_unlock(&lock);
			sleep(FARM_BUILD_TIME);
			base->farmCount += 1;
			printf("Farm %d created\n", base->farmCount);
		}
		
		//eventual prevention of infinite loop
		if(base->currentGold <= 0 && base->sumGold >= MINE_START_GOLD)
			break;
	}
}

