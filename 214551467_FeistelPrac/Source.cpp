#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <bitset>

using namespace std;

void convertToArrayStream(int arr[], string txt)		//Function used to convert a string into the ASCII version
{														//Stores result in an array of integers
	string stream;
	for (int i = 0; i < txt.length(); i++)
		stream += std::bitset<8>((int)(txt.at(i))).to_string();	//Converts string to ASCII stream of bits

	for (int i = 0; i < (txt.length() * 8); i++)
		arr[i] = stream[i] - '0';				//Stores either a 1 or 0 in each element of the array
}

int binaryToBase10(int n)		//Function used to convert a binary number to base 10 number
{
	int ans = 0;
	for (int i = 0; n > 0; i++)
	{
		if (n % 10 == 1)
			ans += (1 << i);
		n /= 10;
	}
	return ans;
}

string asciiToText(string txt)		//Converts ASCII to text and returns a string
{
	string ans;
	for (int j = 0; j < txt.length(); j = j + 8)
	{
		char x = binaryToBase10(atoi((txt.substr(j, 8)).c_str()));
		ans += x;
	}
	return ans;
}

int main()
{
	int rounds;
	string usertext = "";
	
	cout << "Please enter text : ";
	getline(cin, usertext);								//Gets a sentence from the user

	for (int i = 0; usertext[i] != '\0'; i++)			//Removes whitespaces from text
		if (usertext[i] == ' ')
			for (int j = i; usertext[j] != '\0'; j++)
				usertext[j] = usertext[j + 1];
	
	if ((usertext.length() % 2) != 0)					//Ensures length of text is an even number
		usertext += "X";
	
	cout << "Enter the number of rounds : ";
	cin >> rounds;										//Gets number of rounds from user

	bool enc;
	cout << "\n\nWould you like to Encrypt the text or Decrypt it ? (1 = Encrypt, 0 = Decrypt) ";
	cout << "\n** For Encryption, the Keys will be randomly generated.";
	cout << "\n** For Decryption, you will have to enter the keys manually in the form of text.\n";
	cin >> enc;

	int **keys = new int*[rounds];						//Dynamically create 2D array to store keys
	for (int i = 0; i < rounds; i++)
		keys[i] = new int[usertext.length() * 4];

	if (enc)
	{	
		srand(time(NULL));								//Randomly generates keys for encryption operation
		for (int i = 0; i < rounds; i++)
			for (int j = 0; j < (usertext.length() * 4); j++)
				keys[i][j] = rint(rand() % 2);
	}
	else
	{
		string inpt;
		for (int i = 0; i < rounds; i++)				//Gets keys from user as text
		{
			cout << "Enter key used in round " << (i + 1) << " of Encryption : ";
			cin >> inpt;
			convertToArrayStream(keys[i], inpt);		//Converts the text ASCII and stores in array
		}
	}

	string lusertxt = usertext.substr(0, (usertext.length() / 2));		//Splits user text into two halves
	string rusertxt = usertext.substr(usertext.length() / 2);
	
	int *leftASCII = new int[lusertxt.length() * 8];		//Array used to store ASCII form of left half of the user text
	int *rightASCII = new int[rusertxt.length() * 8];		//Array used to store ASCII form of right half of the user text
	int *newR = new int[rusertxt.length() * 8];

	convertToArrayStream(leftASCII, lusertxt);		//Converts string to ASCII and stores in respective array
	convertToArrayStream(rightASCII, rusertxt);

	for (int r = 0; r < rounds; r++)
	{
		if (enc)		//Performs Fiestel Encryption
		{
			for (int i = 0; i < (lusertxt.length() * 8); i++)	//Calculates the new right half of text
				newR[i] = (leftASCII[i] + keys[r][i]) % 2;		//leftASCII[i] XOR keys[r][i]	
		}
		else
		{
			for (int i = 0; i < (lusertxt.length() * 8); i++)	//Calculates the new right half of text
				newR[i] = (leftASCII[i] + keys[rounds - r - 1][i]) % 2;		//leftASCII[i] XOR keys[r][i]	
															//Keys are used in reverse order for Decryption
		}
		memcpy(leftASCII, rightASCII, (lusertxt.length() * 8 * sizeof(int)));	//leftASCII = rightASCII
		memcpy(rightASCII, newR, (rusertxt.length() * 8 * sizeof(int)));		//rightASCII = newR
	}
	
	string ans;
	for (int i = 0; i < (lusertxt.length() * 8); i++)			//Converting the Result to one string variable
		ans += to_string(leftASCII[i]);
	for (int i = 0; i < (rusertxt.length() * 8); i++)
		ans += to_string(rightASCII[i]);

	string C = asciiToText(ans);			//Converting the result from ASCII to text form
	
	if (enc){								//Displaying results
		cout << "\n\nCiphertext : " << C << "\n\nKeys: \n";
		
		string tmp = "";					//Displays Keys used
		for (int j = 0; j < rounds; j++)
		{
			for (int i = 0; i < (usertext.length() * 4); i++)
				tmp += to_string(keys[j][i]);
			cout << "Round " << (j + 1) << " : " << asciiToText(tmp) << endl;
			tmp = "";
		}
	}
	else{
		cout << "\n\nPlaintext : " << C << "\n";
	}

	system("pause");
	return 0;
}

