#include "stdafx.h"
#include "program.h"
#include <algorithm>
#include "dll.h"

IMPLEMENT_SERIAL(command, CObject, 1)
IMPLEMENT_SERIAL(nop, command, 1)
IMPLEMENT_SERIAL(jle_r_r, command, 1)
IMPLEMENT_SERIAL(jle_r_m, command, 1)
IMPLEMENT_SERIAL(jle_m_r, command, 1)
IMPLEMENT_SERIAL(jle_m_m, command, 1)
IMPLEMENT_SERIAL(jle_r_c, command, 1)
IMPLEMENT_SERIAL(jge_r_r, command, 1)
IMPLEMENT_SERIAL(jge_r_m, command, 1)
IMPLEMENT_SERIAL(jge_m_r, command, 1)
IMPLEMENT_SERIAL(jge_m_m, command, 1)
IMPLEMENT_SERIAL(jge_r_c, command, 1)
IMPLEMENT_SERIAL(jl_r_r, command, 1)
IMPLEMENT_SERIAL(jl_r_m, command, 1)
IMPLEMENT_SERIAL(jl_m_r, command, 1)
IMPLEMENT_SERIAL(jl_m_m, command, 1)
IMPLEMENT_SERIAL(jl_r_c, command, 1)
IMPLEMENT_SERIAL(jg_r_r, command, 1)
IMPLEMENT_SERIAL(jg_r_m, command, 1)
IMPLEMENT_SERIAL(jg_m_r, command, 1)
IMPLEMENT_SERIAL(jg_m_m, command, 1)
IMPLEMENT_SERIAL(jg_r_c, command, 1)
IMPLEMENT_SERIAL(add_r_c, command, 1)
IMPLEMENT_SERIAL(add_r_r, command, 1)
IMPLEMENT_SERIAL(sub_r_c, command, 1)
IMPLEMENT_SERIAL(sub_r_r, command, 1)
IMPLEMENT_SERIAL(xchg_r_r, command, 1)
IMPLEMENT_SERIAL(xchg_r_m, command, 1)
IMPLEMENT_SERIAL(xchg_m_r, command, 1)
IMPLEMENT_SERIAL(xchg_m_m, command, 1)
IMPLEMENT_SERIAL(mov_r_r, command, 1)
IMPLEMENT_SERIAL(mov_r_m, command, 1)
IMPLEMENT_SERIAL(mov_m_r, command, 1)
IMPLEMENT_SERIAL(mov_m_m, command, 1)
IMPLEMENT_SERIAL(mov_r_c, command, 1)
IMPLEMENT_SERIAL(program, CObject, 1)

int ra(int var)    // An integer random number from a given range
{	
	return rand()/32768.*var;
}

const char* err(int ER)
{
	 switch(ER)
		 { case 0:
			  return "No errors";
			 case 1:
			  return "Memory access error";
			 case 2:
				 return "Loop error";	
			 case 255:
				 return "Command code generation error";	
       default:
				 return "Unknown error";
		 }

}
 
//*******GENERAL ENVIRONMENT SETTINGS*********//

proc prc; // Global processor instance
proc sample_prc;
rules rule;
population pop;
vector<program> hist;
program temp_prg;
int ind_command;

struct check : base_check {
program *pr;
// Get register pointer
virtual int * get_reg();
// Get a pointer to an array
virtual int * get_mem();
// Run the algorithm (count - the number of steps for which the algorithm was completed)
virtual int run(int *count);
} ch;

int maps::n_comm=PROGRAM_GROUPS;   // Number of custom command groups
int maps::n_comm_tot=TOTAL_COMMANDS; // Total commands
int maps::prob[MAX_PROGRAM_GROUPS];
int maps::mut[5];
int maps::mut_x[5];
int maps::map[100];
int maps::map_mut[100];
int maps::map_mut_x[100];
int maps::program_length=12;
int maps::pop_length=100;
int maps::group[MAX_PROGRAM_GROUPS]={20, 4, 5, 4, 1}; // Number of commands in each group
CString maps::mnem[MAX_PROGRAM_GROUPS]={"Comparison and transition",          // Group designations
												 "Arithmetic operations", 
												 "Assignment",
												 "Exchange",
													"Empty operation"}; 

int program::cp=0;

void maps::def_prob()
{
	prob[0]=20;prob[1]=20;prob[2]=20;prob[3]=20;prob[4]=20; // probability 1/5 for each type of command
	mut[0]=50;mut[1]=50;mut[2]=0;mut[3]=0;mut[4]=0;
	mut_x[0]=100;mut_x[1]=0;mut_x[2]=0;mut_x[3]=0;mut_x[4]=0;
}

void maps::init_map()
{ 
	int ind_map=0; 
	for(int i=0;i<n_comm;i++)  // Loop through command groups
		for(int j=0;j<prob[i];j++)	map[ind_map++] = i; //  We remember the index of the command group in the group of map cells, by the size of the proportional probability of the command group
	
	ind_map=0; 	
	for(int i=0;i<5;i++)
		for(int j=0;j<mut[i];j++)	map_mut[ind_map++] = i; 

	ind_map=0; 	
	for(int i=0;i<5;i++)
		for(int j=0;j<mut_x[i];j++)	map_mut_x[ind_map++] = i; 
}

int good[16];
void rules::init_rules()
{
	test_qual=get_base_qual();
	max_qual=get_max_qual();
}

int rules::get_qual()
{	
	int n=0; 
	for(int i=0;i<16;i++) weight[ good[i] ] = i+1;
	for(int i=0;i<16;i++) {n+=(i+1)*weight[ prc.a[i] & 0x7fff];weight[ prc.a[i] & 0x7fff]=0;}
	return n;
}

//******* COMMAND *********//

void command::Serialize(CArchive& ar)
{ 
	CObject::Serialize(ar); 
	if( ar.IsStoring())
	{   ar<<op_1<<op_2<<label;}
	else	
	{
		ar>>op_1>>op_2>>label;
	}
}

void command::operator=(const command& c) 
{
	memcpy(this,&c,sizeof(command));
} 

command* maps::make_command(program* prg, int i)
{  
	int len=prg->prog_len;
	int n=map[ra(100)];  // Random group number
	int m= ra(group[n]);  // Random command number in the group
	void *ptr=prg->cmds+i;
	switch(n)
	 {
		case 0: // Comparison commands
		ind_command=i;
		switch(m)
		{ 
		case 0:
			return new(ptr) jle_r_r(len);  // <=
		case 1:
			return  new(ptr) jle_r_m(len);
		case 2:
			return  new(ptr) jle_m_r(len);
		case 3:
			return  new(ptr) jle_m_m(len);	
		case 4:
			return  new(ptr) jge_r_r(len);  // >=
		case 5:
			return  new(ptr) jge_r_m(len);
		case 6:
			return  new(ptr) jge_m_r(len);
		case 7:
			return	new(ptr) jge_m_m(len);
		case 8:
			return  new(ptr) jl_r_r(len);  // <
		case 9:
			return  new(ptr) jl_r_m(len);
		case 10:
			return	new(ptr) jl_m_r(len);
		case 11:
			return  new(ptr) jl_m_m(len);	
		case 12:
			return  new(ptr) jg_r_r(len);  // >
		case 13:
			return  new(ptr) jg_r_m(len);
		case 14:
			return  new(ptr) jg_m_r(len);
		case 15:
			return  new(ptr) jg_m_m(len);
		case 16:
			return  new(ptr) jl_r_c(len);  // >
		case 17:
			return  new(ptr) jg_r_c(len);
		case 18:
			return  new(ptr) jle_r_c(len);
		case 19:
			return  new(ptr) jge_r_c(len);
		default:
			return NULL;
		}
		case 1: // Arithmetic operations
			switch(m)
			{ case 0:
				return  new(prg->cmds+i) add_r_r;
				case 1:
				return  new(prg->cmds+i) add_r_c;
				case 2:
				return  new(prg->cmds+i) sub_r_r;
				case 3:
				return  new(prg->cmds+i) sub_r_c;
				default:
				return NULL;
			}
			case 2: //  Assignment
			switch(m)
			{ case 0:
				return  new(prg->cmds+i) mov_r_r;
				case 1:
				return  new(prg->cmds+i) mov_r_m;
				case 2:
				return  new(prg->cmds+i) mov_m_r;
				case 3:
				return  new(prg->cmds+i) mov_m_m;
				case 4:
				return  new(prg->cmds+i) mov_r_c;
				default:
				return NULL;
			}
			case 3: //  Exchange
			switch(m)
			{ case 0:
				return  new(prg->cmds+i) xchg_r_r;
				case 1:
				return  new(prg->cmds+i) xchg_r_m;
				case 2:
				return  new(prg->cmds+i) xchg_m_r;
				case 3:
				return  new(prg->cmds+i) xchg_m_m;
				default:
				return NULL;
			}
			case 4: //  Empty operation
			switch(m)
			{ case 0:
				return  new(prg->cmds+i) nop;
				default:
				return NULL;
			}
			default:
				return NULL;
	  }
     
}

void maps::make_command2(program& prg, int ind_comm, int ind_op1, int ind_op2, int label)
{    
	void* ptr=prg.cmds+prg.prog_len;
	command* c = nullptr;
	switch(ind_comm)
		{
			case 0: //MOV
			if(ind_op1 < 5)
				{  if(ind_op2 < 5)	{c=new(ptr) mov_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
				  	else if(ind_op2 < 10) { c=new(ptr) mov_r_m;c->op_1=ind_op1;c->op_2=ind_op2-5;}
									else 	{c= new(ptr) mov_r_c; c->op_1=ind_op1;c->op_2=ind_op2-10;} 
				}
			else	
				{  if(ind_op2 < 5) { c=new(ptr) mov_m_r;c->op_1=ind_op1-5;c->op_2=ind_op2;}
				  	else if(ind_op2 < 10) { c=new(ptr) mov_m_m;c->op_1=ind_op1-5;c->op_2=ind_op2-5;}		 
				}
   		break;
			case 1: //ADD
			if(ind_op2 < 5)	{c=new(ptr) add_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
			else	{c=new(ptr) add_r_c;c->op_1=ind_op1;c->op_2=1;} 
			break;
			case 2: //SUB
			if(ind_op2 < 5)	{c=new(ptr) sub_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
			else	{c=new(ptr) sub_r_c;c->op_1=ind_op1;c->op_2=1;} 
			break;
			case 3:  //XCHG
			if(ind_op1 < 5 &&  ind_op2 < 5)	{c=new(ptr) xchg_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
			else if (ind_op1 < 5 && ind_op2 >= 5) { c=new(ptr) xchg_r_m;c->op_1=ind_op1;c->op_2=ind_op2-5;}
					 else	if (ind_op1>= 5 &&  ind_op2 < 5) { c=new(ptr) xchg_m_r;c->op_1=ind_op1-5;c->op_2=ind_op2;}
								else {c= new(ptr) xchg_m_m; c->op_1=ind_op1-5;c->op_2=ind_op2-5;} 
   		break;
			case 4:  //JL
			if(ind_op1 < 5)
				{  if(ind_op2 < 5)	{c=new(ptr) jl_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
				  	else if(ind_op2 < 10) { c=new(ptr) jl_r_m;c->op_1=ind_op1;c->op_2=ind_op2-5;}
									else 	{c= new(ptr) jl_r_c; c->op_1=ind_op1;c->op_2=ind_op2-10;} 
				}
			else	
				{  if(ind_op2 < 5) { c=new(ptr) jl_m_r;c->op_1=ind_op1-5;c->op_2=ind_op2;}
				  	else if(ind_op2 < 10) { c=new(ptr) jl_m_m;c->op_1=ind_op1-5;c->op_2=ind_op2-5;}		 
				}
			c->label=label;
   		break;
			case 5: // JG
			if(ind_op1 < 5)
				{  if(ind_op2 < 5)	{c=new(ptr) jg_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
				  	else if(ind_op2 < 10) { c=new(ptr) jg_r_m;c->op_1=ind_op1;c->op_2=ind_op2-5;}
									else 	{c= new(ptr) jg_r_c; c->op_1=ind_op1;c->op_2=ind_op2-10;} 
				}
			else	
				{  if(ind_op2 < 5) { c=new(ptr) jg_m_r;c->op_1=ind_op1-5;c->op_2=ind_op2;}
				  	else if(ind_op2 < 10) { c=new(ptr) jg_m_m;c->op_1=ind_op1-5;c->op_2=ind_op2-5;}		 
				}
			c->label=label;
   		break;
			case 6:  //JLE
			if(ind_op1 < 5)
				{  if(ind_op2 < 5)	{c=new(ptr) jle_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
				  	else if(ind_op2 < 10) { c=new(ptr) jle_r_m;c->op_1=ind_op1;c->op_2=ind_op2-5;}
									else 	{c= new(ptr) jle_r_c; c->op_1=ind_op1;c->op_2=ind_op2-10;} 
				}
			else	
				{  if(ind_op2 < 5) { c=new(ptr) jle_m_r;c->op_1=ind_op1-5;c->op_2=ind_op2;}
				  	else if(ind_op2 < 10) { c=new(ptr) jle_m_m;c->op_1=ind_op1-5;c->op_2=ind_op2-5;}		 
				}
			c->label=label;
   		break;
			case 7: //JGE
			if(ind_op1 < 5)
				{  if(ind_op2 < 5)	{c=new(ptr) jge_r_r;c->op_1=ind_op1;c->op_2=ind_op2;} 
				  	else if(ind_op2 < 10) { c=new(ptr) jge_r_m;c->op_1=ind_op1;c->op_2=ind_op2-5;}
									else 	{c= new(ptr) jge_r_c; c->op_1=ind_op1;c->op_2=ind_op2-10;} 
				}
			else	
				{  if(ind_op2 < 5) { c=new(ptr) jge_m_r;c->op_1=ind_op1-5;c->op_2=ind_op2;}
				  	else if(ind_op2 < 10) { c=new(ptr) jge_m_m;c->op_1=ind_op1-5;c->op_2=ind_op2-5;}		 
				}

			c->label=label;
   		break;
		case 8:
			c=new(ptr) nop;
   		break;

		}

}

void maps::init_array(proc& p)
{ 
	for(int i=0;i<256;i++)
	{
		int fl=1;
		while(fl)
		{	
			fl=0;
			p.a[i]=rand();
			for(int j=0;j<i;j++) {if( i <16 &&	p.a[j] == p.a[i]) fl=1;}
		}
	}	
	for(int i=0;i<4;i++) p.r[i]=0;
	p.r[4]=16;
}
 
void maps::make_array(proc& p, proc& sample)
{ 
	memcpy(prc.a,sample.a,256*4);
	memcpy(prc.r,sample.r,8*4);
}

int nop::run(program* prg)
{ 
	return 0;
}

int add_r_r::run(program* prg)
{
	prc.r[op_1]+=prc.r[op_2];
	return 0;
}

int add_r_c::run(program* prg)
{
	prc.r[op_1]+=op_2;
	return 0;
}

int sub_r_r::run(program* prg)
{
	prc.r[op_1]-=prc.r[op_2];
	return 0;
}

int sub_r_c::run(program* prg)
{
	prc.r[op_1]-=op_2;
	return 0;
}

int mov_r_r::run(program* prg)
{
	prc.r[op_1]=prc.r[op_2];
	return 0;
}

int mov_r_c::run(program* prg)
{
	prc.r[op_1]=op_2;
	return 0;
}

int mov_r_m::run(program* prg)
{ 
	proc& p=prc; int o_2 = p.r[op_2];
	if(o_2 > 255 || o_2 < 0) return 1;
	p.r[op_1]=p.a[o_2];
	return 0;
}

int mov_m_r::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1];
	if(o_1 > 255 || o_1 < 0) return 1;
	p.a[o_1]=p.r[op_2];
	return 0;
}

int mov_m_m::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1];int o_2 = p.r[op_2];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(o_2 > 255 || o_2 < 0) return 1;
    p.a[o_1]=p.a[o_2];
	return 0;
}

int xchg_r_r::run(program* prg)
{ 
	int temp = prc.r[op_1];
	prc.r[op_1]=prc.r[op_2]; 
	prc.r[op_2]=temp;
	return 0;
}

int xchg_r_m::run(program* prg)
{ 
	int o_2 = prc.r[op_2];
	if(o_2 > 255 || o_2 < 0) return 1;
	int temp = prc.r[op_1];
	prc.r[op_1]=prc.a[o_2]; 
	prc.a[o_2]=temp;
	return 0;
}

int xchg_m_r::run(program* prg)
{ 
	int o_1 = prc.r[op_1];
	if(o_1 > 255 || o_1 < 0) return 1;
	int temp = prc.a[o_1];
	prc.a[o_1]=prc.r[op_2]; 
	prc.r[op_2]=temp;
	return 0;
}

int xchg_m_m::run(program* prg)
{ 
	int o_1 = prc.r[op_1];
	int o_2 = prc.r[op_2];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(o_2 > 255 || o_2 < 0) return 1;
	int temp = prc.a[o_1];
	prc.a[o_1]=prc.a[o_2];
	prc.a[o_2]=temp;
	return 0;
}


int jle_r_r::run(program* prg)
{
	if(prc.r[op_1] <= prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jle_r_m::run(program* prg)
{ 
	proc& p=prc; int o_2 = p.r[op_2];
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.r[op_1] <= prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jle_m_r::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(prc.a[o_1] <= prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jle_m_m::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1]; int o_2 = p.r[op_2];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.a[o_1] <= prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jle_r_c::run(program* prg)
{
	if(prc.r[op_1] <= op_2) prg->cp=label-1;
	return 0;
}

int jge_r_r::run(program* prg)
{
	if(prc.r[op_1] >= prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jge_r_m::run(program* prg)
{ 
	proc& p=prc; int o_2 = p.r[op_2];
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.r[op_1] >= prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jge_m_r::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(prc.a[o_1] >= prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jge_m_m::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1]; int o_2 = p.r[op_2];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(o_2 > 255 || o_2 < 0) return 1;
    if(prc.a[o_1] >= prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jge_r_c::run(program* prg)
{
	if(prc.r[op_1] >= op_2) prg->cp=label-1;
	return 0;
}

int jl_r_r::run(program* prg)
{
	if(prc.r[op_1] < prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jl_r_m::run(program* prg)
{ 
	proc& p=prc; int o_2 = p.r[op_2];
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.r[op_1] < prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jl_m_r::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(prc.a[o_1] < prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jl_m_m::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1]; int o_2 = p.r[op_2];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.a[o_1] < prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jl_r_c::run(program* prg)
{
	if(prc.r[op_1] < op_2) prg->cp=label-1;
	return 0;
}


int jg_r_r::run(program* prg)
{
	if(prc.r[op_1] > prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jg_r_m::run(program* prg)
{ 
	proc& p=prc; int o_2 = p.r[op_2];
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.r[op_1] > prc.a[o_2]) prg->cp=label-1;
	return 0;
}

int jg_m_r::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(prc.a[o_1] > prc.r[op_2]) prg->cp=label-1;
	return 0;
}

int jg_m_m::run(program* prg)
{ 
	proc& p=prc; int o_1 = p.r[op_1]; int o_2 = p.r[op_2];
	if(o_1 > 255 || o_1 < 0) return 1;
	if(o_2 > 255 || o_2 < 0) return 1;
	if(prc.a[o_1] > prc.a[o_2]) prg->cp=label-1;
	return 0;
}


int jg_r_c::run(program* prg)
{
	if(prc.r[op_1] > op_2) prg->cp=label-1;
	return 0;
}

void nop::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="nop;";
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jle_r_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+="<=";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jle_r_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+="<=";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jle_m_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+="<=";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);

}

void jle_m_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+="<=";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jle_r_c::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+="<=";str+=_itoa(op_2, buf,10); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}


void jge_r_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+=">=";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jge_r_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+=">=";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jge_m_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+=">=";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);

}

void jge_m_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+=">=";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jge_r_c::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+=">=";str+=_itoa(op_2, buf,10); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jl_r_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+="<";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jl_r_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+="<";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jl_m_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+="<";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);

}

void jl_m_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+="<";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jl_r_c::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+="<";str+=_itoa(op_2, buf,10); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}


void jg_r_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+=">";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jg_r_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+=">";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jg_m_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+=">";str+=char('A'+ op_2); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);

}

void jg_m_m::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+="array["; str+=char('A'+ op_1); str+="]"; 
	 str+=">";str+="array["; str+=char('A'+ op_2); str+="]"; str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void jg_r_c::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="if ("; str+=char('A'+op_1); 
	 str+=">";str+=_itoa(op_2, buf,10); str+=") ";
	 str+="goto ";str+=_itoa(label, buf,10);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void add_r_r::print(program* prg, CEdit* ed)
{ 
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';str+=char('A'+op_1);str+="+";str+=char('A'+ op_2);str+=';';
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void add_r_c::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';str+=char('A'+op_1);str+="+";str+=_itoa(op_2, buf,10);str+=';';
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void sub_r_r::print(program* prg, CEdit* ed)
{
   CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';str+=char('A'+op_1);str+="-";str+=char('A'+ op_2);str+=';';
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void sub_r_c::print(program* prg, CEdit* ed)
{
   CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';str+=char('A'+op_1);str+="-";str+=_itoa(op_2, buf,10); str+=';';
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void mov_r_r::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';str+=char('A'+op_2);;str+=';';
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void mov_r_m::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';
	 str+="array["; str+=char('A'+ op_2); str+="]";str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void mov_m_r::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="array["; str+=char('A'+ op_1); str+="]";str+='=';
	 str+=char('A'+op_2);str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void mov_m_m::print(program* prg, CEdit* ed)
{
 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="array["; str+=char('A'+ op_1); str+="]";str+='=';
	 str+="array["; str+=char('A'+ op_2); str+="]";str+=';'; 
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void mov_r_c::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+=char('A'+op_1);str+='=';str+=_itoa(op_2, buf,10);str+=';';
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void xchg_r_r::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="xchg("; str+=char('A'+op_1);
	 str+=',';str+=char('A'+op_2);str+=");";
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void xchg_r_m::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="xchg("; str+=char('A'+op_1);
	 str+=',';str+="array["; str+=char('A'+ op_2); str+="]"; str+=");";
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}


void xchg_m_r::print(program* prg, CEdit* ed)
{
   CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="xchg("; str+="array["; str+=char('A'+ op_1); str+="]";
	 str+=',';str+=char('A'+op_2); str+=");";
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}

void xchg_m_m::print(program* prg, CEdit* ed)
{
	 CString str;
	 char buf[10];
	 str+=_itoa(prg->cp, buf,10);str+=":  ";
	 str+="xchg("; str+="array["; str+=char('A'+ op_1); str+="]";
	 str+=',';str+="array["; str+=char('A'+ op_2); str+="]";str+=");";
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
}


//******* PROGRAM *********//

program::program() 
{
	prog_len=0;age=0;sign[0]=0;qual=0;status=255;takt=-1;
}

program::program(const program& prg) 
{
	memcpy(this,&prg,sizeof(program));
} 

void program::operator=(const program& prg) 
{
	memcpy(this,&prg,sizeof(program));
} 

int program::gen_prog(int len) // Program Instance Generation
{ 
	prog_len=len;
	for(int i=0;i<7;i++) sign[i]='A'+ra(26);  // Signature
	sign[7]=0;
	for(int i=0;i<prog_len;i++) 
		{  
			 if(!maps::make_command(this,i)) return 255; // Command code generation error
		}
  return 0;
}

void program::sign_user() 
{ 
	sign[0]='U';for(int i=1;i<7;i++) sign[i]='0'+ra(10);  // Signature
	sign[7]=0;age=0;qual=0;status=255;
}

void program::test() 
{	
	status=0;
	ch.pr=this;
	if(check_alg)
		qual=check_alg(&ch);
}


void program::gen_sample() // Generating Sample Sorting Program
{
	command* c= new(cmds) mov_r_c; //0: A=0;
	c->op_1=0;					// A
	c->op_2=0;	        // 0
  c= new(cmds + 1) mov_r_c; //1: D=0;
	c->op_1=3;					// D
	c->op_2=0;	        // 0
 	c= new(cmds + 2) mov_r_c; //2: B=1;
	c->op_1=1;					// B
	c->op_2=1;	        // 1
 	c= new(cmds + 3) jle_m_m(10); //3: if(array[A]<=array[B]) goto 6;
	c->op_1=0;					// A
	c->op_2=1;	        // B
	c->label=6;					// goto 6
 	c= new(cmds + 4) xchg_m_m; //4: xchg(array[A],array[B])
	c->op_1=0;					// A
	c->op_2=1;	        // B
 	c= new(cmds + 5) mov_r_c; //5:  D=1;
	c->op_1=3;					// D
	c->op_2=1;	        // 1
	c= new(cmds + 6) add_r_c; //6:  A=A+1;
	c->op_1=0;					// A
	c->op_2=1;	        // 1
	c= new(cmds + 7) add_r_c; //7:  B=B+1;
	c->op_1=1;					// B
	c->op_2=1;	        // 1
	c= new(cmds + 8) jl_r_r(10); //8:  if(B<E) goto 3;
	c->op_1=1;					// B
	c->op_2=4;	        // E
	c->label=3;					// goto 3
	c= new(cmds + 9) jg_r_r(10); //9:  if (D>C) goto 0;
	c->op_1=3;					// D
	c->op_2=2;	        // C
	c->label=0;         // goto 0;
	prog_len=10;
	sign[0]='U';for(int i=1;i<7;i++) sign[i]='0';	sign[7]=0;	
}


void program::make_mutation(BOOL lab)
{
  int d;  // Insertion/deletion position		
  int xxx=maps::map_mut_x[ra(100)]; // Number of cycles
  for(int x=0;x<xxx+1;x++)  // Cycle by the number of simultaneous mutations
  {	
	int n;
	if(prog_len==0) n=0;
	else	n=maps::map_mut[ra(100)]; // Mutation type index
	switch(n)
	{ case 0:    // Insert
		if(prog_len >= MAX_PROGRAM_LENGTH) break;
		d=ra(prog_len+1);  // Insertion point
		if(d<prog_len) // If not the last
					{		memcpy(cmds+d+1, cmds+d,(prog_len-d)*sizeof(command));  // tail shift to the right
							prog_len++;											
							if(lab) for(int i=0;i<prog_len;i++) {if(cmds[i]._label() && cmds[i].label>=d )	cmds[i].label++;}
							maps::make_command(this,d);
					}
		else 
					{prog_len++;
					if(lab) for(int i=0;i<prog_len-1;i++) {if(cmds[i]._label() && cmds[i].label>=d )	cmds[i].label++;}
					maps::make_command(this,d);
					}
		
		age++;
		break;		
		case 1:    // deletion
		d=ra(prog_len);
		prog_len--;  
		memcpy(cmds+d, cmds+d+1,(prog_len-d)*sizeof(command));  // сдвиг хвоста влево
		if(lab)	for(int i=0;i<prog_len;i++) {if(cmds[i]._label() && cmds[i].label>d)  cmds[i].label--;}// Корректировка ссылок		
		age++;
		break;
		case 2:    // Replacement
		d=ra(prog_len);
		maps::make_command(this,d);
		age++;
		break;
		case 3:    // Drifting
		{d=ra(prog_len);	
		CRuntimeClass* rc=cmds[d].GetRuntimeClass();
		command* c=(command *)rc->CreateObject();
		cmds[d]=*c;
		delete c;
		}
		age++;
		break;	
		case 4: // Translocation
		{d=ra(prog_len);	// What
		command c;c=cmds[d]; // Buffer
		if(d<prog_len-1) memcpy(cmds+d, cmds+d+1,(prog_len-d-1)*sizeof(command)); // Cut out
		if(lab)	for(int i=0;i<prog_len-1;i++) {if(cmds[i]._label() && cmds[i].label>d)  cmds[i].label--;} // Link Correction	
		d=ra(prog_len);  // Точка вставки
		if(d<prog_len-1) 	memcpy(cmds+d+1, cmds+d,(prog_len-d-1)*sizeof(command));  // tail shift to the right
		if(lab) for(int i=0;i<prog_len;i++) {if(cmds[i]._label() && cmds[i].label>=d )	cmds[i].label++;}
		cmds[d]=c;
		}
		age++; 
		break;
	} // switch
  }   // for(x)
}

 int program::run(int *t) // Program execution
 { 	int count=0; 
	int cycle=prog_len*256;
	takt=0;
	for(cp=0;cp<prog_len;cp++) 
	{ 
		takt++;
		if(cmds[cp].run(this)==1) { status=1;return 1;}
		if(count++ > cycle) { status=2;return 2;}
	}
	*t=takt;
	return 0;
}


void program::listing(CEdit* ed)
{
	CString str;	
	for(cp=0;cp<prog_len;cp++)	cmds[cp].print(this,ed);	
}

// Saving and extracting a program from a file archive
void program::Serialize(CArchive& ar)
{
	if( ar.IsStoring())
	{  
		ar << prog_len<<age; 
		ar.Write(sign,8);	
		for(int i=0;i<prog_len;i++) 	ar <<cmds+i;	
	}
	else	
	{
		ar>>prog_len>>age;
		ar.Read(sign,8);
		CObject* ob;
		for(int i=0;i<prog_len;i++) 	
		{  
			ar>>ob;	
			memcpy(cmds+i, ob, sizeof(command));
			delete ob;
		}
	}
}

//******* POPULATION *********//

// Random population generation
void population::make_population(int len)
{	 	 
	prg.clear(); // Cleaning up old content
	int i=0;
	while(i<len) 	
	{ 
		program pr;
		pr.gen_prog(maps::program_length);	
		pr.test();
		if(pr.status == 0) {prg.push_back(pr); i++;}
	}
	pp=0;
}

// Writing a population to a file
void population::save_population(CArchive& ar)
{	
	int sz=prg.size();
	ar << sz;		
	vector<program>::iterator it=prg.begin();	  
	for(int i=0;i<sz;i++)
	{   
		program& p =*it++;
		ar << &p;
	}
}

// Loading a population from a file
void population::load_population(CArchive& ar)
{	 
	prg.clear(); 
	CObject* ob= NULL; 
	int pop_len;
	ar >> pop_len;		
	for(int i=0;i<pop_len;i++) 	
	{ 
		 ar >> ob;
		 program& pr=*(program *)ob;
		 prg.push_back(pr);
		 delete ob;
	}
	pp=0;
}

bool program::operator==(const program& pr2) 
{	
	if(prog_len != pr2.prog_len) return false;
	if(!memcmp(cmds,pr2.cmds,prog_len*sizeof(command))) return true;
	else return false;		   
} 

int* check::get_reg()
{ return prc.r;}

int* check::get_mem()
{ return prc.a;}

int check::run(int *count)
{ return pr->run(count);}