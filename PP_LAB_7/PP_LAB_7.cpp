#include "stdafx.h" 
#include <iostream> 
#include <string> 
#include <fstream> 
#include <vector> 
#include <ctime> 
#include <pthread.h>
using namespace std;
struct Letter
{
	vector<string> sender_address;
	vector<string> recipient_address;
	vector<int> date;
};
struct Parcel
{
	vector<string> sender_address;
	vector<string> recipient_address;
	vector<int> date;
	vector<int> weight;
	vector<int> volume;
};
string sender_address[10] = { "st. Lenina, 11","st. Pushkina, 15","st. Babushkina, 11","st. Borisova, 6","st. Sazonova, 8","st. Selyutina, 1","st. Demidova, 21","st. Japova, 17","st. Krasnoyarskaya, 11","st. Irkutskaya, 11" };
string recipient_address[10] = { "st. Sazonova, 8","st. Selyutina, 1","st. Demidova, 21","st. Japova, 17","st. Krasnoyarskaya, 11","st. Irkutskaya, 11", "st. Lenina, 11","st. Pushkina, 15","st. Babushkina, 11","st. Borisova, 6" };
Letter letter;
Parcel parcel;

pthread_rwlock_t rwlock_Letter;
pthread_rwlock_t rwlock_Parcel;

void *Add_Parcel_Letter(void *param)
{
	// добавление посылки и письма 
	while (1)
	{
		int k = 0;

		k = rand() % 10;
		pthread_rwlock_wrlock(&rwlock_Letter);
		letter.sender_address.push_back(sender_address[k]);
		letter.recipient_address.push_back(recipient_address[k]);
		k = rand() % 31 + 1;
		letter.date.push_back(k);
		pthread_rwlock_unlock(&rwlock_Letter);
		pthread_rwlock_wrlock(&rwlock_Parcel);
		k = rand() % 10;
		parcel.sender_address.push_back(sender_address[k]);
		parcel.recipient_address.push_back(recipient_address[k]);
		k = rand() % 31 + 1;
		parcel.date.push_back(k);
		k = rand() % 10 + 1;
		parcel.weight.push_back(k);
		k = rand() % 10 + 1;
		parcel.volume.push_back(k);
		pthread_rwlock_unlock(&rwlock_Parcel);
	}
}

void *Delete_Parcel_Letter(void *param)
{
	// удаление письма или посылки 
	while (1)
	{
		int k = rand() % 10;
		pthread_rwlock_wrlock(&rwlock_Letter);
		for (int i = 0; i < letter.recipient_address.size(); i++)
		{
			if (recipient_address[k] == letter.recipient_address[i])
			{
				
				letter.recipient_address.erase(letter.recipient_address.begin() + i);
			
			}
		}
		pthread_rwlock_unlock(&rwlock_Letter);
		pthread_rwlock_wrlock(&rwlock_Parcel);
		for (int i = 0; i < parcel.recipient_address.size(); i++)
		{
			if (recipient_address[k] == parcel.recipient_address[i])
			{
				
				parcel.recipient_address.erase(parcel.recipient_address.begin() + i);
				
			}
		}
		pthread_rwlock_unlock(&rwlock_Parcel);
	}
	}

void *Delete_Old(void *param)
{
	while (1)
	{
		//удаление долго находящихся посылок в отделении 
		pthread_rwlock_wrlock(&rwlock_Letter);
		for (int i = 0; i < letter.date.size(); i++)
		{
			if (letter.date[i] + 15 >= 31)
			{
				
				letter.date.erase(letter.date.begin() + i);
				
			}
		}
		pthread_rwlock_unlock(&rwlock_Letter);
		
		pthread_rwlock_wrlock(&rwlock_Parcel); 
		for (int i = 0; i < parcel.date.size(); i++)
		{
			if (parcel.date[i] + 15 >= 31)
			{
				
				parcel.date.erase(parcel.date.begin() + i);
				
			}
		}
		pthread_rwlock_unlock(&rwlock_Parcel);
	}
	}
void *Printf_Letter_Parcel(void *param)
{
		//вывод текущего состояния 
		//закрыть блокировку для чтения
		//	pthread_rwlock_rdlock(&rwlock);
		printf("Письма: \n");
		pthread_rwlock_rdlock(&rwlock_Letter);
		for (int i = 0; i < letter.date.size(); i++)
		{
			
			cout << "Адрес отправителя: " << letter.sender_address[i] << ";" << " Адрес получателя : " << letter.recipient_address[i] << "; " " Дата поступления : " << letter.date[i] << "; " << endl;
		}
		pthread_rwlock_unlock(&rwlock_Letter);
		printf("Посылки: \n");
		pthread_rwlock_rdlock(&rwlock_Parcel);
		for (int i = 0; i < parcel.date.size(); i++)
		{
			
			cout << "Адрес отправителя: " << parcel.sender_address[i] << ";" << " Адрес получателя : " << parcel.recipient_address[i] << "; " " Дата поступления : " << parcel.date[i] << "; " << " Вес: " << parcel.weight[i] << "; " << " Объем: " << parcel.volume[i] << "; " << endl;
			
		}
		pthread_rwlock_unlock(&rwlock_Parcel);
		return 0;
}

void *Printf_3(void *param)
{
	//получатель с 3мя и более письмами/посылками 
	int check = 0;
	for (int i = 0; i < 10; i++)
	{
		check = 0;
		pthread_rwlock_rdlock(&rwlock_Letter);
		for (int j = 0; j < letter.date.size(); j++)
		{
			if (letter.recipient_address[j] == recipient_address[i])
				check++;
		}
		pthread_rwlock_unlock(&rwlock_Letter);
		pthread_rwlock_rdlock(&rwlock_Parcel);
		for (int j = 0; j < parcel.date.size(); j++)
		{
			if (parcel.recipient_address[j] == recipient_address[i])
				check++;
		}
		pthread_rwlock_unlock(&rwlock_Parcel);
		if (check >= 3) // если >= 3 тогда выводим 
		{
			pthread_rwlock_rdlock(&rwlock_Letter);
			for (int j = 0; j < letter.date.size(); j++)
			{
				if (letter.recipient_address[j] == recipient_address[i])
				{
					
					cout << "Адрес отправителя: " << letter.sender_address[j] << ";" << " Адрес получателя : " << letter.recipient_address[j] << "; " " Дата поступления : " << letter.date[j] << "; " << endl;
					
				}
			}
			pthread_rwlock_unlock(&rwlock_Letter);
			pthread_rwlock_rdlock(&rwlock_Parcel);
			for (int j = 0; j < parcel.date.size(); j++)
			{
				if (parcel.recipient_address[j] == recipient_address[i])
				{
					
					cout << "Адрес отправителя: " << parcel.sender_address[j] << ";" << " Адрес получателя : " << parcel.recipient_address[j] << "; " " Дата поступления : " << parcel.date[j] << "; " << " Вес: " << parcel.weight[j] << "; " << " Объем: " << parcel.volume[j] << "; " << endl;
				
					
				}
			}
			pthread_rwlock_unlock(&rwlock_Parcel);
		}
	}	return 0;
}
void *Printf_min_max(void *param)
{
	// вывод самой легкой и тяжелой посылки 
	int max = parcel.weight[0], min = parcel.weight[0];
	int ind_max = 0, ind_min = 32768;
	pthread_rwlock_rdlock(&rwlock_Parcel);
	if (parcel.date.size() != 0)
	{
		for (int j = 1; j < parcel.date.size(); j++)
		{
			if (parcel.weight[j] > max)
			{

				max = parcel.weight[j];

				ind_max = j;
			}
			if (parcel.weight[j] < min)
			{

				min = parcel.weight[j];
				ind_min = j;
			}
		}		printf("Максимальный вес\n");
		cout << "Адрес отправителя: " << parcel.sender_address[ind_max] << ";" << " Адрес получателя : " << parcel.recipient_address[ind_max] << "; " " Дата поступления : " << parcel.date[ind_max] << "; " << " Вес: " << parcel.weight[ind_max] << "; " << " Объем: " << parcel.volume[ind_max] << "; " << endl;
		printf("Минимальный вес\n");
		cout << "Адрес отправителя: " << parcel.sender_address[ind_min] << ";" << " Адрес получателя : " << parcel.recipient_address[ind_min] << "; " " Дата поступления : " << parcel.date[ind_min] << "; " << " Вес: " << parcel.weight[ind_min] << "; " << " Объем: " << parcel.volume[ind_min] << "; " << endl;	}	pthread_rwlock_unlock(&rwlock_Parcel);	return 0;
}
int main()
{
	for (int i = 0; i < 30; i++)
	{
		int k = 0;
		k = rand() % 10;
		letter.sender_address.push_back(sender_address[k]);
		letter.recipient_address.push_back(recipient_address[k]);
		k = rand() % 31 + 1;
		letter.date.push_back(k);


		k = rand() % 10;
		parcel.sender_address.push_back(sender_address[k]);
		parcel.recipient_address.push_back(recipient_address[k]);
		k = rand() % 31 + 1;
		parcel.date.push_back(k);
		k = rand() % 10 + 1;
		parcel.weight.push_back(k);
		k = rand() % 10 + 1;
		parcel.volume.push_back(k);

	}
	//int Printf_Letter_Parcel();
	Printf_Letter_Parcel(0);
	pthread_rwlock_init(&rwlock_Letter, NULL);
	pthread_rwlock_init(&rwlock_Parcel, NULL);
	//заполнение общего массива

	pthread_t threadR[4];
	for (int l = 0; l < 4; l++)
		pthread_create(&threadR[l], NULL, Add_Parcel_Letter, NULL);

	pthread_t threadW[1];
	//	for (int l = 0; l<2; l++)
	pthread_create(&threadW[0], NULL, Delete_Old, NULL);
	pthread_t threadS[1];
	//	for (int l = 0; l<3; l++)
	pthread_create(&threadS[0], NULL, Delete_Parcel_Letter, NULL);
	pthread_t threadK[1];
	pthread_create(&threadK[0], NULL, Printf_Letter_Parcel, NULL);
	pthread_t threadC[1];
	pthread_create(&threadC[0], NULL, Printf_min_max, NULL);
	pthread_t threadP[1];
	pthread_create(&threadP[0], NULL, Printf_3, NULL);
	Add_Parcel_Letter(NULL);
	system("pause");
	return 1;
}