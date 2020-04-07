
#include "provided.h"
#include "/Users/bigboi01/Documents/CS32/proj4/proj4/support.h"
using namespace std;
typedef pair<double, GeoCoord> pi;


class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
        

private:
    const StreetMap* m_sm;
    double heuristic(const StreetSegment seg, const GeoCoord directCourse)  const;
    list<StreetSegment> reconstructRoute(ExpandableHashMap<GeoCoord, StreetSegment>* cf, GeoCoord cn, double& totalDist) const;
    struct customCompare{
        bool operator()(const pi& lhs, const pi& rhs){
            return lhs.first > rhs.first;
        }
    };
    priority_queue<pi, vector<pi>, customCompare> m_findPath;
};


PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_sm = sm;

}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

//compute the heuristic for a given segment
double PointToPointRouterImpl::heuristic(const StreetSegment seg, const GeoCoord directCourse) const{
    double x = distanceEarthKM(seg.end, directCourse);
    return x;
}

list<StreetSegment> PointToPointRouterImpl::reconstructRoute(ExpandableHashMap<GeoCoord, StreetSegment>* cf, GeoCoord cn, double& totalDist) const{
    list<StreetSegment> ans;
    int i = 0;
    totalDist = 0;
    while((cf->find(cn)) != nullptr){
        i++;
        totalDist += distanceEarthMiles((*((*cf).find(cn))).end, (*((*cf).find(cn))).start);
        ans.push_front(*((*cf).find(cn)));
        cn = ((cf->find(cn)))->start;
        if(i > 20000){
            cerr <<"while loop in reconstruct route is taking to long, error" << endl;
            return ans;
        }
    }
    return ans;
}
DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    //A* algorithm
    std::vector<StreetSegment> segs;
    if(!m_sm->getSegmentsThatStartWith(start, segs)|| !m_sm->getSegmentsThatStartWith(start, segs))
        return BAD_COORD;
    segs.clear();
    if(start == end){
        route.clear();
        return DELIVERY_SUCCESS;
    }
    priority_queue<pi, vector<pi>, customCompare> findPath;
    GeoCoord startPtr(start);
    GeoCoord endPtr(end);
    ExpandableHashMap<GeoCoord, double> gScore;
    //Association is from neighbor to current
    ExpandableHashMap<GeoCoord, StreetSegment> cameFrom;
    StreetSegment* globalDirectSeg = new StreetSegment(start,end,"glovbal direct segment");
    double heur = heuristic(*globalDirectSeg, endPtr);
    delete globalDirectSeg;
    findPath.push(make_pair(heur,startPtr));
    gScore.associate(start, 0);
    //int count = 0;
    while (!findPath.empty()){
        GeoCoord currNode = findPath.top().second;
        findPath.pop();
        if (currNode == end) {
            //cerr <<"O(N): " << count << endl;
            route = reconstructRoute(&cameFrom,currNode, totalDistanceTravelled);
            return DELIVERY_SUCCESS;
        }
        segs.clear();
        if(m_sm->getSegmentsThatStartWith(currNode, segs)){
            for(int i = 0; i < segs.size(); i++){
                //count++;
                GeoCoord neighborNode = segs[i].end;
                double tentativegScore = *(gScore.find(currNode));
                tentativegScore += abs(distanceEarthKM(currNode, neighborNode));
                if(gScore.find(neighborNode) == nullptr || (tentativegScore < *gScore.find(neighborNode))){
                    cameFrom.associate(neighborNode,segs[i]);
                    gScore.associate(neighborNode, tentativegScore);
                    heur = heuristic(segs[i], endPtr);
                    findPath.push(make_pair(heur + *gScore.find(neighborNode),neighborNode));
                }
            }
        }
    }
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
