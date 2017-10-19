﻿#include "provided.h"
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
		//for (int u = 0; u < s.getSegments(q->coords).size(); u++)
		//{
			//vg.push_back(s.getSegments(q->coords)[u]);
		//}
		for (int i = 0; i < vg.size(); i++)
		{
			Node* news = new Node;
			//news->parent = q;
			bool b = true;

			if (q->coords.latitudeText == vg[i].segment.start.latitudeText && q->coords.longitudeText == vg[i].segment.start.longitudeText)
				news->coords = vg[i].segment.end;
			else /**if (q->coords.latitudeText == s.getSegments(q->coords)[i].segment.end.latitudeText && q->coords.longitudeText == s.getSegments(q->coords)[i].segment.end.longitudeText)**/
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
			

			//if (vg[i].latitudeText == q->coords.latitudeText && vg[i].longitudeText == q->coords.longitudeText)
				//string sklk = "";
			//else
				//news->coords = vg[i];
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
					//cout << partone.size() << endl;
					vector <GeoCoord> parttwo;
					for (int j = partone.size() - 1; j >= 0; j--)
					{
						parttwo.push_back(partone[j]);
						//cout << partone[j].latitudeText + " " + partone[j].longitudeText << endl;
					}
					//cout << parttwo[0].latitudeText << endl;
					//cout << distanceEarthMiles(parttwo[0], begin->coords) << endl;
					//cout << parttwo.size() << endl;
					//for (int i = 0; i < parttwo.size(); i++)
					//{
						//cout << parttwo[i].latitudeText + " " + parttwo[i].longitudeText << endl;
					//}
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
						//cout << thanamez + " " + thastartz.latitudeText + " " + thastartz.longitudeText << endl;
						GeoSegment thasegz;
						thasegz.start = thastartz;
						thasegz.end = thaendz;
						double thadistz = distanceEarthMiles(thastartz, thaendz);
						double thaangz = angleOfLine(thasegz);
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
						NavSegment navs(thadirecz, thanamez, thadistz, thasegz);
						if (directions.size() > 0)
						{
							int whereat = directions.size() - 1;
							NavSegment thanavs = directions[directions.size() - 1];
							while (thanavs.m_command == static_cast<NavSegment::NavCommand>(1))
							{
								if (whereat > 0)
								{
									thanavs = directions[whereat - 1];
								}
								whereat--;
							}
							double theangle = angleBetween2Lines(thanavs.m_geoSegment, thasegz);
							string whichdirec;
							string thenamezzz;
							if (theangle < 180)
							{
								if (thanavs.m_streetName != navs.m_streetName)
								{
									whichdirec = "left";
									thenamezzz = navs.m_streetName;
									NavSegment turning(whichdirec, thenamezzz);
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
									thenamezzz = navs.m_streetName;
									NavSegment turning(whichdirec, thenamezzz);
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
					/**while (directions[e].m_command != static_cast<NavSegment::NavCommand>(1))
					{
						cout << directions[e].m_distance << endl;
						e++;
					}
					cout << "hi" << endl;
					int w = directions.size() - 1;
					while (directions[w].m_command != static_cast<NavSegment::NavCommand>(1))
					{
						cout << directions[w].m_distance << endl;
						w--;
					}
					cout << "hi" << endl;
					for (int x = 0; x < directions.size(); x++)
					{
						cout << directions[x].m_geoSegment.start.latitudeText + " " +directions[x].m_geoSegment.start.longitudeText<< " ";
						cout << directions[x].m_geoSegment.end.latitudeText + " " + directions[x].m_geoSegment.end.longitudeText << endl;
					}**/

					/**for (int jjj = 0; jjj < directions.size(); jjj++)
					{
						NavSegment thanavss = directions[directions.size() - 1];
						while (thanavss.m_command == static_cast<NavSegment::NavCommand>(1))
						{

						}
					}**/
					//for (int uu = 0; uu < directions.size(); uu++)
					//{
						//cout << directions[uu].m_streetName + " " + directions[uu].m_geoSegment.start.latitudeText + " " + directions[uu].m_geoSegment.start.longitudeText + " " + directions[uu].m_geoSegment.end.latitudeText + " " + directions[uu].m_geoSegment.end.longitudeText << endl;
					//}
					return NAV_SUCCESS;
				}

			}
			/**bool b111 = true;
			bool b222 = true;
			for (int j = 0; j < opens.size(); j++)
			{
				if (news->coords.latitudeText == opens[j]->coords.latitudeText && news->coords.longitudeText == opens[j]->coords.longitudeText && opens[j]->f < news->f)
					b111 = false;
			}

			for (int j = 0; j < closed.size(); j++)
			{
				if (news->coords.latitudeText == closed[j]->coords.latitudeText && news->coords.longitudeText == closed[j]->coords.longitudeText && closed[j]->f < news->f)
					b222 = false;
			}
			if (b111 == true && b222 == true)
				opens.push_back(news);**/
			bool bggg = true;
			bool b4b = true;
			for (int jy = 0; jy < closed.size(); jy++)
			{
				if (news->coords.latitudeText == closed[jy]->coords.latitudeText && news->coords.longitudeText == closed[jy]->coords.longitudeText)
					b4b = false;
			}
			if (b4b)
			{
				bool kjl = true;
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
						kjl = false;
					}
				}
				if (kjl)
					opens.push_back(news);
		}
		}
		//closed.push_back(q);
	}
	return NAV_NO_ROUTE;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

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
