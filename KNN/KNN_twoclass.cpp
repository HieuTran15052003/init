#include <bits/stdc++.h>

using namespace std;

//xay dung kieu du lieu cho moi sample.
struct Data {
	int ID;
	float att[4];
	string label;
	float distance;
	int rank;
	string predictedLabels[10];
};

// dinh nghia lop: 0 = so am; 1 = so duong.
struct Labels {
	string labels[2];
	float count[2];
};


class KNNClassifier {
	public:
		struct Data TrainingData[242];
		struct Data TestData[242];
		struct Labels LabelData;
		string label_predicted;
		int features;
		int records;
		int testings;
		int labels;

		void init() {
			// dataset, labelSet, testSet;
			ifstream g("twoclass_labelDatas.dat");
			g >> labels;
			for (int i = 0; i < labels; i++) {
				g >> LabelData.labels[i];
			}
			g.close();
			ifstream f("twoclass_trainingDatas.dat");
			f >> records;
			f >> features;
			string temp1;
			string temp2;
			for (int i = 1; i <= records; i++) {
				TrainingData[i].ID = i;
				for (int j = 0; j < features; j++) {
					f >> TrainingData[i].att[j];
				}
				f >> temp1;
				f >> temp2;
				TrainingData[i].label = temp1 + temp2;
			}
			f.close();

			ifstream h("twoclass_testingDatas.dat");
			h >> testings;
			for (int i = 1; i <= testings; i++) {
				TestData[i].ID = i;
				for (int j = 0; j < features; j++) {
					h >> TestData[i].att[j];
				}
				h >> temp1;
				h >> temp2;
				TestData[i].label = temp1 + temp2;
			}
			h.close();
//			viewData(TrainingData, records);
//			viewData(TestData, testings);
		}

		void viewData(Data data[], int records) {
			cout << "so luong mau: " << records << endl;
			printf("  ID       f1         f2         f3          f4         class \n");
			for (int i = 1; i <= records; i++) {
				printf("%4d     ", data[i].ID);
				for (int j = 0; j < features; j++) {
					printf("%10.6f  ", data[i].att[j]);
				}
				cout << "    " << data[i].label;
				cout << endl;
			}
			printf("===================================================================== \n");
			printf("===================================================================== \n");
		}
		
		void distanceComputing(struct Data testData) {
			float sum = 0.0;
			for (int i = 1; i <= records; i++) {
				sum = 0.0;
				for (int j = 0; j < features; j++) {
					sum += pow(testData.att[j] - TrainingData[i].att[j], 2);
				}
				sum = sqrt(sum);
				TrainingData[i].distance = sum;
			}
		}
		
		void sort() {
			int minIndex;
			for (int i = 1; i < records; i++) {
				minIndex = i;
				for (int j = i + 1; j <= records; j++) {
					if (TrainingData[j].distance < TrainingData[minIndex].distance) {
						minIndex = j;
					}
				}
				swap(TrainingData[minIndex], TrainingData[i]);
			}
		}
		
		void KNN_classifier() {
			init();
			int max = 0;
			int index = 0;
			for (int i = 1; i <= testings; i++) {
				distanceComputing(TestData[i]);
				sort();
				for (int k = 1; k <= 9; k += 2) {
					for (int j = 0; j < labels; j++) {
						LabelData.count[j] = 0.0;
					}
					for (int z = 1; z <= k; z++) {
						for (int l = 0; l < labels; l++) {
							if (TrainingData[z].label == LabelData.labels[l]) {
								LabelData.count[l] += 1 / TrainingData[z].distance;
								break;
							}
						}
					}
					index = 0;
					max = LabelData.count[0];
					for (int l = 0; l < labels; l++) {
						if (LabelData.count[l] > max) {
							index = l;
						}
					}
					TestData[i].predictedLabels[k] = LabelData.labels[index];
				}
			}
			viewResult();
			confusionMaxtrix();
		}
		
		void viewResult() {
			printf(" ID | k = 1 | k = 3 | k = 5 | k = 7 | k = 9 | true Label| \n");
			for (int j = 1; j <= testings; j++) {
				printf("%3d |", j);
				for (int i = 1; i <= 9; i += 2) {
					printf("%5s  |", TestData[j].predictedLabels[i].c_str());
				}
				printf(" %5s     |",TestData[j].label.c_str());
				printf("\n--------------------------------------------------------| \n");
			}
		}
		
		void confusionMaxtrix() {
			int confusionMatrix[labels][labels];
			int GTIndex = 0;
			int PLIndex = 0;
			int corrects = 0;
			printf(" PL = Predicted Label \n");
			printf(" GT = Ground Truth \n");
			for (int k = 1; k <= 9; k += 2) {
				printf("with k = %d \n", k);
				for (int i = 0; i < labels; i++) {
					for (int j = 0; j < labels; j++) {
						confusionMatrix[i][j] = 0;	
					}
				}
				corrects = 0;
				for (int i = 1; i <= testings; i++) {
					for (int j = 0; j < labels; j++) {
						if (TestData[i].label == LabelData.labels[j]) {
							GTIndex = j;
						}
						if (TestData[i].predictedLabels[k] == LabelData.labels[j]) {
							PLIndex = j;
						}
					}
					confusionMatrix[GTIndex][PLIndex]++;
				}
				printf("----------|---------|---------| \n");
				printf("total =%3d|", testings);
				for (int i = 0; i < labels; i++) {
					printf(" PL = %s |", LabelData.labels[i].c_str());
				}
				printf("\n");
				printf("----------|---------|---------| \n");
				for (int i = 0; i < labels; i++) {
				printf(" GT = %s  |", LabelData.labels[i].c_str());
					for (int j = 0; j < labels; j++) {
						printf("    %2d   |", confusionMatrix[i][j]);	
					}
					printf("\n");
					printf("----------|---------|---------| \n");
				}
			for (int i = 0; i < labels; i++) {
				corrects += confusionMatrix[i][i];
			}
			printf("accuracy = %.2f \n \n", (float)corrects / testings);
			}
		}
};

main() {
	KNNClassifier a;
	a.KNN_classifier();
}
