// rgen.cpp : Defines the entry point for the application.
//
#include <unistd.h>
#include <iostream>
#include <list>

#include <vector>

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;
// #defines
#define MAX_TIRES 25

//Global vars
int count = 0;


class Point
{
public:
   int x;
   int y;
   Point() {}
   Point(int _x, int _y)
   {
      x = _x;
      y = _y;
   }
};

// Function declarations
bool intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
int first_bigger_second(int first, int second);
int first_smaller_second(int first, int second);
bool cross_product(int x1, int y1, int x2, int y2, int x3, int y3);
int coord_generator(int max_value);
string gen_random(int len);
void Add_Streets(list<list<Point>>street_list, list<string> name_list, string command);
void Remove_Streets(list<string> name_list, string command);
bool Invalid_Street(list<Point> point_list);
bool Check_Point_Repetition(list<Point> point_list, Point point_to_add);
bool Check_Overlapping_Streets(list<list<Point>>street_list, list<Point> point_list);

// Main Code
int main(int argc, char *argv[])
{
// 0 Init  Variables
   int c;
   string s_input;
   string n_input;
   string l_input;
   string c_input;
   int s_val = 10;
   int n_val = 5; 
   int l_val = 5; 
   int c_val = 20;
   list<string> prev_name_list;
// 0 END

// 1 Check Arguments recieved and if they are valid
   while ((c = getopt(argc, argv, "s:n:l:c:")) != -1)
      switch (c)
      {
      case 's':
         s_input = optarg;
         s_val = atoi(s_input.c_str());
         break;
      case 'n':
         n_input = optarg;
         n_val = atoi(n_input.c_str());
         break;
      case 'l':
         l_input = optarg;
         l_val = atoi(l_input.c_str());
         break;
      case 'c':
         c_input = optarg;
         c_val = atoi(c_input.c_str());
         break;
      case '?':
            std::cerr << "Error: unknown option: " << optopt << std::endl;
         return 1;
      default:
         return 0;
      }

   if( s_val < 2){std::cerr << "Error: argument -s needs to be >= 2 " << std::endl;   exit(0);}
   if( n_val < 1){std::cerr << "Error: argument -n needs to be >= 1 " << std::endl;   exit(0);}
   if( l_val < 5){std::cerr << "Error: argument -l needs to be >= 5 " << std::endl;   exit(0);}
   if( c_val < 1){std::cerr << "Error: argument -c needs to be >= 1 " << std::endl;   exit(0);}
// 1 END

   // open /dev/urandom to read
   std::ifstream urandom("/dev/urandom");

   // check that it did not fail
   if (urandom.fail()) {
      std::cerr << "Error: cannot open /dev/urandom\n";
      return 1;
   }


   while(1)
   {
      string output_var;
      list<list<Point>> street_list;
      list<string> name_list;
      int street_num_val;
      char street_num = 'a';

      // Street number random generator
      urandom.read(&street_num, 1);
      street_num = (street_num % char(s_val + 1));
      if (street_num < 2) { street_num = 2; }
      street_num_val = (unsigned int)street_num;
 
      // Predefine street names
      for (int i = 0; i <= street_num_val - 1; i++)
      {
         name_list.emplace_back(gen_random(5));
      }

      //Proceed to generate "x" streets 
      for (int i = 0; i <= street_num_val -1; i++)
      {
         list<Point> point_list;
         int x = 0;
         int y = 0;
         Point p1(0, 0);
         int line_seg_val;
         char line_seg = 'a';

         // Line Segment number random generator
         urandom.read(&line_seg, 1);
         line_seg = (line_seg % char(n_val + 1));
         if (line_seg < 1) { line_seg = 1; }
         line_seg_val = (unsigned int)line_seg;

         //Loop generator 
         do {
            int j = 0;
               while ( j <= line_seg_val)
               {
                  do {
                        p1.x = coord_generator(c_val);
                        p1.y = coord_generator(c_val);
                  } while (Check_Point_Repetition(point_list, p1));

                  point_list.emplace_back(p1);

                  if (Invalid_Street(point_list))
                  {
                     while (!(point_list.empty()))
                     {
                        point_list.pop_front();
                     }
                     j=0; 
                  }
                  else 
                  { 
                     j++; 
                  }
               }
         } while (Check_Overlapping_Streets(street_list, point_list));
         street_list.emplace_back(point_list);
      }
      // Streets generated clear fail counter
      count = 0; 
      // Delete previos streets if there is any
      if ((!(prev_name_list.empty()))) 
      {
         Remove_Streets(prev_name_list, "r"); 
      }
      // Clear previos street after deletion 
      while (!(prev_name_list.empty()))
      {
         prev_name_list.pop_front();
      }
      // Save new previos streets for next iteration
      for (list<string>::iterator itr = name_list.begin(); itr != name_list.end(); itr++)
      {
         prev_name_list.emplace_back(*itr);
      }
      // Output generated streets
      Add_Streets(street_list, name_list,"a");
      // Wait designated time
      sleep(l_val);
   }
}

// Function Definitions
bool intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
   float ax = x2 - x1;
   float ay = y2 - y1;

   float bx = x3 - x4;
   float by = y3 - y4;

   float dx = x3 - x1;
   float dy = y3 - y1;

   float det = ax * by - ay * bx;

   if (det == 0) { return false; }

   float r = (dx * by - dy * bx) / det;
   float s = (ax * dy - ay * dx) / det;

   return !(r < 0 || r > 1 || s < 0 || s > 1);
}


int first_bigger_second(int first, int second)
{
   if (first >= second) { return first; }
   if (first < second) { return second; }
}


int first_smaller_second(int first, int second)
{
   if (first >= second) { return second; }
   if (first < second) { return first; }
}


bool cross_product(int x1, int y1, int x2, int y2, int x3, int y3)
{
   float xa = x3 - x1;
   float ya = y3 - y1;
   float xb = x2 - x1;
   float yb = y2 - y1;
   float cross = (xa * yb) - (ya * xb);

   if (cross == 0)
   {
      // Check if it is in range
      int xbig = first_bigger_second(x1, x2);
      int ybig = first_bigger_second(y1, y2);
      int xsmall = first_smaller_second(x1, x2);
      int ysmall = first_smaller_second(y1, y2);
      if ((x3 > xbig) || (x3 < xsmall)) { return false; }
      if ((y3 > ybig) || (y3 < ysmall)) { return false; }
      else { return true; }
   }
   return false;
}


int coord_generator(int max_value)
{
   std::ifstream urandom("/dev/urandom");
   // check that it did not fail
   if (urandom.fail()) { std::cerr << "Error: cannot open /dev/urandom\n"; }

   int coord;
   char coord_char = 'a';

   urandom.read(&coord_char, 1);
   coord_char = (coord_char % char(max_value + 1));
   coord = coord_char;

   return coord;
}


string gen_random(int len)
{
   string s = "";
   static const char characters[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

   for (int i = 0; i < len; ++i)
   {
      s += characters[rand() % (sizeof(characters) - 1)];
   }

   return s;
}


void Add_Streets(list<list<Point>>street_list, list<string> name_list, string command)
{
   string x;
   string output_var = "";

   for (list<list<Point>>::iterator itr = street_list.begin(); itr != street_list.end(); itr++)
   {
      output_var += command;
      output_var += " \"";
      x = name_list.front();
      name_list.pop_front();
      output_var += x;
      output_var += "\" ";

      for (list<Point>::iterator it = itr->begin(); it != itr->end(); it++)
      {
         output_var += "(";
         output_var += to_string(it->x);
         output_var += ",";
         output_var += to_string(it->y);
         output_var += ") ";
      }
      std::cout << output_var << std::endl;
      output_var = "";
   }
   std::cout << "g" << std::endl;
}


void Remove_Streets(list<string> name_list, string command)
{
   int i = 0;
   string x;
   string output_var = "";

   while (!(name_list.empty()))
   {
      output_var += command;
      output_var += " \"";
      x = name_list.front();
      name_list.pop_front();
      output_var += x;
      output_var += "\" ";
      std::cout << output_var << std::endl;
      output_var = "";
   }
}


bool Invalid_Street(list<Point> point_list)
{
   int size = point_list.size();
   Point *p = new Point[point_list.size()];

   for (int w = 0; w < size; w++)
   {
      p[w] = point_list.front();
      point_list.pop_front();
   }

   if (size > 3)
   {
      for (int k = 0; k < size - 4; k++)
      {
         bool Intercept = intersection(p[k].x, p[k].y, p[k + 1].x, p[k + 1].y, p[k + 2].x, p[k + 2].y, p[k + 3].x, p[k + 3].y);
         if (Intercept)
         {
            count++;
            if (count >= MAX_TIRES)
            {
               std::cerr << "Error: failed to generate valid input for " << count << " simultaneous attempts." << std::endl;
               exit(0);
            }
            return true;
         }
         else
         {
            bool overlap = (cross_product(p[k].x, p[k].y, p[k + 1].x, p[k + 1].y, p[k + 2].x, p[k + 2].y)) || (cross_product(p[k].x, p[k].y, p[k + 1].x, p[k + 1].y, p[k + 3].x, p[k + 3].y));
            if (overlap)
            {
               count++;
               if (count >= MAX_TIRES)
               {
                  std::cerr << "Error: failed to generate valid input for " << count << " simultaneous attempts." << std::endl;
                  exit(0);
               }
               return true;
            }
         }
      }
   }

   if (size == 3)
   {
      bool overlap = cross_product(p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y);
      if (overlap)
      {
         count++;
         if (count >= MAX_TIRES)
         {
            std::cerr << "Error: failed to generate valid input for " << count << " simultaneous attempts." << std::endl;
            exit(0);
         }
         return true;
      }
   }

   if (p != NULL) // only delete if its not null
   {
      delete[] p;
      p = NULL;
   }

   return false;
}


bool Check_Point_Repetition(list<Point> point_list, Point point_to_add)
{
   if (point_list.empty())
   {
      return false;
   }

   for (std::list<Point>::iterator it = point_list.begin(); it != point_list.end(); ++it)
   {
      if ((it->x == point_to_add.x) && (it->y == point_to_add.y))
      {
         count++;
         if (count >= MAX_TIRES)
         {
            std::cerr << "Error: failed to generate valid input for " << count << " simultaneous attempts." << std::endl;
            exit(0);
         }
         return true;
      }
   }
   return false;
}


bool Check_Overlapping_Streets(list<list<Point>>street_list, list<Point> point_list)
{
   int x = 0;

   if (street_list.empty())
   {
      return false;
   }

   for (std::list<list<Point>>::iterator itr = street_list.begin(); itr != street_list.end(); ++itr)
   {
      for (list<Point>::iterator it(itr->begin()), next(it); it != itr->end() && ++next != itr->end(); it = next)
      {
         int x1 = it->x;
         int y1 = it->y;
         int x2 = next->x;
         int y2 = next->y;

         for (list<Point>::iterator it2(point_list.begin()), next2(it2); it2 != point_list.end() && ++next2 != point_list.end(); it2 = next2)
         {
            int x3 = it2->x;
            int y3 = it2->y;
            int x4 = next2->x;
            int y4 = next2->y;

            bool overlap = cross_product(x1, y1, x2, y2, x3, y3) || cross_product(x1, y1, x2, y2, x4, y4);
            if (overlap)
            {
               count++;
               if (count >= MAX_TIRES)
               {
                  std::cerr << "Error: failed to generate valid input for " << count << " simultaneous attempts." << std::endl;
                  exit(0);
               }
               return true;
            }
         }
      }
   }
   return false;
}
