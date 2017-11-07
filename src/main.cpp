#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

#include "Vector.h"
#include "LinkedList.h"

using namespace std::chrono;

namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

void perfomTest()
{
  aisdi::LinkedList<std::string> list( {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Praesent", "posuere", "tortor", "quis", "iaculis", "dignissim.", "Vestibulum", "nec", "tortor", "malesuada,", "semper", "enim", "eu,", "faucibus", "lectus.", "Morbi", "urna", "risus,", "interdum", "in", "nisl", "eu,", "convallis", "tincidunt", "dolor.", "Nunc", "a", "augue", "dui.", "Nam", "ut", "dignissim", "odio.", "Quisque", "vitae", "lectus", "eget", "risus", "volutpat", "rutrum.", "Sed", "dignissim."} );
  aisdi::Vector<std::string> vect( {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Praesent", "posuere", "tortor", "quis", "iaculis", "dignissim.", "Vestibulum", "nec", "tortor", "malesuada,", "semper", "enim", "eu,", "faucibus", "lectus.", "Morbi", "urna", "risus,", "interdum", "in", "nisl", "eu,", "convallis", "tincidunt", "dolor.", "Nunc", "a", "augue", "dui.", "Nam", "ut", "dignissim", "odio.", "Quisque", "vitae", "lectus", "eget", "risus", "volutpat", "rutrum.", "Sed", "dignissim."} );

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  list.prepend("Test");
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration1 = duration_cast<microseconds>( t2 - t1 ).count();
  std::cout << "List prepend time: " << duration1 << std::endl;
  
  high_resolution_clock::time_point t3 = high_resolution_clock::now();
  vect.prepend("Test");
  high_resolution_clock::time_point t4 = high_resolution_clock::now();
  auto duration2 = duration_cast<microseconds>( t4 - t3 ).count();
  std::cout << "Vector prepend time: " << duration2 << std::endl;
  
  high_resolution_clock::time_point t5 = high_resolution_clock::now();
  list.begin() + (list.getSize() - 1);
  high_resolution_clock::time_point t6 = high_resolution_clock::now();
  auto duration3 = duration_cast<microseconds>( t6 - t5 ).count();
  std::cout << "List get last element iterator: " << duration3 << std::endl;
  
  high_resolution_clock::time_point t7 = high_resolution_clock::now();
  vect.begin() + (vect.getSize() - 1);
  high_resolution_clock::time_point t8 = high_resolution_clock::now();
  auto duration4 = duration_cast<microseconds>( t8 - t7 ).count();
  std::cout << "Vector get last element iterator: " << duration4 << std::endl;
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}
