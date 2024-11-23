#include "compiler.h"
#include <iostream>
#include <set>
#include <algorithm>
#include<stack>
#include<fstream>

std::map<std::string, std::map<std::string, std::vector<std::string> > > MM;
std::map<std::string, std::vector<std::vector<std::string> > > NN;
std::vector<std::string> SS = std::vector<std::string>{"A", "B", "C", "D", "E", "F","H","Q", "K", "P", "T", "L"};
std::map<std::string, std::set<std::string> > F;

std::fstream error("Error.tmp",std::ios::out);
bool fail;
void printSS();
void init();
void checkMM(std::string x);
inline bool NULLABLE(std::string s);


void Error(std::string x0)
{
	fail=true;
	error<<x0<<"\n";
}


bool VT(std::string x);
void printSS();
void checkMM(std::string x)
{
	for (int i = 0; i < NN[x].size(); i++)
	{
		if (NN[x][i][0]=="NULL")
		{
			MM[x][NN[x][i][0]].push_back("NULL");
			F[x].insert("NULL");
		}
		else if(VT(NN[x][i][0]))
		{
			MM[x][NN[x][i][0]]=NN[x][i];
			F[x].insert(NN[x][i][0]);
		}
	}
}

void init()
{
	for(auto x:SS)
	{
		for(auto y:SYS_STRING)
		{
			MM[x][y]=std::vector<std::string>();
		}
		MM[x]["NULL"]=std::vector<std::string>();
	}
	for (auto x : SS)
	{
		F[x] = std::set<std::string>();
	}

	NN["A"] = std::vector<std::vector<std::string>>();
	NN["B"] = std::vector<std::vector<std::string>>();
	NN["C"] = std::vector<std::vector<std::string>>();
	NN["D"] = std::vector<std::vector<std::string>>();
	NN["E"] = std::vector<std::vector<std::string>>();
	NN["Q"] = std::vector<std::vector<std::string>>();
	NN["K"] = std::vector<std::vector<std::string>>();
	NN["P"] = std::vector<std::vector<std::string>>();
	NN["T"] = std::vector<std::vector<std::string>>();
	NN["L"] = std::vector<std::vector<std::string>>();

	NN["A"].push_back({"const", "ident", ":=", "B"});

	NN["B"].push_back({"ident"});
	NN["B"].push_back({"number"});

	NN["C"].push_back({"var", "ident"});

	NN["D"].push_back({"begin", "E", "end"});

	NN["E"].push_back({"Q", ";", "H"});
	NN["H"].push_back({"Q", ";", "H"});
	NN["H"].push_back({"NULL"});

	NN["F"].push_back({"A", ";", "F"});
	NN["F"].push_back({"C", ";", "F"});
	NN["F"].push_back({"D", "F"});
	NN["F"].push_back({"NULL"});

	NN["Q"].push_back({"call", "ident"});
	NN["Q"].push_back({"if", "L", "then", "Q"});
	NN["Q"].push_back({"while", "L", "do", "Q"});
	NN["Q"].push_back({"D"});
	NN["Q"].push_back({"ident", ":=", "K"});

	NN["K"].push_back({"+", "T", "P"});
	NN["K"].push_back({"-", "T", "P"});
	NN["K"].push_back({"T", "P"});

	NN["P"].push_back({"+", "T"});
	NN["P"].push_back({"-", "T"});
	NN["P"].push_back({"NULL"});

	NN["T"].push_back({"ident"});
	NN["T"].push_back({"(", "K", ")"});

	NN["L"].push_back({"K", "<>", "K"});
	NN["L"].push_back({"K", "odd", "K"});

	for (auto x : SS)
		checkMM(x);
}

inline bool NULLABLE(std::string s)
{
	return MM[s]["NULL"].size() == 0;
}

/*
	MM["A"]  -- A对应的表达式集合
	{
		'var':"A","B"，“C"
		'NULL':"NULL"
	}


*/

struct biaodashi
{
	std::vector<std::string> first, follow;
};


bool VT(std::string x) // 判断string x 是不是终结符,是终结符返回true,否则返回假
{
	for (auto &t : SYS_STRING)
	{
		if (x == t)
			return true;
	}
	return false;
}

void First_create()
{
	// printSS();
	// for(auto x: SS)
	// {
	// 	std::cout<<x<<" ";
	// }
	bool change = 1;
	while (change)
	{
		change = 0;
		for (auto q:SS)
		{
			for (auto t : NN[q])
			{
				for (auto j : t)
				{
					if (!VT(j))
					{
						int _size = F[q].size();
						for(auto y : F[j])
						{
							MM[q][y]=t;
							F[q].insert(y);
						}
						if (_size < F[q].size())
							change = 1;
					}
					else if (F[q].find(j) == F[q].end())
					{
						MM[q][j]=t;
						F[q].insert(j);
						change = 1;
					}
					else break;
					if (NULLABLE(j))
						break;
				}
			}
		}
	}
}

std::vector<biaodashi> TOT;
void printSS()
{
	for (int i = 0; i < SS.size(); i++)
	{
		std::cout << SS[i] << '\n';
		for (auto j : F[SS[i]])
		{
			std::cout << j << ' ';
		}
		std::cout << "\n\n\n";
	}
}
void printMM()
{
	for(auto j :SS)
	{
		std::cout<<"********************************************\n";
		std::cout<<j<<"\n";
		std::cout<<"\n";
		
		for(auto k : SYS_STRING)
		{
			if(MM[j][k].size()==0)continue;

			std::cout<<"	"<<k<<'\n';
			std::cout<<"		";
			std::cout<<j<<"-->";
			for(auto x: MM[j][k])std::cout<<x<<' ';
			std::cout<<'\n';
		}
	}
}

void solve()
{
	int i=0;
	std::stack<std::string>ST;
	auto get=[&]()
	{
		std::string ans;
		if(i>=Tokens.size())return std::string("ERROR");
		if(Tokens[i].id== SYS_IDENT)ans=std::string("ident");
		else if(Tokens[i].id ==SYS_NUMBER)ans=std::string("number");
		else ans=Tokens[i].name;
		i++;
		return ans;
	};

	std::string ans;
	ST.push("#");
	ST.push("F");
	ans=get();
	while(!ST.empty())
	{
		if(VT(ST.top()))
		{
			if(ST.top()==ans)
			{
				ST.pop();
				ans=get();
			}
			else
			{
				Error("期望得到"+ans+ "符号");
				break;
			} 
				
		}
		else 
		{
			if(ST.top()=="#")
			{
				if(ans==".")
				{
					break;
				}
				else 
				{
					Error("缺少文件结束符!!");
					break;
				}
			}
			else 
			{
				std::string q;
				
			
				q=ST.top();
				ST.pop();
				if(MM[q][ans].size()==0&&MM[q]["NULL"].size()==0)
				{
					Error("未定义符号"+ans);
					break;
				}
				else if(MM[q]["NULL"].size()!=0&&MM[q][ans].size()==0)
				{
					continue;
				}
				for(int j=MM[q][ans].size()-1;j>=0;j--)
				{
					if(MM[q][ans][j]!="NULL")
					ST.push(MM[q][ans][j]);
				}
			}
			
		}
	}
	if(fail)std::cout<<"编译失败!";
	else std::cout<<"编译成功!";

}
void process()
{
	init();
	First_create();
	// printSS();
	printMM();
	solve();
}

int main()
{
	CreateMap();

	in = fopen("in", "r");
	out = fopen("out", "w");

	compile(in);

	fclose(in);
	fclose(out);

	in = fopen("out", "r");
	trans_token(in);
	process();
	return 0;
}