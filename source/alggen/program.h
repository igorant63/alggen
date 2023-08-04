#include <afxtempl.h>
struct command; 
struct program;
struct proc;
extern int ra(int var);   
extern const char* err(int ER); 

const int MAX_PROGRAM_LENGTH=20;
const int MAX_PROGRAM_GROUPS=20;
const int PROGRAM_GROUPS=5;
const int TOTAL_COMMANDS=34;

struct maps  // General settings tables
{
static int map[100]; // Command Distribution Map
static int map_mut[100];  // the number of elements with a group index is proportional to its probability
static int map_mut_x[100];  
static CString mnem[MAX_PROGRAM_GROUPS];  // Command notations for the probabilities setting dialog
static int prob[MAX_PROGRAM_GROUPS];   // Probability distribution by types of commands
static int mut[5]; // Probability distribution
static int mut_x[5]; // Probability distribution for one-time multiple mutations
static int n_comm;    // Number of custom command groups
static int n_comm_tot; // Total number of different command types
static int program_length; // Program length to generate
static int pop_length; // Population size to generate
static int group[MAX_PROGRAM_GROUPS]; // Number of commands in each custom command group
static void init_map(); // Filling out the command distribution map
static void def_prob(); // Initialization of the probability distribution by instruction types
static command* make_command(program *prg, int ind); // Command generation (returns an error)
static void make_command2(program& p, int,int,int,int);  
static void init_array(proc& prc); // Generation of the initial configuration of memory and registers
static void make_array(proc& prc, proc& sample); // Copying the initial memory and registers configuration to the processor
};


struct proc          // virtual machine
{
  int a[256];  // Memory
  int r[8];    // Registers
	void reset() {memset(a,0,1024);memset(r,0,32);}
	proc() {reset();}
};

extern proc  prc;
extern proc  sample_prc;
extern int ind_command;

struct command : public CObject     // Operation
{ 
  int op_1;			// Code of the first operand
  int op_2;			// Second operand code
  int label;		// Label to go to
  virtual void print(program* prg, CEdit* ed){}; // command output
  virtual int run(program* prg){return 0;}     // Command execution
  virtual bool _label(){return false;}
	command(){op_1=0;op_2=0;label=0;}        // Constructor
	DECLARE_SERIAL(command)
	void Serialize(CArchive& ar);
	void operator=(const command&); // copy operator
};

struct nop : command  
{	
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(nop)
};

struct jle_r_r : command  // Comparison <=
{	
	jle_r_r(int len=0)  {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	virtual bool _label(){return true;}
	DECLARE_SERIAL(jle_r_r)
};

struct jle_r_m : command  
{	
	jle_r_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jle_r_m)
};

struct jle_m_r : command  
{	
	jle_m_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed);
	DECLARE_SERIAL(jle_m_r)
};
struct jle_m_m : command  
{	
	jle_m_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jle_m_m)
};

struct jle_r_c : command  
{	
	jle_r_c(int len=0)  {op_1=ra(5);op_2=ra(2);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jle_r_c)
};

struct jge_r_r : command  // Comparison >=
{
	jge_r_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed);
	DECLARE_SERIAL(jge_r_r)
};

struct jge_r_m : command  
{ 
	jge_r_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed);
	DECLARE_SERIAL(jge_r_m)
};
struct jge_m_r : command  
{
	jge_m_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
  DECLARE_SERIAL(jge_m_r)
};
struct jge_m_m : command  
{
	jge_m_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
  DECLARE_SERIAL(jge_m_m)
};

struct jge_r_c : command  
{	
	jge_r_c(int len=0)  {op_1=ra(5);op_2=ra(2);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jge_r_c)
};

struct jl_r_r : command  // Comparison <
{
	jl_r_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
  DECLARE_SERIAL(jl_r_r)
};
struct jl_r_m : command  
{
	jl_r_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
  DECLARE_SERIAL(jl_r_m)
};
struct jl_m_r : command  
{
	jl_m_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jl_m_r)
};
struct jl_m_m : command  
{
	jl_m_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jl_m_m)
};

struct jl_r_c : command  
{	
	jl_r_c(int len=0)  {op_1=ra(5);op_2=ra(2);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jl_r_c)
};

struct jg_r_r : command  // Comparison >
{
	jg_r_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jg_r_r)
};
struct jg_r_m : command  
{
	jg_r_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jg_r_m)
};
struct jg_m_r : command  
{
	jg_m_r(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jg_m_r)
};
struct jg_m_m : command  
{
	jg_m_m(int len=0) {op_1=ra(5);op_2=ra(5);do label=ra(len+1); while (len && ind_command==label);} 
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jg_m_m)
};

struct jg_r_c : command  
{	
	jg_r_c(int len=0)  {op_1=ra(5);op_2=ra(2);do label=ra(len+1); while (len && ind_command==label);}
	virtual int run(program* prg);
  virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(jg_r_c)
};

struct add_r_r : command  // Arithmetic
{ add_r_r(){op_1=ra(5);op_2=ra(5);} ;
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(add_r_r)
};
struct add_r_c : command  
{
	add_r_c(){op_1=ra(5);op_2=1;}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(add_r_c)
};
struct sub_r_r : command  
{	sub_r_r(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(sub_r_r)
};
struct sub_r_c : command  
{
	sub_r_c(){op_1=ra(5);op_2=1;}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(sub_r_c)
};


struct mov_r_r : command  // Assignment
{	mov_r_r(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(mov_r_r)
};

struct mov_r_m : command 
{ mov_r_m(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(mov_r_m)
};

struct mov_m_r : command 
{  mov_m_r(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(mov_m_r)
};

struct mov_m_m : command 
{ mov_m_m(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);	
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(mov_m_m)
};

struct mov_r_c : command  
{ mov_r_c(){op_1=ra(5);op_2=ra(2);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(mov_r_c)
};


struct xchg_r_r : command  // Exchange
{ xchg_r_r(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(xchg_r_r)
};
struct xchg_r_m : command  
{ xchg_r_m(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(xchg_r_m)
};
struct xchg_m_r : command  
{ xchg_m_r(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(xchg_m_r)
};
struct xchg_m_m :  command  
{ xchg_m_m(){op_1=ra(5);op_2=ra(5);}
	virtual int run(program* prg);
	virtual void print(program* prg, CEdit* ed); 
	DECLARE_SERIAL(xchg_m_m)
};

template<class Arg1, class Arg2, class Result>
    struct ag_binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
    };

template<class T>
struct less_sz  : public ag_binary_function<T, T, bool> {        // Size optimization
    bool operator()(const T& pr1, const T& pr2) const
		{
			if(pr1.qual>pr2.qual) return true;
			if(pr1.qual<pr2.qual) return false;         
			else
			{ if(pr1.prog_len < pr2.prog_len) return true;
			  else return false;   		
			}
			return false;
		}
 };

template<class T>
struct less_sz_un  : public ag_binary_function<T, T, bool> {        // Size optimization
    bool operator()(const T& pr1, const T& pr2) const
		{
			if(pr1.qual>pr2.qual) return true;
			if(pr1.qual<pr2.qual) return false;         
			else
			{ if(pr1.prog_len < pr2.prog_len) return true;
				if(pr1.prog_len > pr2.prog_len) return false;
			  else 
				{	if(memcmp(pr1.cmds,pr2.cmds,pr1.prog_len*sizeof(command)) < 0) return true;
					else return false;
				}	
			}
			return false;
		}
 };



template<class T>
struct great_sp  : public ag_binary_function<T, T, bool> 
{   // Speed optimization
    bool operator()(const T& pr1, const T& pr2) const
		{
			if(pr1.qual>pr2.qual) return true;
			if(pr1.qual<pr2.qual) return false;         
 		 else
			{ if(pr1.takt < pr2.takt) return true;
			  else return false;   
			}
		return false;
		}
};

template<class T>
struct great_sp_un  : public ag_binary_function<T, T, bool> 
{   // Speed optimization
    bool operator()(const T& pr1, const T& pr2) const
		{
			if(pr1.qual>pr2.qual) return true;
			if(pr1.qual<pr2.qual) return false;         
 		  else
				{ if(pr1.takt < pr2.takt) return true;
					if(pr1.takt > pr2.takt) return false;
					else 
					{ if(pr1.prog_len < pr2.prog_len) return true;
						if(pr1.prog_len > pr2.prog_len) return false;
						else 
						{	if(memcmp(pr1.cmds,pr2.cmds,pr1.prog_len*sizeof(command)) < 0) return true;
							else return false;
						}	
					}			   
				}
			return false;
		}
};


template<class T>
struct great_qu  : public ag_binary_function<T, T, bool> 
{   // Without optimization
    bool operator()(const T& pr1, const T& pr2) const
		{
			if(pr1.qual>pr2.qual) return true;
			else return false;
		}
 };

template<class T>
struct great_qu_un  : public ag_binary_function<T, T, bool> 
{   // Without optimization
    bool operator()(const T& pr1, const T& pr2) const
		{
			if(pr1.qual>pr2.qual) return true;
			if(pr1.qual<pr2.qual) return false;         
			else
			{ if(pr1.prog_len < pr2.prog_len) return true;
				if(pr1.prog_len > pr2.prog_len) return false;
			  else 
				{	if(memcmp(pr1.cmds,pr2.cmds,pr1.prog_len*sizeof(command)) < 0) return true;
					else return false;
				}	
			}
			return false;
		}
 };


template<class T> 
struct pr_eq : public ag_binary_function<T, T, bool>
{
	 bool operator()(const T& pr1, const T& pr2) const
		{	if(pr1.prog_len != pr2.prog_len) return false;
			if(!memcmp(pr1.cmds,pr2.cmds,pr1.prog_len*sizeof(command))) return true;
			else return false;		   
		} 
};


struct program : public CObject// Program instance
{ 
	command cmds[MAX_PROGRAM_LENGTH]; // List of operators
	int prog_len;	// Program length
	int age;      // Generation number
	char sign[8];  // signature (unique program identifier)
	int qual;      // Level of quality
	int status;      // Functional status: 0 - operational, 1 - looping, 2 - memory error, 255 - undefined
	int takt; // Number of ticks in execution
	static int cp; // program counter
	int gen_prog(int len); // Program generation (returns an error)
	void gen_sample(); // Sample program generation
	int run(int *);	  // Program execution (returns an error)
	void listing(CEdit* ed); // Listing in the window
	program(); // Constructor
	program(const program&); // copy constructor
	void operator=(const program&); // copy operator
	void Serialize(CArchive& ar);
	bool operator==(const program& pr);     
	void sign_user(); // Custom Signature
	void test(); // Quality and Status Check
	void make_mutation(BOOL lab); // Command mutation
	DECLARE_SERIAL(program)
};


struct rules  // Result parser
{ int max_qual;
	int test_qual;
	int weight[32768];
  void init_rules();
	int get_qual();
};
extern rules rule;
using namespace std;

struct population 
{
	int pp; 
	vector<program> prg;
	void make_population(int len);
	void save_population(CArchive& ar);
	void load_population(CArchive& ar);	
	population(){pp=0; prg.clear();}
};

extern population pop;
extern vector<program> hist;
extern program temp_prg;

struct base_check;
extern int (__stdcall *check_alg)(base_check *);
extern int (__stdcall *get_max_qual)();
extern int (__stdcall *get_base_qual)();
extern int* (__stdcall *get_reg_one)();
extern int* (__stdcall *get_mem_one)();
extern void (__stdcall *init_array)();
extern const char* (__stdcall *about)();