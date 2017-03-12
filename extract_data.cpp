#include <cstdio>

int main() {
	FILE* inputFile = fopen("train-images.idx3-ubyte", "rb");
	if (!inputFile) {
		printf("file NOT found. Exiting...\n");
		return 0;
	} else {
		printf("Reading train-images.idx3-ubyte...\n");
	}

	FILE* outputFile = fopen("Mnist.ds", "w");

	fseek(inputFile, 16, SEEK_SET);

	unsigned char *buffer = new unsigned char[1];
	for (int i = 1; i <= 60000; i++) {
		fprintf(outputFile, "%d ", i);
		for (int j = 0; j < 783; j++) {
			fread(buffer, sizeof(unsigned char), 1, inputFile);
			fprintf(outputFile, "%d ", (unsigned int)buffer[0]);
		}
		fread(buffer, sizeof(unsigned char), 1, inputFile);
		fprintf(outputFile, "%d\n", (unsigned int)buffer[0]);
	}

	fclose(inputFile);
	fclose(outputFile);
	delete[] buffer;
}
