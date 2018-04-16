/*


*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;


class call_record
{
public:
	string firstname;
	string lastname;
	string cell_number;
	int relays;
	int call_length;
	double net_cost;
	double tax_rate;
	double call_tax;
	double total_cost;
};

class call_class
{
public:
	call_class();
	~call_class(); //de-allocates all memory allocate to call_DB by operator new.
	bool is_empty(); //inline implementation
	bool is_full();//inline implementation
	int search(const string key);//returns location if item in listl; otherwise return -1
	void add( ); //adds a call record to call_DB
	call_class & operator-(const string key); //removes an item from the list
	void double_size();
	void process();
	friend ostream & operator<<(ostream & out_to_file, call_class & Org); //prints all the elements in the
                                                        //list to the screen.
private:
	int count;
	int size;
	call_record *call_DB;
};




/************************************************************************************************************************************/
//Name: default constructor
//Precondition: Data file unread
//Postcondition: firstname, lastname, cell_number, relays, call_length read from file.
//Decription: Reads the data file of call information (cell number, relays and call length) into the dynamic array of call record,
//call_DB. If the count because equal to the size the function double_size is called and the memory allocated to call_DB is doubled.
/************************************************************************************************************************************/
call_class::call_class()
{
	count = 0;
	size = 10;
	call_DB = new call_record[size];
	ifstream in;
	in.open("callstats_data.txt");

	while (!in.eof()) {
		call_record record;
		in >> record.firstname
			 >> record.lastname
			 >> record.cell_number
			 >> record.relays
			 >> record.call_length;

		 if (count == size) {
			 double_size();
		 }
		 call_DB[count++] = record;
	}
}

/***********************************************************************************************************************************/
//Name: is_empty
//Precondition: Check empty class
//Postcondition: empty call_DB
//Decription: returns true if call_DB is empty
/**********************************************************************************************************************************/
bool call_class::is_empty()
{
	return count == 0;
}

/**********************************************************************************************************************************/
//Name: is_full
//Precondition: Check capacity.
//Postcondition: return false if call_DB empty.
//Decription: returns true if call_DB is full
/*********************************************************************************************************************************/
bool call_class::is_full()
{
	return count == size;
}

/**********************************************************************************************************************************/
//Name: search
//Precondition: check if there is a database.
//Postcondition: database located.
//Decription: locates key in call_DB if it is there; otherwise -1 is returned
/*********************************************************************************************************************************/
int call_class::search(const string key)
{
	for (int i = 0; i < count; i++){
		if (key == call_DB[i].cell_number){
			return i;
		}
	}
	return -1;
}

/*********************************************************************************************************************************/
//Name: add
//Precondition: prompts user for specified information.
//Postcondition: information from user entered.
//Decription: adds a call_record to call_DB; if call_DB is full, double_size is called to increase the size of call_DB. The user
//            is prompted to enter the firstname, lastname, cell number, relays and call length.
/********************************************************************************************************************************/
void call_class::add( )
{
	if (size == count) {
		double_size();
	}
	call_record record;
	cout << "Enter first name";
	cin >> record.firstname;
	cout << "Enter last name";
	cin >> record.lastname;
	cout << "Enter cell number";
	cin >> record.cell_number;
	cout << "Enter number of relays";
	cin >> record.relays;
	cout << "Enter call length";
	cin >> record.call_length;

	if (count == size) {
		double_size();
	}
	call_DB[count++] = record;
}

/********************************************************************************************************************************/
//Name: operator-
//Precondition:
//Postcondition:
//Decription: remove key from call_DB if it is there.
/*******************************************************************************************************************************/
call_class & call_class::operator-(const string key)
{
	for (int i = count - 1; i >= 0; i--) {
		if (call_DB[i].cell_number == key) {
			for (int j = i; j < count - 1; j++) {
				call_DB[j] = call_DB[j+1];
			}
			count--;
		}
	}
  return *this;
}

/******************************************************************************************************************************/
//Name: double_size
//Precondition: check capacity.
//Postcondition: capacity doubled.
//Decription: doubles the size (capacity) of call_DB
/******************************************************************************************************************************/
void call_class::double_size( )
{
	size *=2;
	call_record *temp = new call_record[size];

	for(int i=0; i<count; i++)
	{
		temp[i] = call_DB[i];
	}

	delete [ ] call_DB;
	call_DB = temp;
}


/******************************************************************************************************************************/
//Name: process
//Precondition: retrieves information from call_DB.
//Postcondition: calculations processed.
//Decription: calculate the net cost, tax rate, call tax and total cost for every call record in call_DB.
/*****************************************************************************************************************************/
void call_class::process()
{
	for (int i = 0; i < count; i++) {
		if (call_DB[i].relays <= 5) {
			call_DB[i].call_tax = 0.01;
		}	else if (call_DB[i].relays <= 11) {
			call_DB[i].call_tax = 0.03;
		}	else if (call_DB[i].relays <= 20) {
			call_DB[i].call_tax = 0.05;
		}	else if (call_DB[i].relays <= 50) {
			call_DB[i].call_tax = 0.08;
		} else if(call_DB[i].relays > 50) {
			call_DB[i].call_tax = 0.12;
		}

	call_DB[i].net_cost = call_DB[i].relays / 50.0 * 0.40 * call_DB[i].call_length;
	call_DB[i].tax_rate = call_DB[i].net_cost * call_DB[i].call_tax;
	call_DB[i].total_cost = call_DB[i].net_cost + call_DB[i].tax_rate;
	}
}


/****************************************************************************************************************************/
//Name: operator<<
//Precondition:
//Postcondition:
//Decription: Overloading operator<< as a friend function. Prints every field of every call_record in call_DB
//                   formatted to the screen and a file called "stats7_output.txt".
/***************************************************************************************************************************/
ostream & operator<<(ostream & out, call_class & Org)
{
	ofstream outFile;
	outFile.open("stats7_output.txt");



	if (outFile.fail()) // if problem opening file, print message and exit program
	{
		cout << "Output file did not open correctly" << endl;
		exit(1);
	}

	for(int i=0; i<Org.count; i++)
	{
		out << std::left << setw(30)<< Org.call_DB[i].firstname <<"  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].lastname <<"  "<< endl;
	  out << std::left << setw(30)<< Org.call_DB[i].relays <<"  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].cell_number <<"  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].call_length <<"  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].net_cost << "  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].tax_rate << "  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].call_tax << "  "<< endl;
		out << std::left << setw(30)<< Org.call_DB[i].total_cost << endl << endl;

		outFile << std::left << setw(30)<< Org.call_DB[i].firstname <<"  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].lastname <<"  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].relays <<"  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].cell_number <<"  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].call_length <<"  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].net_cost << "  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].tax_rate << "  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].call_tax << "  "<< endl;
		outFile << std::left << setw(30)<< Org.call_DB[i].total_cost << endl << endl;
	}

       //Put code to OPEN and CLOSE an ofstream and print to the file "stats7_output.txt".
	outFile.close();
  return out;
		 //must have this statement

}

/****************************************************************************************************************************/
//Name: destructor
//Precondition:
//Postcondition:
//Decription: de-allocates all memory allocated to call_DB.  This should be the last function to be called before the program
//            is exited.
/***************************************************************************************************************************/
call_class::~call_class()
{
}


//driver to test the functionality of your class.
int main()
{

	cout<<"TEST1: Testing the Default Constructor, is_full and double_size\n";
	call_class MyClass;
	MyClass.process();
	cout<<"Finish TEST1: testing the default constructor\n\n\n\n";

	cout<<"Test2: Testing add, double_size, process, and is_full() \n";
    //MyClass.add();
	cout<<"Finish TEST2\n\n\n\n";

	cout<<"Test3: Testing operator-, serach, and is_empty\n";
	//MyClass - "5617278899" - "9546321555" - "1234567890";
	cout<<"Finish TEST3\n\n\n\n";

	cout<<"Test4: Testing operator<<\n\n";
	cout<<MyClass<<endl;

	cout<<"Finish TEST4\n\n\n\n";

	cout<<"The destructor will be called automatically\n";

	return 0;
}
