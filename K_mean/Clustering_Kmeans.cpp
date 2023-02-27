#include <bits/stdc++.h>

using namespace std;

struct Point {
	int ID;
	float att[2];// 2 thuoc tinh
	float distance[5];
	int clustering;
};

struct center {
	int ID;
	float att[2];
	int points;
	int pointIDs[10];
};

class KMeansClustering {
	public:
		struct Point trainingPoint[10];
		struct center clusterCenters[10];
		struct center results[10];
		int k_clusters;
		int records;//tong mau
		int features;//tong thuoc tinh
		bool updated;

		KMeansClustering() {
			init();
		}

		void init() {
			ifstream f("Clustering_point_datas.txt");
			f >> records;
			f >> features;
			for (int i = 0; i < records; i++) {
				trainingPoint[i].ID = i + 1;
				for (int j = 0; j < features; j++) {
					f >> trainingPoint[i].att[j];
				}
			}
			f.close();
			ifstream g("Clustering_center_datas.txt");
			g >> k_clusters;
			for (int i = 0; i < k_clusters; i++) {
				clusterCenters[i].ID = i + 1;
				clusterCenters[i].points = 0;
				for (int j = 0; j < features; j++) {
					g >> clusterCenters[i].att[j];
				}
			}
			g.close();
			updated = true;
			viewPoint(trainingPoint, records);
		}

		void viewPoint(Point data[], int records) {
			printf("so luong mau: %2d \n", records);
			printf("  ID");
			for (int i = 0; i < features; i++) {
				printf("    f%d", i + 1);
			}
			printf("\n");
			for (int i = 0; i < records; i++) {
				printf("%4d", data[i].ID);
				for (int j = 0; j < features; j++) {
					printf("   %3.0f", data[i].att[j]);
				}
				printf("\n");
			}
			printf("====================== \n");
			printf("====================== \n");
		}

		void clustering() {
			viewClusterDimensions();
			kMeans();
			viewResult();
			while(updated) {
				saveResult();
				UpdateClusters();
				viewClusterDimensions();
				kMeans();
				viewResult();
				compareResult();
			}
			conclude();
		}

		void kMeans() {
			int clusterIndex = 0;
			int point = 0;
			for (int j = 0; j < k_clusters; j++) {
				clusterCenters[j].points = 0;
			}
			for (int i = 0; i < records; i++) {
				clusterIndex = 0;
				for (int j = 0; j < k_clusters; j++) {
					trainingPoint[i].distance[j] = distanceComputing(clusterCenters[j], trainingPoint[i]);
					if (trainingPoint[i].distance[j] < trainingPoint[i].distance[clusterIndex]) {
						clusterIndex = j;
					}
				}
				trainingPoint[i].clustering = clusterIndex;
//				point = clusterCenters[clusterIndex].points;
				clusterCenters[clusterIndex].pointIDs[point] = trainingPoint[i].ID;
				clusterCenters[clusterIndex].points++;
			}
		}

		float distanceComputing(center clusterCenter, Point point) {
			float sum = 0.0;
			for (int j = 0; j < features; j++) {
				sum += pow(clusterCenter.att[j] - point.att[j], 2);
			}
			sum = sqrt(sum);
			return sum;
		}

		void saveResult() {
			for (int i = 0; i < k_clusters; i++) {
				results[i].points = clusterCenters[i].points;
				for (int k = 0; k < clusterCenters[i].points; k++) {
					results[i].pointIDs[k] = clusterCenters[i].pointIDs[k];
				}
			}
		}

		bool compareResult() {
			for (int i = 0; i < k_clusters; i++) {
				if (results[i].points != clusterCenters[i].points) {
					updated = true;
					return updated;
				}
			}
			for (int i = 0; i < k_clusters; i++) {
				for (int k = 0; k < clusterCenters[i].points; k++) {
					if (results[i].pointIDs[k] != clusterCenters[i].pointIDs[k]) {
						updated = true;
						return updated;
					}
				}
			}
			updated = false;
			return updated;
		}
/////////////////////////////////////////////////////////////////////////
		void UpdateClusters() {
			int points = 0;
			for (int i = 0; i < k_clusters; i++) {
				for (int k = 0; k < features; k++) {
					clusterCenters[i].att[k] = 0;
				}
				points = clusterCenters[i].points;
				for (int j = 0; j < points; j++) {
					for (int k = 0; k < features; k++) {
						clusterCenters[i].att[k] += trainingPoint[clusterCenters[i].pointIDs[j] - 1].att[k] / points;
					}
				}
			}
		}

		void viewResult() {
			printf("      ");
			for (int i = 1; i <= records; i++) {
				printf("A%d     ", i);
			}
			printf("\n");
			for (int i = 0; i < k_clusters; i++) {
				printf("C%d", i);
				for (int j = 0; j < records; j++) {
					printf(" %6.2f", trainingPoint[j].distance[i]);
				}
				printf("\n");
			}

			for (int i = 0; i < k_clusters; i++) {
				printf("cluster %1d: \n", i);
				for (int j = 0; j < clusterCenters[i].points; j++) {
					printf("point %2d ||", clusterCenters[i].pointIDs[j]);
				}
				printf("\n---------- \n");
			}
			printf("=========================================================== \n");
		}

		void viewClusterDimensions() {
			printf("Cluster   f1    f2 \n");
			for (int i = 0; i < k_clusters; i++) {
				printf(" %2d     ", i);
				for (int j = 0; j < features; j++) {
					printf(" %2.2f ", clusterCenters[i].att[j]);
				}
				printf("\n");
			}
		}

		void conclude() {
			printf("\n============\n");
			printf("Conclusion:\n");
			printf("============\n");
			printf("               Clusters:%2d \n", k_clusters);
			printf("             ================ \n");
				for (int i = 0; i < k_clusters; i++) {
				printf("cluster %1d: \n", i);
				for (int j = 0; j < clusterCenters[i].points; j++) {
					printf("point %2d ||", clusterCenters[i].pointIDs[j]);
				}
				printf("\n---------- \n");
			}
		}
};

main() {
	KMeansClustering A;
	A.clustering();
}
