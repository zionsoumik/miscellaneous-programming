%{


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int check();
int lookup(char *);
int print_output();
   
enum{org,mov,mvi,add,adc,sub,sbb,inr,inx,dcr,dcx,ana,xra,ora,cmp,lxi,lda,sta,jmp,jnz,jz,call,ret,label,lhld};
int pc,type,len;
char code[3];
FILE *fp;

struct symtbl {
	char *symbol;
	int address;
	struct symtbl *next;
}*symroot,*sym_current=NULL;
struct mcode {
	int s_add;
	char *code;
	int flag;
	struct mcode *next;
}*mcode_root,*s_begin,*mcode_current=NULL;
%}

%%
[ \t]+ 
org {type=0;}
mov {type=1;len=1;}
mvi {type=2;len=2;}
add {type=3;len=1;}
adc {type=4;len=1;}
sub {type=5;len=1;}
sbb {type=6;len=1;}
inr {type=7;len=1;}
inx {type=8;len=1;}
dcr {type=9;len=1;}
dcx {type=10;len=1;}
ana {type=11;len=1;}
xra {type=12;len=1;}
ora {type=13;len=1;}
cmp {type=14;len=1;}
lxi {type=15;len=2;}
lda {type=16;len=3;}
sta {type=17;len=3;}
jmp {type=18;len=3;add2code(pc,"C3",1);}
jnz {type=19;len=3;}
jz {type=20;len=3;}
call {type=21;len=3;}
ret {type=22;len=1;}
[a-z,:0-9A-Z]+\: {type=23;len=0;newsymbol(yytext,pc);}
lhld {type=24;len=3;add2code(pc,"2A",1);}
macro
 
[a-z,:0-9A-Z]+ {
		 
		   switch(type)
                    {
				
                       case org:
                                pc=char2num(yytext);
				break;

		       case jmp:
				add2code(pc+1,yytext,0);
				add2code(pc+2,yytext,0);
				pc+=len;
				break;
		       case jnz:
				add2code(pc+1,yytext,0);
				add2code(pc+2,yytext,0);
				pc+=len;
				break;
		      /* case lhld:
				printf("%s ",yytext);
				char *tail;
				int converted_hex = strtol (yytext, &tail, 16);
				int last_2digit = converted_hex % 256;
				int first_2digit = converted_hex / 256;
				printf(" - %X - %X",first_2digit,last_2digit);				
				add2code(pc+1,last_2digit,2);
				add2code(pc+2,first_2digit,2);
				pc+=len;
				break; */

                       case mov:
                                if(strcmp(yytext,"a,a")==0)
                                {add2code(pc,"7F",1);}
                                if(strcmp(yytext,"a,b")==0)
                                {add2code(pc,"7F",1);}
                                if(strcmp(yytext,"a,c")==0)
                                {add2code(pc,"7F",1);}
                                if(strcmp(yytext,"a,d")==0)
                                {add2code(pc,"7F",1);}
                       
         			if(strcmp(yytext,"a,e")==0)
                                {add2code(pc,"7F",1);}
                                if(strcmp(yytext,"a,h")==0)
                                {add2code(pc,"7F",1);}
                                if(strcmp(yytext,"a,l")==0)
                                {add2code(pc,"7F",1);}
				if(strcmp(yytext,"b,a")==0)
				{add2code(pc,"7F",1);}     
				if(strcmp(yytext,"b,b")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"b,c")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"b,d")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"b,e")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"b,h")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"b,l")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"c,a")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"c,b")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"c,c")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"c,d")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"c,e")==0)
				{add2code(pc,"7F",1);}
				if(strcmp(yytext,"c,h")==0)
				{add2code(pc,"7F",1);}                 
				pc+=len;
				break;
				
                    }
                    
               }
                                  
%%

main()
{
   yylex();
   check();
   print_output();
}

int char2num(char *str)
{
   int i=0,sum=0,x;
   for(i=0;i<=3;i++)
   {
     if(str[i]>=65&&str[i]<=70)
     x=str[i]-55;
     else if(str[i]>=48&&str[i]<=57)
     x=str[i]-'0'; 
     sum=sum+(x*pow(16,(3-i)));
   }
    return(sum);
}


int newsymbol(char *symbol, int address)
{
	struct symtbl *ptr,*nptr;
	symbol[strlen(symbol)-1] = '\0';
	ptr=sym_current;	
	if(symroot==NULL)
	{	symroot = (struct symtbl *) malloc(sizeof(struct symtbl));
		symroot->symbol=symbol;
		symroot->address=address;	
		symroot->next=NULL;
		sym_current=symroot;
	}
	if(symroot!=NULL)
	{
		nptr = (struct symtbl *) malloc(sizeof(struct symtbl));
		nptr->symbol=symbol;
		nptr->address=address;
		sym_current->next=nptr;
		sym_current=nptr;			
	}
return 0;
}

int add2code(int s_add, char *code, int flag)
{
	struct mcode *ptr,*nptr;
	ptr=mcode_current;	
	if(mcode_root==NULL)
	{	mcode_root = (struct mcode *) malloc(sizeof(struct mcode));
		mcode_root->s_add=s_add;
		mcode_root->code=code;	
		mcode_root->flag=flag;		
		mcode_current=mcode_root;
	}
	if(mcode_root!=NULL)
	{
		nptr = (struct mcode *) malloc(sizeof(struct mcode));
		nptr->s_add=s_add;
		nptr->code=code;
		nptr->flag=flag;
		nptr->next=NULL;
		nptr->next=mcode_current->next;
		mcode_current->next=nptr;
		mcode_current=nptr;		
	}
return 0;
}

int check()
{
	int add,last2,first2;
	struct mcode *tmp_ptr=mcode_root;
	tmp_ptr = (struct mcode *) malloc(sizeof(struct mcode));
		
	struct mcode *current = mcode_root->next;
	
	while (current != NULL) {
	if(current->flag==0)
	{
		add=lookup(current->code);
		last2=add%256;
		first2=add/256;
		current->flag=2;
		if((current->next)->flag==0)
		current->code=last2;
		else current->code=first2;
			
	}
	current = current->next;
	}

	

}

int print_output()
{
	fp=fopen("output.txt","a");
	struct mcode *tmp_ptr=mcode_root;
	struct mcode *current = mcode_root->next;
	while (current != NULL) {
	if(current->flag==2)
	{fprintf(fp,"\n%X | %02X",current->s_add,current->code);}
	else fprintf(fp,"\n%X | %s",current->s_add,current->code);	
	current = current->next;
	}
return 0;
}

int lookup(char *code)
{
char *p1,*p2;
struct symtbl *current1 = symroot;
	
	while (current1 != NULL) {

	if(strspn(code,current1->symbol)>0)
	
	{		
		return (current1->address);
			
	}	
	current1 = current1->next;
	}
	
}

                                

