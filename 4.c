#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
struct Process {
	int time,Atime,Btime,id;
	clock_t arrival;
	int flag,completed,p;
	sem_t se;
	struct Process *next;
	int priority;
	int WT;
};
int i=0,k=0;
typedef struct Process node;
clock_t start;
float A_TAT=0,A_WT=0;
node *front=NULL,*rear=NULL,*temp;
void *processor(node *S) {
	clock_t count;
	while(1) {
		sem_wait(&S->se);
		if((S->Atime<=(clock()-start)/CLOCKS_PER_SEC && S->p==1)) {
			S->p=0;
			count=clock();
		}
		if(S->flag==1) {
			printf("\nProcess-%d Running \nTimer :%d",S->id,(clock()-start)/CLOCKS_PER_SEC);
			S->flag=0;
			S->arrival=clock();
		}
		if((clock()-count)/CLOCKS_PER_SEC==1) {
			count=clock();
			printf("\nTimer :%d",(clock()-start)/CLOCKS_PER_SEC);
			S->time-=1;
			if(S->time==0) {
				S->WT=((clock()-start)/CLOCKS_PER_SEC)-S->Btime-S->Atime;
				A_TAT+=(clock()-start)/CLOCKS_PER_SEC-S->Atime;
				A_WT+=((clock()-start)/CLOCKS_PER_SEC)-S->Btime-S->Atime;
				sleep(2);
				node *start=rear;
				while(start!=NULL) {
					if(start->next==S) {
						start->next=S->next;
						break;
					}
					if(rear==S) {
						rear=rear->next;
						break;
					}
					start=start->next;
				}
				printf("\nProcess-%d Completed ",S->id);
				if(rear!=NULL){
					printf("next Process-%d",rear->id);
				}
				S->completed=7;
				if(rear!=NULL){
					sem_post(&rear->se);
				}
			}
			}
					if(S->completed==7) {
			break;
		}
	sem_post(&S->se);
	}
} 
void spush(node *temp) {
	int k;
	node *start=rear;
	k=temp->priority;
	k=1+(temp->WT/temp->time);
	if(rear==NULL) {
		rear=temp;
		rear->next=NULL;
	}
	else{
	int t=temp->time;
	if(start->priority<k){
		temp->next = rear;
        rear=temp;
	}
	else if (start->time > t) {
        	temp->next = rear;
        	rear=temp;
    	}
    else {
        	while (start->next != NULL && start->next->time< t) {
            		start = start->next;
        	}
        temp->next = start->next;
        start->next = temp;
    }
	}
}
void push() {
	temp=(node *)malloc(sizeof(node));
	printf("\nEnter Arrival Time of %d Process :",(i+1));
	scanf("%d",&temp->Atime);
	printf("Enter Burst Time :");
	scanf("%d",&temp->time);
	temp->id=i+1;
	temp->p=1;
	temp->flag=1;
	temp->Btime=temp->time;
	temp->priority=1+(temp->WT/temp->time);
	sem_init(&temp->se,0,0);
	int t=temp->Atime;
	node *start=front;
	if (start->Atime > t) {
        	temp->next = front;
        	front=temp;
    	}
    	else {
        	while (start->next != NULL && start->next->Atime < t) {
            		start = start->next;
        	}
        temp->next = start->next;
        start->next = temp;
    }
}
void main() {
	int n,l=1;
	pthread_t p[10];
	printf("\nEnter No.of Processes :");
	scanf("%d",&n);
	while(i<n) {
		if(front==NULL) {
			front=(node *)malloc(sizeof(node));
			printf("Enter Arrival Time of %d Process :",(i+1));
			scanf("%d",&front->Atime);
			printf("Enter Burst Time :");
			scanf("%d",&front->time);
			front->id=i+1;
			front->flag=1;
			front->p=1;
			front->WT=0;
			front->Btime=front->time;
			printf("here");
			front->priority=1+(front->WT/front->time);
			sem_init(&front->se,0,0);
			front->next=NULL;
		}
		else {
			push();
		}
		i++;
	}
	i=0;
	
	start=clock();
	while(i<n) {
		temp=front;	
		if(temp->Atime<=0) {
			printf("Process-%d is Dicarded Due to Incorrect Arrival Time",temp->id);
			front=temp->next;
			temp=front;
			i++;
		}
		if(l==1) {
				l=0;
				sem_post(&temp->se);
			}
		if((clock()-start)/CLOCKS_PER_SEC==temp->Atime) {
		//	printf("Process-%d is created",temp->id);
			pthread_create(&p[i],NULL,processor,temp);
			front=front->next;
			spush(temp);
			i++;
		}
	}
	for(i=0;i<n;i++) {
		pthread_join(p[i],NULL);
	}
	printf("\nAverage Waiting Time :%f\nAverage Turn Around Time :%f",(float)A_WT/n,(float)A_TAT/n);
}