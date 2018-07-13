#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap <string, GeoCoord> mm;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment s;
		GeoCoord g;
		string thename;
		ml.getSegment(i, s);
		for (int j = 0; j < s.attractions.size(); j++)
		{
			Attraction a;
			a = s.attractions[j];
			thename = a.name;
			std::transform(thename.begin(), thename.end(), thename.begin(), ::tolower);
			g = a.geocoordinates;
			mm.associate(thename, g);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string att = attraction;
	std::transform(att.begin(), att.end(), att.begin(), ::tolower);
	if (mm.find(att) != nullptr)
	{
		gc = *(mm.find(att));
		return true;
	}
	else
		return false;
}

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
