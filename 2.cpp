#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<typeinfo>
const double eps = 10E-8;
class Polynom{
    private:
        std::string str;
        std::vector<double> coefs;
        bool isNum(int n){
            if(str[n]>='0'&&str[n]<='9'||str[n]=='.'){
                return true;
            }
            return false;
        }
        bool isSign(int n){
            if(str[n]=='+'||str[n]=='-'){
                return true;
            }
            return false;
        }
        bool invalidSym(int n){
            if(!isNum(n)&&!isSign(n)&&str[n]!='x'&&str[n]!='^'){
                return true;
            }
            return false;
        }
        void init(){
            int i = 1;
            while(i<str.size()){
                std::string monomial = "";
                monomial.push_back(str[i-1]);
                while(i<str.size()&&!isSign(i)){
                    monomial = monomial+str[i];
                    ++i;
                }
                int j = 1;
                bool temp = true;
                std::string coefStr = "";
                while(j<monomial.size()&&monomial[j]!='x'){
                    if(monomial[j]>='0'&&monomial[j]<='9'||monomial[j]=='.'){
                        coefStr = coefStr+monomial[j];
                        j++;
                    }else{
                        std::cout<<"error0";
                        exit(1);
                    }
                }
                int coef = 1;
                if(coefStr.size()!=0){
                    coef = std::stod(coefStr);
                }
                if(monomial[0]=='-'){
                    coef*=-1;
                }
                int pow = 0;
                if(j==monomial.size()){//Ax^0==A
                    coefs[0]=coef;
                }
                else if(j==monomial.size()-1){//Ax^1==Ax
                    if(coefs.size()==1){
                        coefs.resize(2);
                    }
                    coefs[1]=coef;
                }else{
                    ++j;
                    if(monomial[j]!='^'){
                        std::cout<<monomial[j];
                        std::cout<<"error1";
                        exit(0);
                    }
                    ++j;
                    std::string powStr = "";
                    while(j<monomial.size()){
                        if(!(monomial[j]>='0'&&monomial[j]<='9')){
                            std::cout<<"error2";
                            exit(1);
                        }
                        powStr = powStr+monomial[j];
                        j++;
                    }
                    pow = std::stoi(powStr);
                    if(coefs.size()<=pow){
                        coefs.resize(pow+1);
                    }
                    coefs[pow] = coef;
                }
                i++;
            }
        }
        void formatString(){
            str.erase(std::remove(str.begin(),str.end(),' '),str.end());
            if(str[0]!='+'||str[0]!='-'){
                str = "+"+str;
            }
            for(int i = 1;i<str.size();i++){
                if(!isNum(i)&&!isSign(i)&&str[i]!='+'&&str[i]!='-'&&str[i]!='x'&&str[i]!='^'&&str[i]!='.'){
                    std::cout<<"invalid char";
                    exit(1);    
                }
                if((isSign(i)&&isSign(i-1))||(str[i]=='x'&&str[i-1]=='x')||(str[i-1]=='.'&&(str[i]>='9'||str[i]<='0'))){
                    std::cout<<"invalid signs";
                    exit(1);
                }
                
            }
        }
        void minimize(){
            int sz = coefs.size()-1;
            while (coefs[sz]==0&&sz>0){
                --sz;
            }
            coefs.resize(sz+1);
        }
    public:
        std::vector<double> Coefs(){
            return coefs;
        }
        Polynom(const std::string& str_):str(str_){
            coefs = std::vector<double>(1);
            formatString();
            init();
            minimize();
        }
        Polynom(const std::vector<double>& coefs_){
            coefs = coefs_;
            minimize();
        }
        Polynom(const Polynom& pol):coefs(pol.coefs){
            
        }
        ~Polynom() = default;
        Polynom& operator=(const Polynom &pol){
            if (this == &pol){
                return *this;
            }
            this->coefs = pol.coefs;
            return *this;
        }
        
        friend bool operator==(const Polynom& pol1,const Polynom& pol2);
        friend Polynom operator+(const Polynom& pol1,const Polynom& pol2);
        friend Polynom operator-(const Polynom& pol1,const Polynom& pol2);
        friend std::ostream& operator<< (std::ostream &out, const Polynom &pol);
        friend std::istream& operator>> (std::istream &in, Polynom &pol);
        void print() const{
            for(int i = 0;i<coefs.size();++i){
                std::cout<<coefs[i]<<" ";
            }
            std::cout<<std::endl;
        }
        Polynom& operator-(){
            for(int i = 0;i<coefs.size();i++){
                coefs[i] = -coefs[i];
            }
            return *this;
        }
        Polynom& operator*(const Polynom& pol){
            int l = coefs.size()+pol.coefs.size()-1;
            std::vector<double> newCoefs = std::vector<double>(l); 
            for(int i = 0;i<pol.coefs.size();++i){
                if(abs(pol.coefs[i])>eps){
                    for(int j=0;j<coefs.size();++j){
                        if(abs(coefs[i])>eps){
                            newCoefs[i+j] += pol.coefs[i]*coefs[j];
                        }
                    }
                }
            }
            coefs = newCoefs;
            return *this;
        }
        Polynom& operator/(const double& n){
            if(abs(n)<eps){
                std::cout<<"division by zero";
                exit(1);
            }
            for(int i = 0;i<coefs.size();i++){
                coefs[i]=coefs[i]/n;
            }
            return *this;
        }
        Polynom& operator-=(const Polynom& pol){
            *this = (*this - pol);
            return *this;
        }
        Polynom& operator+=(const Polynom& pol){
            *this = (*this + pol);
            return *this;
        }
        Polynom& operator*=(const Polynom& pol){
            *this = (*this * pol);
            return *this;
        }
        Polynom& operator/=(const double& n){
            *this = *this/n;
            return *this;
        }
        double operator[](const int& i){
            if(i>=coefs.size()||i<0){
                std::cout<<"index in polynom out of range!";
                exit(1);
            }
            return coefs[i];
        }
    };
bool operator==(const Polynom& pol1,const Polynom& pol2){
    if(pol1.coefs.size()!=pol2.coefs.size()){
        return false;
    }
    for(int i = 0;i<pol1.coefs.size();i++){
        if(pol1.coefs[i]!=pol2.coefs[i]){
            return false;
        }
    }
    return true;
};
bool operator!=(const Polynom& pol1,const Polynom& pol2){
    return !(pol1==pol2);
}
Polynom operator-(const Polynom& pol1,const Polynom& pol2){
    int l = std::max(pol1.coefs.size(),pol2.coefs.size());
    std::vector<double> ans = std::vector<double>(l);
    int i = 0;
    for(i;i<std::min(pol1.coefs.size(),pol2.coefs.size());i++){
        ans[i] = pol1.coefs[i]-pol2.coefs[i];
    }
    if(pol1.coefs.size()>pol2.coefs.size()){
        for(i;i<l;i++){
            ans[i] = pol1.coefs[i];
        }
    }else{
        for(i;i<l;i++){
            ans[i] = -pol2.coefs[i];
        }
    }
    Polynom out(ans);
    return out;
}
Polynom operator+(const Polynom& pol1,const Polynom& pol2){
    int l = std::max(pol1.coefs.size(),pol2.coefs.size());
    std::vector<double> coefs = std::vector<double>(l);
    int i = 0;
    for(i;i<std::min(pol1.coefs.size(),pol2.coefs.size());i++){
        coefs[i] = pol1.coefs[i]+pol2.coefs[i];
    }
    if(pol1.coefs.size()>pol2.coefs.size()){
        for(i;i<l;i++){
            coefs[i] = pol1.coefs[i];
        }
    }else{
        for(i;i<l;i++){
            coefs[i] = pol2.coefs[i];
        }
    }
    Polynom out(coefs);
    return out;
}
std::ostream& operator<<(std::ostream &out, const Polynom &pol){
    int l = pol.coefs.size();
    out<<pol.coefs[l-1]<<"x^"<<l-1;
    for(int i = 1;i<pol.coefs.size()-2;i++){
        if(pol.coefs[l-i-1]!=0){
            if(pol.coefs[l-i-1]>0){
                out<<"+";
            }
            if(abs(pol.coefs[l-i-1]-1)<eps){
                out<<"x^"<<l-i-1;
            }else if(abs(pol.coefs[l-i-1]+1)<eps){
                out<<"-x^"<<l-i-1;
            }else{
                out<<pol.coefs[l-i-1]<<"x^"<<l-i-1;
            }
        }
    }
    if(l>1){
        if(pol.coefs[1]!=0){
            if(pol.coefs[1]>0){
                out<<"+";
            }
            if(abs(pol.coefs[1]-1)<eps){
                out<<"x";
            }else if(abs(pol.coefs[1]+1)<eps){
                out<<"-x^";
            }else{
                out<<pol.coefs[1]<<"x";
            }
        }
    }
    if(pol.coefs[0]!=0){
        if(pol.coefs[0]>0){
                out<<"+";
            }
        out<<pol.coefs[0];
    }
    return out;
}
std::istream& operator>> (std::istream &in, Polynom &pol){
    std::string str;
    in>>str;
    pol = Polynom(str);
    return in;
}
int main(){
    std::string s = "2+1x+34x^2";
    std::string s1 = "1+1x+2x^2";
    Polynom test(s);
    Polynom test1(s1);
    Polynom res("2x");
    res = test*test1*res/77-Polynom("x+x^2");
    std::cout<<res<<std::endl<<res[0];
}