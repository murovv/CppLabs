#include<pugixml.hpp>
#include<iostream>
#include<vector>
#include<typeinfo>
#include<map>
#include <algorithm>
#include<math.h>
using std::wstring;
using std::vector;
using std::pair;
using std::cout;
const char* XML_FILE_PATH = "/home/paperblade/study/prog/data.xml";
const long long EARTH_RAD =  6372795;
const double PI= 3.14159265;
const double INF = 1e15;
int prim = 0;
pugi::xml_document doc;
pugi::xml_node root;
std::map<wstring,vector<wstring>> toponim = {{L"проспект",{L"проспект",L"пр.",L"пр"}},{L"улица",{L"улица",L"ул."}},{L"шоссе",{L"шоссе",L"ш."}},{L"аллея",{L"аллея",L"а."}},{L"переулок",{L"переулок",L"пер."}},{L"проезд",{L"проезд"}},{L"набережная",{L"набережная",L"наб."}},{L"площадь",{L"площадь",L"пл."}},{L"дорога",{L"дорога",L"дор."}},{L"бульвар",{L"бульвар",L"бул."}},{L"мост",{L"мост"}},{L"линия",{L"линия"}}};
vector<std::map<wstring,vector<std::pair<double,double>>>> routes = vector<std::map<std::wstring,vector<std::pair<double,double>>>>(3);//0 - трамвай,1 - троллейбус,2 - автобус
vector<wstring> locations = vector<wstring>();

std::pair<double,double> split_coordinates(wstring coordinates){
    int pos = coordinates.find(',');
    if(pos == wstring::npos){
        std::cout<<"splitting error!!";
        exit(1);
    }else{
        double lat = stod(coordinates.substr(0, pos).replace(2,1,L","));
        coordinates.erase(0,pos+1);
        double lon = stod(coordinates.replace(2,1,L","));
        return std::make_pair(lat,lon);
    }
}
void readFile(){
    if(!doc.load_file(XML_FILE_PATH)){
        std::cout<<"Read error";
        exit(1);
    }
    root = doc.document_element();
}
void parse(){
    for(pugi::xml_node transport_station = root.first_child();transport_station;transport_station = transport_station.next_sibling()){
        std::wstring routes_names = transport_station.child(L"routes").text().get();
        std::wstring location_names = transport_station.child(L"location").text().get();
        std::pair<double,double> station_coords = split_coordinates(transport_station.child(L"coordinates").text().get());
        std::wstring vehicle_type_str = transport_station.child(L"type_of_vehicle").text().get();
        int vehicle_type = 0;
        if(vehicle_type_str == L"Троллейбус"){
            vehicle_type = 1;
        }
        else if(vehicle_type_str == L"Автобус"){
            vehicle_type = 2;
        }
        int pos = 0;
        std::wstring token;
        do {
            pos = routes_names.find(',');
            if(pos!=std::wstring::npos){
                token = routes_names.substr(0, pos);
                routes_names.erase(0, pos + 1);
            }else{
                token = routes_names;
            }
            
            bool isValid = true;
            for(int i = 0;i<token.size();i++){
                if(token.substr(i,1)==L"."){
                    token = token.substr(0,i);
                    break;
                }
            }
            if(isValid){
                if(routes[vehicle_type].find(token)==routes[vehicle_type].end()){
                    routes[vehicle_type][token] = vector<pair<double,double>>();
                }
                routes[vehicle_type][token].push_back(station_coords);
            }  
        }while (pos != std::wstring::npos);
        do{
            pos = location_names.find(',');
            if(pos!=std::wstring::npos){
                token = location_names.substr(0, pos);
                location_names.erase(0, pos + 1);
            }else{
                token = location_names;
            }
            int count = 0;
            while(token.substr(count,count+1)==L" "){
                count++;
            }
            if(count>0){
                token.erase(0,count);
            }
            std::transform(token.begin(), token.end(),token.begin(),towlower);
            locations.push_back(token);
        }while ((pos ) != std::wstring::npos);
    }
    
}
double distance(pair<double,double> point1,pair<double,double> point2){
    double phi1 = (point1.first*PI)/180;
    double phi2 = (point2.first*PI)/180;
    double lambda1 = (point1.second*PI)/180;
    double lambda2 = (point2.second*PI)/180;
    double dist = 2*EARTH_RAD*std::asin(sqrtl( powl(sin((phi1-phi2)/2),2)+cos(phi1)*cos(phi2)*powl(sin((lambda1-lambda2)/2),2) ));
    return dist;
}

void task1(){
    std::map<std::wstring,vector<pair<double,double>>>::iterator it;
    std::wcout<<L"task1\n";
    for(int vehicle_type = 0;vehicle_type<3;vehicle_type++){
        std::wstring longestRoute = L"";
        int longestRouteLen = 0;
        for(it = routes[vehicle_type].begin();it!=routes[vehicle_type].end();it++){
            if(longestRouteLen<it->second.size()){
                longestRouteLen = it->second.size();
                longestRoute = it->first;
            }
        }
        switch (vehicle_type)
        {
        case 0:
            std::wcout<<L"Трамвай "<<longestRoute<<L" "<<std::to_wstring(longestRouteLen)<<L" остановок\n";
            break;
        case 1:
            std::wcout<<L"Троллейбус "<<longestRoute<<L" "<<std::to_wstring(longestRouteLen)<<L" остановок\n";
            break;
        default:
            std::wcout<<L"Автобус "<<longestRoute<<L" "<<std::to_wstring(longestRouteLen)<<L" остановок\n";
            break;
        }
    } 
}
void task2(){
    std::map<std::wstring,vector<pair<double,double>>>::iterator it;
    std::wcout<<L"task2\n";
    for(int vehicle_type = 0;vehicle_type<3;vehicle_type++){
        std::wstring longestRoute = L"";
        double longestRouteLen = 0;
        for(it = routes[vehicle_type].begin();it!=routes[vehicle_type].end();it++){
            //построение mst
            int n = it->second.size();
            vector<double> dist = vector<double>(n,INF);
            vector<bool> used = vector<bool>(n,false);
            dist[0] = 0;
            double max = 0;
            for(int i = 0;i<n;i++){
                int next = -1;
                for(int j = 0;j<n;j++){
                    if(!used[j]&&(next==-1||dist[j]<dist[next])){
                        next = j;
                    }
                }
                if(dist[next]==INF){
                    std::wcout<<L"mst building error";
                    exit(1);
                }
                max+=dist[next];
                used[next] = true;
                for(int j = 0;j<n;j++){
                    double edge = distance(it->second[next],it->second[j]);
                    if(dist[j]>edge){
                        dist[j] = edge;
                    } 
                }
            }
            
            if(max>longestRouteLen){
                longestRouteLen = max;
                longestRoute = it->first;
            }

        }
        switch (vehicle_type)
        {
        case 0:
            std::wcout<<L"Трамвай "<<longestRoute<<L" "<<longestRouteLen<<L"m\n";
            break;
        case 1:
            std::wcout<<L"Троллейбус "<<longestRoute<<L" "<<longestRouteLen<<L"m\n";
            break;
        default:
            std::wcout<<L"Автобус "<<longestRoute<<L" "<<longestRouteLen<<L"m\n";
            break;
        }
    }
}
void task3(){
    std::map<wstring,int> count;
    std::wstring ans = L"";
    int len = 0;
    for(int k = 0;k<locations.size();k++){
        std::wstring temp = locations[k];
        int pos = locations[k].find(' ');
        wstring token;
        bool defined = false;
        std::wstring name = L"";
        std::wstring type = L"";
        do {
            pos = locations[k].find(' ');
            if(pos!=std::wstring::npos){
                token = locations[k].substr(0, pos);
                locations[k].erase(0, pos + 1);
            }else{
                token = locations[k];
            }
            
            bool add = true;
            std::map<std::wstring,vector<std::wstring>>::iterator it;
            for(it = toponim.begin();it!=toponim.end();it++){
                for(int i = 0;i<it->second.size();i++){
                    if(it->second[i]==token){
                        defined = true;
                        type = it->first;
                        add = false;
                    }
                }
            }if(add&&token!=L" "&&token.size()!=0){
                if(name==L""){
                    name = token;
                }else{
                    name = name+L" "+token;
                }
            }
        }while (pos != std::wstring::npos);
        if(name.size()>1){
            if(defined){
                name = type +L" "+name;
                
            }
            if(count.find(name)!=count.end()){
                    count[name]++;
            }else{
                count[name] = 1;
            }
        }
        
    }
    std::map<std::wstring,int>::iterator it;
    for(it = count.begin();it!=count.end();it++){
        if(it->second>len){
            len = it->second;
            ans = it->first;
        }
    }
    std::wcout<<L"task3\n"<<ans<<L" "<<len;
    
}
int main(){
    setlocale (LC_ALL, "");
    readFile();
    parse();
    task1();
    task2();
    task3();
    
}