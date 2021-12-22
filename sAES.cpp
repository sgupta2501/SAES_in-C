#include<iostream>
#include<bits\stdc++.h>
using namespace std;
static int addrk[16];
int SAES_NibbleSubstitution(int w)
{
	string s = to_string(w);
	int number_of_zeros = 4-s.length();
	string dest=s;
	while (number_of_zeros-->0)
		dest = '0'+dest;
	
	if (dest.compare("0000")==0)
		s = "1001";
	else if (dest.compare("0001")==0)
		s = "0100";
	else if (dest.compare("0010")==0)
		s = "1010";
	else if (dest.compare("0011")==0)
		s = "1011";
	else if (dest.compare("0100")==0)
		s = "1101";
	else if (dest.compare("0101")==0)
		s = "0001";
	else if (dest.compare("0110")==0)
		s = "1000";
	else if (dest.compare("0111")==0)
		s = "0101";
	else if (dest.compare("1000")==0)
		s = "0110";
	else if (dest.compare("1001")==0)
		s = "0010";
	else if (dest.compare("1010")==0)
		s = "0000";
	else if (dest.compare("1011")==0)
		s = "0011";
	else if (dest.compare("1100")==0)
		s = "1100";
	else if (dest.compare("1101")==0)
		s = "1110";
	else if (dest.compare("1110")==0)
		s = "1111";
	else if (dest.compare("1111")==0)
		s = "0111";

	w=stoi(s);
	return w;	
}
int RotnibtoSubnib(int *rotnib,int i)
{
	int subnib = rotnib[i]*1000 + rotnib[i+1]*100 + rotnib[i+2]*10 + rotnib[i+3];//subnib
	return subnib;
}

void SubnibtoRotnib(int *rotnib,int subnib,int i)
{
	rotnib[i] = subnib/1000;
	subnib = subnib%1000; 
	rotnib[i+1] = subnib/100;
	subnib = subnib%100;
	rotnib[i+2] = subnib/10;
	subnib = subnib%10;
	rotnib[i+3] = subnib;
}
void SAES_ShiftRow(int *s_matrix)
{
	int i,temp;
	for(i=0;i<4;i++)
	{
		temp = s_matrix[i+4];
		s_matrix[i+4] = s_matrix[i+12];
		s_matrix[i+12] = temp;
	}
}

void SAES_KeyExpansion(int *K)
{
		int i,rotnib[8],subnib1,subnib2;
		//w2
		K[16] = K[0]^1; //XOR with Rcon1
		for(i=1;i<8;i++)
		{
			K[i+16] = K[i]^0;
		}
		
		for(i=0;i<4;i++) //rotnib(w1)
		{
			rotnib[i] = K[(i+12)];
		}
		for(i=4;i<8;i++) //rotnib(w1)
		{
			rotnib[i] = K[(i+4)];
		}
		
		subnib1 = RotnibtoSubnib(rotnib,0);
		subnib2 = RotnibtoSubnib(rotnib,4);
		subnib1 = SAES_NibbleSubstitution(subnib1);
		subnib2 = SAES_NibbleSubstitution(subnib2);		
		SubnibtoRotnib(rotnib,subnib1,0);
		SubnibtoRotnib(rotnib,subnib2,4);

		for(i=0;i<8;i++)
		{
			K[i+16] = K[i+16]^rotnib[i];
		} //w2 complete	
		//w3
		for(i=0;i<8;i++)
		{
			K[i+24] = K[i+8]^K[i+16];
		}//w3 complete
		//w4
		K[32] = K[16]^0; //XOR with Rcon2
		K[33] = K[17]^0;
		K[34] = K[18]^1;
		K[35] = K[19]^1;
		for(i=20;i<24;i++)
		{
			K[i+16] = K[i]^0;
		}
		
		for(i=0;i<4;i++) //rotnib(w3)
		{
			rotnib[i] = K[(i+28)];
		}
		for(i=4;i<8;i++) //rotnib(w3)
		{
			rotnib[i] = K[(i+20)];
		}

		subnib1 = RotnibtoSubnib(rotnib,0);
		subnib2 = RotnibtoSubnib(rotnib,4);
		subnib1 = SAES_NibbleSubstitution(subnib1);
		subnib2 = SAES_NibbleSubstitution(subnib2);		
		SubnibtoRotnib(rotnib,subnib1,0);
		SubnibtoRotnib(rotnib,subnib2,4);
		for(i=0;i<8;i++)
		{
			K[i+32] = K[i+32]^rotnib[i];
		} //w4 complete
		//w5
		for(i=0;i<8;i++)
		{
			K[i+40] = K[i+24]^K[i+32];
		}//w5 complete
}
int* SAES_AddRoundKey(int *plaintext, int *K,int idx)
{
	int i;
	for(i=0;i<16;i++)
	{
		addrk[i] = plaintext[i]^K[idx+i];
	}
	return addrk;
}
void SAES_MixColumns(int *s_matrix)
{
	int i,temp[16];
	temp[0] = s_matrix[0]^s_matrix[6];
	temp[1] = s_matrix[1]^s_matrix[4]^s_matrix[7];
	temp[2] = s_matrix[2]^s_matrix[4]^s_matrix[5];
	temp[3] = s_matrix[3]^s_matrix[5];
	temp[4] = s_matrix[2]^s_matrix[4];
	temp[5] = s_matrix[0]^s_matrix[3]^s_matrix[5];
	temp[6] = s_matrix[0]^s_matrix[1]^s_matrix[6];
	temp[7] = s_matrix[1]^s_matrix[7];
	temp[8] = s_matrix[8]^s_matrix[14];
	temp[9] = s_matrix[9]^s_matrix[12]^s_matrix[15];
	temp[10] = s_matrix[10]^s_matrix[12]^s_matrix[13];
	temp[11] = s_matrix[11]^s_matrix[13];
	temp[12] = s_matrix[10]^s_matrix[12];
	temp[13] = s_matrix[8]^s_matrix[11]^s_matrix[13];
	temp[14] = s_matrix[8]^s_matrix[9]^s_matrix[14];
	temp[15] = s_matrix[9]^s_matrix[15];
	for(i=0;i<16;i++)
	{
		s_matrix[i] = temp[i];
	}
}
void SAES_Encrypt(int *plaintext, int *K, int * ciphertext)
{
	int i,*addrk,subnib1, subnib2, subnib3, subnib4;
	SAES_KeyExpansion(K); //key expansion
	addrk = SAES_AddRoundKey(plaintext,K,0); //add round key with K0
	subnib1 = RotnibtoSubnib(addrk,0); //Nibble substitution
	subnib2 = RotnibtoSubnib(addrk,4);
	subnib3 = RotnibtoSubnib(addrk,8);
	subnib4 = RotnibtoSubnib(addrk,12);
	subnib1 = SAES_NibbleSubstitution(subnib1);
	subnib2 = SAES_NibbleSubstitution(subnib2);		
	subnib3 = SAES_NibbleSubstitution(subnib3);
	subnib4 = SAES_NibbleSubstitution(subnib4);		
	SubnibtoRotnib(addrk,subnib1,0);
	SubnibtoRotnib(addrk,subnib2,4);
	SubnibtoRotnib(addrk,subnib3,8);
	SubnibtoRotnib(addrk,subnib4,12);
	SAES_ShiftRow(addrk); //shift row
	SAES_MixColumns(addrk);//mix columns
	addrk = SAES_AddRoundKey(addrk,K,16); //add round key with K1
	subnib1 = RotnibtoSubnib(addrk,0); //Nibble substitution
	subnib2 = RotnibtoSubnib(addrk,4);
	subnib3 = RotnibtoSubnib(addrk,8);
	subnib4 = RotnibtoSubnib(addrk,12);
	subnib1 = SAES_NibbleSubstitution(subnib1);
	subnib2 = SAES_NibbleSubstitution(subnib2);		
	subnib3 = SAES_NibbleSubstitution(subnib3);
	subnib4 = SAES_NibbleSubstitution(subnib4);		
	SubnibtoRotnib(addrk,subnib1,0);
	SubnibtoRotnib(addrk,subnib2,4);
	SubnibtoRotnib(addrk,subnib3,8);
	SubnibtoRotnib(addrk,subnib4,12);
	SAES_ShiftRow(addrk); //shift row
	addrk = SAES_AddRoundKey(addrk,K,32); //add round key with K2
	cout<<"Generated Ciphertext is \n";
	for(i=0;i<16;i++)
	{
		cout<<addrk[i]<<" ";
		ciphertext[i] = addrk[i];
	}
	cout<<"\n";
}

int SAES_InvNibbleSub(int w)
{
	string s = to_string(w);
	int number_of_zeros = 4-s.length();
	string dest=s;
	while (number_of_zeros-->0)
		dest = '0'+dest;
	
	if (dest.compare("0000")==0)
		s = "1010";
	else if (dest.compare("0001")==0)
		s = "0101";
	else if (dest.compare("0010")==0)
		s = "1001";
	else if (dest.compare("0011")==0)
		s = "1011";
	else if (dest.compare("0100")==0)
		s = "0001";
	else if (dest.compare("0101")==0)
		s = "0111";
	else if (dest.compare("0110")==0)
		s = "1000";
	else if (dest.compare("0111")==0)
		s = "1111";
	else if (dest.compare("1000")==0)
		s = "0110";
	else if (dest.compare("1001")==0)
		s = "0000";
	else if (dest.compare("1010")==0)
		s = "0010";
	else if (dest.compare("1011")==0)
		s = "0011";
	else if (dest.compare("1100")==0)
		s = "1100";
	else if (dest.compare("1101")==0)
		s = "0100";
	else if (dest.compare("1110")==0)
		s = "1101";
	else if (dest.compare("1111")==0)
		s = "1110";

	w=stoi(s);
	return w;	
}
void SAES_InverseMixColumns(int *s_matrix)
{
	int s0,s1,s2,s3,i,temp[16],rotnib[4];
	map<int,int> invMC_2,invMC_9;
	invMC_2.insert(pair<int,int>(0,0));
	invMC_2.insert(pair<int,int>(1,10));
	invMC_2.insert(pair<int,int>(10,100));
	invMC_2.insert(pair<int,int>(11,110));
	invMC_2.insert(pair<int,int>(100,1000));
	invMC_2.insert(pair<int,int>(101,1010));
	invMC_2.insert(pair<int,int>(110,1100));
	invMC_2.insert(pair<int,int>(111,1110));
	invMC_2.insert(pair<int,int>(1000,11));
	invMC_2.insert(pair<int,int>(1001,1));
	invMC_2.insert(pair<int,int>(1010,111));
	invMC_2.insert(pair<int,int>(1011,101));
	invMC_2.insert(pair<int,int>(1100,1011));
	invMC_2.insert(pair<int,int>(1101,1001));
	invMC_2.insert(pair<int,int>(1110,1111));
	invMC_2.insert(pair<int,int>(1111,1101));

	invMC_9.insert(pair<int,int>(0,0));
	invMC_9.insert(pair<int,int>(1,1001));
	invMC_9.insert(pair<int,int>(10,1));
	invMC_9.insert(pair<int,int>(11,1000));
	invMC_9.insert(pair<int,int>(100,10));
	invMC_9.insert(pair<int,int>(101,1011));
	invMC_9.insert(pair<int,int>(110,11));
	invMC_9.insert(pair<int,int>(111,1010));
	invMC_9.insert(pair<int,int>(1000,100));
	invMC_9.insert(pair<int,int>(1001,1101));
	invMC_9.insert(pair<int,int>(1010,101));
	invMC_9.insert(pair<int,int>(1011,1100));
	invMC_9.insert(pair<int,int>(1100,110));
	invMC_9.insert(pair<int,int>(1101,1111));
	invMC_9.insert(pair<int,int>(1110,111));
	invMC_9.insert(pair<int,int>(1111,1110));
	
	s0 = RotnibtoSubnib(s_matrix,0);
	s1 = RotnibtoSubnib(s_matrix,4);
	s2 = RotnibtoSubnib(s_matrix,8);
	s3 = RotnibtoSubnib(s_matrix,12);
	
	auto itr = invMC_9.find(s0);
	SubnibtoRotnib(temp,itr->second,0);
	itr = invMC_2.find(s1);
	SubnibtoRotnib(rotnib,itr->second,0);
	for(i=0;i<4;i++)
	{
		s_matrix[i] = temp[i]^rotnib[i];
	}
	
	itr = invMC_2.find(s0);
	SubnibtoRotnib(temp,itr->second,4);
	itr = invMC_9.find(s1);
	SubnibtoRotnib(rotnib,itr->second,0);
	for(i=0;i<4;i++)
	{
		s_matrix[i+4] = temp[i+4]^rotnib[i];
	}
	
	itr = invMC_9.find(s2);
	SubnibtoRotnib(temp,itr->second,8);
	itr = invMC_2.find(s3);
	SubnibtoRotnib(rotnib,itr->second,0);
	for(i=0;i<4;i++)
	{
		s_matrix[i+8] = temp[i+8]^rotnib[i];
	}
	
	itr = invMC_2.find(s2);
	SubnibtoRotnib(temp,itr->second,12);
	itr = invMC_9.find(s3);
	SubnibtoRotnib(rotnib,itr->second,0);
	for(i=0;i<4;i++)
	{
		s_matrix[i+12] = temp[i+12]^rotnib[i];
	}
}

void SAES_Decrypt(int *ciphertext, int *K)
{
	int i,*addrk,subnib1, subnib2, subnib3, subnib4;
	SAES_KeyExpansion(K); //key expansion
	addrk = SAES_AddRoundKey(ciphertext,K,32); //add round key with K2
	SAES_ShiftRow(addrk); //shift row
	subnib1 = RotnibtoSubnib(addrk,0); //Inverse Nibble substitution
	subnib2 = RotnibtoSubnib(addrk,4);
	subnib3 = RotnibtoSubnib(addrk,8);
	subnib4 = RotnibtoSubnib(addrk,12);
	subnib1 = SAES_InvNibbleSub(subnib1);
	subnib2 = SAES_InvNibbleSub(subnib2);		
	subnib3 = SAES_InvNibbleSub(subnib3);
	subnib4 = SAES_InvNibbleSub(subnib4);		
	SubnibtoRotnib(addrk,subnib1,0);
	SubnibtoRotnib(addrk,subnib2,4);
	SubnibtoRotnib(addrk,subnib3,8);
	SubnibtoRotnib(addrk,subnib4,12);	
	addrk = SAES_AddRoundKey(addrk,K,16); //add round key with K1
	SAES_InverseMixColumns(addrk);//Inverse mix columns
	SAES_ShiftRow(addrk); //shift row
	subnib1 = RotnibtoSubnib(addrk,0); //Inverse Nibble substitution
	subnib2 = RotnibtoSubnib(addrk,4);
	subnib3 = RotnibtoSubnib(addrk,8);
	subnib4 = RotnibtoSubnib(addrk,12);
	subnib1 = SAES_InvNibbleSub(subnib1);
	subnib2 = SAES_InvNibbleSub(subnib2);		
	subnib3 = SAES_InvNibbleSub(subnib3);
	subnib4 = SAES_InvNibbleSub(subnib4);		
	SubnibtoRotnib(addrk,subnib1,0);
	SubnibtoRotnib(addrk,subnib2,4);
	SubnibtoRotnib(addrk,subnib3,8);
	SubnibtoRotnib(addrk,subnib4,12);
	addrk = SAES_AddRoundKey(addrk,K,0); //add round key with K0
	cout<<"Plaintext is \n";
	for(i=0;i<16;i++)
	{
		cout<<addrk[i]<<" ";
	}
	cout<<"\n";

}
int main()
{
	//Used this to test and debug
	//plaintext =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	//K = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	//Q 5.10 stallings
	/*
	Ans ciphertext 0000 0111 0011 1000
	Stallings ans 5.10	
	Key expansion:
		W0 = 1010 0111 W1 = 0011 1011 W2 = 0001 1100 
		W3 = 0010 0111
		W4 = 0111 0110 W5 = 0101 0001
	Round 0:
		After Add round key: 1100 1000 0101 0000
	Round 1:
		After Substitute nibbles: 1100 0110 0001 1001
		After Shift rows: 1100 1001 0001 0110
		After Mix columns: 1110 1100 1010 0010
		After Add round key: 1110 1100 1010 0010
	Round 2:
		After Substitute nibbles: 1111 0000 1000 0101
		After Shift rows: 0111 0001 0110 1001
		After Add round key: 0000 0111 0011 1000

	*/

	
	int plaintext[16] =  {0,1,1,0,1,1,1,1,0,1,1,0,1,0,1,1};//{1,1,0,1,0,1,1,1,0,0,1,0,1,0,0,0};
	//;
	int K[48] = {1,0,1,0,0,1,1,1,0,0,1,1,1,0,1,1};//{0,1,0,0,1,0,1,0,1,1,1,1,0,1,0,1};
	//
	int ciphertext[16] =  {0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,0};
	int i,menu = 0,ch = 0,userp[16], userk[48], userc[16];

	while(1)
	{
		cout<<"------------------------------------------------------------------------------------------------------------------------\n";
		cout<<"Default Plaintext: ";
		for(i=0;i<16;i++)
		{
			cout<<plaintext[i]<<" ";
		}
		cout<<"\n";
		cout<<"Default Key: ";
		for(i=0;i<16;i++)
		{
			cout<<K[i]<<" ";
		}
		cout<<"\n";
	
		cout<<"Default Ciphertext: ";
		for(i=0;i<16;i++)
		{
			cout<<ciphertext[i]<<" ";
		}
		cout<<"\n";
		cout<<"1. Use default Plaintext, Key\n";
		cout<<"2. Enter Plaintext, Key\n";
		cout<<"3.Exit\n"; 
		cout<<"------------------------------------------------------------------------------------------------------------------------\nOption: ";
		cin>>menu;
				
		switch(menu)
		{
			case 1:
				SAES_Encrypt(plaintext,K,ciphertext);
				cout<<"1. Decrypt the obtained Ciphertext\n";
				cout<<"2. Return to menu\nOption: ";
				cin>>ch;
				if(ch == 1)
					SAES_Decrypt(ciphertext,K);				
				break;
				
			case 2:
				cout<<"Enter 16 bit plaintext in Binary\n";
				for(i=0;i<16;i++)
				{
					cin>>userp[i];
				}
				cout<<"Enter 16 bit key in Binary\n";
				for(i=0;i<16;i++)
				{
					cin>>userk[i];
				}
				SAES_Encrypt(userp,userk,userc);
				cout<<"1. Decrypt the obtained Ciphertext\n";
				cout<<"2. Return to menu\nOption: "; 
				cin>>ch;
				if(ch == 1)
					SAES_Decrypt(userc,userk);
				break;
			default:
				exit(0);
		}
	}
	
	return 0;
}
