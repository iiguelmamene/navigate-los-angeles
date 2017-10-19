#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector <StreetSegment> v;
	int thesize;
};

MapLoaderImpl::MapLoaderImpl()
{
	thesize = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	string inputs;
	ifstream texts(mapFile);
	if (texts.is_open())
	{
		int i = 0;
		int numofa = 0;
		string thename;
		GeoSegment geoseg;
		vector<Attraction> vv;
		string atname;
		GeoCoord atg;
		while (getline(texts, inputs))
		{
			int totals = 0;
			for (int iii = 0; i < inputs.size(); i++)
			{
				if (isalpha(inputs[i]))
					totals++;
			}
			if (inputs.find('|') == std::string::npos && inputs.find(',') == std::string::npos && totals > 0)
			{
				thename = inputs;
			}
			else if (inputs.find(',') != std::string::npos && inputs.find('|') == std::string::npos)
			{
				string gl1s = "";
				int j = 0;
				for (j; inputs[j] != ','; j++)
				{
					if (inputs[j] != ' ' && inputs[j] != '|' && inputs[j] != ',')
						gl1s += inputs[j];
				}
				while (inputs[j] == ' ' || inputs[j] == ',')
				{
					j++;
				}
				string gl1e = "";
				for (j; inputs[j] != ' '; j++)
				{
					if (inputs[j] != ' ' && inputs[j] != '|' && inputs[j] != ',')
						gl1e += inputs[j];
				}
				while (inputs[j] == ' ')
				{
					j++;
				}
				string gl2s = "";
				for (j; inputs[j] != ','; j++)
				{
					if (inputs[j] != ' ' && inputs[j] != '|' && inputs[j] != ',')
						gl2s += inputs[j];
				}
				while (inputs[j] == ' ' || inputs[j] == ',')
				{
					j++;
				}
				string gl2e = "";
				for (j; j < inputs.size(); j++)
				{
					if (inputs[j] != ' ' && inputs[j] != '|' && inputs[j] != ',')
						gl2e += inputs[j];
				}
				GeoCoord gs(gl1s, gl1e);
				GeoCoord ge(gl2s, gl2e);
				GeoSegment gss(gs, ge);
				geoseg = gss;
			}
			else if (isdigit(inputs[0]) && isdigit(inputs[inputs.size() - 1]) && inputs.find('|') == std::string::npos)
			{
				if (inputs == "0")
				{
					StreetSegment s;
					s.streetName = thename;
					s.segment = geoseg;
					numofa = 0;
					v.push_back(s);
					i = 0;
				}
				else
					numofa = stoi(inputs);
			}
			else if (numofa > 0 && inputs.find('|') != std::string::npos)
			{
				Attraction aaa;
				numofa--;
				string ss = "";
				int k = 0;
				for (k; inputs[k] != '|'; k++)
				{
					ss += inputs[k];
				}
				string gl1 = "";
				int j = k;
				for (j; inputs[j] != ','; j++)
				{
					if (inputs[j] != ' ' && inputs[j] != '|' && inputs[j] != ',')
						gl1 += inputs[j];
				}
				string gl2 = "";
				for (int l = j; l < inputs.size(); l++)
				{
					if (isdigit(inputs[l]) || inputs[l] == '-' || inputs[l] == '.')
						gl2 += inputs[l];
				}
				GeoCoord guuu(gl1, gl2);
				atname = ss;
				atg = guuu;
				aaa.name = atname;
				aaa.geocoordinates = atg;
				vv.push_back(aaa);
				if (numofa == 0)
				{
					StreetSegment s;
					s.streetName = thename;
					s.segment = geoseg;
					s.attractions = vv;
					v.push_back(s);
					vv.clear();
					i = 0;
				}
			}
		}
		texts.close();
}
	else
	{
		return false;
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return v.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= getNumSegments())
		return false;
	else
	{
		seg = v[segNum];
		return true;
	}
}


MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}
