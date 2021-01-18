#include <cstdlib>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>        
#include <SDL2/SDL_image.h>
#include <fstream>
#include <ctime>
#include <iomanip>      // std::setw
#include <chrono>
#include <cmath>
#include <algorithm>

#include "parametres.hpp"
#include "galaxie.hpp"

#define USE_MPI 1
 
#if USE_MPI == 1
#include <mpi.h>
int main(int argc, char ** argv)
{
    char commentaire[4096];
    int width, height;
    SDL_Event event;
    SDL_Window   * window;


    MPI_Init(&argc, &argv);
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
    int nbp;
    int rank;
    MPI_Comm_size(globComm, &nbp);
    MPI_Comm_rank(globComm, &rank);


    parametres param;


    std::ifstream fich("parametre.txt");
    fich >> width;
    fich.getline(commentaire, 4096);
    fich >> height;
    fich.getline(commentaire, 4096);
    fich >> param.apparition_civ;
    fich.getline(commentaire, 4096);
    fich >> param.disparition;
    fich.getline(commentaire, 4096);
    fich >> param.expansion;
    fich.getline(commentaire, 4096);
    fich >> param.inhabitable;
    fich.getline(commentaire, 4096);
    fich.close();

    std::cout << "Resume des parametres (proba par pas de temps): " << std::endl;
    std::cout << "\t Chance apparition civilisation techno : " << param.apparition_civ << std::endl;
    std::cout << "\t Chance disparition civilisation techno: " << param.disparition << std::endl;
    std::cout << "\t Chance expansion : " << param.expansion << std::endl;
    std::cout << "\t Chance inhabitable : " << param.inhabitable << std::endl;
    std::cout << "Proba minimale prise en compte : " << 1./RAND_MAX << std::endl;
    std::srand(std::time(nullptr));

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Galaxie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);

    galaxie_renderer gr(window);

    int deltaT = (20*52840)/width;
    std::cout << "Pas de temps : " << deltaT << " années" << std::endl;

    std::cout << std::endl;

    galaxie g(width + nbp - 1, height, param.apparition_civ);
    /* galaxie g_next(width + nbp - 1, height); */
    galaxie built_g = galaxie(width, height);
    unsigned int split_width;
    split_width = (width + nbp - 1 )/ (nbp - 1);
    built_g.build(g, split_width);
    gr.render(g);
    unsigned long long temps = 0;

    std::chrono::time_point<std::chrono::system_clock> start, end1, end2;

    if (rank == nbp - 1) {
      while (1) {
          start = std::chrono::system_clock::now();
          // This is thread 0
          MPI_Request reqs[nbp-1];
          MPI_Request req;
          for (int i = 0; i < nbp - 1; i++) {
            auto strip_width = split_width;
            if (i == 0)
              strip_width = split_width - 1;
            if (i == nbp - 2)
              strip_width = width + nbp - 2 - i * split_width;

            MPI_Send(g.data()+i*height * split_width, height * strip_width, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Irecv(g.data()+i*height * split_width, height * strip_width, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[i]);
          }
          std::cout << "Waiting..." << std::endl;
          MPI_Waitall(nbp-1, reqs, MPI_STATUSES_IGNORE);
          end1 = std::chrono::system_clock::now();
          // Here we should build g from g_next
          built_g.build(g, split_width); // this copies the content, omitting the extra slices
          for (int i = 1; i < nbp - 1; i++) {
            for (int j = 0; j < height; j++) {
              char left = g.data()[j*width+(i*(split_width+1))];
              char right = g.data()[j*width+(i*(split_width+1))+1];

              if (left == -1 || right == -1) {
                built_g.rend_planete_inhabitable(i*split_width, j);
                left = right = -1;
              }
              else if (left == 1 || right == 1) {
                built_g.rend_planete_habitee(i*split_width, j);
                left = right = 1;
              }
              else {
                built_g.rend_planete_inhabitee(i*split_width, j);
                left = right = 0;
              }
            }
          }

          gr.render(built_g);
          /* g_next.swap(g); */
          end2 = std::chrono::system_clock::now();
          std::chrono::duration<double> elaps1 = end1 - start;
          std::chrono::duration<double> elaps2 = end2 - end1;
          
          temps += deltaT;
          std::cout << "Temps passe : "
                    << std::setw(10) << temps << " années"
                    << std::fixed << std::setprecision(3)
                    << "  " << "|  CPU(ms) : calcul " << elaps1.count()*1000
                    << "  " << "affichage " << elaps2.count()*1000
                    << "\r" << std::flush;
          //_sleep(1000);
          if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            std::cout << std::endl << "The end" << std::endl;
            break;
          }
      }
    } else {
      while (1) {
          auto strip_width = split_width;
          if (rank == 0)
            strip_width = split_width - 1;
          if (rank == nbp - 2)
            strip_width = width + nbp - 2 - rank * split_width;
        // This is worker threads
          galaxie g(strip_width, height);
          galaxie g_next(strip_width, height);
          MPI_Recv(g.data(), strip_width * height, MPI_CHAR, nbp - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          mise_a_jour(param, strip_width, height, g.data(), g_next.data());
          MPI_Send(g_next.data(), strip_width * height, MPI_CHAR, nbp - 1, 0, MPI_COMM_WORLD);
       }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
#else
int main(int argc, char ** argv)
{
    char commentaire[4096];
    int width, height;
    SDL_Event event;
    SDL_Window   * window;

    parametres param;


    std::ifstream fich("parametre.txt");
    fich >> width;
    fich.getline(commentaire, 4096);
    fich >> height;
    fich.getline(commentaire, 4096);
    fich >> param.apparition_civ;
    fich.getline(commentaire, 4096);
    fich >> param.disparition;
    fich.getline(commentaire, 4096);
    fich >> param.expansion;
    fich.getline(commentaire, 4096);
    fich >> param.inhabitable;
    fich.getline(commentaire, 4096);
    fich.close();

    std::cout << "Resume des parametres (proba par pas de temps): " << std::endl;
    std::cout << "\t Chance apparition civilisation techno : " << param.apparition_civ << std::endl;
    std::cout << "\t Chance disparition civilisation techno: " << param.disparition << std::endl;
    std::cout << "\t Chance expansion : " << param.expansion << std::endl;
    std::cout << "\t Chance inhabitable : " << param.inhabitable << std::endl;
    std::cout << "Proba minimale prise en compte : " << 1./RAND_MAX << std::endl;
    std::srand(std::time(nullptr));

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Galaxie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);

    galaxie g(width, height, param.apparition_civ);
    galaxie g_next(width, height);
    galaxie_renderer gr(window);

    int deltaT = (20*52840)/width;
    std::cout << "Pas de temps : " << deltaT << " années" << std::endl;

    std::cout << std::endl;

    gr.render(g);
    unsigned long long temps = 0;

    while (1) {
      mise_a_jour(param, width, height, g.data(), g_next.data());
        gr.render(g);
        g_next.swap(g);
        temps += deltaT;
        std::cout << "Temps passe : "
                  << std::setw(10) << temps << " années"
                  << "\r" << std::flush;
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
          std::cout << std::endl << "The end" << std::endl;
          SDL_DestroyWindow(window);
          SDL_Quit();
        }
  }

    return EXIT_SUCCESS;
}
#endif
