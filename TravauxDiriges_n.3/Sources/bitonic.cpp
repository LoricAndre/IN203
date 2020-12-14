# include <algorithm>
# include <utility>
# include <vector>
# include <iostream>
# include <functional>
# include <random>
# include <cassert>
# include <exception>
# include <chrono>
# include <stdexcept>
# include "Vecteur.hpp"
#include <pthread.h>

using namespace Algebra;

// Tri Parallèle Bitonic
namespace Bitonic
{
  template<typename Obj>
  struct thread_data {
    bool up;
    Obj* objs;
    int len;
  };
  template<typename Obj>
  void _compare( bool up, Obj* objs, int len )
  {
    int dist = len/2;
    for ( int i = 0; i < dist; ++i ) {
      if ( (objs[i] > objs[i+dist]) == up ) {
	std::swap(objs[i],objs[i+dist]);
      }
    }
  }
  // --------------------------------------------
  template<typename Obj> std::pair<Obj*,int>
  _merge( bool up, Obj* objs, int len )
  {
    if (len == 1) return std::make_pair(objs,len);
    _compare(up,objs,len);
    auto first  = _merge(up, objs, len/2);
    auto second = _merge(up, objs+len/2, len-(len/2));
    return std::make_pair(first.first, first.second + second.second);
  }
  // --------------------------------------------
  template<typename Obj>
  void* _sort_thread(void*);
  template<typename Obj> std::pair<Obj*,int>
  _sort( bool up, Obj* objs, int len )
  {
    if (len <= 1) return std::make_pair(objs,1);
    std::pair<Obj*, int>* sorted[2];
    pthread_t threads[2];
    thread_data<Obj> first_data = {
      true,
      objs,
      len/2
    };
    thread_data<Obj> second_data = {
      true,
      objs,
      len/2
    };
    pthread_create(&threads[0], NULL, _sort_thread<Obj>, (void*) &first_data);
    pthread_create(&threads[1], NULL, _sort_thread<Obj>, (void*) &second_data);
    for (int i = 0; i < 2; i++)
      pthread_join(threads[i], (void**) &sorted[i]);
    return _merge(up, sorted[0]->first, sorted[0]->second + sorted[1]->second );
  }
  template<typename Obj> void*
    _sort_thread(void* raw_args) {
      thread_data<Obj>* args = (thread_data<Obj>*) raw_args;
      std::pair<Obj*, int>* res = (std::pair<Obj*, int>*) malloc(sizeof(std::pair<Obj*, int>));
      *res = _sort(args->up, args->objs, args->len);
      pthread_exit((void*) res);
    }
  // --------------------------------------------
  template<typename Obj> std::vector<Obj>&
  sort( bool up, std::vector<Obj>& x )
  {
    _sort(up, x.data(), int(x.size()));
    return x;
  }  
}

int main()
{     
    std::chrono::time_point<std::chrono::system_clock> start, end;
    const size_t N = (1UL << 21);
    const size_t dim = 40;
    std::random_device rd;
    std::mt19937 generator1(rd());
    std::mt19937 generator2(rd());
    
    std::uniform_int_distribution<int> intDistrib(-163845,163845);
    auto genInt = std::bind( intDistrib, generator1 );
    std::uniform_real_distribution<double> coordDistrib(-10.,10.);
    auto genDouble = std::bind( coordDistrib, generator2 );

    // Trie sur les entiers :
    std::vector<double> tab(N);
    for ( auto& x : tab ) x = genInt();
    start = std::chrono::system_clock::now();
    Bitonic::sort(true, tab);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Temps calcul tri sur les entiers : " << elapsed_seconds.count() 
              << std::endl;
    for ( size_t i = 1; i < tab.size(); ++i ) {
        if ( tab[i] < tab[i-1] ) {
            throw std::logic_error("Erreur de tri pour les entiers !");
        }
    }
  
    // Trie sur les vecteurs :
    std::vector<Vecteur> vtab(N);
    for ( auto& x : vtab ) {
        x = Vecteur(dim);
        x[0] = genDouble();
        x[1] = genDouble();
        x[2] = genDouble();
        x[3] = genDouble();
    }
    start = std::chrono::system_clock::now();
    Bitonic::sort(true, vtab);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cout << "Temps calcul tri sur les vecteurs : " << elapsed_seconds.count() 
              << std::endl;
    for ( size_t i = 1; i < vtab.size(); ++i ) {
        if ( vtab[i] < vtab[i-1] ) {
            throw std::logic_error("Erreur de tri pour les vecteurs !");
        }
    }
    
    return 0;
}
