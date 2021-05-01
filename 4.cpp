
#include<vector>
#include<iostream>
template<typename Iter,typename Pred>
Iter find_not(Iter begin,Iter end,Pred pred){
    while(begin<end&&pred(*begin)){
        begin++;
    }
    return begin;
}
template<typename Iter,typename Pred>
bool all_of(Iter begin,Iter end,Pred pred){
    return end==find_not(begin,end,pred);
}
template<typename Iter,typename Pred>
bool none_of(Iter begin,Iter end,Pred pred){
    while(begin<end){
        if(pred(*begin)){
            return false;
        }
        begin++;
    }
    return true;
}
template<typename Iter,typename Pred>
bool any_of(Iter begin,Iter end,Pred pred){
    return !none_of(begin,end,pred);
}
template<typename Iter,typename Pred>
bool one_of(Iter begin,Iter end,Pred pred){
    if(pred(*begin)&&none_of(begin+1,end,pred)){
        return true;
    }
    for(Iter separator = begin+1;separator<end;separator++){
        if(pred(*separator)&&none_of(begin,separator,pred)&&none_of(separator+1,end,pred)){
            return true;
        }
    }
    return false;
}
template<typename Iter,typename Pred>
Iter find_backward(Iter begin,Iter end,Pred pred){
    Iter temp = end;
    do{
        temp--;
        if(pred(*temp)){
            return temp;
        }
    }while(begin<temp);
    return end;
}
template<typename Iter,typename Pred>
bool is_sorted(Iter begin,Iter end,Pred pred){
    if(all_of(begin,end,pred)||none_of(begin,end,pred)){
        return true;
    }
    for(Iter separator = begin+1; separator<end;separator++){
        if(none_of(begin,separator,pred)&&all_of(separator,end,pred)){
            return true;
        }
    }
    return false;
}
template<typename Iter,typename Pred>
bool is_partitioned(Iter begin,Iter end,Pred pred){
    for(Iter separator = begin+1; separator<end;separator++){
        std::cout<<none_of(begin,separator,pred)<<" "<<all_of(separator,end,pred)<<"\n";
        if(none_of(begin,separator,pred)&&all_of(separator,end,pred)||none_of(separator,end,pred)&&all_of(begin,separator,pred)){
            return true;
        }
    }
    return false;
}
template<typename Iter,typename Pred>
bool is_palindrome(Iter begin,Iter end,Pred pred){
    end--;
    while(begin<end){
        if(pred(*begin)!=pred(*end)){
            return false;
        }
        begin++;
        end--;
    }
    return true;
}
int main(){
    std::vector<int> a = {8,2,3,4,4,9};
    bool b = is_palindrome(a.begin(),a.end(),[](int i){return i>5;});
    std::cout<<b;
}