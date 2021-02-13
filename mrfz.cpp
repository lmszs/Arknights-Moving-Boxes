#include <bits/stdc++.h>
#define MAXN 15
using std::make_pair;
using std::pair;
using std::set;
using std::vector;
typedef pair<int, int> Position;
typedef set<Position> Boxes;
struct Map {
	int n;
	int m;
	int data[MAXN][MAXN] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3},
							{-2, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, 0, 1, -1, 0, 0, 0, -3},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 1, 0, 0, -1, 1, 0, 0, -3},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, -1, 1, 0, 0, 0, 0, 0},
							{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3}};
	Map() { n = 0, m = 0; }
	Map(int x, int y) { n = x, m = y; }
	void readMap() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				scanf("%d,", &data[i][j]);
			}
		}
	}
	void printMap() {
		printf("Map %d X %d\n", n, m);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				char pr = 0;
				switch (data[i][j]) {
					case 2:	 // Box
						pr = 'B';
						break;
					case 1:	 // Wall
						pr = '0';
						break;
					case 0:	 // Path
						pr = ' ';
						break;
					case -1:  // Change
						pr = 'C';
						break;
					case -2:  // Entrance
						pr = 'R';
						break;
					case -3:  // Door
						pr = 'E';
						break;
					default:
						break;
				}
				putchar(pr);
			}
			printf("\n");
		}
		printf("\n");
	}
	void print() {
		for (int i = 0; i < n; i++) {
			printf("{");
			for (int j = 0; j < m; j++) {
				printf("%d,", data[i][j]);
			}
			printf("},\n");
		}
	}
};
struct Enemy {
	int x;
	int y;
	bool color;
	Enemy() { x = y = color = 0; }
	Enemy(int a, int b, bool c) { x = a, y = b, color = c; }
	void changeColor() { color = 1 - color; }
	Position getPosition() { return make_pair(x, y); }
	void moveTo(int a, int b, int change) {
		x = a, y = b;
		if (change) changeColor();
	}
};

const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};

#define CHECK(x, y) (((x) >= 0) && ((x) < map.n) && ((y) >= 0) && ((y) < map.m))
bool generateDis(const Map& map, Position st, Position end, int dis[][MAXN]) {
	dis[end.first][end.second] = 0;
	using std::queue;
	queue<Position> q;
	q.push(end);
	while (!q.empty()) {
		Position t = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			int x = t.first + dx[i], y = t.second + dy[i];
			if (CHECK(x, y) && map.data[x][y] <= 0 && dis[x][y] > 1000) {
				dis[x][y] = dis[t.first][t.second] + 1;
				q.push(make_pair(x, y));
			}
		}
	}
	// Can not arrive
	if (dis[st.first][st.second] > 1000)
		return 1;
	else
		return 0;
}
bool checkIndividual(const Map& map, Enemy enemy, Position end, bool endColor,
					 const int dis[][MAXN], const int fullDis[][MAXN]) {
	struct trp {
		int x, y, z;
		trp() { x = y = z = 0; }
		trp(int a, int b, int c) : x(a), y(b), z(c) {}
		bool operator<(const trp& rho) const {
			return (x == rho.x) ? (y == rho.y ? z < rho.z : y < rho.z)
								: (x < rho.x);
		}
	};
	while (enemy.getPosition() != end) {
		Position now = enemy.getPosition();
		trp tmp[4];
		for (int i = 0; i < 4; i++) {
			tmp[i] = trp(dis[now.first + dx[i]][now.second + dy[i]],
						 fullDis[now.first + dx[i]][now.second + dy[i]], i);
		}
		std::sort(tmp, tmp + 4);
		int next_i = tmp[0].z;
		int x = now.first + dx[next_i], y = now.second + dy[next_i];
		enemy.moveTo(x, y, map.data[x][y] == -1);
	}
	return enemy.color == endColor;
}

bool check(Boxes& boxes, Map& map) {
	Position doors[] = {make_pair(0, 10), make_pair(3, 10), make_pair(5, 10),
						make_pair(8, 10)};
	int doorColor[] = {1, 1, 0, 0};
	// First Group
	// Enemy enemies[] = {Enemy(1, 1, 0), Enemy(3, 1, 0), Enemy(5, 1, 1),
	// 				   Enemy(7, 1, 1)};
	// Second Group
	Enemy enemies[] = {Enemy(1, 1, 1), Enemy(3, 1, 1), Enemy(5, 1, 0),
					   Enemy(7, 1, 0)};
	// Third Group
	// Enemy enemies[] = {Enemy(1, 1, 0), Enemy(3, 1, 1), Enemy(5, 1, 0),
	// 				   Enemy(7, 1, 1)};

	int dis[4][MAXN][MAXN] = {0};
	memset(dis, 0x7f, sizeof(dis));
	for (int i = 0; i < 4; i++) {
		if (generateDis(map, enemies[i].getPosition(), doors[i], dis[i])) {
			return 0;
		}
	}
	// To solve the conflict of same length path
	// Not necessary
	int fullDis[MAXN][MAXN] = {0};
	// First calculation
	// for (int i = 0; i < map.n; i++) {
	// 	for (int j = 0; j < map.m; j++) {
	// 		long long tmp = 0;
	// 		for (int k = 0; k < 4; k++) {
	// 			tmp += dis[k][i][j];
	// 		}
	// 		fullDis[i][j] = tmp > 0x7f7f7f7fll ? 0x7f7f7f7f : tmp;
	// 	}
	// }

	// Second calculation
	// for (int i = 0; i < map.n; i++) {
	// 	for (int j = 0; j < map.m; j++) {
	// 		for (int k = 0; k < 4; k++) {
	// 			fullDis[i][j] += (i - doors[k].first) * (i - doors[k].first) +
	// 							 (j - doors[k].second) * (j - doors[k].second);
	// 		}
	// 	}
	// }

	for (int i = 0; i < 4; i++) {
		if (checkIndividual(map, enemies[i], doors[i], doorColor[i], dis[i],
							fullDis) == 0)
			return 0;
	}
	return 1;
}

vector<Boxes> Ans;
void dfs(Boxes& boxes, int lim, int pre_i, int pre_j, Map& map) {
	if (boxes.size() == lim) {
		for (auto x : boxes) {
			map.data[x.first][x.second] = 2;
		}
		if (check(boxes, map)) {
			Ans.push_back(boxes);
			map.printMap();
		}
		for (auto x : boxes) {
			map.data[x.first][x.second] = 0;
		}
		return;
	}
	for (int i = pre_i; i < map.n; i++) {
		int j = 0;
		if (i == pre_i) j = pre_j;
		for (; j < map.m; j++) {
			if (map.data[i][j] == 0) {
				boxes.insert(make_pair(i, j));
				dfs(boxes, lim, i, j + 1, map);
				boxes.erase(boxes.find(make_pair(i, j)));
			}
		}
	}
}

int main() {
	Map map = Map(9, 11);
	// map.readMap();
	map.printMap();
	int boxes_limit = 1;
	Boxes boxes;
	dfs(boxes, boxes_limit, 0, 0, map);
	printf("%d", Ans.size());
	system("pause");
}

/*
	原地图
	int data[MAXN][MAXN] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3},
							{-2, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, 0, 0, -1, 0, 0, 0, -3},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, 0, 0, -1, 0, 0, 0, -3},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, -1, 1, 0, 0, 0, 0, 0},
							{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3}};

	突袭地图
	int data[MAXN][MAXN] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3},
							{-2, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, 0, 1, -1, 0, 0, 0, -3},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 1, 0, 0, -1, 1, 0, 0, -3},
							{1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
							{-2, 0, 0, 0, -1, 1, 0, 0, 0, 0, 0},
							{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3}};


*/
