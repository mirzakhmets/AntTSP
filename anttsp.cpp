#include <cstdio>

#include <vector>

#include <cstdlib>

#include <set>

using namespace std;

const int max_n = 1000;
const int max_ant = 2000;

double tau[max_n][max_n];
double eta[max_n][max_n];
double len[max_n][max_n];

double rho = 0.5;
double Q = 100;

int n, m;

int random(int n) {
  int k = rand();
  int l = (k * k) % n;
  return (l + n) % n;
}

class Ant {
  public:
    int start;
  vector < int > path;
  Ant() {}
  Ant(int _start) {
    start = _start;
    path.push_back(start);
  }
  bool can(int t) {
    for (int i = 0; i < path.size(); ++i)
      if (path[i] == t) return false;
    return true;
  }
  int next() {
    int s = path[path.size() - 1];
    int ci = -1;
    double vmax = 0, vall = 0;
    for (int i = 0; i < n; ++i)
      if (can(i)) {
        vall += tau[s][i] * eta[s][i];
        if ((tau[s][i] * eta[s][i]) > vmax || ci < 0) {
          ci = i;
          vmax = tau[s][i] * eta[s][i];
        }
      }
    return ci;
  }
  void reuse() {
    while (path.size() > 0) path.pop_back();
    path.push_back(start);
  }
  double getlen() {
    double tlen = 0;
    for (int i = 1; i < path.size(); ++i) {
      int src = path[i - 1];
      int dst = path[i];
      tlen += len[src][dst];
    }
    return tlen;
  }
  void step() {
    double tlen = getlen();
    for (int i = 1; i < path.size(); ++i) {
      int src = path[i - 1];
      int dst = path[i];
      tau[src][dst] = tau[src][dst] * rho + Q / tlen;
    }
  }
  void step(int t) {
    path.push_back(t);
  }
};

int main(int argc, char **argv) {
  int i, j;
  
  if (argc < 2) {
  	printf ("Usage: anttsp <Ants number> [<TSP file>]\n");
  	printf ("TSP file format:\n");
  	printf ("n\n");
  	printf ("... ... ...\n... nxn ...\n... ... ...");
  	return 1;
  }
  
  m = atoi(argv[1]);
  
  if (argc == 3) {
  	freopen(argv[2], "r", stdin);
  }
  
  scanf("%d", &n);
  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j) {
      scanf("%lf", len[i] + j);
      eta[i][j] = 1.0 / len[i][j];
      tau[i][j] = 1.0;
    }

  Ant * ants = new Ant[m];
  for (i = 0; i < m; ++i)
    ants[i].start = random(n);
  double sol = 1e+100;
  int NC = 0;
  vector < int > best_tour;
  for (NC = 0; NC < 10000; ++NC) {
    for (i = 0; i < m; ++i) ants[i].reuse();
    for (;;) {
      bool b_ok = false;
      int dec[max_ant];
      for (i = 0; i < m; ++i) {
        dec[i] = ants[i].next();
        if (dec[i] >= 0) {
          b_ok = true;
          ants[i].step(dec[i]);
        }
      }
      if (!b_ok) break;
    }
    for (i = 0; i < m; ++i) {
      ants[i].step();
      if (ants[i].path.size() == n) {
        double tlen = ants[i].getlen();
        if (tlen < sol) {
          sol = tlen;
          best_tour = ants[i].path;
        }
      }
    }
  }
  
  printf("%.2lf\n", sol);
  for (int i = 0; i < n; ++i) printf("%d ", best_tour[i]);
  
  printf ("\n");
  return 0;
}
