#include<iterator>
#include<algorithm>
#include<iostream>
#include<vector>
#include<cstring>
int* a = new int[13];


template<class T>
class MyIterator:std::iterator<std::random_access_iterator_tag,T>{
    private:
        T* real_start_;
        T* ptr_;
        int capacity_;
    public:
        using difference_type = int;
        using value_type = T;
        using pointer = T*;
        using reference = value_type&;
        using iterator_category = std::random_access_iterator_tag;
        friend const MyIterator<T> operator+(const int n,MyIterator<T> it);
        MyIterator(const MyIterator& it):ptr_(it.ptr_),capacity_(it.capacity_),real_start_(it.real_start_){};
        MyIterator(T* data,int capacity,T* real_start):ptr_(data),capacity_(capacity),real_start_(real_start){};
        int operator-(const MyIterator<T>& it){
            return (this->ptr_-it.ptr_+capacity_)%capacity_;
        }
        bool operator!=(MyIterator<T> const& it){
            return ptr_!=it.ptr_;
        }
        bool operator==(MyIterator<T> const& it){
            return ptr_==it.ptr_;
        }
        T& operator*() const{
            return *ptr_;
        }
        MyIterator<T>& operator++(){
            ++ptr_;
            if(ptr_==(real_start_+capacity_+1)){
                ptr_ = real_start_;
            }
            return *this;
        }
        MyIterator<T>& operator--(){
            if(ptr_==real_start_){
                ptr_ = real_start_+capacity_-1;
            }else{
                --ptr_;
            }
            return *this;
        }
        MyIterator<T> operator--(int) {
            MyIterator<T> tmp = *this;
            --(*this);
            return tmp;
        }
        MyIterator<T> operator++(int) {
            MyIterator<T> tmp = *this;
            ++(*this);
            return tmp;
        }
        MyIterator<T>& operator+=(const int n){
            int dist = (ptr_+n-real_start_+capacity_)%capacity_;
            this->ptr_= real_start_+dist; 

            return *this;
        }
        const MyIterator<T> operator+(const int n){
            return MyIterator<T>(*this)+=n;
        }
        MyIterator<T>& operator-=(const int n){
            int dist = (((ptr_-n-real_start_)%capacity_)+capacity_)%capacity_;
            this->ptr_ = real_start_+dist;
            return *this;
        }
        const MyIterator<T> operator-(const int n){
            return MyIterator<T>(*this)-=n;
        }
        const T& operator[](const int n){
            return *(this+n);
        }
        
};

template<class T>
const MyIterator<T> operator+(const int n,MyIterator<T> it){
    return MyIterator<T>(*it)+=n;
}
template<class T>
class Circular_Buffer{
    private:
        T* data_;
        MyIterator<T> begin_;
        MyIterator<T> end_;
        int capacity_;
        bool is_full_ = false;//на самом деле, показывает, в каком положении пересекались последний раз концы
    public:
        
        Circular_Buffer(const Circular_Buffer& cb):capacity_(cb.capacity_),is_full_(cb.is_full_){
            memcpy(data_,cb.data_,capacity_);
            begin_ = MyIterator<T>(data_+(cb.begin_.ptr_ - cb.data_));
            end_ = MyIterator<T>(data_+(cb.end_.ptr_ - cb.data_));
        }
        Circular_Buffer(int capacity):capacity_(capacity){
            data_ = new T[capacity_];
            begin_ = MyIterator<T>(data_);
            end_ = MyIterator<T>(data_);
        }
        MyIterator<T>& begin(){
            return begin_;
        }
        MyIterator<T>& end(){
            return end_;
        }

        void push_back( const T& value){
            *end_ = value;
            ++end_;
            if(is_full_){
                ++begin_;
            }
            if(end_ == begin_){
                is_full_ = true;
            }

        }
        void push_forward(const T& value){
            --begin_
            *begin_ = value;
            if(is_full_){
                --end_;
            }
            if(end==begin){
                is_full_ = true;
            }
        }
        void pop_back(){
            if(begin_==end_&&is_full_==false){
                throw std::range_error("can't pop back of empty buffer");
            }
            --end_;
            if(begin_==end_){
                is_full_ == false;
            }
        }
        void pop_forward(){
            if(begin_==end_&&is_full_==false){
                throw std::range_error("can't pop front of empty buffer");
            }
            ++begin_;
            if(begin_==end_){
                is_full_==false;
            }
        }
        const T& back(){
            if(begin_==end_&&is_full_==false){
                throw std::range_error("can't get back of empty buffer");
            }
            return *(end-1);
        }
        const T& forward(){
            if(begin_==end_&&is_full_==false){
                throw std::range_error("can't get front of empty buffer");
            }
            return *begin;
        }
        
};

template<class InputIt, class T>
InputIt Find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}
int main(){
    for(int i = 0;i<13;i++){
        a[i] = i;
    }
    MyIterator<int> begin(a,13,a);
    MyIterator<int> end(a+13,13,a);
    auto it = std::find(begin,end,3);
    std::cout<<*it;
    
}