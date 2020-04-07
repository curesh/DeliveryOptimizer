#include "provided.h"
#include <vector>
#include "/Users/bigboi01/Documents/CS32/proj4/proj4/support.h"
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    double getCrowDist(const vector<DeliveryRequest> delivs, const GeoCoord depot) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

double DeliveryOptimizerImpl::getCrowDist(const vector<DeliveryRequest> delivs, const GeoCoord depot) const{
    double crowDistance = 0;
    if(delivs.size() == 0)
        return crowDistance;
    GeoCoord prev = depot;
    for(int i = 0; i < delivs.size(); i++){
        crowDistance += distanceEarthMiles(prev, delivs[i].location);
        prev = delivs[i].location;
    }
    crowDistance += distanceEarthMiles(prev, depot);
    return crowDistance;
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0;  // Delete these lines and implement this function correctly
    newCrowDistance = 0;
    //calculate oldCrowDistance
   //DELETE THIS
    oldCrowDistance = getCrowDist(deliveries, depot);
    DeliveryRequest depotDR("depot", depot);
    deliveries.push_back(depotDR);
    vector<DeliveryRequest> minPath = deliveries;
    //k moves around the reference node
    //i is the current reference node during an iteration of k
    //j is all the other nodes during an iteration of i
    int minReference = 0;
    for(int k = 0; k < deliveries.size(); k++){
        int reference = k;
        DeliveryRequest temp = deliveries[0];
        deliveries[0] = deliveries[reference];
        deliveries[reference] = temp;
        for(int i = 0; i < deliveries.size()-1; i++){
            //reference = i;
            pair<int, double> m(k, 100000.0);
            for(int j = i+1; j < deliveries.size()-1; j++){
                double distCurr = distanceEarthMiles(deliveries[i].location, deliveries[j].location);
                if(distCurr < m.second){
                    m.first = j;
                    m.second = distCurr;
                }
            }
            DeliveryRequest temp = deliveries[i+1];
            deliveries[i+1] = deliveries[m.first];
            deliveries[m.first] = temp;
        }
        if(newCrowDistance != 0)
            cerr << "K: " << k << ", " << getCrowDist(deliveries, deliveries[k].location) << endl;
        if(newCrowDistance == 0 || getCrowDist(deliveries, deliveries[k].location) < newCrowDistance){
            minReference = k;
            newCrowDistance = getCrowDist(deliveries, deliveries[k].location);
            minPath = deliveries;
        }
    }
    deliveries.clear();
    int findDepot;
    for(findDepot = 0; findDepot < minPath.size(); findDepot++){
        if(minPath[findDepot].location == depotDR.location){
            minPath.erase(minPath.begin()+findDepot);
            break;
        }
    }
    int i = findDepot;
    if(findDepot == 0){
        deliveries = minPath;
        return;
    }
    do{
        if(i == minPath.size())
            i = 0;
        deliveries.push_back(minPath[i]);
        i++;
    }while(i!=findDepot);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
