#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <queue>
#include <string.h>
#include <iostream>

using namespace std;

class process{
	public:
	string name;
	int arrivalTime;
	int executionTime;
	int executedTime;
	bool alldone; // Feature added after finishing FCFS - Thank you Luke
	int startTime;
	int endTime;

};

class scheduler{
	public:
	void FCFS(process array[], int asize);
	void SJF(process array[], int asize);
};

void scheduler::FCFS(process parray[], int asize){
	queue<process> q;
	process temp;
        temp.name = "temp";
        temp.arrivalTime = 100;
        temp.executionTime = 100;
	int minatime =100;
	int x = 0; // used to properly use for loop # 2
	process finalparray[asize] = {};

	for (int i =0; i < asize; i++){

		minatime = 100;
		for(int j =0; j < asize-i; j++){

			if(parray[j].arrivalTime < minatime){
				minatime = parray[j].arrivalTime;
			}
		} // find the minimum arrival time in parray

		x = 0;
		for(int k = 0; k < asize-i; k++){ // for loop #2 - uses x
			if(parray[k].arrivalTime == minatime){
				q.push(parray[k]);
				//cout << "Adding: " << parray[k].name << " to the queue - i: " << i << endl;
				x--;
			}else{
				finalparray[x] = parray[k];
				//cout << "finalparray " << x << " : " << finalparray[x].name << endl;
			}
			x++;
		}// put the smallest arrival time process into queue & put all other values of parray into a fresh array

		for(int z = 0; z < asize-i - 1; z++){
			parray[z] = finalparray[z];
		} // reset parray so that it only has values that are unsorted

		for(int y = asize - 1; y > (asize - i); y-- ){
			parray[y] = temp;
			//cout << "parray  " << y << ": " << parray[y].name << endl;
		}// fill parray with other values to make sure it is ready to find next minimum arrival time

	}// Now everything is organized in the queue based off arrivalTime

	//Time to output everything neatly
	cout << endl << "FCFS Schedular" << endl << endl;
	int time = q.front().arrivalTime;//sets the start time to the head of the queue

	for(int i = 0; i < asize; i++){ // printing/ executing all of the processes
		if(time < q.front().arrivalTime){
			time = q.front().arrivalTime;
		}
		cout << "Running process: " << q.front().name << " which arrived at " << q.front().arrivalTime;
		cout << " which will start execution at "<< time << " until "; 
		cout << time + q.front().executionTime << endl;
		cout << "-----------------------------------------" << endl;

		time = time + q.front().executionTime;
		q.pop();
	}
}



void scheduler::SJF(process parray[], int asize){

	int time = 0;
	int shortestJobTime;
	int pcount = 0;
	int counter = 0;

//Need to find the shortest job available to run at each time.
//parray[i].executionTime != parray[i].executedTime    Checks if the process is done or not
//Could also use alldone trait parray[i].alldone == T/F
//parray[i].arrivalTime <= time    Checks if the Process is actually there

	cout << endl << "SJF Schedular" << endl << endl;

	while(pcount < asize){
		counter = 0;

		for(int i =0; i < asize; i++){ //increment time if there are no processes to run   --- accounting for dead time

			if(parray[i].alldone == false && (parray[i].arrivalTime > time) ){
				counter++;
			}//increment counter for each process that it not done AND has not arrived
			else if(parray[i].alldone == true){
				counter++;
			}//increment counter for each process that is done
			if(counter == asize){
				time++;
				cout << "No process can execute, so time had been incremented from " << time -1 << " to " << time;
				cout << endl;
			}//will increment time ONLY IF counter = asize = number of processes that are either done or havent arrived
			//if counter == asize, that means that all of the processes are either done, or have not arrived
		}

		shortestJobTime = 100; //reset SJT
		for(int i =0; i < asize; i++){  //find the first process to arrive and if there are any other processes to arrive

			if((parray[i].executionTime <= shortestJobTime) && (parray[i].alldone == false) && (parray[i].arrivalTime <= time)){
				shortestJobTime = parray[i].executionTime;
                        }// find the shortestJobTime of all of the processes that are not done AND arrived

			if((parray[i].executionTime == parray[i].executedTime) && (parray[i].alldone != true)){
				pcount++; // working towards finishing the over arching while loop
				parray[i].alldone = true;
				parray[i].endTime = time;
				cout << parray[i].name << " has finished" << endl << endl;
			}// sort through to set all processes that are done, have the all done trait, increment pcount

		}
		for(int i =0; i < asize; i++){
			if((parray[i].executionTime <= shortestJobTime) && (parray[i].alldone != true) && (parray[i].arrivalTime <= time)){
				parray[i].executedTime = parray[i].executedTime + 1;
				if(parray[i].startTime == (-1)){
					parray[i].startTime = time;
				}
				cout << parray[i].name << " executed from " << time;
				time++;
				cout << " to " << time << endl;
			}// execute the shortest job and increment time
		}


	}

	cout << endl << "SJF Schedular" << endl << endl;
	cout << "---------------------------------------" << endl;
	for(int i =0; i < asize; i++){
		cout << "Process: " << parray[i].name << " Started at: " << parray[i].startTime << " And ended at: ";
		cout << parray[i].endTime << endl;
		cout << "---------------------------------------" << endl;
	}//output all processes with their start time and end time
}



int main(int argc, char **argv){
 	process processes[(argc-2)/3] = {};
	for(int i = 0; i < (argc - 2)/3; i++){ //collect input - we subtract 2 from argc to get rid of ./schedular and schedular type
		int j = i*3 +1;

		processes[i].name = argv[j];
		processes[i].arrivalTime = atof(argv[j+1]);
		processes[i].executionTime = atof(argv[j+2]);
		processes[i].alldone = false;
		processes[i].executedTime = 0;
		processes[i].startTime = -1;
		processes[i].endTime = 0;
	}//collects inputs and initializes variables - the /3 accounts for the process having three input traits

	scheduler myscheduler;

	string type = argv[argc-1]; //collects type of schedular
	int size = (argc-2) /3; 

	if(type == "FCFS"){
		myscheduler.FCFS(processes, size);
	}
	else if(type == "SJF"){
                myscheduler.SJF(processes, size);
        }
	else{
		cout << "Else Triggered - Did not input 'FCFS' or 'SJS'" << endl;
	}
return 0;
}

