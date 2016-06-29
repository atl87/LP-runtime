/* 
 * File:   main.cpp
 * Author: atl05
 *
 * Created on den 3 juni 2015, 16:08
 */

#include <cstdlib>
#include<iostream>
#include<stdio.h>
#include "Tasks.h"
#include "Task_Gen.h"
#include "scheduler.h"
#include"edf_schedulers.h"
#include<math.h>
#include <fstream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    srand (time(NULL));
    fstream f_weighted, f_normal;
    f_weighted.open("./results/weighted_results_varying_NPR.txt",ios::out);
    if(!f_weighted)
    {
        cout<<"\nError opening file: ./results/weighted_results_varying_NPR.txt";
        exit(1);
    }
    f_normal.open("./results/normal_results_varying_NPR.txt",ios::out);
    if(!f_normal)
    {
        cout<<"\nError opening file: ./results/normal_results_varying_NPR.txt";
        exit(1);
    }
    
/******************************CONTROL VARIABLES******************************/    
    int print_basic=0;
    int print_log=0;
    float NO_OF_PROCESSORS=(float)4.0000;    
    int MAX_NO_OF_TASKS=30;
    int MAX_TASKSETS_PER_SIMULATION=20;
    int MAX_PERIOD=450;
    int MIN_PERIOD=50;
    int MAX_TIME=5000;
    float DEADLINE_FRACTION=1;
    float npr_percentage=0.1;
    int print_inter=1;
    float cur_util=1;
    float max_util=NO_OF_PROCESSORS/2;
    int DM=1;
    int DC=2;
/******************************CONTROL VARIABLES******************************/
    
    int no_of_preemptions_fps=0;
    int no_of_preemptions_rds=0;
    int no_of_preemptions_ads=0;
    
    int no_of_preemptions_fps_d_m=0;
    int no_of_preemptions_rds_d_m=0;
    int no_of_preemptions_ads_d_m=0;
    
    int no_of_preemptions_edf=0;
    int no_of_preemptions_rds_edf=0;
    int no_of_preemptions_ads_edf=0;
    
    double fps[20], rds[20], ads[20], fps_d_m[20], rds_d_m[20], ads_d_m[20];;
    double edf[20], rds_edf[20], ads_edf[20];
    double util_sum=0.0000;   
    
        

    for(int i=0;i<20;i++)
    {
        fps[i]=0.0000;
        rds[i]=0.0000;
        ads[i]=0.0000;
        
        fps_d_m[i]=0.0000;
        rds_d_m[i]=0.0000;
        ads_d_m[i]=0.0000;
        
        edf[i]=0.0000;
        rds_edf[i]=0.0000;
        ads_edf[i]=0.0000;
    }
        
    util_sum=0.0000;

    no_of_preemptions_fps=0;
    no_of_preemptions_rds=0;
    no_of_preemptions_ads=0;
    
    no_of_preemptions_fps_d_m=0;
    no_of_preemptions_rds_d_m=0;
    no_of_preemptions_ads_d_m=0;

    no_of_preemptions_edf=0;
    no_of_preemptions_rds_edf=0;
    no_of_preemptions_ads_edf=0;        
        
        
    cout<<"\nNumber of tasks: "<<MAX_NO_OF_TASKS<<"\n";  

    while(cur_util<=max_util){
        if(print_basic)
            cout<<"\nUtilization: "<<cur_util<<"\n";

        int counter=1;
        while(counter<=MAX_TASKSETS_PER_SIMULATION)
        {
            int successful_parameter_generation=0;
            real_time_taskset *taskset=(real_time_taskset*) malloc(sizeof(real_time_taskset));
            if(print_log)
                cout<<"\n\nCreating real-time tasks: ";
            create_tasks(MAX_NO_OF_TASKS,taskset);  
            if(taskset==NULL)
            {
                cout<<"\n*****************task set not created ! *****************";
                exit(1);
            }
            else if(print_log)
                cout<<"\nCreated task set :"<<counter<<"\n";

            while(!successful_parameter_generation)
            {                   
                if(UUniFast(MAX_NO_OF_TASKS,cur_util,taskset,DEADLINE_FRACTION, MAX_PERIOD, MIN_PERIOD)==1)
                {
                    successful_parameter_generation=1; 
                    if(taskset==NULL)
                    {
                        cout<<"\n\n***************** parameters not generated ! *****************";
                        exit(1);
                    }
                    taskset=sort_task_set(taskset, DM);
                }
            }      
/****************************************************************************************YOUR CODE GOES HERE****************************************************************************************/                              
            int round=0;
            while(round<20)
            {
                npr_percentage=(float)0.05*(round+1);
                create_NPRS(taskset,NO_OF_PROCESSORS,npr_percentage);
                //print_tasks(taskset);

                float taskset_util=return_utilization(taskset);
                int preemptions_fps=0, preemptions_rds=0, preemptions_ads=0, count=0, count1=0, count2, count3, count4, count5,count6;               
                int preemptions_edf=0, preemptions_rds_edf=0, preemptions_ads_edf=0;
                int preemptions_fps_d_m=0, preemptions_rds_d_m=0, preemptions_ads_d_m=0, count7=0, count8=0, count9; 

                util_sum+=taskset_util;
                count=count_tasks(taskset);
                if(count!=MAX_NO_OF_TASKS)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks generated (expected: "<<MAX_NO_OF_TASKS<<" Actual: "<<count<<" )\n\n";
                    exit(1);

                }

                taskset=sort_task_set(taskset, DM);
                preemptions_fps=g_fps_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                no_of_preemptions_fps+=preemptions_fps;
                fps[round]+=taskset_util*preemptions_fps;

                count1=count_tasks(taskset);
                if(count1!=MAX_NO_OF_TASKS)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS (expected: "<<MAX_NO_OF_TASKS<<" Actual: "<<count1<<" )\n\n";
                    exit(1);          
                }

                preemptions_rds=rds_schedule_modified(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds[round]+=taskset_util*preemptions_rds;
                no_of_preemptions_rds+=preemptions_rds;

                count2=count_tasks(taskset);
                if(count2!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS (expected: "<<count1<<" Actual: "<<count2<<" )\n\n";
                    exit(1);       
                }

                preemptions_ads=ads_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads[round]+=taskset_util*preemptions_ads;
                no_of_preemptions_ads+=preemptions_ads;

                count3=count_tasks(taskset);
                if(count3!=count2)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after ADS (expected: "<<count2<<" Actual: "<<count3<<" )\n\n";
                    exit(1);       
                }

                /******EDF starts here****/

                preemptions_edf=g_edf_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                edf[round]+=taskset_util*preemptions_edf;
                no_of_preemptions_edf+=preemptions_edf;

                count4=count_tasks(taskset);
                if(count4!=count3)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after P-EDF (expected: "<<count3<<" Actual: "<<count4<<" )\n\n";
                    exit(1);       
                }                

                preemptions_rds_edf=rds_edf_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds_edf[round]+=taskset_util*preemptions_rds_edf;
                no_of_preemptions_rds_edf+=preemptions_rds_edf;

                count5=count_tasks(taskset);
                if(count5!=count4)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after EDF-RDS (expected: "<<count4<<" Actual: "<<count5<<" )\n\n";
                    exit(1);       
                }   

                preemptions_ads_edf=ads_edf_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads_edf[round]+=taskset_util*preemptions_ads_edf;
                no_of_preemptions_ads_edf+=preemptions_ads_edf;

                count6=count_tasks(taskset);
                if(count6!=count5)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after P-EDF (expected: "<<count5<<" Actual: "<<count6<<" )\n\n";
                    exit(1);       
                }
                
                
                taskset=sort_task_set(taskset, DC);
                preemptions_fps_d_m=g_fps_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                no_of_preemptions_fps_d_m+=preemptions_fps_d_m;
                fps_d_m[round]+=taskset_util*preemptions_fps_d_m;

                count7=count_tasks(taskset);
                if(count7!=MAX_NO_OF_TASKS)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS_d_m (expected: "<<MAX_NO_OF_TASKS<<" Actual: "<<count7<<" )\n\n";
                    exit(1);          
                }

                preemptions_rds_d_m=rds_schedule_modified(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds_d_m[round]+=taskset_util*preemptions_rds_d_m;
                no_of_preemptions_rds_d_m+=preemptions_rds_d_m;

                count8=count_tasks(taskset);
                if(count8!=count7)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS_d_m (expected: "<<count7<<" Actual: "<<count8<<" )\n\n";
                    exit(1);       
                }

                preemptions_ads_d_m=ads_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads_d_m[round]+=taskset_util*preemptions_ads_d_m;
                no_of_preemptions_ads_d_m+=preemptions_ads_d_m;
                
                count9=count_tasks(taskset);
                if(count9!=MAX_NO_OF_TASKS)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS_d_m (expected: "<<MAX_NO_OF_TASKS<<" Actual: "<<count9<<" )\n\n";
                    exit(1);          
                }                
                
                round++;

            }

/****************************************************************************************YOUR CODE GOES ABOVE****************************************************************************************/                

            delete_taskset(taskset,print_log); 
            counter++;
        }

        f_normal<<"\n"<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
        f_normal<<"\t"<<(no_of_preemptions_edf/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds_edf/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads_edf/MAX_TASKSETS_PER_SIMULATION);
        f_normal<<"\t"<<(no_of_preemptions_fps_d_m/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds_d_m/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads_d_m/MAX_TASKSETS_PER_SIMULATION);

        if(print_inter)
        {
            cout<<"\nUtilization: "<<cur_util;
            cout<<"\nPreemptions P-FPS: "<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS : "<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION);
            cout<<" Preemptions ADS-FPS : "<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
            
            cout<<"\nPreemptions P-FPS_d_m: "<<(no_of_preemptions_fps_d_m/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS : "<<(no_of_preemptions_rds_d_m/MAX_TASKSETS_PER_SIMULATION);
            cout<<" Preemptions ADS-FPS : "<<(no_of_preemptions_ads_d_m/MAX_TASKSETS_PER_SIMULATION);


            cout<<"\nPreemptions P-EDF: "<<(no_of_preemptions_edf/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-EDF : "<<(no_of_preemptions_rds_edf/MAX_TASKSETS_PER_SIMULATION);
            cout<<" Preemptions ADS-EDF : "<<(no_of_preemptions_ads_edf/MAX_TASKSETS_PER_SIMULATION);
        }

        cur_util=cur_util+0.1;
    }
    
    
    for(int i=0;i<20;i++)
    {
        f_weighted<<(fps[i]/util_sum)<<"\t"<<(rds[i]/util_sum)<<"\t"<<(ads[i]/util_sum)<<"\t"<<(edf[i]/util_sum)<<"\t"<<(rds_edf[i]/util_sum)<<"\t"<<(ads_edf[i]/util_sum)<<"\t"<<(fps_d_m[i]/util_sum)<<"\t"<<(rds_d_m[i]/util_sum)<<"\t"<<(ads_d_m[i]/util_sum)<<"\n";
        f_normal<<"\n\n";
        
        cout<<"\n\nWeighted preemptions P-FPS: "<<(fps[i]/util_sum)<<" Weighted preemptions RDS-FPS: "<<(rds[i]/util_sum)<<" Weighted preemptions ADS-FPS: "<<(ads[i]/util_sum);
        cout<<"\n\nWeighted preemptions P-FPS_d_m: "<<(fps_d_m[i]/util_sum)<<" Weighted preemptions RDS-FPS_d_m: "<<(rds_d_m[i]/util_sum)<<" Weighted preemptions ADS-FPS_d_m: "<<(ads_d_m[i]/util_sum);
        cout<<"\nWeighted preemptions P-EDF: "<<(edf[i]/util_sum)<<" Weighted preemptions RDS-EDF: "<<(rds_edf[i]/util_sum)<<" Weighted preemptions ADS-EDF: "<<(ads_edf[i]/util_sum)<<"\n\n";
    }
        
    
    
    f_normal.close();
    f_weighted.close();

    return 0;
}