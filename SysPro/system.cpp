DFFGFVSSVNMGSG DFFVBV RFFBS MA/*

1. Create a symbol table with name of the labels and their "addresses" in the asm file 

   1a. The length of instructions are given in the MOT.

   1b. The inputs in the input file need to be parsed(spces to be adjusted) and the symbols need to be identified.

   1c. The symbols will be stored in another hash-table The symbol table for fast access in the net stage.

2. Replace the mneumonics with the machine code and the labels with the addresses from the symbol table

*/

#include<iostream>

#include<string>

#include<list>//to use doubly linked lists

#include<fstream>//for file I/O

#include<vector>//for re-sizable array

#define TSIZE 151//MOT size

#define STSIZE 23//symbol table size



using namespace std;

struct MOTentry

{

 string mneu;//the mneumonic

 string mcode;//the machine code

 int length;//the instruction length

};





struct symTable

{

 string symbol;   

 int loc;

};



/* hash table operations*/

list<MOTentry>MOTtable[TSIZE];//the hash table containing the MOT entries

list<symTable>sTable[STSIZE];//the hash table containing the symbol table entries





int generate_hash(int key,int sz)

{

 return key%sz;

}



int hash(string s_name,int sz)

{

 int l=s_name.length();

 int k=0;

 for(int i=0;i<l;i++)

   {

     k+=s_name[i]-'\0';//ASCII code of the character

   }

 return generate_hash(k,sz);

}



MOTentry searchFromMOT(string opcode)

{

 int key=hash(opcode,TSIZE);

 list<MOTentry>::iterator it;

 for(it=MOTtable[key].begin();it!=MOTtable[key].end();it++)

   {

     if(it->mneu==opcode)//found

         return *it;

   }

 cout<<"fatal error opcode"<<opcode<<"not found in MOT!!";

}



symTable searchFromSymTable(string symbol)

{

 int key=hash(symbol,STSIZE);

 list<symTable>::iterator it;

 for(it=sTable[key].begin();it!=sTable[key].end();it++)

   {

     if(it->symbol==symbol)//found

         return *it;

   }

 cout<<"\nfatal error- symbol:"<<symbol<<"not found in Symbol Table!!";

}





int plc;



void insertIntoSymTable(string sym)

{

    int k=hash(sym,STSIZE);

    symTable S;

    S.symbol=sym;

    S.loc=plc;//global variable

    sTable[k].push_back(S);

}



/*Hash table operations over*/



void getMOT()//the MOT resides in a file say MOT.txt

{

  ifstream fin;

  fin.open("MOT.txt"); 

  MOTentry en;

  int key;

  while(!fin.eof())

   {

     fin>>en.mneu;//mneu is the mneumonic

     fin>>en.length;

     fin>>en.mcode;

     key=hash(en.mneu,TSIZE);

     MOTtable[key].push_back(en);//the new MOTentry is inserted into the hash table at position key.    

   }

  fin.close();

}

/* printing functions*/

void printMOT()

{

  int i;

  cout<<"\nPrinting The MOT:\n" ;

  list<MOTentry>::iterator it;

  for(i=0;i<TSIZE;i++)

  {

      for(it=MOTtable[i].begin();it!=MOTtable[i].end();it++)

          cout<<it->mcode<<" "<<it->mneu<<" "<<it->length<<"\n";

  }

}



void printSymTable()

{

   int i;

  cout<<"\nPrinting The symbol table:\n" ;

  list<symTable>::iterator it;

  for(i=0;i<STSIZE;i++)

  {

      for(it=sTable[i].begin();it!=sTable[i].end();it++)

          cout<<it->symbol<<" "<<it->loc<<"\n";

  } 

}





/*End of printing functions*/

int findchar(string S,char c)

 {

   int i,l=S.length();

   for(i=0;i<l;i++)

     if(S[i]==c)

        return i;

   return -1;//not found



  }

string toUpperCase(string str)

{

 int i,l=str.length();

 string up="";

 for(i=0;i<l;i++)

  {

   if(str[i]>='A' && str[i]<='Z')

     up+=str[i];

   else

     up+='A'+str[i]-'a';

  }

 return up;

}



bool isPseudoOP(string opc)

{

 string up;

 up=toUpperCase(opc);

 

 if(up=="END")

   return true;

 return false;

}



void formatLine(string line)

{



//determine whether there is a label in the line

 string label,opc;   

 int pos;

 pos=findchar(line,';');

 if(pos!=-1)//there exists a comment

 {

  line=line.substr(0,pos);//remove everything after the ; including it

 }

 

 pos=findchar(line,':');

 if(pos!=-1)//a colon exists

 {

     label=line.substr(0,pos);

     line=line.substr(pos+1);

     insertIntoSymTable(label);

 }

  

 //line has no labels

 pos=findchar(line,' ');//position of the first space



 if(pos==-1)//there is no space

     pos=findchar(line,'\n');//then there is a new line



 if(pos!=-1)

 {

   opc=line.substr(0,pos); 

   if(!isPseudoOP(opc))//if it is not a pseudo opcode

     {

       MOTentry Me=searchFromMOT(opc);

       plc+=Me.length;//increment plc by the length of the instruction

     }

 }



}



void processFile()

{

  ifstream fin;

  fin.open("inpt.txt");

  string in;

  //cout<<"\nProcessing the file!";

  while(getline(fin,in))

   {

    in+='\n';

    formatLine(in);

   }

} 





main()

{

 plc=0;//initialize the plc   

 getMOT();

 processFile();

 //printMOT();

 printSymTable();

}


