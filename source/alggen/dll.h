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