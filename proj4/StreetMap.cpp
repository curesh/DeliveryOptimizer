#include "provided.h"
#include <string>
#include <vector>
#include <functional>

#include <iostream>
#include <fstream>
#include "ExpandableHashMap.h"
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
//    std::cerr<<"hfld"<<std::endl;
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment> > m_map;
    //ExpandableHashMap<StreetSegment*, int> m_streetSegments;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if(!infile){
        cerr << "Error mapFile could not be loaded for some weird reason" << endl;
        return false;
    }
    string street;
    while(getline(infile,street)){
        int num;
        infile >> num;
        //gets rid of the newline after num
        infile.ignore();
        
        string geoCord;
        for(int i = 0; i < num; i++){
            getline(infile, geoCord);
            string geoCord2 = geoCord.substr(geoCord.size()/2 + 1);
            geoCord = geoCord.substr(0,geoCord.size()/2);
            GeoCoord g1( geoCord.substr(0,geoCord.size()/2-1),geoCord.substr(geoCord.size()/2));
            GeoCoord g2( geoCord2.substr(0,geoCord2.size()/2-1),geoCord2.substr(geoCord2.size()/2));
            if(m_map.find(g1) == nullptr){
                vector<StreetSegment> vecseg;
                m_map.associate(g1, vecseg);
            }
            m_map.find(g1)->push_back(StreetSegment(g1, g2, street));
            if(m_map.find(g2) == nullptr){
                vector<StreetSegment> vecseg;
                m_map.associate(g2, vecseg);
            }
            m_map.find(g2)->push_back(StreetSegment(g2, g1, street));
        }
    }
    return true;  // Delete this line and implement this function correctly
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    if(m_map.find(gc) == nullptr)
        return false;
    segs = *m_map.find(gc);
    return true;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
