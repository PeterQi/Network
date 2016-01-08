#include <iostream>
#include<stdlib.h>
#define MAX_NUM 99999
#define ZERO 1e-5
using namespace std;
float abs(float a)
{
    if(a>0)
        return a;
    else
        return -a;
}

bool zero(float a)
{
    if (abs(a)<ZERO)
        return true;
    else
        return false;
}

bool k_means(int num, int k, float *samples, float *center, int *type)
{
    for (int i=0;i<num;i++)
    {
        float min_dis = MAX_NUM;
        for (int j = 0;j<k;j++)
        {
            float tmp_dis=abs(samples[i]-center[j]);
            if (tmp_dis<min_dis)
            {
                min_dis = tmp_dis;
                type[i]=j;
            }
        }
    }
    float new_center[k];
    int new_type_num[k];
    for (int i=0;i<k;i++)
    {
        new_center[i]=0;
        new_type_num[i]=0;
    }
    for (int i = 0;i<num;i++)
    {
        new_center[type[i]]+=samples[i];
        new_type_num[type[i]]++;
    }
    bool flag = false;
    for(int i=0;i<k;i++)
    {
        if(zero(new_type_num[i]))
        {
            new_center[i] = center[i];
            continue;
        }
        new_center[i]/=(float)new_type_num[i];

        bool find_flag = false;
        for(int j=0;j<k;j++)
            if(zero(new_center[i]-center[j]))
            {
                find_flag = true;
                break;
            }
        if(!find_flag)
        {
            flag = true;
        }

    }
    if(flag)
    {
        for(int i =0;i<k;i++)
            center[i]=new_center[i];
        return false;
    }
    return true;
}

bool k_medoids(int num, int k, float *samples, int *center, int *type)
{
    for (int i=0;i<num;i++)
    {
        float min_dis = MAX_NUM;
        for (int j = 0;j<k;j++)
        {
            float tmp_dis=abs(samples[i]-samples[center[j]]);
            if (tmp_dis<min_dis)
            {
                min_dis = tmp_dis;
                type[i]=j;
            }
        }
    }
    int new_center[k];
    float sample_cost[num];
    int new_type_num[k];
    for (int i=0;i<k;i++)
    {
        new_center[i]=center[i];
        new_type_num[i]=0;
    }

    for (int i = 0;i<num;i++)
    {
        sample_cost[i]=0;
        for(int j =0;j<num;j++)
        {
            if(type[j]==type[i])
            {
                sample_cost[i]+=abs(samples[j]-samples[i]);
            }
        }
    }
    for (int i =0;i<num;i++)
    {
        if(sample_cost[i]<sample_cost[new_center[type[i]]])
        {
            new_center[type[i]]=i;
        }
    }
    bool flag = false;
    for(int i=0;i<k;i++)
    {
        bool find_flag = false;
        for(int j=0;j<k;j++)
            if(new_center[i]==center[j])
            {
                find_flag = true;
                break;
            }
        if(!find_flag)
        {
            flag = true;
        }

    }
    if(flag)
    {
        for(int i =0;i<k;i++)
            center[i]=new_center[i];
        return false;
    }
    return true;
}

int main()
{
    int num;
    int k;
    cout<<"Please input the number of samples:"<<endl;
    cin >> num;
    cout<<"Please input the samples:"<<endl;
    float *samples = new float[num];
    int *ty = new int [num];
    for (int i = 0;i<num;i++)
    {
        ty[i] = i;
        cin>>samples[i];
    }
    cout<<"Please input the k:"<<endl;
    cin>>k;
    float *center = new float[k];
    for (int i = 0;i < k; i++)
    {
        center[i] = i;
    }
    int co = 0;
    while(!k_means(num,k,samples,center,ty));
    cout<<"Centers:"<<endl;
    for(int i =0;i<k;i++)
    {
        cout<<center[i]<<' ';
    }
    cout<<endl;
    for(int i =0;i<k;i++)
    {
        cout<<"Type "<<i+1<<":";
        for(int j =0;j<num;j++)
        {
            if(ty[j]==i)
                cout<<samples[j]<<' ';
        }
        cout<<endl;
    }
    cout<<endl;
    int *center2 = new int[k];
    for (int i = 0;i < k; i++)
    {
        center2[i] = k-i;
    }

    while(!k_medoids(num,k,samples,center2,ty));
    cout<<"Centers:"<<endl;
    for(int i =0;i<k;i++)
    {
        cout<<samples[center2[i]]<<' ';
    }
    cout<<endl;
    for(int i =0;i<k;i++)
    {
        cout<<"Type "<<i+1<<":";
        for(int j =0;j<num;j++)
        {
            if(ty[j]==i)
                cout<<samples[j]<<' ';
        }
        cout<<endl;
    }
}
