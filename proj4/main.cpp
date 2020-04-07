#include "ExpandableHashMap.h"


#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char *argv[])
{
//
//     //DELETE THIS STUFF, TESTING STUFF
////    ExpandableHashMap<GeoCoord, StreetSegment> test;
////    test.associate(GeoCoord("1","2"), StreetSegment(GeoCoord("20", "30"), GeoCoord("40", "50"), "FirstStreet"));
////    cerr << test.find(GeoCoord("1","2"))->end.latitudeText << endl;
////    cerr << (test.find(GeoCoord("2","2")) == nullptr) << endl;
////    test.associate(GeoCoord("4","5"), StreetSegment(GeoCoord("21", "31"), GeoCoord("41", "51"), "SecondStreet"));
////    cerr << test.find(GeoCoord("4","5"))->end.latitudeText << endl;
////    test.associate(GeoCoord("4","5"), StreetSegment(GeoCoord("22", "32"), GeoCoord("42", "52"), "ThirdStreet"));
////        cerr << test.find(GeoCoord("4","5"))->end.latitudeText << endl;
////    cerr << "Size: " << test.size() << endl;
////    cerr << "Second test" << endl;
////    test.reset();
////    string cordX = "";
////    string cordY = "";
////    string checkX;
////    string checkY;
////    for(int i = 0; i <10000; i++){
////        cordX = to_string(i);
////        cordY = to_string(i);
////        test.associate(GeoCoord(cordX, cordY), StreetSegment(GeoCoord(cordX, cordY), GeoCoord(cordX+cordX, cordY+cordY), cordY + cordX));
////        cerr << "Size :" << test.size() << endl;
////        if(i > 20 && (test.find(GeoCoord(checkX, checkY))) == nullptr){
////            cerr << "Eror" << endl;
////        }
////
////        if(i == 20){
////            checkX = cordX;
////            checkY = cordY;
////            cerr << 11 << "Name: " << (test.find(GeoCoord(checkX, checkY)))->name << endl;
////        }
//////        if(i == 14)
//////            cerr << 14 << "Name: " << (test.find(GeoCoord(checkX, checkY)))->name << endl;
////
////    }
////
////    cordX = "";
////    cordY = "";
////
////    for(int i = 0; i < 10000; i++){
////        cordX = to_string(i);
////        cordY = to_string(i);
////        StreetSegment* check65 =(test.find(GeoCoord(cordX, cordY)));
////        cerr << i << "Name: " << check65->name << endl;
////    }
////    cordX = "";
////    cordY = "";
////    for(int i = 0; i < 50; i++){
////        cordX = to_string(i);
////        cordY = to_string(i);
////        test.associate(GeoCoord(cordX, cordY), StreetSegment(GeoCoord(cordX, cordY), GeoCoord(cordX+cordX, cordY+cordY), cordY + cordX +"updated"));
////        cerr << "Size :" << test.size() << endl;
////    }
////cordX = "";
////cordY = "";
////
////for(int i = 0; i < 500; i++){
////    cordX = to_string(i);
////    cordY = to_string(i);
////    cerr << i << "Name: " << test.find(GeoCoord(cordX, cordY))->name << endl;
////}
////    return 1;
//
//    /*End of isolated testing code for ExpandableHashMap.h*/
//    //Begin testing of StreetMap.cpp
//    StreetMap sm2;
//    sm2.load("/Users/bigboi01/Documents/CS32/proj4/proj4/mapdata.txt");
//    GeoCoord g1("34.0691982" ,"-118.4576838");//34.0625329 -118.4470263
//    GeoCoord g2("34.0685657","-118.4489289");
//    cerr <<distanceEarthKM(g1, g2) <<endl;
////    vector<StreetSegment> seg;
////    cerr << sm2.getSegmentsThatStartWith(g1, seg) << endl;
////    cerr << "size: " << seg.size() << endl;
////    for(int i = 0; i < seg.size(); i++){
////        cerr << seg[i].name << ": " << seg[i].start.latitudeText <<", " << seg[i].end.latitudeText << endl;
////    }
////
////    return 1;
//    /*End of isolated testing code for Streetmap.cpp*/
//    //Begin testing of PointToPoitnRouter.cpp
//
////    list<StreetSegment> route;
////    double tot = 0;
////    PointToPointRouter ppr(&sm2);
////    GeoCoord start1;
////    GeoCoord end;
////    ppr.generatePointToPointRoute(g1, g2, route, tot);
////    cerr << "size" << route.size() << endl;
////    for(auto iter = route.begin(); iter != route.end(); iter++){
////        cerr << iter->name << ", " << iter->start.latitudeText << ", " << iter->start.longitudeText << ": : " << iter->end.latitudeText << ", " << iter->end.longitudeText << endl;
////    }
////    cerr << "TOtaldist: " << tot << endl;
////    //ppr.generatePointToPointRoute(g1, g2, route, tot);
////
////    return 1;
//    //End of isolated testing of p2prouter
//    //Begin of isolated testing of delivaryOptimizer.cpp
//    /*Broxton and Weyburn: 34.0625329 -118.4470263
//    Sproul Landing Intersection 34.0712323 -118.4505969
//    De Neve Suites Intersection 34.0718238 -118.4525699
//    Levering and Strathmore 34.0656797 -118.4505131
//    Kelton and Ophir 34.0683189 -118.4536522*/
//    GeoCoord test1("34.0625329", "-118.4470263");
//    GeoCoord test2("34.0712323", "-118.4505969");
//    GeoCoord test3("34.0718238", "-118.4525699");
//    GeoCoord test4("34.0656797", "-118.4505131");
//    GeoCoord test5("34.0683189", "-118.4536522");
//    vector<DeliveryRequest> testVect;
//
//    DeliveryRequest d1("Broxton and Weyburn", test1);
//    DeliveryRequest d2("Sproul Landing Intersection", test2);
//    DeliveryRequest d3("De Neve Suites Intersection", test3);
//    DeliveryRequest d4("Levering and Strathmore", test4);
//    DeliveryRequest d5("Kelton and Ophir", test5);
//
//    //testVect.push_back(d1);
//    testVect.push_back(d2);
//    testVect.push_back(d3);
//    //testVect.push_back(d4);
//    //testVect.push_back(d5);
//
//    double oldCD = 0;
//    double newCD = 0;
//    double totDistTraveled = 0;
//    vector<DeliveryCommand> commands;
//    DeliveryPlanner DP(&sm2);
//    DP.generateDeliveryPlan(test1,testVect, commands, totDistTraveled);
////    DeliveryOptimizer testDO(&sm2);
////    testDO.optimizeDeliveryOrder(g1, testVect, oldCD, newCD);
//    for(int i = 0; i < commands.size(); i++){
//        cerr << commands[i].description() << endl;
//    }
//    return 1;
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
        return 1;
    }

    StreetMap sm;
        
    if (!sm.load(argv[1]))
    {
        cout << "Unable to load map data file " << argv[1] << endl;
        return 1;
    }

    GeoCoord depot;
    vector<DeliveryRequest> deliveries;
    if (!loadDeliveryRequests(argv[2], depot, deliveries))
    {
        cout << "Unable to load delivery request file " << argv[2] << endl;
        return 1;
    }

    cout << "Generating route...\n\n";

    DeliveryPlanner dp(&sm);
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}
