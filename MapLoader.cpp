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
				//cout << inputs;
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
				//cout << inputs;
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
				//cout << gl1;
				string gl2 = "";
				for (int l = j; l < inputs.size(); l++)
				{
					if (isdigit(inputs[l]) || inputs[l] == '-' || inputs[l] == '.')
						gl2 += inputs[l];
				}
				//cout << gl2;
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
		/**for (int i = 0; i < v.size(); i++)
		{
				cout << v[i].streetName << endl;
				cout << v[i].segment.start.latitudeText << "," << v[i].segment.start.longitudeText << " " << v[i].segment.end.latitudeText << "," << v[i].segment.end.longitudeText << endl;
				if (v[i].attractions.size() > 0)
				{
					cout << v[i].attractions.size() << endl;
					for (int c = 0; c < v[i].attractions.size(); c++)
					{
						cout << v[i].attractions[c].name << "|" << v[i].attractions[c].geocoordinates.latitudeText << ","
							<< v[i].attractions[c].geocoordinates.longitudeText << endl;
					}
				}
				else
					cout << "0" << endl;
		}**/
		/**for (int i = 0; i < getNumSegments(); i++)
		{
			for (int c = 0; c < v[i].attractions.size(); c++)
			{
				if (v[i].attractions[c].name == "The Playboy Mansion")
					cout << "true " << v[i].attractions[c].geocoordinates.latitudeText << v[i].attractions[c].geocoordinates.longitudeText;
			}
		}**/
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

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

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
