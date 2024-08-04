#include <cstdio>

#include <vector>

#include <cstdlib>

#include <set>

#include <ctime>

using namespace std;
const int max_n = 4000;
const int max_ant = 2000;
double tau[max_n][max_n];
double eta[max_n][max_n];
double len[max_n][max_n];
double rho = 0.5;
double Q = 100;
int n, m, nc;
int random(int n) {
	return (((rand() * rand() * rand()) % n) + n) % n;
}
class Ant {
	public: int start;
	vector < int > path;
	bool b_can[max_n];
	double t_len = 0;
	Ant() {
		for(int i = 0; i < max_n; ++i) b_can[i] = true;
	}
	Ant(int _start) {
		start = _start;
		path.push_back(start);
		for(int i = 0; i < max_n; ++i) b_can[i] = true;
	}
	bool can(int t) {
		return b_can[t];
	}
	int next() {
		int s = path[path.size() - 1];
		int ci = -1;
		double vmax = 0, vall = 0;
		for(int i = 0; i < n; ++i)
			if(can(i)) {
				vall += tau[s][i] * eta[s][i];
				if((tau[s][i] * eta[s][i]) > vmax || ci < 0) {
					ci = i;
					vmax = tau[s][i] * eta[s][i];
				}
			}
		return ci;
	}
	void reuse() {
		for(int i = 0; i < path.size(); ++i) {
			b_can[path[i]] = true;
		}
		while(path.size() > 0) {
			path.pop_back();
		}
		path.push_back(start);
		b_can[start] = false;
		t_len = 0;
	}
	double getlen() {
		return t_len;
	}
	void step() {
		double tlen = getlen();
		for(int i = 1; i < path.size(); ++i) {
			int src = path[i - 1];
			int dst = path[i];
			tau[src][dst] = tau[src][dst] * rho + Q / tlen;
		}
	}
	void step(int t) {
		path.push_back(t);
		b_can[t] = false;
		t_len += len[path[path.size() - 2]][path[path.size() - 1]];
	}
};
int main(int argc, char ** argv) {
	scanf("%d %d %d", & n, & m, & nc);
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j) {
			scanf("%lf", len[i] + j);
			eta[i][j] = 1.0 / len[i][j];
			tau[i][j] = 1.0;
		}
	srand(time(NULL));
	Ant * ants = new Ant[m];
	for(int i = 0; i < m; ++i) ants[i].start = random(n);
	double sol = 1e+100;
	int NC = 0;
	vector < int > best_tour;
	for(NC = 0; NC < nc; NC += best_tour.size() == n ? 1 : 0) {
		for(int i = 0; i < m; ++i) ants[i].reuse();
		for(;;) {
			bool b_ok = false;
			int dec[max_ant];
			for(int i = 0; i < m; ++i) {
				dec[i] = ants[i].next();
				if(dec[i] >= 0) {
					b_ok = true;
					ants[i].step(dec[i]);
				}
			}
			if(!b_ok) break;
		}
		for(int i = 0; i < m; ++i) {
			ants[i].step();
			if(ants[i].path.size() == n) {
				double tlen = ants[i].getlen();
				if(tlen < sol) {
					sol = tlen;
					best_tour = ants[i].path;
				}
			}
		}
	}
	printf("%.2lf\n", sol);
	for(int i = 0; i < best_tour.size(); ++i) {
		printf("%d ", best_tour[i] + 1);
	}
	printf("\n");
	return 0;
}
