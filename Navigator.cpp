#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	AttractionMapper a;
	SegmentMapper s;
	MapLoader ml;

	struct Node
	{
		Node* parent;
		GeoCoord coords;
		double f, g, h;
	};
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	bool b = ml.load(mapFile);
	if (b)
	{
		a.init(ml);
		s.init(ml);
	}
	return b;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	directions.clear();
	vector <Node*> opens;
	vector <Node*> closed;
	Node* begin = new Node;
	begin->parent = nullptr;
	Node* ends = new Node;
	if (!a.getGeoCoord(start, begin->coords))
		return NAV_BAD_SOURCE;
	if (!a.getGeoCoord(end, ends->coords))
		return NAV_BAD_DESTINATION;
	if (start == end)
		return NAV_SUCCESS;
	begin->g = 0;
	begin->h = distanceEarthMiles(begin->coords, ends->coords);
	begin->f = begin->g + begin->h;
	opens.push_back(begin);
	while (!opens.empty())
	{
		int c = 0;
		Node* q = opens[0];
		for (int o = 0; o < opens.size(); o++)
		{
			if (opens[o]->f < q->f)
			{
				q = opens[o];
				c = o;
			}
		}
		opens.erase(opens.begin() + c);
		closed.push_back(q);
		vector <GeoCoord> v;
		vector <StreetSegment> vg = s.getSegments(q->coords);
		vector <StreetSegment> vgs = s.getSegments(begin->coords);
		
		for (int i = 0; i < vg.size(); i++)
		{
			Node* news = new Node;
			bool b = true;

			if (q->coords.latitudeText == vg[i].segment.start.latitudeText && q->coords.longitudeText == vg[i].segment.start.longitudeText)
				news->coords = vg[i].segment.end;
			else
				news->coords = vg[i].segment.start;
			for (int h = 0; h < vgs.size(); h++)
			{
				if ((news->coords.latitudeText == vgs[h].segment.start.latitudeText && news->coords.longitudeText == vgs[h].segment.start.longitudeText) || (news->coords.latitudeText == vgs[h].segment.end.latitudeText && news->coords.longitudeText == vgs[h].segment.end.longitudeText))
				{
					b = false;
					h = vgs.size();
				}
			}
			if (b == false)
				news->parent = begin;
			else
				news->parent = q;
			

			news->g = q->g + distanceEarthMiles(q->coords, news->coords);
			news->h = distanceEarthMiles(news->coords, ends->coords);
			news->f = news->g + news->h;
			for (int k = 0; k < vg[i].attractions.size(); k++)
			{
				if (vg[i].attractions[k].geocoordinates.latitudeText == ends->coords.latitudeText && vg[i].attractions[k].geocoordinates.longitudeText == ends->coords.longitudeText)
				{
					Node* news1 = new Node;
					vector <GeoCoord> partone;
					news1->coords = ends->coords;
					news1->parent = q;
					partone.push_back(news1->coords);
					Node* temp = news1->parent;
					while (temp != nullptr)
					{
						partone.push_back(temp->coords);
						temp = temp->parent;
					}
					vector <GeoCoord> parttwo;
					for (int j = partone.size() - 1; j >= 0; j--)
					{
						parttwo.push_back(partone[j]);
					}
					
					for (int k = 0; k < parttwo.size() - 1; k++)
					{
						StreetSegment sss;
						vector <StreetSegment> beginz;
						vector <StreetSegment> endz;
						beginz = s.getSegments(parttwo[k]);
						endz = s.getSegments(parttwo[k + 1]);
						bool lll = false;
						for (int h = 0; h < beginz.size(); h++)
						{
							for (int y = 0; y < endz.size(); y++)
							{
								if (beginz[h].streetName == endz[y].streetName && beginz[h].segment.start.latitudeText == endz[y].segment.start.latitudeText && beginz[h].segment.start.longitudeText == endz[y].segment.start.longitudeText && beginz[h].segment.end.latitudeText == endz[y].segment.end.latitudeText && beginz[h].segment.end.longitudeText == endz[y].segment.end.longitudeText)
								{
									sss = beginz[h];
									lll = true;
									y = endz.size();
								}
								if (lll == true)
									h = beginz.size();
							}

						}
						string thanamez = sss.streetName;
						GeoCoord thastartz = parttwo[k];
						GeoCoord thaendz = parttwo[k + 1];
						GeoSegment the_segment;
						the_segment.start = thastartz;
						the_segment.end = thaendz;
						double thadistz = distanceEarthMiles(thastartz, thaendz);
						double thaangz = angleOfLine(the_segment);
						string thadirecz;
						if (thaangz >= 0 && thaangz <= 22.5)
							thadirecz = "east";
						else if (thaangz > 22.5 && thaangz <= 67.5)
							thadirecz = "northeast";
						else if (thaangz > 67.5 && thaangz <= 112.5)
							thadirecz = "north";
						else if (thaangz > 112.5 && thaangz <= 157.5)
							thadirecz = "northwest";
						else if (thaangz > 157.5 && thaangz <= 202.5)
							thadirecz = "west";
						else if (thaangz > 202.5 && thaangz <= 247.5)
							thadirecz = "southwest";
						else if (thaangz > 247.5 && thaangz <= 292.5)
							thadirecz = "south";
						else if (thaangz > 292.5 && thaangz <= 337.5)
							thadirecz = "southeast";
						else if (thaangz > 337.5 && thaangz <= 360)
							thadirecz = "east";
						NavSegment navs(thadirecz, thanamez, thadistz, the_segment);
						if (directions.size() > 0)
						{
							int whereat = directions.size() - 1;
							NavSegment thanavs = directions[directions.size() - 1];
							while (thanavs.m_command == static_cast<NavSegment::NavCommand>(1))
							{
								if (whereat > 0)
									thanavs = directions[whereat - 1];
								whereat--;
							}
							double theangle = angleBetween2Lines(thanavs.m_geoSegment, the_segment);
							string whichdirec;
							string nameofstreet;
							if (theangle < 180)
							{
								if (thanavs.m_streetName != navs.m_streetName)
								{
									whichdirec = "left";
									nameofstreet = navs.m_streetName;
									NavSegment turning(whichdirec, nameofstreet);
									directions.push_back(turning);
									directions.push_back(navs);
								}
								else
									directions.push_back(navs);
							}
							else if (theangle >= 180)
							{
								if (thanavs.m_streetName != navs.m_streetName)
								{
									whichdirec = "right";
									nameofstreet = navs.m_streetName;
									NavSegment turning(whichdirec, nameofstreet);
									directions.push_back(turning);
									directions.push_back(navs);
								}
								else
									directions.push_back(navs);
							}
						}
						else
							directions.push_back(navs);
					}
					int e = 0;
					
					return NAV_SUCCESS;
				}

			}
			
			bool b4b = true;
			for (int jy = 0; jy < closed.size(); jy++)
			{
				if (news->coords.latitudeText == closed[jy]->coords.latitudeText && news->coords.longitudeText == closed[jy]->coords.longitudeText)
					b4b = false;
			}
			if (b4b)
			{
				bool check_bool = true;
				for (int jh = 0; jh < opens.size(); jh++)
				{
					if (news->coords.latitudeText == opens[jh]->coords.latitudeText && news->coords.longitudeText == opens[jh]->coords.longitudeText)
					{
						if (news->g < opens[jh]->g)
						{
							opens[jh]->parent = q;
							opens[jh]->g = news->g;
							opens[jh]->f = opens[jh]->g + opens[jh]->h;
						}
						check_bool = false;
					}
				}
				if (check_bool)
					opens.push_back(news);
		}
		}
	}
	return NAV_NO_ROUTE;
}

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}
