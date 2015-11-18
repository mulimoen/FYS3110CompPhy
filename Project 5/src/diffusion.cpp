#include "vector.hpp"
#include "diffusion.hpp"
#include <random>
#include <functional>
#include <chrono>

Vector<double> diffusion::forward_euler(const Vector<double> &init_vec,
                             const double alpha, const int steps)
{
  auto new_vec = init_vec;
  
  const double a = 1-2*alpha;
  const double b = alpha;
  for (int ii=0; ii<steps; ii++){
    multiply_inplace(new_vec, a, b);
  }
  return new_vec;
}

Vector<double> diffusion::backward_euler(const Vector<double>& init_vec,
                                const double alpha, const int steps)
{
  auto vec = init_vec;
  
  const double a = 1 + 2*alpha;
  const double b = -alpha;
  for (int ii=0; ii<steps; ii++){
    solve_inplace(vec, a, b);
  }
  
  return vec;
}


Vector<double> diffusion::Crank_Nicolson(const Vector<double>& init_vec,
                              const double alpha, const int steps)
{
  auto vec = init_vec;
  
  const double a_forward = 1 + alpha;
  const double b_forward = -alpha/2;
  
  const double a_backward = 1 - alpha;
  const double b_backward = alpha/2;
  
  for (int ii=0; ii<steps; ii++){
    multiply_inplace(vec, a_backward, b_backward);
    solve_inplace(vec, a_forward, b_forward);
  }
  
  return vec;
}

Vector<int> Monte_Carlo_step(const Vector<int> &start_vec, std::mt19937 &generator)
{
  const int Nbins = start_vec.size();
  Vector<int> out_vec(Nbins);
  
  for (int ii=0; ii<Nbins; ii++){
    out_vec[ii] = 0;
  }
  
  for (int ii=0; ii<Nbins; ii++){
    const int N = start_vec[ii];
    std::uniform_int_distribution<int> right_moves(0, N); // how many moves right?
    const int Nrights = right_moves(generator);
    const int Nlefts = N - Nrights;
    
    if (ii!=Nbins-1){ // not the last element
      out_vec[ii+1] += Nrights;
    }
    if (ii != 0){ // not the first element
      out_vec[ii-1] += Nlefts;
    }
  }
  
  return out_vec;
}


Vector<int> diffusion::Monte_Carlo(const Vector<int>& init_vec,
                                   const int steps, const int fill_rigth)
{
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  
  auto vec = init_vec;
  
  for (int ii=0; ii<steps; ii++){
    vec = Monte_Carlo_step(vec, generator);
    vec[0] = fill_rigth;
    vec[init_vec.size()-1] = 0;
  }
  return vec;
}

