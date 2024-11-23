
class token
{
public:
    int id;
    std::string name;
    token(int x,char s[]) : id(x),name(s)
    {
    }

    token(std::string x)
    {
        char s[MaxLenOfCommand];
        sscanf(x.c_str(), "(%d,%[^)])", &this->id,&s);
        name=s;
    }

    void show()
    {
        std::cout << "(" << id << ',' << name<< ')' << '\n';
    }
};
std::vector<token> Tokens; // 存读取得出的token序列

void trans_token(FILE *in)
{
    int id;
    char name[MaxLenOfCommand]={0};

    while (fscanf(in, "(%d ,%s ) ", &id, name) != EOF)
    {
        for(int i=0;i<10;i++)if(name[i]==' ')name[i]='\0';
        Tokens.push_back(token(id,name));
    }

    for (auto &x : Tokens)
    {
        x.show();
    }
}

// 还要定义First 和 Follow 集合
