#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


struct base_check {
//�������� ��������� �� ��������
virtual int * get_reg()=0;
//�������� ��������� �� ������
virtual int * get_mem()=0;
//��������� �������� (count - ����� ����� �� ������� �������� ��������)
virtual int run(int *count)=0;
};


__declspec(dllexport) int __stdcall check_alg(base_check *);
__declspec(dllexport) int __stdcall get_max_qual();
__declspec(dllexport) int __stdcall get_base_qual();
__declspec(dllexport) int * __stdcall get_reg_one();
__declspec(dllexport) int * __stdcall get_mem_one();
__declspec(dllexport) void __stdcall init_array();
__declspec(dllexport) const char * __stdcall about();