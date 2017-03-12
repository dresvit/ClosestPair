#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int maxn = 60005;
const int maxm = 105;
const int maxd = 800;
const int maxcp = 5;
const double inf = 1e30;

struct Proj {
	int id;
	double val;
} proj[maxm][maxn];

struct CP {
	int id1, id2;
	double dist;
	
	CP() {}
	CP(int _id1, int _id2, double _dist) {
		id1 = _id1;
		id2 = _id2;
		dist = _dist;
	}
};

int n;
int d;
int m;
int obj[maxn][maxd];                 //数据集中的对象
double random_vector[maxm][maxd];    //随机向量
double elapsed_time[maxcp];          //运行时间
CP ans[maxcp];                       //答案
FILE *fp;

double sqr(double x) {
	return x * x;
}

CP cp_min(CP cp1, CP cp2) {
	if (cp1.dist < cp2.dist) return cp1;
	else return cp2;
}

void swap(Proj &a, Proj &b) {
	Proj c = a;
	a = b;
	b = c;
}

//输入数据
void inputdata() {
	int x;
	for (int i = 1; i <= n; i++) {
		fscanf(fp, "%d", &x);
		for (int j = 1; j <= d; j++) {
			fscanf(fp, "%d", &obj[x][j]);
		}
	}
}

//正态分布生成函数
double normal_rand(double mu, double sigma) {
    const double pi = acos(-1.0);
    double x1, x2, y;
    do {
        x1 = 1 - (double)rand() / RAND_MAX;
        x2 = (double)rand() / RAND_MAX;
    } while (x1 <= 1e-10);
    y = sqrt(-2.0 * log(x1)) * cos(2.0 * pi * x2);
    return y * sigma + mu;
}

//生成随机向量
void generate_random_vectors() {
	m = 100;
	for (int i = 1; i <= m; i++) {
		double norm = 0;
		for (int j = 1; j <= d; j++) {
			random_vector[i][j] = normal_rand(0, 1);
			norm += sqr(random_vector[i][j]);
		}
		norm = sqrt(norm);
		for (int j = 1; j <= d; j++)
			random_vector[i][j] /= norm;
	}
}

//点积
double dot(int *a, double *b) {
	double c = 0;
	for (int i = 1; i <= d; i++)
		c += a[i] * b[i];
	return c;
}

//随机投影函数
void random_projection() {
	generate_random_vectors();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) {
			proj[j][i].id = i;
			proj[j][i].val = dot(obj[i], random_vector[j]);
		}
}

//求序列第K小函数
void select(Proj *S, int total, int K) {
	int i, j;
	Proj tmp;
	i = 1; j = total;
	swap(S[1], S[rand() % total + 1]);
	tmp = S[1];
	while (i < j) {
		while (i < j && S[j].val > tmp.val) j--;
		if (i < j) { S[i] = S[j]; i++; }
		while (i < j && S[i].val < tmp.val) i++;
		if (i < j) { S[j] = S[i]; j--; }
	}
	S[i] = tmp;
	if (i == K) return;
	if (K <= i - 1) select(S, i - 1, K);
	else select(S + i, total - i, K - i);
}

//随机生成分界点，并划分集合
int rand_pivot(Proj *S, int total) {
	int i, j;
	Proj tmp;
	i = 1; j = total;
	swap(S[1], S[rand() % total + 1]);
	tmp = S[1];
	while (i < j) {
		while (i < j && S[j].val > tmp.val) j--;
		if (i < j) { S[i] = S[j]; i++; }
		while (i < j && S[i].val < tmp.val) i++;
		if (i < j) { S[j] = S[i]; j--; }
	}
	S[i] = tmp;
	return i;
}

//求一维直线上的最近点对
CP closest_pair_line(Proj *S, int total, int type) {
	if (total == 1) return CP(0, 0, inf);
	if (total == 2) {
		if (S[1].val > S[2].val) swap(S[1], S[2]);
		return CP(S[1].id, S[2].id, S[2].val-S[1].val);
	}
	int pivot;
	CP cp1, cp2, cp3;
	if (type == 1) {
		pivot = total / 2;
		select(S, total, pivot);
	} else {
		do {
			pivot = rand_pivot(S, total);
		} while (pivot == total);
	}
	cp1 = closest_pair_line(S, pivot, type);
	cp2 = closest_pair_line(S + pivot, total - pivot, type);
	cp3 = CP(S[pivot].id, S[pivot + 1].id, S[pivot + 1].val - S[pivot].val);
	return cp_min(cp_min(cp1, cp2), cp3);
}

//计算欧氏距离
double calc_Euclidean_dist(int *a, int *b) {
	double sum = 0;
	for (int i = 1; i <= d; i++)
		sum += sqr(a[i] - b[i]);
	return sqrt(sum);
}

//求解最近点对
CP closest_pair(int type) {
	CP result, cur_cp;
	double cur_dist;
	random_projection();
	result = CP(0, 0, inf);
	for (int i = 1; i <= m; i++) {
		cur_cp = closest_pair_line(proj[i], n, type);
		cur_dist = calc_Euclidean_dist(obj[cur_cp.id1], obj[cur_cp.id2]);
		if (result.dist > cur_dist) {
			result = cur_cp;
			result.dist = cur_dist;
		}
	}
	return result;
}

//打印图片
void print_picture(int x) {
	int y = 0;
	for (int i = 1; i <= 28; i++) {
		for (int j = 1; j <= 28; j++) {
			if (obj[x][++y]) printf(" ");
			else printf("*");
		}
		printf("\n");
	}
}

//输出数据
void outputdata() {
	for (int i = 1; i <= 2; i++) {
		printf("------------");
		printf("Algorithm %d ", i + 2);
		if (i == 1) printf("(median)");
		else printf("(random pivot)");
		printf("------------\n");
		printf("The closest pair is %d and %d\n", ans[i].id1, ans[i].id2);
		printf("The minimum distance is %f\n", ans[i].dist);
		printf("Picture:\n");
		print_picture(ans[i].id1);
		printf("\n");
		print_picture(ans[i].id2);
		printf("\n");
		printf("Running time: %.3fs\n", elapsed_time[i]);
		printf("\n\n\n");
	}
}

int main(int argc, char *argv[]) {
	if (argc < 7) {
		printf("Error!\n");
		return 1;
	}
	fp = NULL;
	for (int i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], "-n") == 0) {
			n = atoi(argv[i + 1]);
		} else
		if (strcmp(argv[i], "-d") == 0) {
			d = atoi(argv[i + 1]);
		} else
		if (strcmp(argv[i], "-f") == 0) {
			fp = fopen(argv[i + 1], "r");
		} else {
			printf("Error!\n");
			return 1;
		}
	}
	if (fp == NULL) {
		printf("Error!\n");
		return 1;
	}
	
	srand((unsigned)time(NULL));
	inputdata();
	
	double start_time, end_time;
	for (int i = 1; i <= 2; i++) {
		start_time = clock();
		ans[i] = closest_pair(i);
		end_time = clock();
		elapsed_time[i] = (end_time - start_time) / CLOCKS_PER_SEC;
	}
	
	outputdata();
	fclose(fp);
	return 0;
}

