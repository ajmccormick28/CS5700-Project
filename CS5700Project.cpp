/*
	**********************************************************************
	*                                                                    *
	*       Programmers:   Naresh Kshetri & Andrew McCormick             *
	*       Program cpp.:  CS5700Project                                 *
	*       Class:         CS 5700 (Computer Systems)                    *
	*       Instructor:    Dr. Ali Hurson                                *
	*       Due Date:      April 9th, 2019 Tuesday                       *
	*                                                                    *
	**********************************************************************
*/

/*
	*********************************************************
	*                                                       *
	*             Preprocessor Directive                    *
	*                                                       *
	*********************************************************
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

/*
	*********************************************************
	*                                                       *
	*                    Prototypes                         *
	*                                                       *
	*********************************************************
*/

void doProblem(string &, string &, int &, int &, char &, string &);
void isPositiveFunc(char &, int &, string &, string &, string &);

string hexToBinary(string);
string twosComplement(string);
string shiftZeros(string, int &, int &);
string shiftOnes(string, int &, int &);
string getA(string);
string addBits(string, string, int &);
string subDivisor(string, string, int &);
string addDivisor(string, string, int &);
string fixRemainder(string);
string addZeros(string, int);

int checkForInvalidInputs(string, string);

/*
	**********************************************************************
	*                                                                    *
	*                       Main Function                                *
	*                                                                    *
	**********************************************************************
*/

int main()
{
	char Enter = ' ',
		isPositive = '0',
		doAgain = 'y';

	string A = " ",
		divisor = " ",
		divend = " ",
		normDivisor = " ",
		adjDivend = " ",
		tempNormDivisor = " ",
		tempComDivisor = " ";

	int numDivisorAdjust = 0,
		delay = 0,
		totalNoOfShifts = 0,
		invalidInputs = 0;

	do
	{
		delay = 0;
		totalNoOfShifts = 0;
		numDivisorAdjust = 0;

		cout << "Input Dividend (please enter a period sign before dividend, for example .14): ";
		cin >> divend;

		cout << "\n";
		cout << "Input Divisor (please enter a period sign before divisor, for example .12) : ";
		cin >> divisor;

		//check and convert the hexadecimal input to binary by calling function hexToBinary
		divend = hexToBinary(divend);
		divisor = hexToBinary(divisor);

		//get rid of "." for calculations
		divend = divend.substr(1, divend.length());
		divisor = divisor.substr(1, divisor.length());

		//check if the dividend is twice the length of the divisor
		if (divend.length() < 2 * divisor.length())
		{
			divend = addZeros(divend, 2 * divisor.length() - divend.length()); // Call to function addZeros
		}

		cout << "The dividend length is: " << divend << " " << divend.length() << "\n";
		cout << "The divisor length is : " << divisor << " " << divisor.length() << "\n\n";

		//Check if the inputs are valid
		invalidInputs = checkForInvalidInputs(divend, divisor); // Call to function checkForInvalidInputs

		if (invalidInputs == -1)
		{
			cout << "You can not divide by zero\n";
		}

		else
		{
			if (invalidInputs == -2)
			{
				A = divend.substr(0, divend.length() / 2);

				while (A.compare(divisor) >= 0)
				{
					divend = divend.insert(0, "00");
					divisor = divisor.insert(0, "0");
					A = divend.substr(0, divend.length() / 2);
				}
			}

			normDivisor = divisor;

			//normalizing the Divisor
			for (int j = 0; j < divisor.length(); j++)
			{
				if (divisor[j] == '0')
				{
					//put the zero in the back
					normDivisor.push_back('0');

					//triming zero in the front
					normDivisor = normDivisor.substr(1);
					numDivisorAdjust++;
				}

				else
				{
					break;
				}
			}

			delay = delay + numDivisorAdjust * 3; // 3 Delta T for each shift required for normalizing the divisor

			cout << "Dividend = " << "." << divend.substr(0, divend.length() / 2) << "  " << divend.substr(divend.length() / 2, divend.length()) << "\n";
			cout << "Divisor  = " << "." << divisor << "\n\n";
			cout << "Normalized Divisor = " << "." << normDivisor << setw(15) << delay << " Delta T\n\n";

			// Converting the normalized Divisor to two's compliment
			tempNormDivisor = normDivisor;
			tempComDivisor = twosComplement(tempNormDivisor.insert(0, "0")); // Call to function twosComplement
			delay = delay + divisor.length(); // length of operand times Delta T

			cout << "Complement of Normalized Divisor = " << tempComDivisor.substr(0, 1) << "." << tempComDivisor.substr(1, tempComDivisor.length()) << "     " << delay << " Delta T\n\n\n";
			cout << "SRT Method" << "\n\n";
			cout << setw(20) << "." << divend.substr(0, divend.length() / 2) << "  " << divend.substr(divend.length() / 2, divend.length()) << "\n";

			// Adjusting the Dividend

			delay = delay + numDivisorAdjust * 3; // 3 Delta T for each shift required for adjusting the Dividend
			adjDivend = divend;

			for (int j = 0; j < divend.length(); j++)
			{
				if (divend[j] == '0' && numDivisorAdjust > 0)
				{
					adjDivend.push_back('*');
					adjDivend = adjDivend.substr(1);
					numDivisorAdjust--;
				}
			}

			divend = adjDivend;

			cout << "Adjusted Dividend: " << "." << adjDivend.substr(0, divend.length() / 2) << "  " << divend.substr(divend.length() / 2, divend.length()) <<
				setw(15) << delay << " Delta T\n";

			// Shifting the zeros
			adjDivend = shiftZeros(divend, delay, totalNoOfShifts); // Call to function shiftZeros

			divend = adjDivend;

			doProblem(divend, divisor, totalNoOfShifts, delay, isPositive, normDivisor); // Call to function doProblem

			isPositiveFunc(isPositive, delay, A, divend, normDivisor); // Call to function isPositiveFunc
		}

		cout << "Would you like to do another problem? Enter 'y' for yes and 'n' for no: ";
		cin >> doAgain;
		cout << "\n\n\n";
	}
	while (doAgain == 'y');

	cin.ignore(); // Clear out previous input 
	cin.get(Enter); // Hold the screen

	return 0;
}

/*
	**********************************************************************
	*                                                                    *
	*                     Function doProblem                             *
	*                                                                    *
	*          This function runs the main part of the SRT method.       *
	*                                                                    *
	**********************************************************************
*/

void doProblem(string &divend, string &divisor, int &totalNoOfShifts, int &delay, char &isPositive, string &normDivisor)
{
	string resultingDivisor = " ";

	while (totalNoOfShifts <= divisor.length())
	{
		if (isPositive == '0')
		{
			// Subtract the Divisor
			resultingDivisor = subDivisor(divend, normDivisor, delay); // Call to function subDivisor
		}

		else
		{
			resultingDivisor = addDivisor(divend, normDivisor, delay); // Call to function addDivisor
		}

		//Result is either positive or negative
		if (resultingDivisor.substr(0, 1) == "1")
		{
			cout << "Negative Result : " << resultingDivisor.substr(0, 1) << "." <<
				resultingDivisor.substr(1, resultingDivisor.length() / 2) << "  " << resultingDivisor.substr(resultingDivisor.length() / 2 + 1, resultingDivisor.length())
				<< setw(15) << delay << " Delta T\n";
		}

		else
		{
			cout << "Positive Result : " << resultingDivisor.substr(0, 1) << "." <<
				resultingDivisor.substr(1, resultingDivisor.length() / 2) << "  " << resultingDivisor.substr(resultingDivisor.length() / 2 + 1, resultingDivisor.length())
				<< setw(15) << delay << " Delta T\n";
		}

		isPositive = resultingDivisor[0];

		resultingDivisor = resultingDivisor.substr(1);

		if (isPositive == '0')
		{
			resultingDivisor.push_back('1');
			resultingDivisor = resultingDivisor.substr(1);

			delay = delay + 3; // 3 delta t delay for 1 shift

			totalNoOfShifts += 1;

			cout << "Shift AQ left, q0 <- 1:  " << "."
				<< resultingDivisor.substr(0, resultingDivisor.length() / 2) << "  " << resultingDivisor.substr(resultingDivisor.length() / 2, resultingDivisor.length())
				<< setw(15) << delay << " Delta T\n";

			// shift over zeros
			resultingDivisor = shiftZeros(resultingDivisor, delay, totalNoOfShifts); // Call to function shiftZeros
			if (totalNoOfShifts > resultingDivisor.length() / 2)
			{
				divend = resultingDivisor;
				break;
			}

			divend = resultingDivisor;
		}

		else if (isPositive == '1')
		{
			resultingDivisor.push_back('0');
			resultingDivisor = resultingDivisor.substr(1);

			delay = delay + 3; // 3 delta t delay for 1 shift

			totalNoOfShifts += 1;

			cout << "Shift Divisor left, q0 <- 0:  " << "1."
				<< resultingDivisor.substr(0, resultingDivisor.length() / 2) << "  " << resultingDivisor.substr(resultingDivisor.length() / 2, resultingDivisor.length())
				<< setw(15) << delay << " Delta T\n";

			resultingDivisor = shiftOnes(resultingDivisor, delay, totalNoOfShifts); // Call to function shiftOnes

			if (totalNoOfShifts > divisor.length())
			{
				divend = resultingDivisor;
				break;
			}

			divend = resultingDivisor;
		}
	}
}

/*
	**********************************************************************
	*                                                                    *
	*                     Function isPositiveFunc                        *
	*                                                                    *
	*          This function takes the isPositive flag to see if it      *
	*          to adjust the answer before outputing it to the user.     *
	*                                                                    *
	**********************************************************************
*/

void isPositiveFunc(char &isPositive, int &delay, string &A, string &divend, string &normDivisor)
{
	string fixA = " ",
		   finalQ = " ",
		   remainder = " ",
		   quotient = " ";

	if (isPositive == '1')
	{
		finalQ = divend.substr(divend.length() / 2);
		A = getA(divend.insert(0, "1")); // Call to function getA

		//shift right
		fixA = A;
		fixA = fixA.substr(0, A.length() - 1);

		//fixA = addBits(fixA, normDivisor, delay); // Call to function addBits
		delay += 3;

		cout << "Correct remainer by " << "1." << fixA << setw(15) << delay << " Delta T\n";
		cout << "Shifting A and adding B ." << normDivisor << "\n";

		fixA = addBits(fixA, normDivisor, delay); // Call to function addBits

		cout << "0." << fixA << "\n";
		cout << "Results" << "\n";
		cout << "Quotient: ." << finalQ << "\n";
		cout << "Remainder: ." << fixRemainder(fixA) << "\n"; // Call to function fixRemainder
		cout << "Delay: " << delay << " Delta T\n";
		cout << "\n\n\n";
	}

	// Print the output
	else
	{
		remainder = fixRemainder(divend.substr(0, divend.length() / 2)); // Call to fixRemainder
		quotient = divend.substr(divend.length() / 2, divend.length());

		cout << "\n\n";
		cout << "Results\n\n";
		cout << "Quotient:  ." << quotient << "\n";
		cout << "Remainder: ." << remainder << "\n";
		cout << "Delay:     " << delay << " Delta T\n";
		cout << "\n\n\n\n";
	}
}


/*
	**********************************************************************
	*                                                                    *
	*                     Function twosComplement                        *
	*                                                                    *
	*          Get the 2's complement number                             *
	*                                                                    *
	**********************************************************************
*/

string twosComplement(string tempDivisor)
{
	int tempLength = 0,
		carry = 0;

	char tempArr[30];

	tempLength = tempDivisor.length();

	for (int i = tempLength - 1; i >= 0; i--)
	{
		if (tempDivisor[i] == '0')
		{
			tempArr[i] = '1';
		}

		else
		{
			tempArr[i] = '0';
		}
	}

	for (int i = tempLength - 1; i >= 0; i--)
	{
		if (i == tempLength - 1)
		{
			if (tempArr[i] == '0')
			{
				tempArr[i] = '1';
			}

			else
			{
				tempArr[i] = '0';
				carry = 1;
			}
		}

		else
		{
			if (carry == 1 && tempArr[i] == '0')
			{
				tempArr[i] = '1';
				carry = 0;
			}

			else if (carry == 1 && tempArr[i] == '1')
			{
				tempArr[i] = '0';
				carry = 1;
			}
		}
	}

	tempArr[tempLength] = '\0';

	return tempArr;
}

/*
	**********************************************************************
	*                                                                    *
	*                      Function shiftZeros                           *
	*                                                                    *
	*       Shift the zeros in the dividend to left and add a zero.      *
	*                                                                    *
	**********************************************************************
*/

string shiftZeros(string divend, int &delay, int &totalNoOfShifts)
{
	string adjDivend = divend;

	int numOfShifts = 0;

	for (int i = 0; i < divend.length(); i++)
	{
		if (totalNoOfShifts > adjDivend.length() / 2)
		{
			break;
		}

		else if (divend[i] == '0')
		{
			adjDivend.push_back('0');
			adjDivend = adjDivend.substr(1);
			numOfShifts++;
			totalNoOfShifts++;
		}

		else
		{
			break;
		}
	}

	if (numOfShifts > 0)
	{
		delay = delay + 3 * numOfShifts; // each shift takes 3 * t

		cout << "Shift Over 0's:    ." << adjDivend.substr(0, adjDivend.length() / 2) << "  " <<
			adjDivend.substr(adjDivend.length() / 2, adjDivend.length()) << setw(15) << delay << " Delta T\n";
	}

	return adjDivend;
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function shiftOnes                              *
	*                                                                    *
	*       Shift the ones in the dividend to left and add a one.        *
	*                                                                    *
	**********************************************************************
*/

string shiftOnes(string divend, int &delay, int &totalNoOfShifts)
{
	string adjDivend = divend;

	int numOfShifts = 0;

	for (int i = 0; i < divend.length(); i++)
	{
		if (totalNoOfShifts > adjDivend.length() / 2)
		{
			break;
		}

		else if (divend[i] == '1')
		{
			adjDivend.push_back('1');
			adjDivend = adjDivend.substr(1);
			numOfShifts++;
			totalNoOfShifts++;
		}

		else
		{
			break;
		}
	}

	if (numOfShifts > 0)
	{
		delay = delay + 3 * numOfShifts; // each shift takes 3 Delta T
		cout << "Shift Over 1's:    1." << adjDivend.substr(0, adjDivend.length() / 2) << "  " <<
			adjDivend.substr(adjDivend.length() / 2, adjDivend.length()) << setw(15) << delay << " Delta T\n";
	}

	return adjDivend;
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function addBits                                *
	*                                                                    *
	*     Adding the two equal numbers together.                         *
	*                                                                    *
	**********************************************************************
*/

string addBits(string first, string second, int &delay)
{
	string result;  // For storing the sum bits

	int carry = 0,
		firstBit = 0,
		secondBit = 0,
		sum = 0;

	// Add the bits one by one
	for (int i = first.length() - 1; i >= 0; i--)
	{
		if (first.at(i) == '*' || second.at(i) == '*')
		{
			result += '*';
			carry = 0;
		}

		else
		{
			firstBit = first.at(i) - '0';
			secondBit = second.at(i) - '0';

			// boolean expression for sum of 3 bits
			sum = (firstBit ^ secondBit ^ carry) + '0';

			result = (char)sum + result;

			// boolean expression for 3-bit addition
			carry = (firstBit & secondBit) | (secondBit & carry) | (firstBit & carry);
		}
	}

	delay = delay + ((first.length() / 2 - 1) * 2 + 6);

	return result; // Returning a string value 
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function subDivisor                             *
	*                                                                    *
	*      Subtract the normalized divisor from A part of dividend       *
	*                                                                    *
	**********************************************************************
*/

string subDivisor(string divend, string divisor, int &delay)
{
	string A = " ",
		comDivisor = " ";

	A = getA(divend.insert(0, "0")); // Call to getA
	comDivisor = twosComplement(divisor.insert(0, "0"));

	cout << "Subtract B:       " << comDivisor.substr(0, 1) << "." << comDivisor.substr(1, comDivisor.length()) << "\n";
	cout << "\n\n";

	A = addBits(A, comDivisor, delay); // Call to addBits
	A = A.append(divend.substr(divend.length() / 2 + 1, divend.length()));

	return A; // Returning a string value
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function addDivisor                             *
	*                                                                    *
	*         Add the normalized divisor from A part of dividend         *
	*                                                                    *
	**********************************************************************
*/

string addDivisor(string divend, string divisor, int &delay)
{
	string A = " ";

	A = getA(divend.insert(0, "1")); // Call to function getA
	A = addBits(A, divisor.insert(0, "0"), delay); // Call to function addBits

	cout << "Add B:       ." << divisor.substr(1, divisor.length()) << "\n";
	cout << " ---------" << "\n";

	A = A.append(divend.substr(divend.length() / 2 + 1, divend.length()));

	return A; // Returning a string value
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function fixRemainder                           *
	*                                                                    *
	*    Create proper remainder by taking all the bits after the        *
	*    decimal in A until the first *.  Zeros are then added infront.  *
	*                                                                    *
	**********************************************************************
*/

string fixRemainder(string remainder)
{
	int count = 0,
		len = 0;

	string properRem = " ",
		addZeros = "";

	len = 2 * remainder.length();

	for (int i = 0; i < len / 2; i++)
	{
		if (remainder[i] == '*')
		{
			break;
		}

		else
		{
			properRem = properRem + remainder[i];
		}

		count++;
	}

	for (int i = count; i < len; i++)
	{
		addZeros.append("0");
	}

	properRem = addZeros.append(properRem);

	return properRem; // Returning a string value
}

/*
	**********************************************************************
	*                                                                    *
	*              Function checkForInvalidInputs                        *
	*                                                                    *
	*      Check for invaild inputs. Looking for when the divisor is     *
	*      zero or when the bits of the dividend are greater or equal    *
	*      to the total bits of the divisor.                             *
	*                                                                    *
	**********************************************************************
*/

int checkForInvalidInputs(string divend, string divisor)
{
	bool allZeros = true;

	string A = " ";

	// Enter if the divisor is Zero
	for (int i = 0; i < divisor.length(); i++)
	{
		if (divisor[i] == '1')
		{
			allZeros = false;
			break;
		}
	}

	if (allZeros == true)
	{
		return -1; //Return an int value
	}

	A = divend.substr(0, divend.length() / 2);

	if (A.compare(divisor) >= 0)
	{
		return -2; //Return an int value
	}

	return 0; //Return an int value
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function hexToBinary                            *
	*                                                                    *
	*      Checking to see if the number is hexadecimal or binary. If    *
	*      the number is hexadecimal, it is converted into binary.       *
	*                                                                    *
	**********************************************************************
*/

string hexToBinary(string hexaDecimal)
{
	string binaryNum = ".";

	//already a binary number
	if (hexaDecimal.at(hexaDecimal.length() - 2) == 'y')
	{
		return hexaDecimal.substr(0, hexaDecimal.length() - 8);
	}

	for (int i = 1; i < hexaDecimal.length(); i++)
	{
		switch (hexaDecimal[i])
		{
		case '0': 
			binaryNum = binaryNum.append("0000");
			break;

		case '1': 
			binaryNum = binaryNum.append("0001");
			break;

		case '2': 
			binaryNum = binaryNum.append("0010");
			break;

		case '3': 
			binaryNum = binaryNum.append("0011");
			break;

		case '4': 
			binaryNum = binaryNum.append("0100");
			break;

		case '5': 
			binaryNum = binaryNum.append("0101");
			break;

		case '6': 
			binaryNum = binaryNum.append("0110");
			break;

		case '7': 
			binaryNum = binaryNum.append("0111");
			break;

		case '8': 
			binaryNum = binaryNum.append("1000");
			break;

		case '9': 
			binaryNum = binaryNum.append("1001");
			break;

		case 'A': 
			binaryNum = binaryNum.append("1010");
			break;

		case 'B': 
			binaryNum = binaryNum.append("1011");
			break;

		case 'C': 
			binaryNum = binaryNum.append("1100");
			break;

		case 'D': 
			binaryNum = binaryNum.append("1101");
			break;

		case 'E': 
			binaryNum = binaryNum.append("1110");
			break;

		case 'F': 
			binaryNum = binaryNum.append("1111");
			break;

		case 'a': 
			binaryNum = binaryNum.append("1010");
			break;

		case 'b': 
			binaryNum = binaryNum.append("1011");
			break;

		case 'c': 
			binaryNum = binaryNum.append("1100");
			break;

		case 'd': 
			binaryNum = binaryNum.append("1101");
			break;

		case 'e': 
			binaryNum = binaryNum.append("1110");
			break;

		case 'f': 
			binaryNum = binaryNum.append("1111");
			break;

		default: 
			cout << "\nThis digit is not valid " << hexaDecimal[i] << "\n";
			return 0;
		}
	}

	return binaryNum; // Return a string value
}

/*
	**********************************************************************
	*                                                                    *
	*                    Function addZeros                               *
	*                                                                    *
	*      Zeros are added after decimal in the dividend if the total    *
	*      bits in dividend is not TWICE the total bits in the divisor   *
	*                                                                    *
	**********************************************************************
*/

string addZeros(string str, int length)
{
	string updatedString = "";

	for (int i = 0; i < length; i++)
	{
		updatedString = updatedString.insert(0, "0");
	}

	updatedString = updatedString.append(str);

	return updatedString; // Return string value
}

/*
	**********************************************************************
	*                                                                    *
	*                      Function getA                                 *
	*                                                                    *
	**********************************************************************
*/

string getA(string divend)
{
	return divend.substr(0, divend.length() / 2 + 1); // Return a string value 
}
