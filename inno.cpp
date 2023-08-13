//Scheduling Algorithms
#include<bits/stdc++.h>
#include<iomanip>
using namespace std;
const int N = 1e5+6;
vector<int> arrival_time , burst_time , gantt_chart;
vector<int> turn_arround_time(N),completion_time(N),waiting_time(N),response_time(N);
float avg_turn_arround_time , avg_waiting_time  , avg_response_time , throughput ;
float timer;
void _print(int n){
    avg_response_time = avg_waiting_time = avg_turn_arround_time=0;
    throughput = timer/n;
    cout<<endl;
    cout<<"****************"<<endl;
    cout<<endl;
    cout<<" [ Here -1 means cpu is idle ] "<<endl;
    cout<<endl;
    cout<<"Gantt chart : ";
    for(auto &x:gantt_chart){
        cout<<x<<"=>";
    }
    cout<<"-1";
    cout<<endl;
    cout<<setw(5)<<"Completion time :    "<<setw(5);
    for(auto &x:completion_time){
        cout<<setw(5)<<x<<setw(5);
    }
    cout<<endl;
    cout<<setw(5)<<"Turn arround time :  "<<setw(5);
    for(auto &x:turn_arround_time){
        cout<<setw(5)<<x<<setw(5);
        avg_turn_arround_time+=x;
    }
    cout<<endl;
    cout<<setw(5)<<"Waiting time :       "<<setw(5);
    for(auto &x:waiting_time){
        cout<<setw(5)<<x<<setw(5);
        avg_waiting_time+=x;
    }
    cout<<endl;
    cout<<endl;
    cout<<"Average turn arround time is "<<avg_turn_arround_time/n<<endl;
    cout<<"Average waiting time is "<<avg_waiting_time/n<<endl;
    cout<<endl;
    cout<<"****************"<<endl;
    cout<<endl;
}
int main(){
    // input reading from file
    string filename("input.txt");
    ifstream in(filename);
    vector<string> lines;
    string line;
    while (getline(in, line)){
        lines.push_back(line);
    }

    int n = lines.size()-1;
    arrival_time.resize(n);
    burst_time.resize(n);

    for(int i=1 ; i<=n ; i++){
        vector<int> temp;
        for(int j=0 ; j<lines[i].size() ; ){
            while(lines[i][j]==' ')j++;
            temp.push_back(lines[i][j]-'0');
            j++;
        }
        arrival_time[temp[0]-1] = temp[1];
        burst_time[temp[0]-1] = temp[2];
    }
    // testing for input file read
    cout<<"Arrival time : ";
    for(auto &x:arrival_time)cout<<x<<" ";
    cout<<endl;
    cout<<"Burst time : ";
    for(auto &x:burst_time)cout<<x<<" ";
    cout<<endl;

    while(1){
        cout<<"MENU : \n";
        cout<<"Press 1 for FCFS"<<endl;
        cout<<"Press 2 for SJF"<<endl;
        cout<<"Press 3 for SRTF"<<endl;
        cout<<"Press 4 for RR"<<endl;
        cout<<"Press 5 for EXIT"<<endl;
        cout<<"Enter your choise"<<endl;
        int ch ;
        cin>>ch;
        if(ch==1){
            // FCFS
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
            for(int i=0 ; i<n ; i++){
                pq.push({arrival_time[i],i+1});
            }
            turn_arround_time.resize(n);
            waiting_time.resize(n);
            response_time.resize(n);
            completion_time.resize(n);

            timer = 0 ;
            gantt_chart.clear();
            while(!pq.empty()){
                auto take = pq.top();
                pq.pop();
                int id = take.second;
                int arr = take.first;
                while(timer<arr){
                    timer++;
                    gantt_chart.push_back(-1);
                }
                int t = burst_time[id-1];
                while(t--){
                    gantt_chart.push_back(id);
                    timer++;
                }
                completion_time[id-1] = timer;
            }

            for(int i=0 ; i<n ;i++){
                turn_arround_time[i] = completion_time[i] - arrival_time[i];
            }
            for(int i=0 ; i<n ;i++){
                waiting_time[i] = turn_arround_time[i] - burst_time[i];
            }
            // because of non-premptive
            response_time = waiting_time;
            _print(n);
        }
        else if(ch==2){
            // SJF
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq_arrival;
            for(int i=0 ; i<n ; i++){
                pq_arrival.push(make_pair(arrival_time[i],i+1));
            }
            turn_arround_time.resize(n);
            waiting_time.resize(n);
            response_time.resize(n);
            completion_time.resize(n);

            timer = 0 ;
            gantt_chart.clear();
            while(1){
                while(!pq_arrival.empty() && pq_arrival.top().first<=timer){
                    int arr = pq_arrival.top().first;
                    int id = pq_arrival.top().second;
                    pq.push({burst_time[id-1],id});
                    pq_arrival.pop();   
                }

                if(pq.empty()){
                    if(pq_arrival.empty())break;
                    else{
                        auto it = pq_arrival.top();
                        pq_arrival.pop();
                        int arr = it.first;
                        int id = it.second;
                        while(timer<arr){
                            timer++;
                            gantt_chart.push_back(-1);
                        }
                        pq.push(make_pair(burst_time[id-1],id));
                    }
                }
                else{
                    auto take = pq.top();
                    pq.pop();
                    int t = take.first;
                    int id = take.second;
                    while(t--){
                        timer++;
                        gantt_chart.push_back(id);
                    }
                    completion_time[id-1] = timer;
                }
            }
            for(int i=0 ; i<n ;i++){
                turn_arround_time[i] = completion_time[i] - arrival_time[i];
            }
            for(int i=0 ; i<n ;i++){
                waiting_time[i] = turn_arround_time[i] - burst_time[i];
            }
            // because of non-premptive
            response_time = waiting_time;
            _print(n);
        }
        else if(ch==3){
            // SRTF
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq_arrival;
            for(int i=0 ; i<n ; i++){
                pq_arrival.push(make_pair(arrival_time[i],i+1));
            }
            turn_arround_time.resize(n);
            waiting_time.resize(n);
            response_time.resize(n);
            completion_time.resize(n);

            timer = 0 ;
            gantt_chart.clear();
            while(1){
                while(!pq_arrival.empty() && pq_arrival.top().first<=timer){
                    int arr = pq_arrival.top().first;
                    int id = pq_arrival.top().second;
                    pq.push({burst_time[id-1],id});
                    pq_arrival.pop();
                }

                if(pq.empty()){
                    if(pq_arrival.empty())break;
                    else{
                        auto it = pq_arrival.top();
                        pq_arrival.pop();
                        int arr = it.first;
                        int id = it.second;
                        while(timer<arr){
                            timer++;
                            gantt_chart.push_back(-1);
                        }
                        pq.push(make_pair(burst_time[id-1],id));
                    }
                }
                else{
                    auto take = pq.top();
                    pq.pop();
                    int t = take.first;
                    int id = take.second;
                    t--;
                    gantt_chart.push_back(id);
                    timer++;
                    if(t!=0){
                        pq.push({t,id});
                    }
                    else completion_time[id-1] = timer;
                }
            }

            // response time is not equal to waiting time because of premptive
            fill(response_time.begin(),response_time.end(),-1);
            for(int i=0 ; i<gantt_chart.size() ; i++){
                if(response_time[gantt_chart[i]-1]!=-1)continue;
                response_time[gantt_chart[i]-1] = i;
            }
            for(int i=0 ; i<n ;i++){
                response_time[i] -= arrival_time[i];
            }

            for(int i=0 ; i<n ;i++){
                turn_arround_time[i] = completion_time[i] - arrival_time[i];
            }
            for(int i=0 ; i<n ;i++){
                waiting_time[i] = turn_arround_time[i] - burst_time[i];
            }
            _print(n);
        }
        else if(ch==4){
            // RR
            cout<<"Enter time quantum for this method "<<endl;
            int tm = 2;
            cin>>tm;
            queue<pair<int,int>> pq;
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq_arrival;
            for(int i=0 ; i<n ; i++){
                pq_arrival.push(make_pair(arrival_time[i],i+1));
            }
            turn_arround_time.resize(n);
            waiting_time.resize(n);
            response_time.resize(n);
            completion_time.resize(n);

            timer = 0 ;
            gantt_chart.clear();
            while(1){
                if(pq.empty()){
                    if(pq_arrival.empty())break;
                    else{
                        auto it = pq_arrival.top();
                        pq_arrival.pop();
                        int arr = it.first;
                        int id = it.second;
                        while(timer<arr){
                            timer++;
                            gantt_chart.push_back(-1);
                        }
                        pq.push(make_pair(burst_time[id-1],id));
                    }
                }
                else{
                    auto take = pq.front();
                    pq.pop();
                    int t = take.first;
                    int id = take.second;
                    if(t>tm){
                        for(int i=0 ; i<tm ;i++){
                            timer++;
                            gantt_chart.push_back(id);
                        }
                        t-=tm;
                    }
                    else{
                        for(int i=0 ; i<t ;i++){
                            timer++;
                            gantt_chart.push_back(id);
                        }
                        t-=t;
                    }
                    while(!pq_arrival.empty() && pq_arrival.top().first<=timer){
                        int arr = pq_arrival.top().first;
                        int id = pq_arrival.top().second;
                        pq.push({burst_time[id-1],id});
                        pq_arrival.pop();
                    }
                    if(t!=0){
                        pq.push({t,id});
                    }
                    else completion_time[id-1] = timer;
                }
            }

            // response time is not equal to waiting time because of premptive
            fill(response_time.begin(),response_time.end(),-1);
            for(int i=0 ; i<gantt_chart.size() ; i++){
                if(response_time[gantt_chart[i]-1]!=-1)continue;
                response_time[gantt_chart[i]-1] = i;
            }
            for(int i=0 ; i<n ;i++){
                response_time[i] -= arrival_time[i];
            }

            for(int i=0 ; i<n ;i++){
                turn_arround_time[i] = completion_time[i] - arrival_time[i];
            }
            for(int i=0 ; i<n ;i++){
                waiting_time[i] = turn_arround_time[i] - burst_time[i];
            }
            _print(n);
        }
        else if(ch==5){
            break;
        }
        else{
            cout<<"Please enter right choise !"<<endl;
        }
    }

    return 0;
}


/*
OUTPUT


Arrival time : 0 2 4 6 8 
Burst time : 3 6 4 5 2 
MENU : 
Press 1 for FCFS
Press 2 for SJF
Press 3 for SRTF
Press 4 for RR
Press 5 for EXIT
Enter your choise
1

****************

 [ Here -1 means cpu is idle ] 

Gantt chart : 1=>1=>1=>2=>2=>2=>2=>2=>2=>3=>3=>3=>3=>4=>4=>4=>4=>4=>5=>5=>-1
Completion time :        3    9   13   18   20
Turn arround time :      3    7    9   12   12
Waiting time :           0    1    5    7   10

Average turn arround time is 8.6
Average waiting time is 4.6

****************

MENU :
Press 1 for FCFS
Press 2 for SJF
Press 3 for SRTF
Press 4 for RR
Press 5 for EXIT
Enter your choise
2

****************

 [ Here -1 means cpu is idle ]

Gantt chart : 1=>1=>1=>2=>2=>2=>2=>2=>2=>5=>5=>3=>3=>3=>3=>4=>4=>4=>4=>4=>-1
Completion time :        3    9   15   20   11
Turn arround time :      3    7   11   14    3
Waiting time :           0    1    7    9    1

Average turn arround time is 7.6
Average waiting time is 3.6

****************

MENU :
Press 1 for FCFS
Press 2 for SJF
Press 3 for SRTF
Press 4 for RR
Press 5 for EXIT
Enter your choise
3

****************

 [ Here -1 means cpu is idle ]

Gantt chart : 1=>1=>1=>2=>3=>3=>3=>3=>5=>5=>2=>2=>2=>2=>2=>4=>4=>4=>4=>4=>-1
Completion time :        3   15    8   20   10
Turn arround time :      3   13    4   14    2
Waiting time :           0    7    0    9    0

Average turn arround time is 7.2
Average waiting time is 3.2

****************

MENU :
Press 1 for FCFS
Press 2 for SJF
Press 3 for SRTF
Press 4 for RR
Press 5 for EXIT
Enter your choise
4
Enter time quantum for this method 
2

****************

 [ Here -1 means cpu is idle ]

Gantt chart : 1=>1=>2=>2=>1=>3=>3=>2=>2=>4=>4=>3=>3=>5=>5=>2=>2=>4=>4=>4=>-1
Completion time :        5   17   13   20   15
Turn arround time :      5   15    9   14    7
Waiting time :           2    9    5    9    5

Average turn arround time is 10
Average waiting time is 6

****************

MENU :
Press 1 for FCFS
Press 2 for SJF
Press 3 for SRTF
Press 4 for RR
Press 5 for EXIT
Enter your choise
5
*/

