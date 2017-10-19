#include "provided.h"
#include "MyMap.h"
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap <GeoCoord, vector<StreetSegment>> mm;
};

bool operator<(const GeoCoord& a, const GeoCoord& b)
{
	if (a.latitudeText < b.latitudeText)
		return true;
	else if (a.latitudeText == b.latitudeText)
	{
		if (a.longitudeText < b.longitudeText)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool operator==(const GeoCoord& a, const GeoCoord& b)
{
	return (a.latitudeText == b.latitudeText && a.longitudeText == b.longitudeText);
}

bool operator>(const GeoCoord& a, const GeoCoord& b)
{
	if (a.latitudeText > b.latitudeText)
		return true;
	else if (a.latitudeText == b.latitudeText)
	{
		if (a.longitudeText > b.longitudeText)
			return true;
		else
			return false;
	}
	else
		return false;
}

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment s;
		ml.getSegment(i, s);
		if (mm.find(s.segment.start) == nullptr)
		{
			vector <StreetSegment> v;
			v.push_back(s);
			mm.associate(s.segment.start, v);
		}
		else
		{
			vector <StreetSegment> v;
			v = *mm.find(s.segment.start);
			v.push_back(s);
			mm.associate(s.segment.start, v);
		}

		if (mm.find(s.segment.end) == nullptr)
		{
			vector <StreetSegment> v;
			v.push_back(s);
			mm.associate(s.segment.end, v);
		}
		else
		{
			vector <StreetSegment> v;
			v = *mm.find(s.segment.end);
			v.push_back(s);
			mm.associate(s.segment.end, v);
		}

		for (int j = 0; j < s.attractions.size(); j++)
		{
			GeoCoord g;
			Attraction a;
			a = s.attractions[j];
			g = a.geocoordinates;
			if (mm.find(g) == nullptr)
			{
				vector <StreetSegment> va;
				va.push_back(s);
				mm.associate(g, va);
			}
			else
			{
				vector <StreetSegment> va;
				va = *mm.find(g);
				va.push_back(s);
				mm.associate(g, va);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector <StreetSegment> v;
	if (mm.find(gc) == nullptr)
		return v;
	else
	{
		for (int k = 0; k < (*(mm.find(gc))).size(); k++)
		{
			v.push_back((*(mm.find(gc)))[k]);
		}
		return v;
	}
}


SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
