#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <cmath>

const int UNCLASSIFIED= -1;
const int CORE_POINT= 1;
const int BORDER_POINT= 2;
const int NOISE= -2;
const int SUCCESS= 0;
const int FAILURE= -3;

using namespace std;

struct Point
{
    double x, y, z;  // X, Y, Z position
    int clusterID;  // clustered ID

    Point( double xx, double yy, double zz, int id ) : x(xx), y(yy), z(zz), clusterID(id) { }

};

class DBSCAN {
public:    
    DBSCAN(unsigned int minPts, float eps, vector<Point> points){
        m_minPoints = minPts;
        m_epsilon = eps;
        m_points = points;
        m_pointSize = points.size();
	m_numclusters =  0;
    }
    ~DBSCAN(){}

    int run();

    int getTotalPointSize() {return m_pointSize;}
    int getMinimumClusterSize() {return m_minPoints;}
    int getEpsilonSize() {return m_epsilon;}

    int get_num_clusters() const { return m_numclusters; }
    vector<Point> get_cluster( int icluster ) const;


private:

    vector<int> calculateCluster(Point point);
    int expandCluster(Point point, int clusterID);
    inline double calculateDistance(Point pointCore, Point pointTarget);

    int m_numclusters;
    
    vector<Point> m_points;
    unsigned int m_pointSize;
    unsigned int m_minPoints;
    double m_epsilon;


};

#endif // DBSCAN_H
