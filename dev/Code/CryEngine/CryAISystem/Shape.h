/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

// Description : Polygon shapes used for different purposes in the AI system and various shape containers.
//
//               The bin sort point-in-polygon check is based on:
//               Graphics Gems IV: Point in Polygon Strategies by Eric Haines
//               http://tog.acm.org/GraphicsGems/gemsiv/ptpoly_haines/


#ifndef CRYINCLUDE_CRYAISYSTEM_SHAPE_H
#define CRYINCLUDE_CRYAISYSTEM_SHAPE_H
#pragma once


typedef std::vector<Vec3> ShapePointContainer;

namespace CryAISystem
{
    float DistancePointLinesegSq(const Vec3& p, const Vec3& start, const Vec3& end, float& t);
}

class CAIShape
{
public:
    CAIShape();
    ~CAIShape();

    CAIShape* DuplicateData();  // Duplicates data only, not acceleration structure.

    inline void SetName(const string& name) { m_name = name; }
    inline const string& GetName() const { return m_name; }

    void SetPoints(const std::vector<Vec3>& points);
    void BuildBins();
    void BuildAABB();

    bool IsPointInside(const Vec3& pt) const;
    bool IsPointOnEdge(const Vec3& pt, float tol, Vec3* outNormal = 0) const;
    bool IntersectLineSeg(const Vec3& start, const Vec3& end, float& tmin, Vec3* outClosestPoint, Vec3* outNormal = 0, bool bForceNormalOutwards = false) const;
    bool IntersectLineSeg(const Vec3& start, const Vec3& end, float radius) const;

    bool OverlapAABB(const AABB& aabb) const;

    inline ShapePointContainer& GetPoints() { return m_points; }
    inline const ShapePointContainer& GetPoints() const { return m_points; }
    inline const AABB& GetAABB() const { return m_aabb; }

    void DebugDraw();

    size_t MemStats() const;

private:

    struct Edge
    {
        inline Edge(unsigned short _id, float _minx, float _maxx, bool _fullCross)
            : id(_id)
            , fullCross(_fullCross)
            , minx(_minx)
            , maxx(_maxx) {}
        inline bool operator<(const Edge& rhs) const { return minx < rhs.minx; }
        float minx, maxx;
        unsigned short id;
        bool fullCross;
    };

    struct Bin
    {
        inline void AddEdge(float vx0, float vx1, unsigned short id, bool fullCross)
        {
            if (vx0 > vx1)
            {
                std::swap(vx0, vx1);
            }
            // Update bin range
            if (minx > vx0)
            {
                minx = vx0;
            }
            if (maxx < vx1)
            {
                maxx = vx1;
            }
            // Insert edge
            edges.push_back(Edge(id, vx0, vx1, fullCross));
        }

        float minx, maxx;
        std::vector<Edge> edges;
    };

    // Holds the bin sort acceleration structure for the polygon.
    struct BinSort
    {
        std::vector<Bin> bins;
        float ydelta, invYdelta;
    };

    bool IsPointInsideSlow(const Vec3& pt) const;

    bool IsPointOnEdgeSlow(const Vec3& pt, float tol, Vec3* outNormal = 0) const;

    bool IntersectLineSegSlow(const Vec3& start, const Vec3& end, float& tmin,
        Vec3* outClosestPoint = 0, Vec3* outNormal = 0, bool bForceNormalOutwards = false) const;

    bool IntersectLineSegBin(const Bin& bin, float xrmin, float xrmax,
        const Vec3& start, const Vec3& end,
        float& tmin, const Vec3*& isa, const Vec3*& isb) const;

    float GetDrawZ(float x, float y);

    BinSort* m_binSort;
    ShapePointContainer m_points;
    AABB m_aabb;
    string m_name;
};

/*
class CAISpecialArea : public CAIShape
{
public:

    enum EType
    {
        TYPE_WAYPOINT_HUMAN,
        TYPE_VOLUME,
        TYPE_FLIGHT,
        TYPE_WATER,
        TYPE_WAYPOINT_3DSURFACE,
        TYPE_FREE_2D,
        TYPE_TRIANGULATION
    };

    CAISpecialArea() :
        CAIShape(),
        m_buildingID(-1), m_type(TYPE_WAYPOINT_HUMAN),
        m_calculate3DNav(true), m_3DNavVolumeRadius(10.0f), m_waypointConnections(WPCON_DESIGNER_NONE),
        m_vehiclesInHumanNav(false), m_altered(false), m_lightLevel(AILL_NONE),
        m_minZ(FLT_MAX), m_maxZ(-FLT_MAX), m_height(0)
    {
    }

    ~CAISpecialArea() { CAIShape::~CAIShape(); }

    inline bool IsPointInside(const Vec3& pt, bool checkHeight) const
    {
        if (checkHeight && (pt.z < m_minZ || pt.z > m_maxZ)) return false;
        return CAIShape::IsPointInside(pt);
    }

    float m_minZ,m_maxZ;
    float m_height;
    float m_nodeAutoConnectDistance;
    int m_waypointType;
    int m_vehiclesInHumanNav;
    int m_buildingID;
    EType m_type;
    bool  m_calculate3DNav;
    float m_3DNavVolumeRadius;
    EAILightLevel m_lightLevel;
    EWaypointConnections    m_waypointConnections;
    bool    m_altered; // making links unpassible
};
*/

#endif // CRYINCLUDE_CRYAISYSTEM_SHAPE_H
