// a2ece650.cpp : Defines the entry point for the application.
//
#include <list>
#include <iostream>
#include <vector>
#include "a2-ece650.h"
#include <string>
using namespace std;


class Adjacency_Matrix
{

private:
   // Private variables for class
   int vertex_num = 0;
   std::vector<int> *adjacency_matrix = NULL;
   // Private functions for class
   bool check_out_of_bounds_del(int val_1, int val_2)
   {
      if ((val_1 >= vertex_num || val_2 >= vertex_num) || (val_1== val_2))
      {
         delete[] adjacency_matrix;
         adjacency_matrix = new std::vector<int>[vertex_num]; //Reset contents of table
         return false;
      }
      else
      {
         return true;
      }
   }

public:

   bool check_out_of_bounds(int val_1, int val_2)
   {
      if ((val_1 >= vertex_num || val_2 >= vertex_num))
      {
         return false;
      }
      else
      {
         return true;
      }
   }
   void init_matrix(int vertex)
   {
      if (adjacency_matrix != NULL) // only delete if its not null
      {
         delete[] adjacency_matrix;
         adjacency_matrix = NULL;
      }
      vertex_num = vertex;
      adjacency_matrix = new std::vector<int>[vertex_num];
   }
   bool add_to_matrix(int val_1, int val_2)
   {

         if (check_out_of_bounds_del(val_1, val_2))
         {
            adjacency_matrix[val_1].push_back(val_2);
            adjacency_matrix[val_2].push_back(val_1);
            return true;
         }
         return false;

   }
   int read_matrix(int i, int j) 
   {
      return adjacency_matrix[i][j];
   }
   size_t row_size_matrix(int i)
   {
      return adjacency_matrix[i].size();
   }

};

class Ud_Uw_Graph
{

private:
   // Private variables for class
   int vertex_num = 0;
   Adjacency_Matrix adjacency_matrix;
   // Private functions for class
   void print_path(list<int>shortest_path)
   {
      int cont = 0;
      for (auto v : shortest_path)
      {
         if (cont == shortest_path.size() - 1)
         {
            std::cout << v;
         }
         else
         {
            std::cout << v << "-";
         }
         cont++;
      }
      std::cout << endl;
   }

public:
   void init_matrix(int vertex)
   {
      vertex_num = vertex;
      adjacency_matrix.init_matrix(vertex);
   }
   bool add_to_matrix(int val_1, int val_2)
   {

      return adjacency_matrix.add_to_matrix(val_1, val_2);

   }
   bool Search(int src, int dst)
   {
      // allocate dynamic array
      bool *cheked_vertex;
      cheked_vertex = new bool[vertex_num];
      // List needed 
      list<int> queue;
      list<int> predecessor;
      list<int> successor;
      list<int>shortest_path;

      for (int i = 0; i < vertex_num; i++)
      {
         cheked_vertex[i] = false; // Initialize array as false, no vertex has been cheked
      }
      cheked_vertex[src] = true; // Starting at source vertex, array set to true
      queue.push_back(src);      // first item in qeue to begin search

      //Breadth First Search (BFS) implementation
      while (!queue.empty()) 
      {
         int i = queue.front();
         queue.pop_front();
         int x = adjacency_matrix.row_size_matrix(i);
         for (int j = 0; j < adjacency_matrix.row_size_matrix(i); j++)
         {
            int w = adjacency_matrix.read_matrix(i, j);
            if (cheked_vertex[adjacency_matrix.read_matrix(i, j)] == false)
            {
               predecessor.push_back(i);
               successor.push_back(adjacency_matrix.read_matrix(i, j));
               queue.push_back(adjacency_matrix.read_matrix(i, j));
               cheked_vertex[adjacency_matrix.read_matrix(i, j)] = true;
            }
            if (adjacency_matrix.read_matrix(i, j) == dst)
            {
               int last_predecessor = dst;// initialize predecesor as destination
               while (!successor.empty())
               {
                  if (successor.back() == last_predecessor)       // trace back shortest path
                  {
                     shortest_path.push_back(successor.back());   // add this to the path
                     last_predecessor = predecessor.back();       // save predecesor in order to trace the path back
                     successor.pop_back();
                     predecessor.pop_back();
                  }
                  else
                  {
                     successor.pop_back();
                     predecessor.pop_back();
                  }
               }
               shortest_path.push_back(src); // after path is found add the source to the list
               shortest_path.reverse();
               print_path(shortest_path);
               while (!queue.empty()) 
               {
                  queue.pop_front();
               }
               delete[]cheked_vertex; // delete allocated dynamic array
               return true;            // shortest path found
            }
         }
      }
      delete[]cheked_vertex; // delete allocated dynamic array
      return false;          // shortest path  not found
   }
   bool check_out_of_bounds(int val_1, int val_2) 
   {
      return adjacency_matrix.check_out_of_bounds(val_1, val_2);
   }

};

class Input_Parser
{

private:
   Ud_Uw_Graph graph;
   int capture_edges_flag = 0;
   //Adjacency_Matrix adjacency_matrix;
   void vertex_command(string input, size_t space)
   {
      std::cout << input << endl;
      
      input = input.substr(space, input.length() - 1); // Parse command from line
      int vertex_num = atoi(input.c_str());// Convert parsed data
      if (vertex_num > 1)
      {
         /*std::cout << print << endl;*/
         graph.init_matrix(vertex_num);// Assign vertex num
         capture_edges_flag = 1;
      }
      else
      {
         capture_edges_flag = 0;
      }

   }
   void edge_command(string input, size_t space)
   {
      std::cout << input << endl;
      if (capture_edges_flag == 1) {
         /*std::cout << input << endl;*/
         std::string s = input.substr(space, input.length() - 1); // Remove parsed command from line
         std::string delimiter = "}"; // delimiter of end of line
         size_t pos;
         std::string token;
         std::string token2;
         while ((pos = s.find(delimiter)) != std::string::npos) // iterate through line to find data
         {
            size_t x = s.find(',') - 1 - s.find('<');
            size_t y = s.find('>') - 1 - s.find(',');
            token = s.substr(s.find('<') + 1, x);
            token2 = s.substr(s.find(',') + 1, y);

            int var1 = atoi(token.c_str());
            int var2 = atoi(token2.c_str());
            if (!(graph.add_to_matrix(var1,var2)))
            {
               //std::cerr << " Error: a vertex in the list is out of bounds or a verex has a path to itself\n";
               capture_edges_flag = 0;
               break;
            }

            s.erase(0, s.find_first_of(">") + 2);// erase parsed data for next iteration/
            capture_edges_flag = 2;
         }

      }
      else if (capture_edges_flag == 0) 
      {
         //std::cerr << " Error: need to assign number of vertex first use command V \n";
      }
      else if (capture_edges_flag == 2) 
      {
         //std::cerr << " Error: Edge list allready captured to enter new list first change the # of vertexes according to the new list \n";
      }
   
   }
   void path_command(string  input, size_t space)
   {
      if (capture_edges_flag == 2) {
         input = input.substr(space + 1, input.length() - 1); // Remove parsed command from line
         std::size_t space = input.find(" ");
         string var1 = input.substr(0, space);
         string var2 = input.substr(space + 1, input.length() - 1);
         if ((var1.size()!=0) && (var2.size() != 0))
         {
            int src = atoi(var1.c_str()); // convert parsed data
            int dst = atoi(var2.c_str()); // convert parsed data
            if (!graph.check_out_of_bounds(src, dst))//(src == dst)
            {
               std::cerr << " Error: either source or destination are out of bounds\n";

            }
            else 
            {
               if (src == dst)
               {
                  std::cerr << " Error: cannot search path for itself \n";

               }
               else
               {
                  if (!(graph.Search(src, dst))) // search shortest path
                  {
                     std::cerr << " Error: there is no path \n";
                  }
                  
               }
            }
         }
      }
      else 
      {
         std::cerr << " Error: Empty Graph \n";
      }
   }

public:
   void command_parser(string input)
   {
      std::size_t vertex_ = input.find("V");
      std::size_t edge_ = input.find("E");
      std::size_t path_= input.find("s");
      std::size_t space = input.find(" ");
      // VERTEX COMMAND
      if ((vertex_ != -1) && ((space != -1))) { vertex_command(input, space); }
      //EDGE COMMAND
      if ((edge_  != -1) && ((space != -1))) { edge_command(input, space); }
      // PATH COMMAND
      if ((path_  != -1) && ((space != -1))) { path_command(input, space); }
   }

};


int main(int argc, char *argv[])
{
   string input;
   Input_Parser Parser;

   for (int i = 1; i < argc; ++i)
   {
      Parser.command_parser(string(argv[i]));
   }
   while (std::getline(cin, input))
   {
      
      Parser.command_parser(input);
      input = "";
   }

}