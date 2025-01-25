#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;


class Time{
    public:
    int c_hour, c_day; //Stores the current time and date of the program

    // constructor
    Time(){
        c_day = 0;
        c_hour = 0;
    }

    // function to update the current time
    void update(){
        c_hour++;
        if(c_hour>23){
            c_hour = 0;
            c_day++;
        }
    }

};


class Job{
    public:
    int day,hour,exe_time,mem,cpu_req; // variables of the job class

    // default constructor
    Job(){
        day = 0;
        hour = 0;
        exe_time = 0;
        mem = 0;
        cpu_req = 0;


    }

    // constructor
    Job(int a,int b,int c,int d,int e){
        day = a;
        hour = b;
        mem = c;
        cpu_req = d;
        exe_time = e;

    }

};

struct Comp{
    bool operator()(Job& a , Job& b){
        return a.exe_time < b.exe_time;
    }
    
}; 
struct Comp2{
    bool operator()(Job& a , Job& b){
        int val1, val2;
        val1 = a.exe_time*a.cpu_req*a.mem;
        val2 = b.exe_time*b.mem*b.cpu_req;
        return val1 < val2;
    }

};

class Worker{
    public:
    int w_mem,w_core;
    queue<Job> aJob;


    Worker(){
        w_mem = 64;
        w_core = 24;
    }

    bool AssignJob(Job& J){
        if(this->w_mem>= J.mem && this->w_core>= J.cpu_req){
        this->w_mem = this->w_mem - J.mem;
        this->w_core = this->w_core - J.cpu_req;
        aJob.push(J);
        return true;
    }
    else{
        return false;
    }
    }
    void Update(){
    queue<Job> tempQ;
    while(!this->aJob.empty()){
        Job temp = this->aJob.front();
        this->aJob.pop();
        temp.exe_time--;
        if(temp.exe_time == 0){
            this->w_core += temp.cpu_req;
            this->w_mem += temp.mem;
        }
        else{
            tempQ.push(temp);
        }
    }
    this->aJob = tempQ;
}


};
 

class MainNode{
    public:
    vector<int> avg_mem, avg_cpu;
    int avgM = 0;
    int avgC = 0;

    
     template<typename Container>
     void assignFirst(Container& C, vector<Worker>& W, Time& T){
       while(!C.empty()){
        
        Job temp = C.front();
        C.pop();
        int tempday = T.c_day;
        if(temp.day == T.c_day && temp.hour == T.c_hour){
            for(int i =0; i<W.size();++i ){
                if(W[i].AssignJob(temp)){
                    break;
                }
            }
        }
        else{
           
            T.update();
            
            
            for(Worker& a : W){
                int mem_used = 64 - a.w_mem;
                avgM += mem_used;
                int core_used = 24 - a.w_core;
                avgC += core_used;

            }
            if(T.c_day>tempday){
                avgM = avgM/24;
                avgC = avgC/24;
                avg_mem.push_back(avgM);
                avg_cpu.push_back(avgC);
                avgC =0;
                avgM = 0;
            }
            for(Worker& W1 : W){
                W1.Update();
            }
            
        }
    }
    }


template<typename Container>
void assignBest(Container& C, vector<Worker>& W, Time& T){
    while(!C.empty()){
        Job temp = C.front();
        C.pop();
        int tempday = T.c_day;
        if(temp.day == T.c_day && temp.hour == T.c_hour){
            Worker* bestWorker = nullptr;
            int bestFit = 1000; // Initialize to a large value
            for(int i = 0; i < W.size(); ++i){
                if(W[i].w_mem >= temp.mem && W[i].w_core >= temp.cpu_req){
                    int req = W[i].w_mem * W[i].w_core;
                    if(req < bestFit){
                        bestFit = req;
                        bestWorker = &W[i];
                    }
                }
            }
            if(bestWorker != nullptr) {
                bestWorker->AssignJob(temp);
            }
        }
         else{
            
            T.update();
            
            for(Worker& a : W){
                int mem_used = 64 - a.w_mem;
                avgM += mem_used;
                int core_used = 24 - a.w_core;
                avgC += core_used;

            }
            if(T.c_day>tempday){
                avgM = avgM/24;
                avgC = avgC/24;
                avg_mem.push_back(avgM);
                avg_cpu.push_back(avgC);
                avgC =0;
                avgM = 0;
            }
            for(Worker& W1 : W){
                W1.Update();
            }
            
        }
    }
}

    template<typename Container>
void assignWorst(Container& C, vector<Worker>& W, Time& T){
    while (!C.empty()) {
        Job temp = C.front();
        C.pop();
         int tempday = T.c_day;

        // Check if the job's arrival time matches the current time
        if (temp.day == T.c_day && temp.hour == T.c_hour) {
            Worker* worstWorker = nullptr;
            int worstFit = -1;  // Start with a very low number since we are looking for the maximum

            // Iterate through workers to find the worst-fit worker
            for (int i = 0; i < W.size(); ++i) {
                if (W[i].w_mem >= temp.mem && W[i].w_core >= temp.cpu_req) {
                    // Calculate the "worst fit" as the product of available memory and CPU cores
                    int resourceUsage = W[i].w_mem * W[i].w_core;
                    if (resourceUsage > worstFit) {
                        worstFit = resourceUsage;
                        worstWorker = &W[i];  // Update the worst worker
                    }
                }
            }

            // If a suitable worker was found, assign the job
            if (worstWorker != nullptr) {
                worstWorker->AssignJob(temp);
            }
        }
         else{
           
            T.update();
            
            for(Worker& a : W){
                int mem_used = 64 - a.w_mem;
                avgM += mem_used;
                int core_used = 24 - a.w_core;
                avgC += core_used;

            }
            if(T.c_day>tempday){
                avgM = avgM/24;
                avgC = avgC/24;
                avg_mem.push_back(avgM);
                avg_cpu.push_back(avgC);
                avgC =0;
                avgM = 0;
            }
            for(Worker& W1 : W){
                W1.Update();
            }
            
        }
    }
}

};


// Function to read the file and create a queue of Job objects
queue<Job> JobQueue(ifstream& InputFile, queue<Job>& J){
    string line, word;
    int wordcount = 0;
    while(getline(InputFile,line)){
        wordcount = 1;
        stringstream ss(line);
        int day,hour,mem,cpu_req,exe_time;

        while(ss >> word){
           
            if(wordcount == 5){
                day = stoi(word);
                
            }
            else if(wordcount == 8){
                hour = stoi(word);
            }
            else if(wordcount == 10){
                mem = stoi(word);
                
            }
            else if(wordcount == 12){
                cpu_req = stoi(word);
            }
            else if (wordcount == 14){
                exe_time = stoi(word);
                 Job temp(day,hour,mem,cpu_req,exe_time);
                 J.push(temp);
            }
             wordcount++;
        }
      
       
    }
return J;
}
 queue<Job> sortDuration(queue<Job>& J){
        priority_queue<Job, vector<Job>, Comp> pq;
        queue<Job> Exe_job;
        while(!J.empty()){
            pq.push(J.front());
            J.pop();
        }
        while(!pq.empty()){
            Exe_job.push(pq.top());
            pq.pop();
        }
        return Exe_job;
    } 

    queue<Job> sortSmall(queue<Job>& J){
        priority_queue<Job,vector<Job>,Comp2> pq;
        queue<Job> Exe_job;
        while(!J.empty()){
            pq.push(J.front());
            J.pop();
        }
        while(!pq.empty()){
            Exe_job.push(pq.top());
            pq.pop();
        }
        return Exe_job;
       
    }

int main(){

    ifstream InputFile("JobArrival.txt");
    queue<Job> Myq;
    
    vector<Worker> MyWorkers;
    JobQueue(InputFile,Myq);
    Time T;
    for(int i=1;i<128;++i){
        Worker W;
        MyWorkers.push_back(W);
    }
    MainNode Sorter;
    int a,b;
    cout << "Choose the type of sort wanted on the Jobs" << endl;
    cout << "1) FCFS" << endl;
    cout << "2) Smallest Job First" << endl;
    cout << "3) Shortest Duration" << endl;
    cin >> a;
    cout << "Choose the worker fit required" << endl;
    cout << "1) First Fit" << endl;
    cout << "2) Best Fit" << endl;
    cout << "3) Worst Fit" << endl;
    cin >> b;
    if(a==1){
        if(b==1){
            Sorter.assignFirst(Myq,MyWorkers,T);
        }
        else if(b==2){
            Sorter.assignBest(Myq,MyWorkers,T);
        }
        else if(b==3){
            Sorter.assignWorst(Myq,MyWorkers,T);
        }
        else{
            cout << " Invalid Input";
        }
    }
    else if(a==2){
       queue<Job> Exe_queue = sortSmall(Myq);
        
        if(b==1){
            Sorter.assignFirst(Exe_queue,MyWorkers,T);
        }
        else if(b==2){
            Sorter.assignBest(Exe_queue,MyWorkers,T);
        }
        else if(b==3){
            Sorter.assignWorst(Exe_queue,MyWorkers,T);
        }
        else{
            cout << " Invalid Input";
        }
    }
    else if(a==3){
       queue<Job> Exe_queue = sortDuration(Myq);
        
        if(b==1){
            Sorter.assignFirst(Exe_queue,MyWorkers,T);
        }
        else if(b==2){
            Sorter.assignBest(Exe_queue,MyWorkers,T);
        }
        else if(b==3){
            Sorter.assignWorst(Exe_queue,MyWorkers,T);
        }
        else{
            cout << " Invalid Input";
        }
    }
    else{
        cout << "INVALID INPUT";
    } 
    ofstream outputFile("Jobs.csv");
    if(!outputFile){
        cout << "Error creating this file";
    }
    else{
        
        for(int i = 0; i<Sorter.avg_mem.size();++i){
            outputFile << Sorter.avg_mem[i] << "      " << Sorter.avg_cpu[i] << endl;
        }
    }
    outputFile.close();

   
    return 0;
}
