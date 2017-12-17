#include "decision_tree.hpp"
#include <random>
#include <cstdio>
#include <fstream>
using namespace d2;


#ifndef N
#define N 1000000
#endif

#ifndef D
#define D 28
#endif

#ifndef NC
#define NC 2
#endif

#ifndef MD
#define MD 8
#endif

void sample_naive_data(real_t *X, real_t *y, real_t *w) {
  for (size_t i=0; i<N; ++i) {
    y[i] = rand() % 2;
    if (y[i]) {
      for (size_t j=0; j<D; ++j)
	X[i*D+j]=(real_t) rand() / (real_t) RAND_MAX;
    } else {
      for (size_t j=0; j<D; ++j)
	X[i*D+j]=(real_t) rand() / (real_t) RAND_MAX - .1;
    }
    if (w) w[i] = 1.; // (real_t) rand() / (real_t) RAND_MAX;
  }  
}

real_t accuracy(real_t *y_pred, real_t *y_true, size_t n) {
  size_t k=0;
  for (size_t i=0; i<n; ++i)
    if (y_pred[i] == y_true[i]) ++k;
  return (real_t) k / (real_t) n;
}

int main(int argc, char* argv[]) {
  real_t *X, *y, *w=NULL, *y_pred;

  // prepare naive training data
  X = new real_t[D*N];
  y = new real_t[N];
  //w = new real_t[N];
  y_pred = new real_t[N];

  if (argc == 1) {
    sample_naive_data(X, y, w);
  } else {
    std::ifstream train_fs;
    train_fs.open(argv[1]);
    for (auto i=0; i<N; ++i) {
      std::string line;
      getline(train_fs, line);
      const char *_buf = line.c_str();
      sscanf(_buf, "%lf", &y[i]);
      for (auto j=0; j<D; ++j)
	sscanf(_buf, ",%lf", &X[i*D+j]);
    }
    train_fs.close();
  }


  // create classifier
  auto classifier = new Decision_Tree<D, NC, def::gini>();
  classifier->init();
  classifier->set_max_depth(MD);

  // training
  real_t start=getRealTime();
  classifier->fit(X, y, w, N);
  printf("time: %lf seconds\n", getRealTime() - start);

  if (argc == 1) {
    // prepare naive testing data
    sample_naive_data(X, y, w);
    classifier->predict(X, N, y_pred);

    // output result
    printf("accuracy: %.3f\n", accuracy(y_pred, y, N) );  
  }

  return 0;
}
