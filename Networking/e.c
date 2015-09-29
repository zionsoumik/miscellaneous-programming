#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>	
int shmid1,shmid2;
/*void release()
{
	if( shmctl(shmid1, IPC_RMID, NULL) == -1 ) 
	{ 
		perror("shmctl"); 
		exit(-1); 
	}
	if( shmctl(shmid2, IPC_RMID, NULL) == -1 ) 
	{ 
		perror("shmctl"); 
		exit(-1); 
	}
	kill -1;
	printf("\nProcess %d killed\n",getpid());
	exit(0);
}*/
int main()
{
	int n,p,i,d,msg,rp1,rp2,c=0,s=0,temp;
	char *shmptr1,*shmptr2;
	srand(time(0));
	//signal(SIGINT,release);
	there:
	printf("Enter the no. of process you want to create(min 1 and max 10):  ");
	scanf("%d",&n);
	if((n<1)||(n>10))
	{
		printf("\nPlease give value within declared range\n	\n");
		goto there;
	}
	for(i=0;i<n;i++)
	{
		p=fork();
		if(p==0)
		{
			sleep(3);
			printf("\nNode %d created with id %d\n",i+	1,getpid());
			if( (shmid2 = shmget(2041, 32, 0666|IPC_CREAT)) == -1 ) 
	        	{ 
	            		exit(1);  
	        	}
	        	shmptr2=shmat(shmid2,0,0);
	        	if (shmptr2 == (char *) -1)  
			{    
		        	exit(2);
			}
			shmptr2[s++]=getpid()%10;	 
			break;
		}
	}
	if(p==0)
	{
		while(1)
		{
			sleep(5);
			if( (shmid1 = shmget(2081, 32, 0)) == -1 ) 
	        	{             
	        		exit(1); 
	        	}
	        	if( (shmid2 = shmget(2041, 32, 0)) == -1 ) 
	        	{ 
	            		exit(1);  
	        	}  
	        	shmptr1 = shmat(shmid1, 0, 0); 
		        if (shmptr1 == (char *) -1)  
		        {    
		        	exit(2);
		        }
		        shmptr2=shmat(shmid2,0,0);
		        if (shmptr2 == (char *) -1)  
		        {    
		        	exit(2);
		        }
		        rp1=shmptr2[rand()%n]; 
			if(((getpid()%10)==rp1)&&(shmptr1[0]>=1)&&(shmptr1[1]==1))
			{
				temp=shmptr1[0];
				shmptr1[0]=0;
				sleep(3); 
		        	msg=rand()%100;
		        	again:
		        	rp2=shmptr2[rand()%n];
				if(rp1==rp2)
				{
					goto again;
				}
				printf("node id %d sending data %d to node id %d",rp1,msg,rp2);
				shmptr1[3]=rp2;
				shmptr1[4]=rp1;
				shmptr1[5]=msg%100;
				shmptr1[0]=temp;
				shmptr1[1]=0;
				if(c>=4)
				{
					shmptr1[2]=1;
					exit(0);
				}
			}	        	 	        		
			}
			if(((getpid()%10)==shmptr1[3])&&(shmptr1[0]>=1)&&(shmptr1[1]==0))
			{
				sleep(4);
				shmptr1[0]++;
				printf("node id %d receiving data %d from node id %d",shmptr1[3],shmptr1[5],shmptr1[4]);
		        	if(shmptr1[2]==1)
				{
					exit(0);
				}
			}
		}
	}
	else 
    	{ 
        	if( (shmid1 = shmget(2081, 32, 0666 | IPC_CREAT)) ==-1 ) 
	        { 
	            exit(1);  
	        } 
	        if( (shmid2 = shmget(2041, 32, 0666 | IPC_CREAT)== -1 ) 
	        { 
	            exit(1);  
	        } 
	        shmptr1 = shmat(shmid1, 0, 0); 
	        if (shmptr1 == (char *) -1) 
	        {
	            exit(2); 
	        }
	        shmptr2= shmat(shmid2,0,0);
	        if (shmptr2 == (char *) -1) 
/usr/local/bin/bash: line 1: q: command not found
	            exit(2); 
	        }
		shmptr1[0]=1;
		shmptr1[1]=1;
		shmptr1[2]=0;  
	        putchar('\n');
	        sleep(100); 
        } 
	return 0;
}


