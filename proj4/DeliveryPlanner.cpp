#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_sm;
    string turnAngleIntoDirection(double angle) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m_sm = sm;
    
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    DeliveryOptimizer DO(nullptr);
    double oldCrowDistance;
    double newCrowDistance;
    vector<DeliveryRequest> usableDeliveries = deliveries;
    //Note that these old crow and new crow distances are for crows and not for goober eats robots so this isnt the total distance travelled.
    DO.optimizeDeliveryOrder(depot, usableDeliveries, oldCrowDistance, newCrowDistance);
    DeliveryRequest DRDepot("depot", depot);
    usableDeliveries.push_back(DRDepot);
    for(int i = 0; i < usableDeliveries.size(); i++){
        cerr << "New Order: "
         << usableDeliveries[i].item << endl;
    }
    PointToPointRouter P2PR(m_sm);
    GeoCoord prevGeo = depot;
    std::list<StreetSegment> route;
    double totCheck = 0;
    double totCurrCheck = 0;
    DeliveryCommand g;
    for(int i = 0; i < usableDeliveries.size(); i++){
        if(prevGeo == usableDeliveries[i].location){
            g.initAsDeliverCommand(usableDeliveries[i].item);
            commands.push_back(g);
            prevGeo = usableDeliveries[i].location;
            continue;
        }
        DeliveryResult generatePTPRouteReturnValue = P2PR.generatePointToPointRoute(prevGeo, usableDeliveries[i].location, route, totCurrCheck);
        if(generatePTPRouteReturnValue == BAD_COORD)
            return BAD_COORD;
        if(generatePTPRouteReturnValue == NO_ROUTE)
            return NO_ROUTE;
        totCheck += totCurrCheck;
        StreetSegment prevSeg = route.front();
        g.initAsProceedCommand(turnAngleIntoDirection(angleOfLine(prevSeg)), prevSeg.name, distanceEarthMiles(prevSeg.start, prevSeg.end));
        auto iter = route.begin();
        iter++;
        for(; iter != route.end(); iter++){
            double angbtwlines = angleBetween2Lines(prevSeg, *iter);
            if(iter->name == prevSeg.name){
                g.increaseDistance(distanceEarthMiles(iter->start, iter->end));
            }
            else if(angbtwlines < 1 || angbtwlines > 355){
                commands.push_back(g);
                g.initAsProceedCommand(turnAngleIntoDirection(angleOfLine(*iter)), iter->name, distanceEarthMiles(iter->start, iter->end));
                
            }
            else if((angbtwlines <180)){
                commands.push_back(g);
                g.initAsTurnCommand("left", iter->name);
                commands.push_back(g);
                g.initAsProceedCommand(turnAngleIntoDirection(angleOfLine(*iter)), iter->name, distanceEarthMiles(iter->start, iter->end));
            }
            else{
                commands.push_back(g);
                g.initAsTurnCommand("right", iter->name);
                commands.push_back(g);
                g.initAsProceedCommand(turnAngleIntoDirection(angleOfLine(*iter)), iter->name, distanceEarthMiles(iter->start, iter->end));
            }
            prevSeg = *iter;
        }
        
        commands.push_back(g);
        if(i == usableDeliveries.size()-1)
            break;
        g.initAsDeliverCommand(usableDeliveries[i].item);
        commands.push_back(g);
        prevGeo = usableDeliveries[i].location;
    }
    totalDistanceTravelled = totCheck;
    return DELIVERY_SUCCESS;
}
/*0 <= angle < 22.5: east
o 22.5 <= angle < 67.5: northeast
o 67.5 <= angle < 112.5: north
o 112.5 <= angle < 157.5: northwest
o 157.5 <= angle < 202.5: west
o 202.5 <= angle < 247.5: southwest
o 247.5 <= angle < 292.5: south
o 292.5 <= angle < 337.5: southeast*/
string DeliveryPlannerImpl::turnAngleIntoDirection(double angle) const{
    if(angle >=0 && angle < 22.5)
        return "east";
    if(angle >=22.5 && angle < 67.5)
        return "northeast";
    if(angle >=67.5 && angle < 112.5)
        return "north";
    if(angle >=112.5 && angle < 157.5)
        return "northwest";
    if(angle >= 157.5 && angle < 202.5)
        return "west";
    if(angle >=202.5 && angle < 247.5)
        return "southwest";
    if(angle >=247.5 && angle < 292.5)
        return "south";
    if(angle >=292.5 && angle < 337.5)
        return "southeast";
    return "east";
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
