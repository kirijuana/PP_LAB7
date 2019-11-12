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
	// ���������� ������� � ������ 
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
	// �������� ������ ��� ������� 
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
		//�������� ����� ����������� ������� � ��������� 
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
		//����� �������� ��������� 
		//������� ���������� ��� ������
		//	pthread_rwlock_rdlock(&rwlock);
		printf("������: \n");
		pthread_rwlock_rdlock(&rwlock_Letter);
		for (int i = 0; i < letter.date.size(); i++)
		{
			
			cout << "����� �����������: " << letter.sender_address[i] << ";" << " ����� ���������� : " << letter.recipient_address[i] << "; " " ���� ����������� : " << letter.date[i] << "; " << endl;
		}
		pthread_rwlock_unlock(&rwlock_Letter);
		printf("�������: \n");
		pthread_rwlock_rdlock(&rwlock_Parcel);
		for (int i = 0; i < parcel.date.size(); i++)
		{
			
			cout << "����� �����������: " << parcel.sender_address[i] << ";" << " ����� ���������� : " << parcel.recipient_address[i] << "; " " ���� ����������� : " << parcel.date[i] << "; " << " ���: " << parcel.weight[i] << "; " << " �����: " << parcel.volume[i] << "; " << endl;
			
		}
		pthread_rwlock_unlock(&rwlock_Parcel);
		return 0;
}

void *Printf_3(void *param)
{
	//���������� � 3�� � ����� ��������/��������� 
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
		if (check >= 3) // ���� >= 3 ����� ������� 
		{
			pthread_rwlock_rdlock(&rwlock_Letter);
			for (int j = 0; j < letter.date.size(); j++)
			{
				if (letter.recipient_address[j] == recipient_address[i])
				{
					
					cout << "����� �����������: " << letter.sender_address[j] << ";" << " ����� ���������� : " << letter.recipient_address[j] << "; " " ���� ����������� : " << letter.date[j] << "; " << endl;
					
				}
			}
			pthread_rwlock_unlock(&rwlock_Letter);
			pthread_rwlock_rdlock(&rwlock_Parcel);
			for (int j = 0; j < parcel.date.size(); j++)
			{
				if (parcel.recipient_address[j] == recipient_address[i])
				{
					
					cout << "����� �����������: " << parcel.sender_address[j] << ";" << " ����� ���������� : " << parcel.recipient_address[j] << "; " " ���� ����������� : " << parcel.date[j] << "; " << " ���: " << parcel.weight[j] << "; " << " �����: " << parcel.volume[j] << "; " << endl;
				
					
				}
			}
			pthread_rwlock_unlock(&rwlock_Parcel);
		}
	}	return 0;
}
void *Printf_min_max(void *param)
{
	// ����� ����� ������ � ������� ������� 
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
		}		printf("������������ ���\n");
		cout << "����� �����������: " << parcel.sender_address[ind_max] << ";" << " ����� ���������� : " << parcel.recipient_address[ind_max] << "; " " ���� ����������� : " << parcel.date[ind_max] << "; " << " ���: " << parcel.weight[ind_max] << "; " << " �����: " << parcel.volume[ind_max] << "; " << endl;
		printf("����������� ���\n");
		cout << "����� �����������: " << parcel.sender_address[ind_min] << ";" << " ����� ���������� : " << parcel.recipient_address[ind_min] << "; " " ���� ����������� : " << parcel.date[ind_min] << "; " << " ���: " << parcel.weight[ind_min] << "; " << " �����: " << parcel.volume[ind_min] << "; " << endl;	}	pthread_rwlock_unlock(&rwlock_Parcel);	return 0;
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
	//���������� ������ �������

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