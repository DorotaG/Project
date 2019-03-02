#include "lod.h"

std::vector<surfel> tracked;

void DFS(octree* o);
void computeColor();
surfel::normal* computeNormals();
bool fitsCone(surfel::normal n, std::vector<surfel::normal> c);

lod::lod(int level, octree root)
{
	//find centers at current level (or higher levels if leaf found) BFS

	std::queue<octree*> frontend;

	//initialize
	frontend.push(&root);
	octree *temp;
	int depth = 0;
	int depthCount = 0;
	while (!frontend.empty())
	{
		temp = frontend.front();
		frontend.pop();
		depthCount++;
		//when choosen level and not leaf: put in centers
		if (depth == level && temp->getNp() == 0)
		{
			centers.push_back(*temp);
		}
		//when leaf: put points in surfels
		if (depth < level && temp->getNp() != 0)
		{
			surfels.push_back(*temp->getSurfels());
			continue;
		}
		//else: find children of temp and pit them in frontend
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (temp->getChild(i) != nullptr)
				{
					frontend.push(temp->getChild(i));
				}
			}
		}
		if (depthCount == pow(8,depth))
		{
			depthCount = 0;
			depth++;
		}
	}
	//compute colors and normals if needed:
	//track leaves
	//track leaves, average color
	//track leaves, normal magic
	for (octree c : centers)
	{
		//depth search
		//track leaves with depth first search
		DFS(&c);

		//compute color
		float r = 0;
		float g = 0;
		float b = 0;
		//average, easy peasy
		for (surfel s : tracked)
		{
			//compute average
			r += s.getR();
			g += s.getG();
			b += s.getB();
		}
		r = r / (tracked.size());
		g = g / (tracked.size());
		b = b / (tracked.size());

		//compute normals
		surfel::normal* cn = computeNormals();
		//create new surfel and add to surfels
		surfel newSurf = surfel(c.getCenter().getX(), c.getCenter().getY(), c.getCenter().getZ(), r, g, b, cn);
		newSurf.setNC(true);
		surfels.push_back(newSurf);
		tracked.clear();
	}
}

lod::~lod()
{
}

void DFS(octree* o)
{
	//label o as discovered
	o->labelDiscovered(true);
	if (o->getNp() != 0)
	{
		for (int i = 0; i < o->getNp(); i++)
		{
			tracked.push_back(*o->getSurfels());
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (o->getChild(i) != nullptr)
		{
			//if not discovered
			if (o->getChild(i)->getDiscovered() != true)
			{
				DFS(o->getChild(i));
			}
		}
	}
}

surfel::normal* computeNormals()
{
	//get normals
	//compute cone
	//wenn winkel grosser als 90 stopni TEILE
	//compute average for every cone
	//save in surfel (normals in surfel: array6)
	std::vector<surfel::normal> cone[6];
	for (surfel s : tracked)
	{
		//first normal to cone1
		if (cone[0].empty())
		{
			cone[0].push_back(s.getN());
			continue;
		}
		//check if every normal in cone1 dot >=0. Y: add, N:the same with cone++ till cone6
		for (int i = 0; i < 5; i++)
		{
			bool test = fitsCone(s.getN(), cone[i]);
			if (test)
			{
				cone[i].push_back(s.getN());
				goto here;
			}
		}
		//the same with cones 2 till 6
		cone[5].push_back(s.getN());
	here:;
	}
	//compute average for every cone
	surfel::normal avs[6];
	for (int i = 0; i<5; i++)
	{
		if (!cone[i].empty())
		{
			surfel::normal average;
			average.u = 0;
			average.v = 0;
			average.w = 0;
			for (surfel::normal n : cone[i])
			{
				average.u += n.u;
				average.v += n.v;
				average.w += n.w;
			}
			average.u = average.u / cone[i].size();
			average.v = average.v / cone[i].size();
			average.w = average.w / cone[i].size();
			avs[i] = average;
		}
		else
		{
			surfel::normal nullyfied;
			nullyfied.u = 0;
			nullyfied.v = 0;
			nullyfied.w = 0;
			avs[i] = nullyfied;
		}
	}
	return &avs[0];
}

bool fitsCone(surfel::normal n, std::vector<surfel::normal> c)
{
	std::vector<float> x;
	std::vector<float> y;
	x.push_back(n.u);
	x.push_back(n.v);
	x.push_back(n.w);
	for (surfel::normal m : c)
	{
		y.push_back(m.u);
		y.push_back(m.u);
		y.push_back(m.u);
		if (std::inner_product(x.begin(), x.end(), y.begin(), 0.0) < 0)
		{
			return false;
		}
		y.clear();
	}
	return true;
}
