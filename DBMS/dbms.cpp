#include<iostream.h>
#include<fstream.h>
#include<stdio.h>

struct sturec
{
int roll;
int age;
char gender;
int marks;
char name[40];
}s;

void add_record(sturec sq)
{

ofstream pfile;

pfile.open("mydb.dat");
outfile<<sq.roll<<endl;
outfile<<sq.name<<endl;
outfile<<sq.gender<<endl;
outfile<<sq.marks<<endl;
outfile<<sq.age<<endl;
outfile.close();
}

int search_rec(int r)
{

sturec s;
ifstream infile; 
infile.open("mydb.dat"); 
int flag=0;
while(flag!=1 && !infile.eof())
{
infile>>s.roll;
infile>>s.name;
infile>>s.gender;
infile>>s.marks;
infile>>s.age;
if (r==s.roll)
flag=1;
}
infile.close();
return flag;

}

void max_rec()
{


void main()
{
int ch
cout<<"enter the choice: ( 1. add record /n 2.search by roll /n 3.max marks record)"<<endl;
switch(ch){
case 1: 
cout<<"enter the roll number:"<<endl;
cin>>s.roll;
cout<<"enter the name:"<<endl
cin>>s.name;
cout<<"enter the gender:"<<endl;
cin>>s.gender;
cout<<"enter the marks:"<<endl
cin>>s.marks;
cout<<"enter the age:"<<endl
cin>>s.age;
if(search_rec(s.roll))
{
 add_record(s);
}
else
cout<<"roll aleady exists:";
break;
case 2:
cout<<"enter the roll number:"<<endl;
cin>>s.roll;
if(search_rec(s.roll))
{
cout<<"the roll number:"<<s.roll<<endl;
cout<<"the name:"<<s.name<<endl;
cout<<"the gender:"<<s.gender<<endl;
cout<<"the age:"<<s.age<<endl;
cout<<"the marks:"<<s.marks<<endl;
}
break;
case 3:
}



