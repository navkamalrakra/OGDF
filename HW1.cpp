/*
 * HW1.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: Navkamal Rakra
 *
 *      Issues : If number of inputs exceeds some certain number, there is heap corruption by Nodelabel() function
 *      and this issue has been already raised in OGDF : https://github.com/ogdf/ogdf/issues/19
 *
 *      The input file should be named "data.txt" and should be in same directory as the executable.
 */

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/graphalg/ShortestPathWithBFM.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace ogdf;

int main()
{
	Graph G;

	GraphAttributes GA(G, GraphAttributes::nodeGraphics |
			GraphAttributes::nodeLabel |
			GraphAttributes::nodeStyle |
			GraphAttributes::edgeArrow |
			GraphAttributes::edgeStyle |
			GraphAttributes::edgeLabel |
			GraphAttributes::edgeIntWeight |
			GraphAttributes::edgeGraphics );

	ogdf::NodeArray<int> array_for_nodes(G);
	node v, primary_inputs, primary_outputs; edge e;
	int x=1,y=1,level=1;
	string line, source_name, gate_name;
	ifstream myfile;
	myfile.open("data.txt");
	const char *p;
	node n,m;
	int flag = -1; // +ve for hit, -ve for miss
	int no_of_primary_gates=0; // For 2nd part of assignment
	char temp;

	if(myfile.is_open())
	{

		while(getline(myfile,line))
		{


			std::istringstream iss (line);

			while (iss >> std::skipws >> temp)
				//iss >> gate_name
			{
				// Pick the line char by char
				// First char is source

				source_name = temp;

				// Check if source node is already present

				forall_nodes(v,G)
				{
					flag = -1;
					p = GA.label(v).c_str();
					flag = strcmp(p,source_name.c_str());
					if(flag == 0)
					{
						n = v;
						break;
					}

				}
				// If source node is not found create it

				if(flag != 0)
				{
					n = G.newNode();
					GA.label(n) = source_name;
				}

				// Now do same for all nodes till end of line

				while (iss >> std::skipws >> gate_name)
				{
					//Check if GATE already exists

					forall_nodes(v,G)
									{
						flag = -1;
						p = GA.label(v).c_str();
						flag = strcmp(p,gate_name.c_str());
						if(flag == 0)
						{
							m = v;
							break;
						}

									}

					// Create node if it doesnt exist
					if(flag != 0)
					{
						m = G.newNode();
						GA.label(m) = gate_name;
						GA.y(m) = level*10;
						level = level + 10;
					}
					else if (flag == 0)
					{
						GA.y(m) = level*5;
						level = level + 10;
					}

					// Create all edges
					edge e = G.newEdge(n,m);

				} // while for "line" ends

			}

		}

	}
	else {
		cout << "Could not read data from file. Make sure data.txt is in same directory as executable";
	}

	// Color the nodes

	forall_nodes(v,G)
	{
		p = GA.label(v).c_str();
		GA.x(v)=10*(x+25);
		x = x + 10;
		GA.fillColor(v)= Color("#FFFF00");
	}


	// Part 2 of Assignment

	cout << "Set of Primary Inputs are:\t";
	forall_nodes(v,G)
	{
		if (v->indeg() == 0)
		{
			cout << GA.label(v) << "\t";
			no_of_primary_gates++;
		}

	}
	cout << "\n";

	cout << "Set of Primary Outputs are:\t";
	forall_nodes(v,G)
	{
		if (v->outdeg() == 0)
		{
			cout << GA.label(v) << "\t";
		}
	}
	cout << "\n";

	cout << "Set of Internal Gates are:\t";
	forall_nodes(v,G)
	{
		if (v->outdeg() != 0 && v->indeg() !=0)
		{
			cout << GA.label(v) << "\t";
		}
	}
	cout << "\n";

	cout << "The Gate count (Excluding Primary inputs) is :\t" << G.numberOfNodes()-no_of_primary_gates;

	//Part 3 of Assignment (Critical path)

	// Assign all edge cost = -1


	EdgeArray<int> edge_lengths(G);
	NodeArray<edge> pi(G);

	NodeArray<int> distances(G);

	forall_edges(e,G)
	{
		edge_lengths[e] = -1;
	}

	ShortestPathWithBFM sp;
	int least = 0, index = -1;
	string name;
	edge involved_edge;
	int critical = 0;
	string c_from, c_to;


	cout << "\n\nLongest Paths from Primary inputs are :";
	forall_nodes(primary_inputs,G)
	{
		if (primary_inputs->indeg() == 0)
		{
			sp.call(G,primary_inputs,edge_lengths,distances,pi);
			for(int i = 0; i< G.numberOfNodes(); i++)
			{
				if(distances[i] <= least)
				{
					least = distances[i];
					index = i;
					name = GA.label(primary_inputs).c_str();

				}
			}

			forall_nodes(v,G)
			{

				if(v->index() == index)
				{
					cout << "\n" << name << " -> " << GA.label(v) << "\t" << "Length: " <<least*-1 << "\t";
					c_to = GA.label(v);

				}
			}

			if ((least*-1) > critical)
			{
				critical = least *-1;
				c_from = name;
				//c_to = GA.label(v);
			}
			least = 0;
			index = -1;
			name = '.';
		}


		//}
	}
	GraphIO::drawSVG(GA, "HW1_graph.svg");
	cout << "\n\nCritical path length = " << critical << " From node " << c_from <<endl;
}


