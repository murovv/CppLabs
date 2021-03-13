#define _USE_MATH_DEFINES
#include<iostream>
#include<vector>
#include<utility>
#include<math.h>
#include<tuple>
#include<math.h>

const double eps = 10E-6;
class Point{
    public:
        double X() const{
            return x;
        }
        double Y() const{
            return y;
        }
        void setX(double x){
            this->x = x;
        }
        void setY(double y){
            this->y = y;
        }
        Point(double x=0,double y=0){
            this->x = x;
            this->y = y;
        }
        Point(const Point &dot){
            this->x = dot.X();
            this->y = dot.Y();
        }
        ~Point(){

        }
        Point operator=(Point p){
            this->x = p.X();
            this->y = p.Y();
            return *this;
        }
        Point& operator=(std::pair<double,double> p){
            this->x = p.first;
            this->y = p.second;
            return *this;
        }
        
        
    private:
        double x,y;
};
class Chain{
    private:
        
    protected:
        std::vector<Point> points;
        void _print(std::string name){
            std::cout<<name<<", size = "<<points.size()<<std::endl;
            for(int i = 0;i<points.size();i++){
                std::cout<<"("<<round(points[i].X()*100)/100<<";"<<round(points[i].Y()*100)/100<<")";
                if(i!=points.size()-1){
                    std::cout<<",";
                }
            }
            std::cout<<std::endl;
        }
    public:
        std::vector<Point> Points() const{
            return points;
        }
        int size() const{
            return points.size();
        }
        Chain(std::vector<Point> points = std::vector<Point>()){
            this->points = points;
        }
        Chain(const Chain &chain){
            if(&chain==this){
                std::cout<<"что то очень плохое";
            }
            this->points = chain.Points();
        }
        ~Chain(){
            points.clear();
        }
        Chain& operator=(Chain c){
            this->points = c.Points();
            return *this;
        }
        virtual double perimeter() const{
            double len = 0;
            for(int i = 0;i<points.size()-1;i++){
                len+=sqrt(pow(points[i].X() - points[i+1].X(),2)+pow(points[i].Y() - points[i+1].Y(),2));
            }
            return len;
        }
        virtual void print(){
            _print("Chain");
        }
};
class ClosedChain:public Chain{
    private:
    public:
        ClosedChain(std::vector<Point> points = std::vector<Point>()):Chain(points){
            
            //наверно должна быть какая то обработка случая с 2-мя точками
        }
        ClosedChain(const ClosedChain &chain):Chain(chain){   
        }
        ClosedChain& operator=(ClosedChain c){
            Chain::operator=(c);
            return *this;
        }
        ~ClosedChain() = default;
        double perimeter() const override{
            double len = Chain::perimeter();
            len+=sqrt(pow(points[0].X() - points[points.size()-1].X(),2)+pow(points[0].Y() - points[points.size()-1].Y(),2));
            return len;
        }
        virtual void print() override{
            _print("Closed Chain");
        }

};
class Polygon:public ClosedChain{
    protected:
        Polygon(){ 
        }
        bool isConvexAndSimple(){
            int sz = this->size();
            int side_=side(points[0],points[1],points[2]); 
            for(int i = 0;i<sz;i++){
                for(int j = 0;j<sz;j++){
                    if(i!=j&&(i+1)%sz!=j){
                        int check = side(points[i%sz],points[(i+1)%sz],points[j]);
                        if(side_==0){
                            side_ = check;
                        }else if(check!=0&&side_!=check){
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    private:
        int side(Point x,Point y,Point z){
            Point v1(y.X()-x.X(),y.Y()-x.Y());
            Point v2(z.X()-x.X(),z.Y()-x.Y());
            int det = v1.X()*v2.Y()-v1.Y()*v2.X();
            if(det>0){
                return 1;
            }
            if(det<0){
                return -1;
            }
            return 0;
        }
        bool isValid(){
            if(size()>2&&isConvexAndSimple()){
                return true;
            }
        }
    public:
        Polygon(const std::vector<Point> points ):ClosedChain(points){
            if(!this->isValid()){
                std::cout<<"Invalid polygon"<<std::endl;
                exit(1);
            }
        }
        
        Polygon(const Polygon &p):ClosedChain(p){
            if(!this->isValid()){
                std::cout<<"Invalid polygon"<<std::endl;
                exit(1);
            }
        }
        ~Polygon() = default;
        Polygon &operator=(const Polygon p){
            ClosedChain::operator=(p);
            return *this;
        }
        virtual double area() const{
            double s = 0;
            int sz = size();
            for(int i  = 0;i<sz;i++){
                s+=points[i].X()*(points[(i-1+sz)%sz].Y()-points[(i+1)%sz].Y());
            }
            s = abs(s)/2;
            return s;
        }
        virtual void print() override{
            _print("Polygon");
        }
};
class Trapeze:public Polygon{
    private:
        bool isTrapeze(){
            if(size()!=4){
                return false;
            }
            for(int i = 0;i<2;i++){
                Point v1(points[i].X()-points[i+1].X(),points[i].Y()-points[i+1].Y());
                Point v2(points[(i+3)%4].X()-points[(i+2)%4].X(),points[(i+3)%4].Y()-points[(i+2)%4].Y());
                if((v1.X()==0&&v2.X()==0)||(v1.Y()==0&&v2.Y()==0)){
                    return true;
                }
                double k1 = v1.X()/v2.X();
                double k2 = v1.Y()/v2.Y();
                if(abs(k1-k2)<eps){
                    return true;
                }
            }
            return false;
        }
    public:
        virtual void print() override{
            _print("Trapeze");
        }
        Trapeze(std::tuple<Point,Point,Point,Point> p){
            this->points = std::vector<Point>(4);
            this->points[0] = std::get<0>(p);
            this->points[1] = std::get<1>(p);
            this->points[2] = std::get<2>(p);
            this->points[3] = std::get<3>(p);
            if(!isTrapeze()){
                std::cout<<"Invalid trapeze"<<std::endl;
                exit(1);
            }
        }

        Trapeze(const std::vector<Point> points){
            this->points = points;
            if(!this->isTrapeze()){
                std::cout<<"Invalid trapeze"<<std::endl;
                exit(1);
            }
        }
        Trapeze(Trapeze &tr):Polygon(tr){

        }
        Trapeze &operator=(const Trapeze p){
            Polygon::operator=(p);
            return *this;
        }
        ~Trapeze() = default;
};
class Triangle:public Polygon{
    private:
        virtual std::string name() const{
            return "Triangle";
        }
    public:
        Triangle(std::tuple<Point,Point,Point> p){
            this->points = std::vector<Point>(3);
            this->points[0] = std::get<0>(p);
            this->points[1] = std::get<1>(p);
            this->points[2] = std::get<2>(p);
        }
        Triangle(const std::vector<Point> points){
            this->points = points;
            if(points.size()!=3){
                std::cout<<"invalid number of points in triangle"<<std::endl;
                exit(1);
            }
        }
        Triangle(Triangle &triangle):Polygon(triangle){

        }
        ~Triangle() = default;
        Triangle &operator=(const Triangle p){
            Polygon::operator=(p);
            return *this;
        }
        virtual void print() override{
            _print("Triangle");
        }
};
class RegularPolygon:public Polygon{
    private:
        int n;
        void rotateVector(Point* v, double angle){
            double x = v->X();
            double y = v->Y();
            v->setX(x*cos(angle)+y*sin(angle));
            v->setY(-x*sin(angle)+y*cos(angle));
        }
    public:
        virtual void print() override{
            _print("RegularPolygon");
        }
        RegularPolygon(Point a,Point b,int n){
            this->n = n;
            if(n<3){
                std::cout<<"Invalid number of points in regular polygon!";
                exit(1);
            }
            double angle = 2*M_PI/n;
            Point ab  = Point(b.X()-a.X(),b.Y()-a.Y());
            points = std::vector<Point>(n);
            points[0] = a;
            points[1] = b;
            for(int i = 2;i<n;i++){
                rotateVector(&ab,angle);
                points[i] = Point(points[i-1].X()+ab.X(),points[i-1].Y()+ab.Y());
            }

        }
        RegularPolygon(RegularPolygon &rp):Polygon(rp){
        }
        RegularPolygon &operator=(const RegularPolygon p){
            Polygon::operator=(p);
            return *this;
        }
        ~RegularPolygon() = default;
        virtual double area() const override{
            double a = sqrt(pow(points[0].X()-points[1].X(),2)+pow(points[0].Y()-points[1].Y(),2));
            double s = n*pow(a,2)/(4*tan(M_PI/n));
            return s;
        }
        double perimeter() const override{
            double l = sqrt(pow(points[0].X()-points[1].X(),2)+pow(points[0].Y()-points[1].Y(),2));
            return l*n;
        }
         
        
};
int main(){
    int sz = 10;
    srand(2847);
    std::vector<Point> pnts = std::vector<Point>(sz);
    for(int i = 0;i<sz;i++){
        double first = (rand()%1000)/10.0;
        double second = (rand()%1000)/10.0;
        Point x{first,second};
        pnts[i]=x;
    }
    ClosedChain closedChain{pnts};
    Chain chain{pnts};
    RegularPolygon rp(Point(1,0),Point(0,0),3);
    Triangle triangle(std::tuple<Point,Point,Point>(Point(3,9),Point(1,3),Point(2,10)));
    Trapeze trapeze(std::tuple<Point,Point,Point,Point>(Point(0,0),Point(0,10),Point(3,7),Point(3,2)));
    std::vector<Polygon*> polymorhTest1 = std::vector<Polygon*>();
    polymorhTest1.push_back(&triangle);
    polymorhTest1.push_back(&trapeze);
    polymorhTest1.push_back(&rp);
    for(int i = 0;i<polymorhTest1.size();i++){
        std::cout<<polymorhTest1[i]->area()<<std::endl;
    }
    std::vector<Chain*> polymorphTest2 = std::vector<Chain*>();
    polymorphTest2.push_back(&chain);
    polymorphTest2.push_back(&closedChain);
    polymorphTest2.push_back(&triangle);
    polymorphTest2.push_back(&trapeze);
    polymorphTest2.push_back(&rp);
    for(int i=0;i<polymorphTest2.size();i++){
        polymorphTest2[i]->print();
    }
}
