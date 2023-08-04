// dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <time.h>
#include <algorithm>
#include "dll.h"


int a[10][256];
int r[8];
int weight[10][32768];
int good[10][17];
int t_qual[10];
int test_qual;
int max_qual;

void init()   
{
	srand( (unsigned)time( NULL ));
	for(int k=0;k<10;k++)
	{
	for(int i=0;i<256;i++)
			{		int fl=1;
					while(fl)
					{	fl=0;
						a[k][i]=rand();
						for(int j=0;j<i;j++) {if( i <16 &&	a[k][j] == a[k][i]) fl=1;}
					}
			}	
	memset(&weight[k][0],0,32768*4);
	memcpy(&good[k][0],&a[k][0], 16*4);
	std::sort(&good[k][0],&good[k][16]);
	for(int i=0;i<16;i++) weight[k][ good[k][i] ] = i+1;
	int n=0;
	for(int i=0;i<16;i++) n+=(i+1)*weight[k][ a[k][i] ];
	t_qual[k]=n;
	}

	for(int i=0;i<4;i++) r[i]=0;
	r[4]=16;
	max_qual=1496;
	test_qual=(t_qual[0]+t_qual[1]+t_qual[2]+t_qual[3]+t_qual[4]+t_qual[5]+t_qual[6]+t_qual[7]+t_qual[8]+t_qual[9])/10;
}
	

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			init();			
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


int __stdcall  check_alg(base_check * ch)
{	
	int *mem=ch->get_mem();
	int *reg=ch->get_reg();
	int res[10];
	int count;
	for(int k=0;k<10;k++)
		{ memcpy(mem,&a[k][0],1024);
			memcpy(reg,r,32);
			if(ch->run(&count) > 0) return 0; // Ошибка
			int n=0; 
			for(int i=0;i<16;i++) weight[k][ good[k][i] ] = i+1;
			for(int i=0;i<16;i++) {n+=(i+1)*weight[k][ *(mem+i) & 0x7fff];weight[k][*(mem+i) & 0x7fff]=0;}
			res[k]=n;
		}
	
	return ( (res[0]+res[1]+res[2]+res[3]+res[4]+res[5]+res[6]+res[7]+res[8]+res[9])/10 );
}

__declspec(dllexport) int __stdcall get_max_qual()
{return max_qual;}

__declspec(dllexport) int __stdcall get_base_qual()
{return test_qual;}

__declspec(dllexport) int* __stdcall get_reg_one()
{return r;}

__declspec(dllexport) int* __stdcall get_mem_one()
{return &a[9][0];}

__declspec(dllexport) const char* __stdcall about()
{
	return "The task of sorting an array of numbers in memory. The size of the array is passed in register E.";
}

__declspec(dllexport) void __stdcall init_array()
{
	for(int k=0;k<10;k++)
	{
	for(int i=0;i<16;i++)
			{		int fl=1;
					while(fl)
					{	fl=0;
						a[k][i]=rand();
						for(int j=0;j<i;j++) {if( a[k][j] == a[k][i]) fl=1;}
					}
			}	
	memset(&weight[k][0],0,32768*4);
	memcpy(&good[k][0],&a[k][0], 16*4);
	std::sort(&good[k][0],&good[k][16]);
	for(int i=0;i<16;i++) weight[k][ good[k][i] ] = i+1;
	int n=0;
	for(int i=0;i<16;i++) n+=(i+1)*weight[k][ a[k][i] ];
	t_qual[k]=n;
	}
	for(int i=0;i<4;i++) r[i]=0;
	r[4]=16;
	max_qual=1496;
	test_qual=(t_qual[0]+t_qual[1]+t_qual[2]+t_qual[3]+t_qual[4]+t_qual[5]+t_qual[6]+t_qual[7]+t_qual[8]+t_qual[9])/10;
}
	