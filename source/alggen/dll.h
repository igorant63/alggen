#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


struct base_check {
//Получить указатель на регистры
virtual int * get_reg()=0;
//Получить указатель на массив
virtual int * get_mem()=0;
//Запустить алгоритм (count - число шагов за которые выполнен алгоритм)
virtual int run(int *count)=0;
};