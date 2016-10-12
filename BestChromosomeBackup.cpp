#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>
#include<vector>
#include<tuple>
#include<ctime>
#include<algorithm>
#include<conio.h>
using namespace std;

int MAX_LENGTH = 24;

//Randomizer Functions
int Random(int population){
	int roll1 = rand() % population;
	int roll2 = rand() % population;
	
	return min(roll1, roll2);
}

int Random(){
	int roll1 = rand() % MAX_LENGTH;
	int roll2 = rand() % MAX_LENGTH;
	
	return min(roll1, roll2);
}

//Validating the initial Chromosome
bool ValidateChromosome(string binary){
	if (binary.length() != 24)
		return false;
	
	for(int i = 0; i < binary.length(); i++){
		if(binary[i] != '0' && binary[i] != '1')
			return false;
	}
	
	return true;
}

//Functions for Generating the chromosomes
string RandomChromosome(){
	string newChromosome = "";
	for(int i = 0; i < 24; i++){
		newChromosome = newChromosome + to_string(rand() % 2);
	}
	return newChromosome;
}

vector<string> CreatePopulation(int sizeOfPopulation){
	vector<string> population;
	
	for(int i = 0; i < sizeOfPopulation; i++){
		population.push_back(RandomChromosome());
	}
	
	return population;
}


//Conversion Functions
int BinaryToDecimal(string binary){
	return stoi(binary,nullptr,2);
}

string IntegerToString(int number){
	return to_string(number);
}

tuple<int, int, int> MakeRGB(string binary, bool print){
	tuple <int, int, int> RGB;
	int red = BinaryToDecimal(binary.substr(0,8));
	int green = BinaryToDecimal(binary.substr(8,8));
	int blue = BinaryToDecimal(binary.substr(16,8));
	get<0>(RGB) = red;
	get<1>(RGB) = green;
	get<2>(RGB) = blue;
	if (print == true){
		cout<<"Color of chromosome entered is - Red: "<<red<<"    Green: "<<green<<"    Blue: "<<blue<<endl;
	}
	return RGB;
}


//Distance Calculation Function
float ChromosomeDistance(string chromo1, string chromo2){
	tuple <int, int, int> colorOne;
	tuple <int, int, int> colorTwo;
	float distance; 
	
	colorOne = MakeRGB(chromo1, false);
	colorTwo = MakeRGB(chromo2, false);
	distance = sqrt((pow((get<0>(colorOne) - get<0>(colorTwo)), 2)) + (pow((get<1>(colorOne) - get<1>(colorTwo)), 2)) + (pow((get<2>(colorOne) - get<2>(colorTwo)), 2)));
	
	return distance;	
}


//Genetic Helper Functions

vector<string> Selection(vector<string> population, int noOfSelection){
	vector<string> selection;
	
	for(int i = 0; i < noOfSelection; i++){
		selection.push_back(population.at(Random(population.size())));
		cout<<" ";
	}

	return selection;
}

string Mutate(string chromosome, int index){
	string solution = chromosome;
	char bit;
	
	bit = chromosome[index];
	if (bit == '0')
		bit = '1';
	else
		bit = '0';
	
	solution[index] = bit;
	return solution;
}

vector<string> Mutation(vector<string> population, int noOfMutation){
	vector<string> mutation;
	string temp;
	string mutated;
	int index;
	for(int i = 0; i < noOfMutation; i++){
		temp = population.at(Random(population.size()));
		index = Random();
		mutated = Mutate(temp, index);
		mutation.push_back(mutated);
		cout<<" ";
	}
	return mutation;
}


vector<string> Cross(string one, string two, int index){
	int lengthOne = one.length();
	int lengthTwo = two.length();
	vector<string> crossover;
	string temp1 = one.substr(0,index);
	temp1 = temp1 + two.substr(index, (lengthTwo-index));
	string temp2 = two.substr(0, index);
	temp2 = temp2 + one.substr(index, (lengthOne-index));
	crossover.push_back(temp1);
	crossover.push_back(temp2);
	return crossover;
}

vector<string> Crossover(vector<string> population, int noOfCrossover){
	vector<string> crossover(2*noOfCrossover);
	string chromosomeOne;
	string chromosomeTwo;
	int index;
	vector<string> crosses;
	for(int i = 0; i < noOfCrossover; i++){
		chromosomeOne = population.at(Random(population.size()));
		chromosomeTwo = population.at(Random(population.size()));
		index = rand() % MAX_LENGTH;
		crosses = Cross(chromosomeOne, chromosomeTwo, index);
		crossover.at(i*2) = crosses[0];
		crossover.at((i*2)+1) = crosses[1];
		cout<<" ";
	}
	return crossover;
}


//Functions for sorting

struct sorter{
	bool operator() (tuple<string, int> &lhs, tuple<string, int> &rhs){
		return (get<1>(lhs) < get<1>(rhs));
	}
} ;

vector<string> SortPopulation(vector<string> population, string goal){
	tuple<string, int> tempList;
	int distance;
	vector<tuple<string, int>> temp;
	vector<string> sorted;
	for (int i = 0; i < population.size(); i++){
		distance = ChromosomeDistance(population.at(i), goal);
		get<0>(tempList) = population.at(i);
		get<1>(tempList) = distance;
		temp.push_back(tempList);	
	}
	sort(temp.begin(), temp.end(), sorter());
	
	for(int j = 0; j < temp.size(); j++){
		sorted.push_back(get<0>(temp.at(j)));
	}
	return sorted;
}

//Helper Functions for the function that does everything
vector<string> SelectionPushBack(vector<string> population, vector<string> newPopulation, int noOfSelection){
	vector<string> temp;
	temp = Selection(population, noOfSelection);
	for(int i = 0; i < temp.size(); i++){
		newPopulation.push_back(temp.at(i));
	}
	return newPopulation;
}

vector<string> MutationPushBack(vector<string> population, vector<string> newPopulation, int noOfMutation){
	vector<string> temp;
	temp = Mutation(population, noOfMutation);
	for(int i = 0; i < temp.size(); i++){
		newPopulation.push_back(temp.at(i));
	}
	return newPopulation;
}

vector<string> NewBloodPushBack(vector<string> population, vector<string> newPopulation, int noOfNewBlood){
	for(int i = 0; i < noOfNewBlood; i++){
		newPopulation.push_back(RandomChromosome());
	}
	return newPopulation;
}

vector<string> CrossoverPushBack(vector<string> population, vector<string> newPopulation, int noOfCrossover){
	vector<string> temp;
	temp = Crossover(population, noOfCrossover);
	for(int i = 0; i < temp.size(); i++){
		newPopulation.push_back(temp.at(i));
	}
	return newPopulation;
}

//Function that pretty much does everything
void BestChromosomeFinder(string goalChromosome, int noOfChromosome, int noOfSelection, int noOfMutation,
							int noOfNewBlood, int noOfCrossover, int noOfGenerations){
	int generation = 0;
	int bestGeneration = 0;
	
	vector<string> population = CreatePopulation(noOfChromosome);
	population = SortPopulation(population, goalChromosome);
	
	string bestChromosome = population.at(0);
	float bestDistance = ChromosomeDistance(bestChromosome, goalChromosome);
	
	cout<<"For every generation, the best chromosome and the distance is listed below";
	cout<<endl<<"Generation\tBest Chromosome\t\t\t\tDistance";
	cout<<endl<<bestGeneration<<"\t\t"<<bestChromosome<<"\t\t"<<bestDistance;
	
	while(generation < noOfGenerations){
		generation++;
		vector<string> newPopulation;
		string chromosome;
		float distance;
		newPopulation = SelectionPushBack(population, newPopulation, noOfSelection);
		newPopulation = MutationPushBack(population, newPopulation, noOfMutation);
		newPopulation = NewBloodPushBack(population, newPopulation, noOfNewBlood);
		newPopulation = CrossoverPushBack(population, newPopulation, noOfCrossover);
		
		newPopulation = SortPopulation(newPopulation, goalChromosome);
		chromosome = newPopulation.at(0);
		distance = ChromosomeDistance(chromosome, goalChromosome);
		if (distance < bestDistance){
			bestChromosome = chromosome;
			bestDistance = distance;
			bestGeneration = generation;
		}
		cout<<endl<<generation<<"\t\t"<<bestChromosome<<"\t\t"<<bestDistance;
	}
	
	cout<<endl<<"The best chromosome details are: "<<endl;
	cout<<"Generation: "<<bestGeneration<<endl;
	cout<<"Chromosome: "<<bestChromosome<<endl;
	cout<<"Distance:   "<<bestDistance<<endl;
	
}


int main() {
	//Seeding the random function to get random integers everytime function is called.
	srand((int)time(0));
	//Define variables
	string goalChromosome;
	int noOfChromosome;
	int noOfSelection;
	int noOfMutation;
	int noOfNewBlood;
	int noOfCrossover;
	int noOfGenerations;
	char userChoice = 'Y';
	
	while(userChoice != 'n' && userChoice != 'N'){
	
	//Asking for the input
	cout<<"Enter the 24 bit string: ";
	cin>>goalChromosome;
	while(ValidateChromosome(goalChromosome)){
	
	//Convert to RGB
	bool print = true;
	MakeRGB(goalChromosome, print);
	
	cout<<endl<<"Enter the number of chromosomes in the population: ";
	cin>>noOfChromosome;
	cout<<endl<<"How many chromosomes undergo selection: ";
	cin>>noOfSelection;
	cout<<endl<<"How many chromosomes undergo mutation: ";
	cin>>noOfMutation;
	cout<<endl<<"How many new chromosomes in the population: ";
	cin>>noOfNewBlood;
	cout<<endl<<"How many chromosomes undergo crossover: ";
	cin>>noOfCrossover;
	
	//Check for the right number of inputs
	bool input = (noOfChromosome == (noOfSelection + noOfMutation + noOfNewBlood + 2*noOfCrossover));
	if (input == false){
		cout<<"Wrong number of chromosome count!!";
		break;
	}
	
	cout<<endl<<"How many generations: ";
	cin>>noOfGenerations;
	cout<<endl;
	
	//Function that does everything is called
	BestChromosomeFinder(goalChromosome, noOfChromosome, noOfSelection, noOfMutation, noOfNewBlood, noOfCrossover, noOfGenerations);
	
	break;
	}
	
	if(ValidateChromosome(goalChromosome) == false)
		cout<<"Bad Format of target chromosome!!";
	
	cout<<endl<<"Hit an key to continue or press 'N' to exit: ";
	userChoice = getch();
	if (userChoice == 'n' && userChoice == 'N'){
		system("CLS");
		
	}
	else
		system("CLS");
		//cout<<endl<<"Bye bye ={|)"; 
	}
	return 0;
}
