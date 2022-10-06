#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/basic_graph_alg.h>
#include <LEDA/graph/node_array.h>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace boost;


int my_time; 

struct NodeInfo
{
	std::string color; // white,black
	int p; // parent of node 
	int d; // discovery time
	int f; // finishes time
};

std::stack<int> q; // for storing vertices with decreasing finishes time used for reverse graph
int elements = 0; // Vertices of each strongly connected component
int components_1D = 0; // counting the numbger of strongly connected components in 1D vector
std::vector<int> s_components_1D; // 1D vector for storing the separated strongly connected components
std::vector<int> elements_v; // Vector for storing the elements of a specific strongly connected component


typedef adjacency_list<vecS, vecS, directedS, NodeInfo, no_property> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_iterator _vi;
typedef graph_traits<Graph>::edge_iterator _ei;
typedef graph_traits<Graph>::out_edge_iterator _out_it;

typedef leda::graph LGraph;

typedef leda::node node;
typedef leda::edge edge;



void my_DFS_visit(Graph& G, Vertex u,std::stack<int>& q,int reverse);

void my_DFS(Graph& G,int reverse)
{

	_vi vi, vi_end; // vertex iterators

	Vertex u; // for reverse graph purpose


	for (tie(vi, vi_end) = vertices(G); vi != vi_end; vi++)
	{
		G[*vi].color = "white"; // initalization to white
		G[*vi].p = NULL; // Parent node to Null
	}
	my_time = 0;

	if (reverse == 0) // Compute DFS to G
	{
		for (tie(vi, vi_end) = vertices(G); vi != vi_end; vi++)
		{
			if (G[*vi].color == "white")
			{
				my_DFS_visit(G, *vi,q,0);
			}
		}
	}
	else // Compute DFS to G reversed
	{


		while (!q.empty())
		{
			u = q.top();	
			
			my_DFS_visit(G, u, q,1);
			
			q.pop();    


			if(elements>1) // if elements > 1 means that u is the root of a dfs tree and as a result we update the s_componets_1D vector
			{
			
				for(std::vector<int>::iterator it = elements_v.begin(); it!=elements_v.end(); ++it)
				{
					s_components_1D[*it] = components_1D;

				}

				components_1D++;


			}

			else if(elements==1 && s_components_1D[u]==-1)
			{
				
				s_components_1D[u] = components_1D;
				
				components_1D++;

			}
		
                        elements= 0;
		        elements_v.clear(); // when we finish updating then we delete all elements_v contents for storing the nodes of next strongly connected component


		}



	}



	


}


 void my_DFS_visit(Graph& G, Vertex u,std::stack<int>& q,int reverse)
{

	
	_out_it out_it,out_it_end; // adjacency_iterator

	my_time = my_time + 1; 

	G[u].d = my_time; // discovery time
	
	G[u].color = "black"; // discovered set color to black

	for (tie(out_it, out_it_end) = out_edges(u,G); out_it != out_it_end; ++out_it)
	{
		if (G[target(*out_it,G)].color == "white") // if it has not visited yet 
		{
			G[target(*out_it,G)].p = u; // parent of adjacent vertex
			my_DFS_visit(G, target(*out_it,G),q,reverse);
		}

	}


	G[u].color = "black"; // black means discovered
	my_time = my_time + 1;

	G[u].f = my_time; // finishing time

	if(reverse==0)
	{
	   q.push(u);
	}

	else
       {
	
			elements_v.push_back(u); // push back the elements from bottom to top of DFS Tree
			elements++;
       } 
	
}


 void strongly_connected_components(Graph& G,Graph& _G)
 {
	 my_DFS(G,0);

	 my_DFS(_G, 1);

 }



void custom_graph_leda(LGraph& G)
{

	node v[7];

	leda::edge  e;

	for(int i=0; i<7; i++)
	{
		
		v[i] = G.new_node();

	}


	for(int i=0; i<4; i++) // making the first rhombus
        {

                if(i<3)
                   G.new_edge(v[i],v[i+1]);
                else
                   G.new_edge(v[i],v[0]);

        }

        G.new_edge(v[1],v[4]);


	 for(int i=4; i<7; i++)
        {

                if(i<6)
                    G.new_edge(v[i],v[i+1]);

                else
                    G.new_edge(v[i],v[4]);

        }
 
         forall_edges(e,G){
   	   node source=G.source(e);
	
           node target=G.target(e);
           
           std::cout<<source->id()<<" -> "<<target->id() << std::endl;


	}
	

}




void custom_graph_2_leda(LGraph& G)
{

	
        node v[7];

        leda::edge  e;

        for(int i=0; i<6; i++)
        {

                v[i] = G.new_node();

        }


        for(int i=0; i<5; i++) // making the first rhombus
        {

                if(i==4)
                   G.new_edge(v[i],v[0]);
                else
                   G.new_edge(v[i],v[i+1]);

        }


         for(int i=0; i<5; i++)
        {

                G.new_edge(v[5],v[i]);

        }

         forall_edges(e,G){
           node source=G.source(e);

           node target=G.target(e);

           std::cout<<source->id()<<" -> "<<target->id() << std::endl;

        }




}


void clique_graph(LGraph& G,int n,int c) // n->nodes of each clique  c->number of cliques
{
	
	LGraph v[c];
	
	LGraph tmp;

	node v1,v2;

	leda::edge e;

	node v_c[c];
 
        for(int i=0; i<c; i++)
	{
		
		complete_graph(v[i],n);
		v_c[i] = v[i].choose_node();
		
		
	}
		
	
	for(int i=0; i<c; i++)
	{

		G.join(v[i]);

	}


	for(int i=0; i<c-1; i++)
	{

		
		G.new_edge(v_c[i],v_c[i+1]);
		

	}


   /* Not included the self-referencing edges	
	forall_edges(e,G){            
             node t1 = G.source(e);

             node t2 = G.target(e);

             if(t1==t2)
            {
                G.del_edge(e);

            }

        }

    */


   forall_edges(e,G){

    	node s = G.source(e);
    
    	node t =G.target(e);
    
    	std::cout<<s->id()<<" -> "<<t->id()<<std::endl;
   }

}

void Leda_2_Boost(const LGraph& LG, Graph& BG,leda::node_array<NodeInfo>& nodeInfo){

    Edge a;

    leda::edge e;

    leda::node v;

    leda::node_array<Vertex> copy_in_BG(LG);

    forall_nodes(v,LG){
        copy_in_BG[v]=add_vertex(BG);
    }

    bool isAdded;

    forall_edges(e,LG){
        a=add_edge(copy_in_BG[source(e)], copy_in_BG[target(e)], BG).first;
    }
}



int main()
{
	
	LGraph LG; // Leda Graph

	Graph G,_G; // Boost Graph

	_vi vi, vi_end; //Vertex Iterator

	_ei ei, ei_end; // Edge Iterator

	int n;  // number of nodes

	int m; // number of edges

	int k; // number of nodes of each clique

	int cliques; // number of cliques

	std::vector< std::vector<int> >::const_iterator row; // for iterating 2d vectors rows
	std::vector<int>::const_iterator col; // for iterating 2d vectors columns
	std::chrono::time_point<std::chrono::system_clock> start, end; // for measuring time
	std::ofstream myfile; // for storing Leda's and Boost time

	myfile.open("Measurement_Time.txt",std::ios::out | std::ios::app);

	std::cout << "Please give the number of vertices of Graph G  and the number k of nodes of each clique for finding the strongly connected components" << std::endl;

	//std::cout << "Please give the number of vertices of Graph G to compute the n/2 edges and find the SCC" << std::endl;
	
	std::cin >> n >> k;

	//std::cin >> n;

	cliques = n/k;

	m = cliques * k * k; // including the self-referencing edges

	//m = n/2;

	// Random Graph Leda

	//leda::random_graph(LG,n,m);

	clique_graph(LG,k,cliques);
	
	leda::edge e;

	leda::node_array<NodeInfo> nodeInfo(LG);

	//custom_graph_2_leda(LG);

	leda::node_array<int> compnum(LG);

	start = std::chrono::system_clock::now();

	int c = STRONG_COMPONENTS(LG,compnum);

	end = std::chrono::system_clock::now();
	 
	std::chrono::duration<double> elapsed_seconds = end - start ;

	myfile << "Test for Graph with " << LG.number_of_nodes() << " and number of edges " << LG.number_of_edges() << std::endl;
	
	myfile << "Strongly_Connected_Components(LEDA) time: " << elapsed_seconds.count() << std::endl;


	std::cout << "Finding Strongly_Connected_Components in Graph G with vertices= "<< n  <<" and edges= " << m << std::endl;


        std::cout << "Strongly_Connected_Components(LEDA) time: " << elapsed_seconds.count() << std::endl;

	Leda_2_Boost(LG,G,nodeInfo);	

	copy_graph(make_reverse_graph(G), _G); // Reverse Graph Creation	
	
	s_components_1D.resize(num_vertices(G),-1);


	std::cout << "Vertices with the same number belongs to the same Strongly Connected Component" << std::endl;		

	start = std::chrono::system_clock::now();

	strongly_connected_components(G, _G);

	end = std::chrono::system_clock::now();

        elapsed_seconds = end - start; // duration in seconds

	myfile << "Strongly_Connected_Components Boost(Custom) time: " << elapsed_seconds.count() << std::endl;
	
	std::cout << "Strongly_Connected_Components Boost(Custom) time: " << elapsed_seconds.count() << std::endl;

	myfile << "C = " << c << " components_1D = " << components_1D << std::endl;

	std::cout << "C = " << c << " components_1D = " << components_1D << std::endl;

	if(c == components_1D) // Test if LEDA(STRONG_COMPONENTS()) and Custom Boost Algo Agreed on Number of Strongly Connected Components
	{
	
        	myfile << "Number of Strongly Connected components are: " << components_1D  << std::endl;
		
		std::cout << "Number of Strongly Connected components are: " << components_1D  << std::endl;
	
	} 

	myfile << std::endl;

	myfile.close(); 
   
}
