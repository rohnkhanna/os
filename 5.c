#pragma once
#include<list>
#include<iomanip>
#include<iostream>
#include<conio.h>
using namespace std;

class roundrobin 
{
       int *rq;
       int    n;
       int    q;
       int    *w;
       int    *t;
       int *a;
       list<int> order;
public:
       roundrobin(void);
       ~roundrobin(void);
       int read();
       void calc();
       void display();
};
roundrobin::roundrobin(void)
{
       rq=w=t=NULL;
}

roundrobin::~roundrobin(void)
{
       if(rq!=NULL)
       {
              delete[] rq;
              delete[] w;
              delete[] t;
              delete[] a;
       }
}
int roundrobin::read()
{
       int i;
       cout<<"Enter number of processes:";
       cin>>n;
       if(rq!=NULL)
       {
              delete[] rq;
              delete[] w;
              delete[] t;
       }
       try
       {
              rq=new int[n];
              w=new int[n];
              t=new int[n];
              a=new int[n];
       }
       catch(bad_alloc &ba)
       {
              cerr<<ba.what()<<endl;
              exit(1);
       }
       cout<<"Enter arrival times:\n";
       for(i=0;i<n;i++)
       {
              cin>>a[i];
       }
       cout<<"Enter request times:\n";
       for(i=0;i<n;i++)
       {
              cin>>rq[i];
              w[i]=t[i]=0;
       }
       cout<<"Enter time quantum:";
       cin>>q;
       return 1;
}
void roundrobin::calc()
       int j=0;
       int    time;
       int k;
       int i;
       int *r;
       try
       {
              r=new int[n];
       }
       catch(bad_alloc &ba)
       {
              cerr<<ba.what()<<endl;
              exit(1);
       }
       for(i=0;i<n;i++)     r[i]=rq[i];
       bool f=false;
       int sp=0;
       for(i=0;j<n;i=(i+1)%n)
       {
              if(r[i]>0&&sp>=a[i])
              {
                     f=true;
                     if(r[i]<=q)
                            time=r[i];
                     else   time=q;
                    
                     t[i]+=time,r[i]-=time,order.push_back(i+1);
                     if(r[i]==0)   j++;
                     for(k=0;k<n;k++)
                            if(r[k]!=0&&k!=i&&a[k]<sp+time)
                                   if(!(a[k]<=sp))
                                          w[k]+=sp+time-a[k],t[i]+=sp+time-a[k];
                                       else
                                          w[k]+=time,t[k]+=time;
                     sp+=time;
                     continue;
              }
              if(i==n-1)
              {
                     if(!f)
                  
                     {
                            int it;
                            int diff=0;
                            for(it=0;it<n;it++)
                                   if(sp<a[it])
                                   {
                                          if(diff==0)   diff=a[it]-sp;
                                          else if(diff>a[it]-sp)      diff=a[it]-sp;
                                   }
                            sp+=diff;
                     }
                     f=false;
              }
       }
       delete[] r;
}
void roundrobin::display()
{
       int i;
       float tav=0;
       float wav=0;                 
       for(i=0;i<n;i++)
              tav+=t[i],wav+=w[i];
       tav/=n,wav/=n;
       cout<<"Scheduling order:\n";
       list<int>::iterator oi;            
       for(oi=order.begin();oi!=order.end();oi++)
              cout<<*oi<<"\t";
       cout<<"\nAverage turn-around time = "<<tav<<endl<<"Average wait time = "<<wav<<endl;
}
int main()
{
       roundrobin r;
       r.read();
       r.calc();
       r.display();
       cout<<"Press any key to exit...";
       _getch();
}