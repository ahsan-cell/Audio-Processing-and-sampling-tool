#include<iostream>
#include"wavfile.h";
using namespace std;

//--------------------------------------------------------------
void playfile(char* str) {
	if (playWavFile(str) == 0)
		cout << "Error: File not found!" << endl;
}

//Sample function for reading and storing sound data
void read_data(char* str) {

	int sampleRate = 0, size = 100000;
	unsigned char* ptr = new unsigned char[size];
	readWavFile(str, ptr, size, sampleRate);
	delete[] ptr;

}
//--------------------------------------------------------------

void allocateArray(unsigned char* &arr, int size);
void deallocateArray(unsigned char*& arr);
void printArray(unsigned char* const arr, int size);
void doubleArray(unsigned char* const originalArr, unsigned char*& doubleArray, int size, int& doubledSize);
void shrinkArray(unsigned char* const originalArr, unsigned char*& shrinkedArray, int size, int& shrinkedSize);
void FillWithMean(unsigned char* in, unsigned char* &out, int size, int N);
void cutArray(unsigned char*& arr, int length);   // additional function
bool readFromFile(unsigned char*& arr, int& size, int& sampling_rate);
void upSampleAudio(unsigned char*arr, int size, int sampling_rate);
void downSampleAudio(unsigned char* arr, int size, int sampling_rate);
void mergeArrays(unsigned char* arr1, unsigned char* arr2, unsigned char*& mergedArray, int size1, int size2, int& mergedSize);
void movingAverageFilter(unsigned char* const arr1, int size, int sampling_rate, int N);
void mixAudio(unsigned char* const audio1, unsigned char* const audio2, int size1, int size2, int sampling_rate1, int sampling_rate2);
void Menu();
//-------------------------------DESCRIPTION----------------------------------

//   inward arrows(<-----) indicate the allocation of a memory and outward arows(------>) are used to indicate deallocation. this hepls in making 
// sure that no memory is leaked. the numbers of arrows don't necessarily cancel out each other since some arrays maybe allocated inside a function 
// and deallocated outside of the function.

//default size of the array is taken 90000000 to make sure that we can get the accurate length of the array of audio file. then we use another function
// cutArray() to cut the array so that we have an array in the memoy which is precisely the size of the audio file.

// cin.getlin() does not work properly if it is used after a cin>>  statement. to resolve this issue cin.ignore() statement is used in some places

//----------------------------------------------------------------------------

int main()
{
	Menu();
}

void allocateArray(unsigned char*& arr, int size)
{
	arr = new unsigned char[size];
}

void deallocateArray(unsigned char*& arr)
{
	delete[] arr;
	arr = nullptr;
}

void printArray(unsigned char* const arr, int size)
{
	for (int i = 0; i < size; i++)
		cout <<(int) arr[i] << " ";
	cout << endl;
}

void doubleArray(unsigned char*  const originalArr,unsigned char* &doubleArray ,int size, int& doubledSize)
{
	
	doubledSize = size * 2;
	
	allocateArray(doubleArray, doubledSize); //<-----
	int i = 0, j = 0;
	while (j < doubledSize)
	{
		doubleArray[j] = originalArr[i];
		j++;
		doubleArray[j] = originalArr[i];
		j++;
		i++;
	}
	

	
}

void shrinkArray(unsigned char* const originalArr, unsigned char* &shrinkedArray ,int size, int& shrinkedSize)
{
	if (size % 2 == 0)
		shrinkedSize = size / 2;
	else
		shrinkedSize = (size / 2) + 1;
	
	allocateArray(shrinkedArray, shrinkedSize);  //<-----
	int j = 0;
	for (int i = 0; i < size; i = i + 2)
	{
		shrinkedArray[j] = originalArr[i];
		j++;
	}
	
}

void cutArray(unsigned char* &arr, int length)   // additional function 
{

	//length represents the actual length of audio file.

	// we will first create an array of very big size so that we can get the actual size of file from the readWavFile fuinction.
	// then once we have the actual size we will free up the remaining space of the dynamic array

	//for example we first create an array of 1000 size and store the audio data in it which is actully of 900 size
	//we will call this function to shrink/cut the data array to the 900 size.


	unsigned char* arr2 = nullptr;
	allocateArray(arr2, length);      //<---------
	for (int i = 0; i < length; i++)
		arr2[i] = arr[i];
	delete[]arr;                         //-------->
	arr = arr2;
	arr2 = nullptr;

}

void mergeArrays(unsigned char* arr1, unsigned char* arr2, unsigned char* &mergedArray, int size1, int size2, int &mergedSize)
{
	 mergedSize = size1 + size2;
	
	 allocateArray(mergedArray, mergedSize);    //<---------
	int j1 = 0, j2 = 0;
	for (int i = 0; i < mergedSize; i++)
	{
		if (j1 < size1 && j2 < size2)
		{
			mergedArray[i] = arr1[j1];
			j1++;
			i++;
			mergedArray[i] = arr2[j2];
			j2++;

		}
		else if (j1 < size1 && !(j2 < size2))
		{
			mergedArray[i] = arr1[j1];
			j1++;
		}
		else
		{
			mergedArray[i] = arr2[j2];
			j2++;
		}
	}

}

void FillWithMean(unsigned char* in, unsigned char*& out, int size ,int N)
{
	int j = 0;
	int sum = 0;
	allocateArray(out, size);                  //<------------
	while (j < size)
	{
		if (j - N >= 0 && j+N<size)
		{
			for (int i = j - N; i <= j + N; i++)
			{
				sum = sum + in[i];
			}
			out[j] = sum / (2 * N + 1);
			j++;
			sum = 0;
		}


		else if (!(j - N > 0) && j + N < size)
		{
			for (int i = j; i <= j + N; i++)
				sum = sum + in[i];
			if (j - N == -N)
			{
				out[j] = sum /( N + 1);
				j++;
				sum = 0;
			}
			else
			{
				for (int i = -(j - N); i > 0; i--)
					sum = sum + in[i];
				out[j] = sum /( N + 1 + (-(j - N)));
				j++;
				sum = 0;
			}
		}

		else if (j - N > 0 && !(j + N < size))
		{
			for (int i = j - N; i <= j; i++)
				sum = sum + in[i];
			if (j == size - 1)
			{
				out[j] = sum / (N + 1);
				j++;
				sum = 0;

			}
			else
			{
				int count = 0;
				for (int i = j+1; i < size; i++)
				{
					sum = sum + in[i];
					count++;
				}
				out[j] = sum / (N + 1 + count);
				j++;
				sum = 0;
			}
		}

		else
		{
			for (int i = 0; i < size; i++)
			{
				sum = sum + in[i];
			}
			out[j] = sum / size;
			sum = 0;
			j++;
		}
	}
}

bool readFromFile(unsigned char* &arr, int &size, int& sampling_rate)
{
	bool status = true;
	
	char name[100];
	cout << "Enter the name of an audio file: ";
	cin.getline(name, 100);
	
	allocateArray(arr, size);          //<----------
	
	cout << "-----------------------------------------" << endl;
	if (!(readWavFile(name, arr, size, sampling_rate)))              
	{
		cout << "File could not be accessed" << endl;
		status = false;

	}
	else
	  cutArray(arr, size);
	
	cout << "--------------------------------------------" << endl;
	return status;

}

void upSampleAudio(unsigned char*arr, int size, int sampling_rate)
{
	unsigned char* upsampled = nullptr;
	int size2 = 0;
	
	doubleArray(arr, upsampled,size ,size2);//<------
	
	char name[100];
	cout << "enter the name of Up-Sampled audio file:";
	cin.getline(name, 100);
	
	if (!(writeWavFile(name, upsampled, size2, sampling_rate)))
		cout << "Failed to write the file" << endl;
	else
		cout << "Operation successful" << endl;
	//printArray(upsampled,size2);
	
	deallocateArray(upsampled);  //---------->
   
}

void downSampleAudio(unsigned char* arr, int size, int sampling_rate)
{
	unsigned char* downsampled = nullptr;                 
	int size2 = 0;
    
	shrinkArray(arr,downsampled ,size, size2);       //<-------
	
	char name[100];
	cout << "enter the name of Down-Sampled audio file: ";
	cin.getline(name, 100);
	
	if (!(writeWavFile(name, downsampled, size2, sampling_rate)))
		cout << "Failed to write the file" << endl;
	else
		cout << "Operation successful" << endl;
	//printArray(downsampled, size2);
	
	deallocateArray(downsampled);                  //---------->
}

void movingAverageFilter(unsigned char* const arr1,int size,int sampling_rate, int N )
{
	unsigned char* filter = nullptr;
	allocateArray(filter, size);              //<---------
	
	FillWithMean(arr1, filter, size, N);
	
	char name[100];
	cout << "enter the name of audio file: ";
	cin.getline(name, 100);
	
	if(!(writeWavFile(name, filter,size , sampling_rate)))
		cout << "Failed to write the file" << endl;
	else
		cout << "Operation successful" << endl;
	
	deallocateArray(filter);

}

void mixAudio(unsigned char* const audio1, unsigned char* const audio2, int size1, int size2, int sampling_rate1, int sampling_rate2)
{
	unsigned char* mergedArray = nullptr;
	int size3 = 2;
	int choice = 0;
	
	mergeArrays(audio1, audio2, mergedArray, size1, size2, size3);//<----------
	
	char name[100];
	cout << "enter the name of the new audio file: ";
	cin.getline(name, 100);
	
	cout << "choose a sampling rate: 1. " << sampling_rate1 << " 2. " << sampling_rate2<<"  ";
	cin >> choice;
	if (choice == 1)
	{
		if(!(writeWavFile(name, mergedArray, size3, sampling_rate1)))
			cout << "Failed to write the file" << endl;
		else
			cout << "Operation successful" << endl;
	}
	else if (choice == 2)
	{
		if(!(writeWavFile(name, mergedArray, size3, sampling_rate2)))
			cout << "Failed to write the file" << endl;
		else
			cout << "Operation successful" << endl;
	}
	else
		cout << endl<<"invalid input";
	
	deallocateArray(mergedArray); //------->
}

void Menu()
{
	unsigned char* audio1 = nullptr;
	int size1 = 90000000;     //we dont use const here because we will just pass this size by refrence into the function and get the actaul size
	int sampling_rate1 = 8000;
	int choice = -1;

	while (!(readFromFile(audio1, size1, sampling_rate1))) //<--------
	{
		cout << "enter a valid filename" << endl;
	}
	//cout << size1;
	cout << "Choose an operation from the following: " << endl;
	cout << "0. Exit" << endl;
	cout << "1. Up sample the audio" << endl;
	cout << "2. Down sample the audio" << endl;
	cout << "3. Apply moving average filter" << endl;
	cout << "4. Merge the loaded file with another file" << endl;
	cout << "5. Input a new audio file" << endl;
	cout << "6. play an audio file" << endl;
	cin >> choice;
	cin.ignore();
	while (choice != 0)
	{
		if (choice == 0)
			choice = -1;
		else if (choice == 1)
		{
			upSampleAudio(audio1, size1, sampling_rate1);
			cout << "--------------------------------------------" << endl;
		}

		else if (choice == 2)
		{
			downSampleAudio(audio1, size1, sampling_rate1);
			cout << "--------------------------------------------" << endl;
		}

		else if (choice == 3)
		{
			int N;
			cout << "Enter a value of N:";
			cin >> N;
			cin.ignore();
			movingAverageFilter(audio1, size1, sampling_rate1, N);
			cout << "--------------------------------------------" << endl;
		}

		else if (choice == 4)
		{
			unsigned char* audio2 = nullptr;
			int size2 = 90000000;
			int sampling_rate2 = 8000;
			
			while (!(readFromFile(audio2, size2, sampling_rate2)))          //<-------
			{
				cout << "enter a valid filename" << endl;
			}
			mixAudio(audio1, audio2, size1, size2, sampling_rate1, sampling_rate2);
			deallocateArray(audio2);  //------>
			cout << "--------------------------------------------" << endl;
		}
		else if (choice == 5)
		{
			deallocateArray(audio1);
			while (!(readFromFile(audio1, size1, sampling_rate1))) //<--------
			{
				cout << "enter a valid filename" << endl;
			}
		}
		else if (choice == 6)
		{
			char name[100];
			cout << "enter the name of audio file: ";
			cin.getline(name, 100);
			playWavFile(name);
		}
		else
			cout << "Invalid input"<<endl;

		//cout << "--------------------------------------------"<<endl;
		cout << "0. Exit" << endl;
		cout << "1. Up sample the audio" << endl;
		cout << "2. Down sample the audio" << endl;
		cout << "3. Apply moving average filter" << endl;
		cout << "4. Merge the loaded file with another file" << endl;
		cout << "5. input a new audio file" << endl;
		cout << "6. play an audio file" << endl;
		cin >> choice;
		cin.ignore();
	}
	deallocateArray(audio1);              //------->
}
