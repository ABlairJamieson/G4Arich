#include "dbscan.h"
#include <iostream>
using std::cout;
using std::endl;

int DBSCAN::run()
{
    int clusterID = 0;
    vector<Point>::iterator iter;
    for(iter = m_points.begin(); iter != m_points.end(); ++iter)
    {
        if ( iter->clusterID == UNCLASSIFIED )
        {
            if ( expandCluster(*iter, clusterID) != FAILURE )
            {
//cout<<"expandCluster FAILURE"<<endl;
                clusterID += 1;
            } else {
//              cout<<"expandCluster SUCCESS"<<endl;
            }
        }
    }

    m_numclusters = clusterID;
    return 0;
}

vector<Point> DBSCAN::get_cluster( int cluster ) const {
    vector<Point> vret;
    if ( cluster > m_numclusters ){
        cout<<"DBSCAN::get_cluster( "<<cluster
            <<") beyond num_clusters = "<<m_numclusters<<endl;
        return vret;
     }
    for( vector<Point>::const_iterator iter = m_points.begin(); iter != m_points.end(); ++iter)
    {
        if ( iter->clusterID == cluster ) {
             vret.push_back( *iter );
        }
    }
    return vret;
}

int DBSCAN::expandCluster(Point point, int clusterID)
{    
    vector<int> clusterSeeds = calculateCluster(point);

//   cout<<"DBSCAN::expandCluster clusterSeeds.size()="<<clusterSeeds.size()
//        <<" for point "<<point.x<<","<<point.y<<","<<point.z<<" id="<<point.clusterID
//        <<endl;

    if ( clusterSeeds.size() < m_minPoints )
    {
        point.clusterID = NOISE;
        return FAILURE;
    }
    else
    {
        int index = 0, indexCorePoint = 0;
        vector<int>::iterator iterSeeds;
        for( iterSeeds = clusterSeeds.begin(); iterSeeds != clusterSeeds.end(); ++iterSeeds)
        {
//            cout<<" iterSeeds = "<< *iterSeeds <<" setting clusterId to "<<clusterID<<endl;
            m_points.at(*iterSeeds).clusterID = clusterID;
            if (m_points.at(*iterSeeds).x == point.x && m_points.at(*iterSeeds).y == point.y && m_points.at(*iterSeeds).z == point.z )
            {
                indexCorePoint = index;
//                cout<<"indexCorePoint = "<<index<<endl;
            }
            ++index;
        }
        clusterSeeds.erase(clusterSeeds.begin()+indexCorePoint);

        for( vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n; ++i )
        {
           vector<int> clusterNeighors = calculateCluster(m_points.at(clusterSeeds[i]));
//           cout<<"DBSCAN::expandCluster clusterNeighors.size()="<<clusterNeighors.size()
//	   <<" for point "<<m_points.at(clusterSeeds[i]).x<<","<<m_points.at(clusterSeeds[i]).y<<","<<m_points.at(clusterSeeds[i]).z

//	   <<" clusterid="<<m_points.at(clusterSeeds[i]).clusterID<<endl;



            if ( clusterNeighors.size() >= m_minPoints )
            {
                vector<int>::iterator iterNeighors;
                for ( iterNeighors = clusterNeighors.begin(); iterNeighors != clusterNeighors.end(); ++iterNeighors )
                {
                    if ( m_points.at(*iterNeighors).clusterID == UNCLASSIFIED || m_points.at(*iterNeighors).clusterID == NOISE )
                    {
                        if ( m_points.at(*iterNeighors).clusterID == UNCLASSIFIED )
                        {
                            clusterSeeds.push_back(*iterNeighors);
                            n = clusterSeeds.size();
                        }
                        m_points.at(*iterNeighors).clusterID = clusterID;
                    }
                }
            }
        }

        return SUCCESS;
    }
}

vector<int> DBSCAN::calculateCluster(Point point)
{
    int index = 0;
    vector<Point>::iterator iter;
    vector<int> clusterIndex;
    for( iter = m_points.begin(); iter != m_points.end(); ++iter)
    {

        if ( calculateDistance(point, *iter) <= m_epsilon )
        {
            clusterIndex.push_back(index);
        } 
        index++;
    }
// cout<<"DBSCAN::clusterIndex.size() = "<<clusterIndex.size()
//        <<" for (x,y,z)="<<point.x<<","<<point.y<<","<<point.z<<endl;
    return clusterIndex;
}

inline double DBSCAN::calculateDistance( Point pointCore, Point pointTarget )
{
//static int count=0;

    //       cout<<"core (x,y,z)=("<<pointCore.x<<", "<<pointCore.y<<", "<<pointCore.z    
    //<<") target= (x,y,z)"<<pointTarget.x<<", "<<pointTarget.y<<", "<<pointTarget.z<<")" 
    //<<endl;
    double dist = pow(pointCore.x - pointTarget.x,2)+pow(pointCore.y - pointTarget.y,2)+pow(pointCore.z - pointTarget.z,2);
//    if (count%20==0) cout<<"DBSCAN::calculateDistance = "<<dist<<" eps="<<m_epsilon<<endl;
    return dist;
}


