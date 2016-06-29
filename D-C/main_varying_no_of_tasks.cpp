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
    f_weighted.open("./results/weighted_results.txt",ios::out);
    if(!f_weighted)
    {
        cout<<"\nError opening file: ./results/weighted_results.txt";
        exit(1);
    }
    f_normal.open("./results/normal_results.txt",ios::out);
    if(!f_normal)
    {
        cout<<"\nError opening file: ./results/normal_results.txt";
        exit(1);
    }
    
/******************************CONTROL VARIABLES******************************/    
    int print_basic=0;
    int print_log=0;
    int print_inter=1;
    
     
    int MAX_NO_OF_TASKS=30;
    int MAX_TASKSETS_PER_SIMULATION=100;
    int number_of_tasks=6;
    int MAX_PERIOD=495;
    int MIN_PERIOD=5;
    float DEADLINE_FRACTION=1;
    float npr_percentage=0.1;
    
    float NO_OF_PROCESSORS=(float)4.0000;   
    int MAX_TIME=10000;

    int DM=1;
    int DC=2;
/******************************CONTROL VARIABLES******************************/
    
    int no_of_preemptions_fps=0;
    int no_of_preemptions_rds=0;
    int no_of_preemptions_ads=0;
    
    int no_of_preemptions_fps_d_c=0;
    int no_of_preemptions_rds_d_c=0;
    int no_of_preemptions_ads_d_c=0;
    
    int no_of_preemptions_edf=0;
    int no_of_preemptions_rds_edf=0;
    int no_of_preemptions_ads_edf=0;
    
    float fps=0.0000, rds=0.0000, ads=0.0000;
    float fps_d_c=0.0000, rds_d_c=0.0000, ads_d_c=0.0000;
    float edf=0.0000, rds_edf=0.0000, ads_edf=0.0000;
    float util_sum=0.0000;   

    
    while(number_of_tasks<=MAX_NO_OF_TASKS){
        
        float cur_util=1;
        float max_util=NO_OF_PROCESSORS/2;
        
        fps=0.0000;
        rds=0.0000;
        ads=0.0000;
        edf=0.0000;
        rds_edf=0.0000;
        ads_edf=0.0000;
        
        fps_d_c=0.0000;
        rds_d_c=0.0000;
        ads_d_c=0.0000;
        
        util_sum=0.0000;
        
        no_of_preemptions_fps=0;
        no_of_preemptions_rds=0;
        no_of_preemptions_ads=0;
        
        no_of_preemptions_fps_d_c=0;
        no_of_preemptions_rds_d_c=0;
        no_of_preemptions_ads_d_c=0;
        

        no_of_preemptions_edf=0;
        no_of_preemptions_rds_edf=0;
        no_of_preemptions_ads_edf=0;        
        
        
        cout<<"\nNumber of tasks: "<<number_of_tasks<<"\n";  

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
                create_tasks(number_of_tasks,taskset);  
                if(taskset==NULL)
                {
                    cout<<"\n*****************task set not created ! *****************";
                    exit(1);
                }
                else if(print_log)
                    cout<<"\nCreated task set :"<<counter<<"\n";
                
                while(!successful_parameter_generation)
                {                   
                    if(UUniFast(number_of_tasks,cur_util,taskset,DEADLINE_FRACTION, MAX_PERIOD, MIN_PERIOD)==1)
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
                create_NPRS(taskset,NO_OF_PROCESSORS,npr_percentage);
                //print_tasks(taskset);

                float taskset_util=return_utilization(taskset);
                int preemptions_fps=0, preemptions_rds=0, preemptions_ads=0, count=0, count1=0, count2, count3, count4, count5,count6;               
                int preemptions_edf=0, preemptions_rds_edf=0, preemptions_ads_edf=0;
                int preemptions_fps_d_c=0, preemptions_rds_d_c=0, preemptions_ads_d_c=0, count7, count8, count9; 
                
                util_sum+=taskset_util;
                count=count_tasks(taskset);
                if(count!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks generated (expected: "<<number_of_tasks<<" Actual: "<<count<<" )\n\n";
                    exit(1);
                           
                }
        
                preemptions_fps=g_fps_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                no_of_preemptions_fps+=preemptions_fps;
                fps+=taskset_util*preemptions_fps;
                
                count1=count_tasks(taskset);
                if(count1!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS (expected: "<<number_of_tasks<<" Actual: "<<count1<<" )\n\n";
                    exit(1);          
                }
                
                preemptions_rds=rds_schedule_modified(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds+=taskset_util*preemptions_rds;
                no_of_preemptions_rds+=preemptions_rds;
                
                count2=count_tasks(taskset);
                if(count2!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS (expected: "<<count1<<" Actual: "<<count2<<" )\n\n";
                    exit(1);       
                }
                
                preemptions_ads=ads_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads+=taskset_util*preemptions_ads;
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
                edf+=taskset_util*preemptions_edf;
                no_of_preemptions_edf+=preemptions_edf;
                
                count4=count_tasks(taskset);
                if(count4!=count3)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after P-EDF (expected: "<<count3<<" Actual: "<<count4<<" )\n\n";
                    exit(1);       
                }                
                
                preemptions_rds_edf=rds_edf_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds_edf+=taskset_util*preemptions_rds_edf;
                no_of_preemptions_rds_edf+=preemptions_rds_edf;

                count5=count_tasks(taskset);
                if(count5!=count4)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after EDF-RDS (expected: "<<count4<<" Actual: "<<count5<<" )\n\n";
                    exit(1);       
                }   
                
                preemptions_ads_edf=ads_edf_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads_edf+=taskset_util*preemptions_ads_edf;
                no_of_preemptions_ads_edf+=preemptions_ads_edf;

                count6=count_tasks(taskset);
                if(count6!=count5)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after P-EDF (expected: "<<count5<<" Actual: "<<count6<<" )\n\n";
                    exit(1);       
                }   
                
//                if(preemptions_ads_edf>preemptions_ads)
//                {
//                    print_tasks(taskset);
//                    cout<<"\nADS EDF scheduler:\n";
//                    ads_edf_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, 1);
//                    cout<<"\n\n\nADS FPS schedule: ";
//                    
//                    ads_schedule(taskset,NO_OF_PROCESSORS,MAX_TIME,1);
//                    exit(0);
//                }
                
                taskset=sort_task_set(taskset, DC);
                preemptions_fps_d_c=g_fps_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                no_of_preemptions_fps_d_c+=preemptions_fps_d_c;
                fps_d_c+=taskset_util*preemptions_fps_d_c;
                
                count7=count_tasks(taskset);
                if(count7!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS_d_M (expected: "<<number_of_tasks<<" Actual: "<<count1<<" )\n\n";
                    exit(1);          
                }
                
                preemptions_rds_d_c=rds_schedule_modified(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds_d_c+=taskset_util*preemptions_rds_d_c;
                no_of_preemptions_rds_d_c+=preemptions_rds_d_c;
                
                count8=count_tasks(taskset);
                if(count8!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS_d_c (expected: "<<count1<<" Actual: "<<count2<<" )\n\n";
                    exit(1);       
                }
                
                preemptions_ads_d_c=ads_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads_d_c+=taskset_util*preemptions_ads_d_c;
                no_of_preemptions_ads_d_c+=preemptions_ads_d_c;
                
                count9=count_tasks(taskset);
                if(count9!=count2)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after ADS_d_c (expected: "<<count2<<" Actual: "<<count3<<" )\n\n";
                    exit(1);       
                }
                
/****************************************************************************************YOUR CODE GOES ABOVE****************************************************************************************/                
              
                delete_taskset(taskset,print_log); 
                counter++;
            }
            
            f_normal<<"\n"<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
            f_normal<<"\t"<<(no_of_preemptions_edf/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds_edf/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads_edf/MAX_TASKSETS_PER_SIMULATION);
            f_normal<<"\n"<<(no_of_preemptions_fps_d_c/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds_d_c/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads_d_c/MAX_TASKSETS_PER_SIMULATION);
            
            if(print_inter)
            {
                cout<<"\nUtilization: "<<cur_util;
                cout<<"\nPreemptions P-FPS: "<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS : "<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-FPS : "<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
                
                cout<<"\nPreemptions P-FPS_d_c: "<<(no_of_preemptions_fps_d_c/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS_d_c: "<<(no_of_preemptions_rds_d_c/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-FPS_d_c: "<<(no_of_preemptions_ads_d_c/MAX_TASKSETS_PER_SIMULATION);

                cout<<"\nPreemptions P-EDF: "<<(no_of_preemptions_edf/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-EDF : "<<(no_of_preemptions_rds_edf/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-EDF : "<<(no_of_preemptions_ads_edf/MAX_TASKSETS_PER_SIMULATION);
            }
            
            cur_util=cur_util+0.1;
        }
        
        f_weighted<<(fps/util_sum)<<"\t"<<(rds/util_sum)<<"\t"<<(ads/util_sum)<<"\t"<<(edf/util_sum)<<"\t"<<(rds_edf/util_sum)<<"\t"<<(ads_edf/util_sum)<<"\t"<<(fps_d_c/util_sum)<<"\t"<<(rds_d_c/util_sum)<<"\t"<<(ads_d_c/util_sum)<<"\n";
        f_normal<<"\n\n";
        
        cout<<"\n\nWeighted preemptions P-FPS: "<<(fps/util_sum)<<" Weighted preemptions RDS-FPS: "<<(rds/util_sum)<<" Weighted preemptions ADS-FPS: "<<(ads/util_sum);
        cout<<"\n\nWeighted preemptions P-FPS_d_c: "<<(fps_d_c/util_sum)<<" Weighted preemptions RDS-FPS_d_c: "<<(rds_d_c/util_sum)<<" Weighted preemptions ADS-FPS_d_c: "<<(ads_d_c/util_sum);
        cout<<"\nWeighted preemptions P-EDF: "<<(edf/util_sum)<<" Weighted preemptions RDS-EDF: "<<(rds_edf/util_sum)<<" Weighted preemptions ADS-EDF: "<<(ads_edf/util_sum)<<"\n\n";
        
        number_of_tasks+=2;
    }
    
    
    f_normal.close();
    f_weighted.close();

    return 0;
}