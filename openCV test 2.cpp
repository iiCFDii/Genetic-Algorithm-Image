#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream> 		//	|
#include <string>			//	|
#include <limits>			//	| 	All the libraries needed for the program to work.
#include <iomanip>			//	|	
#include <windows.h>		//	|
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>
#include <ctime>
using namespace cv;
using namespace std;

class chromosome {		// this class defines my chromosome

private:
	int p1, p2,p3,p4;

	int fitness;
	vector <chromosome> ChromosomeV;		// this vector stores my chormosome population
	
	vector <chromosome> firstH;		// this vector stores the next population

public:

	chromosome()		// initilaizes class
	{
		fitness = 0;
		int p1 = 0;
		int p2 = 0;
		int p3 = 0;
		int p4 = 0;

	}

	chromosome(int dist1, int fitness1, int p11, int p22, int p33, int p44)        //  |    This allows the variables to be updated.
	{
		fitness = fitness1;
		p1 = p11;
		p2 = p22;
		p3 = p33;
		p4 = p44;
	}

	    //  |   these functions allow me to call and change the chomrosome defintions
	void setFit(int fitness1) { fitness = fitness1; }
	void setP1(int p11) { p1 = p11; }
	void setP2(int p22) { p2 = p22; }
	void setP3(int p33) { p3 = p33; }
	void setP4(int p44) { p4 = p44; }


	int getFit() { return fitness; }
	int getP1() { return p1; }		// these in specific are the coordinates of my lines, p2 is x1,p1 is y1, p4 is x2, p3 is y2
	int getP2() { return p2; }
	int getP3() { return p3; }
	int getP4() { return p4; }


	int randLine(int lineL) {		
		int num1 = 0;
		num1 = rand() % lineL;

		return num1;
	}

	int trueRandom(int range) {		// this functions returns a random number within the range of the new population size for th tournament selection
		int num = 0;
		if (range != 0) {
			num = rand() % range;
		}
		else {
			num = 0;
		}
		return num;

	}

	void createGene(int height, int width) {								// this functions creates a chromosome with all of its genes

		int coord1 = randLine(height);
		int coord2 = randLine(width);

		int coord3 = randLine(height);
		int coord4 = randLine(width);

		
		setP1(coord1);
		setP2(coord2);
		setP3(coord3);
		setP4(coord4);
		setFit(0);

	}



	

void create(int numMax, int height, int width) {		// this function creates the initial population of randomly generated line segments
		int temp = 0;
		chromosome gene;
		while (temp != numMax) {
			gene.createGene(height, width);
			ChromosomeV.push_back(gene);
			//cout << ChromosomeV[temp].getP2() << endl;
			temp++;
		}
	}


void init(int lineN, Mat img, int thickness) {		// this function draws out the inital population onto the input image
	for (int i = 0; i < lineN; i++) {
		Point p1(ChromosomeV[i].getP2(), ChromosomeV[i].getP1()), p2(ChromosomeV[i].getP4(), ChromosomeV[i].getP3());

		line(img, p1, p2, Scalar(255, 0, 0),
			thickness, LINE_4);

		
	}
}



template <typename T>												//	|	Since this function will be used by a two different kinds of classes, I made it arbitrary so it could be used by both
int accountPartiton(vector <T>& cityVec, int i, int k) {			//	| This is the account parition, which is used in the quick sort algorithm for the points vector
	int l = 0;														//	| 
	int h = 0;
	int  midpoint = 0;
	T pivot;
	T temp;
	bool done = false;
	midpoint = i + (k - i) / 2;
	pivot = cityVec[midpoint];
	l = i;
	h = k;
	while (!done) {

		while (cityVec[l].getFit() < pivot.getFit())
		{
			++l;
		}
		while (pivot.getFit() < cityVec[h].getFit())
		{
			--h;
		}
		if (l >= h)
		{
			done = true;
		}
		else {
			temp = cityVec[l];
			cityVec[l] = cityVec[h];
			cityVec[h] = temp;
			++l;
			--h;
		}
	}
	return h;
}


template <typename T>
void accQuickSort(vector <T>& cityVec, int l, int u) {		//	| This uses the accountPartition above to actually sort the vector from smallest fitness to largest fitness.
	int j = 0;
	if (l >= u)
	{
		return;
	}
	j = accountPartiton(cityVec, l, u);
	accQuickSort(cityVec, l, j);
	accQuickSort(cityVec, j + 1, u);
}



void fit(vector<vector<Point> >  lines, int counter1 ) {		// this functions calculates fitness of chromosomes
	float dist1 = 0;
	float dist2 = 0;
	float dist3 = 0;
	float med = 0;
	int thresh = 50;
	float midx; float midy;
	float midx1; float midy1;
	float temp1;

	

	for (int h = 0; h < ChromosomeV.size(); h++) {
		midx = (ChromosomeV[h].getP2() + ChromosomeV[h].getP4()) / 2;
		midy = (ChromosomeV[h].getP1() + ChromosomeV[h].getP3()) / 2;

		for (int i = 0; i < lines.size() ; i++)
		{
			midx1 = (lines[i][0].x + lines[i][lines[i].size() - 1].x) / 2;
			midy1 = (lines[i][0].y + lines[i][lines[i].size() - 1].y) / 2;

			temp1 = sqrt(pow((midx - midx1), 2) + pow((midy - midy1), 2));				// uses the midpoint of the lines and compares to midpoint of edge map
			
			
			for (int j = 0; j < lines[i].size(); j++) {
			//cout << lines[i][j].x << "x" << lines[i][j].y << " ";
				
				midx = (ChromosomeV[h].getP2() + ChromosomeV[h].getP4()) / 2;
				midy = (ChromosomeV[h].getP1() + ChromosomeV[h].getP3()) / 2;

				midx1 = (lines[i][0].x + lines[i][lines[i].size() -1].x) / 2;
				midy1 = (lines[i][0].y + lines[i][lines[i].size() -1].y) / 2;

			    dist3 = sqrt(pow((midx - midx1), 2) + pow((midy -midy1), 2));
				//dist1 = sqrt(pow((ChromosomeV[h].getP2() - lines[i][j].x), 2) + pow((ChromosomeV[h].getP1() - lines[i][j].y), 2));
				//dist2 = sqrt(pow((ChromosomeV[h].getP4() - lines[i][j].x), 2) + pow((ChromosomeV[h].getP3() - lines[i][j].y), 2));

			if (temp1 > dist3) {
				temp1 = dist3;
			}
			


		}
			
	}
		ChromosomeV[h].setFit(temp1);
		//cout << ChromosomeV[h].getFit() << endl;
}

	

	accQuickSort(ChromosomeV, 0, ChromosomeV.size() - 1);		// sorts based off of fitness


	float average = 0;
	float average1;
	for (int i = 0; i < ChromosomeV.size(); i++) {		// calcualtes average fitness
		average = average + ChromosomeV[i].getFit();
		//cout << ChromosomeV[i].getFit() << endl;
	}
	average1 = average / ChromosomeV.size();
	if (counter1 == 1 || counter1 % 10 == 0) {
		cout << "Displaying Fitness Stats of Generation " << counter1 << endl;
		cout << "Max: " << ChromosomeV.back().getFit() << endl;
		cout << "Min: " << ChromosomeV.front().getFit() << endl;
		cout << "Average: " << average1 << endl;
	}

}

int selection() {		// performs the selection function
	int max = 0;
	firstH.clear();		// clears old child population

			for (int i = 0; i < ChromosomeV.size() / 2; i++) {
				firstH.push_back(ChromosomeV[i]);		// takes number of chomrosomes from the sorted top and puts it into the new population vector
			}
			//cout << firstH.size() << endl;
			max = firstH.size();
			return max;
}

void crossover(int max) {		// performs the cross over functions
	int temp1, temp2, temp3;
	chromosome parent1;
	chromosome parent2;
	chromosome child;


	// performs k point selection
	for (int i = 0; i < max  ; i++) {
		temp1 = trueRandom(firstH.size() - 1);
		temp2 = trueRandom(firstH.size() - 1);
		//cout << temp1 << " " << temp2 << endl;
		parent1 = firstH[temp1];
		parent2 = firstH[temp2];

		child.setP1(parent1.getP1());
		child.setP2(parent1.getP2());
		child.setP3(parent2.getP3());
		child.setP4(parent2.getP4());
		firstH.push_back(child);
		
	}
	//cout << firstH.size() << endl;
}

void mutation(vector<vector<Point> >  lines) {		// this function perfrosm adaptive mutation
	float dist1 = 0;
	float dist2 = 0;
	float dist3 = 0;
	float med = 0;
	int thresh = 600;
	int midx; int midy;
	chromosome child;
	midx = (firstH[0].getP2() + firstH[0].getP4()) / 2;
	midy = (firstH[0].getP1() + firstH[0].getP3()) / 2;
	float temp1;
	firstH.clear();
	for (int i = 0; i < lines.size(); i++) {
		child.setP2(lines[i][0].x);
		child.setP1(lines[i][0].y);
		child.setP4(lines[i][lines[i].size() - 1].x);
		child.setP3(lines[i][lines[i].size() - 1].y);
		firstH.push_back(child);
	}


}

void transfer() {		// this transfers the new population into the original chromosome vector so that the process can start all over for the next generation
	ChromosomeV.clear();		// clears old generation
	for (int i = 0; i < firstH.size(); i++) {
		ChromosomeV.push_back(firstH[i]);		// fills new generation
	}

}

void init2(int lineN, Mat img, int thickness) {		// this functino displays the final generation on top of the input image
	for (int i = 0; i < ChromosomeV.size(); i++) {
		Point p1(ChromosomeV[i].getP2(), ChromosomeV[i].getP1()), p2(ChromosomeV[i].getP4(), ChromosomeV[i].getP3());
		//cout << p2 << " and " << p1 <<  endl;

		line(img, p1, p2, Scalar(255, 0, 0),
			thickness, LINE_4);
	}
	
	imwrite("myImageWithRect.jpg", img);
	namedWindow("Output", WINDOW_KEEPRATIO);
	imshow("Final Gen", img);
	resizeWindow("Output", 200, 200);

}

};






int main() {
    srand(time(NULL));
	int lineN = 500;
    int lineL;
    int width;
    int height;
	int thickness = 2;
	int counter = 1;
	int genNum = 10;
	int genNum1 = 10;
	int max = 0;
	chromosome get;
	vector<vector<Point> >  lines;
	vector <Vec4i> hierarchy;
	Mat img = imread("test.jpg");
	Mat img2 = imread("test.jpg");
	if (img.empty())
		    {
		        cout << "Could not open or find the image!\n" << endl;
		        return -1;
		    }

	Mat gray, edge, draw;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Canny(gray, edge, 50, 150, 3);

	edge.convertTo(draw, CV_8U);
	namedWindow("Output", WINDOW_KEEPRATIO);
	imwrite("contours.jpg",draw);
	imshow("image", draw);

	findContours(draw, lines, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);		// creates vector of coordinates of edge map
	cout << lines.size() << endl;
	
	/*for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < lines[i].size(); j++)
			cout << lines[i][j].x << "x" << lines[i][j].y << " ";
		cout << endl;
	}*/
	
    

    width = img.cols;
    height = img.rows;
	cout << width << endl;
	cout << height << endl;

	get.create(lineN, height, width);
	get.init(lineN, img, thickness);
	imshow("Gen 0", img);
	get.fit(lines,counter);

	 for (int i = 0; i < genNum1; i++) {
		max = get.selection();
		get.crossover(max);
		//get.mutation(lines);
		get.transfer();
		//get.init(lineN, img, thickness);
		counter++;
		get.fit(lines, counter);
		genNum--;
		//cout << "Gen Num: " << genNum << endl;
	}

	get.mutation(lines);
	get.transfer();
	get.init2(lineN, img2, thickness);
   
    waitKey(0);


	

	return 0;
}



