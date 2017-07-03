// TriangleTest.cpp : Defines the entry point for the console application.
//
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include "TriangleTest.h"

class ECouldNotTriangulate : public std::runtime_error
{
public:
   ECouldNotTriangulate() : std::runtime_error("Could not triangulate answer") {
   }
};

int calctriangle(int a, int b)
{
   int a2 = pow(a, 2);
   int b2 = pow(b, 2);

   return a2 + b2;
}

struct triangle {
   int c2;
   int b;
   int a;
};

std::vector<triangle> triangleset;

triangle LookupTriangleUsingStrategy1(triangle lookup)
{
   if (lookup.c2 > lookup.b > lookup.a)
   {
      // everything might be ok
      auto it1 = std::find_if(triangleset.begin(), triangleset.end(), [lookup](auto option) { return option.c2 == lookup.c2; });
      if (it1 != triangleset.end()) {
         auto option = *it1;
         if ((option.b == lookup.b) || (option.a == lookup.a)) {
            return option;
         }
      }
   }
   else if (lookup.a > lookup.b < lookup.c2)
   {
      // assume a is corrupt, c2 and b might be correct
      auto it1 = std::find_if(triangleset.begin(), triangleset.end(), [lookup](auto option) { return option.c2 == lookup.c2; });
      if (it1 != triangleset.end()) {
         auto option = *it1;
         if (option.b == lookup.b) {
            return option;
         }
      }
   }
   
   // if c2 is incorrect, but b and a might be correct
   if (lookup.b > lookup.a)
   {
      auto it1 = std::find_if(triangleset.begin(), triangleset.end(), [lookup](auto option) { return (option.a == lookup.a) && (option.b == lookup.b); });
      if (it1 != triangleset.end()) {
         auto option = *it1;

         return option;
      }
   }

   throw new ECouldNotTriangulate();
}

triangle LookupTriangle(triangle lookup)
{
   auto option1 = LookupTriangleUsingStrategy1(lookup);

   return option1;
}

void OutputTriangleSet()
{
   for (auto triangle : triangleset)
   {
      std::cout << triangle.c2 << ";" << triangle.b << ";" << triangle.a << std::endl;
   }
}

void GenerateTriangleSet()
{
   triangleset.clear();

   for (int a = 1; a < 8; ++a)
   {
      for (int b = 1; b < 8; ++b)
      {
         if (b > a)
         {
            int c2 = calctriangle(a, b);
            if (c2 > a)  // this should be always true
            {
               triangleset.push_back({c2, b, a});
            }
         }
      }
   }

   std::sort(triangleset.begin(), triangleset.end(), [](auto option1, auto option2) {
      return (option1.c2 < option2.c2);
   });
}


int main()
{
   GenerateTriangleSet();

   OutputTriangleSet();

   triangle test1 = { 33, 5, 3 };
   auto answer = LookupTriangle(test1);
   if (answer.c2 == 34)
   {
      std::cout << "Test1 TRUE" << std::endl;
   }
   else
   {
      std::cout << "Test1 FALSE" << std::endl;
   }

   return 0;
}
