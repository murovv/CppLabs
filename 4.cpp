#include<iostream>
#include<vector>
#include<forward_list>
#include<math.h>
#include<unordered_set>
using std::cout;
using std::endl;
template<typename Iter,typename Pred>
Iter find_not(Iter begin,Iter end,Pred pred){
    while(begin!=end&&pred(*begin)){
        ++begin;
    }
    return begin;
}
template<typename Iter,typename Pred>
bool all_of(Iter begin,Iter end,Pred pred){
    return end==find_not(begin,end,pred);
}
template<typename Iter,typename Pred>
bool none_of(Iter begin,Iter end,Pred pred){
    while(begin!=end){
        if(pred(*begin)){
            return false;
        }
        ++begin;
    }
    return true;
}
template<typename Iter,typename Pred>
bool any_of(Iter begin,Iter end,Pred pred){
    return !none_of(begin,end,pred);
}
template<typename Iter,typename Pred>
bool one_of(Iter begin,Iter end,Pred pred){
    int count = 0;
    while(begin!=end){
        if(pred(*begin)){
            ++count;
        }
        ++begin;
    }
    if(count==1){
        return true;
    }
    return false;
}
template<typename Iter,typename Pred>
Iter find_backward(Iter begin,Iter end,Pred pred){
    Iter ans = end;
    while(begin!=end){
        if(pred(*begin)){
            ans = begin;
        }
        ++begin;
    }
    return ans;
}
template<typename Iter,typename Pred>
bool is_sorted(Iter begin,Iter end,Pred pred){
    Iter first = begin;
    Iter second = begin;
    if(begin==end){
        return true;
    }
    ++second;
    while(second!=end){
        if(!pred(*first,*second)){
            return false;
        }
        ++first;
        ++second;
    }
    return true;
}
template<typename Iter,typename Pred>
bool is_partitioned(Iter begin,Iter end,Pred pred){
    if(begin==end){
        return false;
    }
    while (begin!=end && pred(*begin)) {
        ++first;
    }
    while (begin!=end) {
        if (pred(*begin)){
            return false;
        }
        ++begin;
    }
    return true;
}
template<typename Iter>
bool is_palindrome(Iter begin,Iter end){
    //проверка наличия -- и копирование в вектор (либо за квадрат времени)
    --end;
    while(begin!=end){
        if(*begin!=*end){
            return false;
        }
        ++begin;
        --end;
    }
    return true;
}

class Point{
    private:
        
        
    public:
        double x;
        double y;
        double dist(){
            return sqrtl(x*x+y*y);
        }
        Point(double x_, double y_):x(x_),y(y_){};
        Point(const Point& p):x(p.x),y(p.y){};
        ~Point() = default;
        
        
};
bool operator<(Point  p1,Point p2){
            return ((p1.dist()) < p2.dist());
        }
        bool operator>(Point  p1,Point p2){
            return ((p1.dist()) >p2.dist());
        }
        bool operator==(Point  p1,Point p2){
            return (p1.x==p2.x&&p1.y==p2.y);
        }
        bool operator!=(Point  p1,Point p2){
            return !(p1==p2);
        }

int main(){
    std::vector<int> a = {1,5,7,2,4,6,2};
    std::forward_list<double> b {1.3,1.5,6.3,7.008,9.3,89.1,500.3};
    std::vector<Point> c  = {Point(1.3,0),Point(20.0,10.0),Point(20.0,13.0),Point(30.1,20.9)};

    cout<<"vector<int>\n";
    cout<<"find not <6: "<<*find_not(a.begin(),a.end(),[](int i){return i<6;})<<endl;
    cout<<"all_of <8: "<<all_of(a.begin(),a.end(),[](int i){return i<8;})<<endl;
    cout<<"none_of ==4: "<<none_of(a.begin(),a.end(),[](int i){return i>5;})<<endl;
    cout<<"any_of ==5: "<<any_of(a.begin(),a.end(),[](int i){return i==5;})<<endl;
    cout<<"one_of ==2: "<<one_of(a.begin(),a.end(),[](int i){return i==2;})<<endl;
    cout<<"find_backward ==6 coord: "<<a.end()-find_backward(a.begin(),a.end(),[](int i){return i==6;})<<endl;
    cout<<"is_sorted : "<<is_sorted(a.begin(),a.end(),[](int i,int j){return i<j;} )<<endl;
    cout<<"is_partitioned : "<<is_partitioned(a.begin(),a.end(),[](int i){return i%2==0;} )<<endl;
    cout<<"is_palindrome : "<<is_palindrome(a.begin(),a.end())<<endl;

    cout<<"\nf_l<double>\n";
    cout<<"find not <6: "<<*find_not(b.begin(),b.end(),[](double i){return i<6;})<<endl;
    cout<<"all_of <8: "<<all_of(b.begin(),b.end(),[](double i){return i<8;})<<endl;
    cout<<"none_of ==4: "<<none_of(b.begin(),b.end(),[](double i){return i>5;})<<endl;
    cout<<"any_of ==5: "<<any_of(b.begin(),b.end(),[](double i){return i==5;})<<endl;
    cout<<"one_of ==2: "<<one_of(b.begin(),b.end(),[](double i){return i==2;})<<endl;
    cout<<"find_backward ==6.3: "<<*find_backward(b.begin(),b.end(),[](double i){return i==6.3;})<<endl;
    cout<<"is_sorted : "<<is_sorted(b.begin(),b.end(),[](double i,double j){return i<j;} )<<endl;
    cout<<"is_partitioned i>6 : "<<is_partitioned(b.begin(),b.end(),[](double i){return i>6;} )<<endl;
    //cout<<"is_palindrome : "<<is_palindrome(b.begin(),b.end())<<endl; - нет оператора --

    cout<<"\nv<Point>\n";

    cout<<"all_of >(1,0): "<<all_of(c.begin(),c.end(),[](Point i){return i>Point(1,0);})<<endl;
    cout<<"none_of <(1,1): "<<none_of(c.begin(),c.end(),[](Point i){return i<Point(1,1);})<<endl;
    cout<<"any_of >(20,30): "<<any_of(c.begin(),c.end(),[](Point i){return i>Point(20,30);})<<endl;
    cout<<"one_of >Point(20,30): "<<one_of(c.begin(),c.end(),[](Point i){return i>Point(20,30);})<<endl;
    cout<<"find_backward (20,30): "<<find_backward(c.begin(),c.end(),[](Point i){return i==Point(20.0,10.0);})->x<<endl;
    cout<<"is_sorted : "<<is_sorted(c.begin(),c.end(),[](Point i,Point j){return i<j;} )<<endl;
    cout<<"is_partitioned : "<<is_partitioned(c.begin(),c.end(),[](Point i){return i.dist()>=Point(20,10).dist();} )<<endl;
    cout<<"is_palindrome : "<<is_palindrome(c.begin(),c.end())<<endl;
}
