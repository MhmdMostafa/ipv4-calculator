#include <iostream>
#include <string>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

string decimalToBinary(int);
string CIDR(int);
int binaryToDecimal(string, int);
int *ip_mask_integers(string);
int *brodcastGenerater(int *, int *);
int *networkGenerater(int *, int *);
string classType(int *);
int *maskGenerater(int *, int);
void print(int *, int *, int *, int *, int *);
int *wildcardMask(int *);
string ipType(int *);

int main()
{ //users[] is an arry that have number of usable hosts in each subnet for ex: 12 have 1048574 usable hosts
    int users[] = {
        2147483646, 1073741822, 536870910, 268435454, 134217726, 67108862,
        33554430, 16777214, 8388606, 4194302, 2097150, 1048574, 524286,
        262142, 131070, 65534, 32766, 16382, 8190, 4074, 2046, 1022,
        510, 254, 126, 62, 30, 14, 6, 2, 0, 0};
    int choice, subnet, firstHost, lastHost, totalHost;
    int *ipSections, *maskSections, *Broadcast, *networkAddress, *wildcard;
    int *subnetInt = new int[4];
    string ipStr, maskStr, binarySubnetStr;

    cout << "CIDR (Classless) Enter 1\nClassful Enter 2\nConfigure Mask depend on subnet Requird Enter 3\nConfigure Mask depend on hosts Requird Enter 4\nyour choice: ";
    cin >> choice;
    while (choice < 1 || choice > 4)
    {
        cout << "WRONG choice please try again\nCIDR (Classless) Enter 1\nClassful Enter 2\nconfigure subnet Enter 3\nyour choice: ";
        cin >> choice;
    }

    switch (choice)
    {
    case 1: // CIDR
        cout << "Enter the ip: ";
        cin >> ipStr;
        cout << "Enter the subnet: ";
        cin >> subnet;
        ipSections = ip_mask_integers(ipStr);
        binarySubnetStr = CIDR(subnet);
        totalHost = (pow(2, 32 - subnet)) - 2;

        for (int i = 0; i < 4; i++)
        {
            *(subnetInt + i) = binaryToDecimal(binarySubnetStr, i * 8);
        }
        wildcard = wildcardMask(subnetInt);
        Broadcast = brodcastGenerater(ipSections, subnetInt);
        networkAddress = networkGenerater(ipSections, subnetInt);
        print(ipSections, subnetInt, Broadcast, networkAddress, wildcard);
        cout << "Class Type: " << classType(subnetInt) << endl;
        cout << "IP Type: " << ipType(ipSections) << endl;
        cout << "Aviable Hosts: " << totalHost << endl;
        break;

    case 2: //Classful
        cout << "enter the ip: ";
        cin >> ipStr;
        cout << "enter the mask: ";
        cin >> maskStr;
        ipSections = ip_mask_integers(ipStr);
        maskSections = ip_mask_integers(maskStr);
        wildcard = wildcardMask(maskSections);
        Broadcast = brodcastGenerater(ipSections, maskSections);
        networkAddress = networkGenerater(ipSections, maskSections);

        for (int i = 0; i < 4; i++)
            if (*(Broadcast + i) - *(networkAddress + i) != 0)
                totalHost = *(Broadcast + i) - *(networkAddress + i) - 1;

        print(ipSections, maskSections, Broadcast, networkAddress, wildcard);
        cout << "Class Type: " << classType(maskSections) << endl;
        cout << "IP Type: " << ipType(ipSections) << endl;
        cout << "Aviable Hosts: " << totalHost << endl;
        break;

    case 3: //subnet Requird
        cout << "Enter the ip: ";
        cin >> ipStr;
        cout << "Enter the subnet Requird: ";
        cin >> subnet;
        ipSections = ip_mask_integers(ipStr);
        maskSections = maskGenerater(ipSections, subnet);
        wildcard = wildcardMask(maskSections);
        Broadcast = brodcastGenerater(ipSections, maskSections);
        networkAddress = networkGenerater(ipSections, maskSections);

        for (int i = 0; i < 4; i++)
            if (*(Broadcast + i) - *(networkAddress + i) != 0)
                totalHost = *(Broadcast + i) - *(networkAddress + i) - 1;

        print(ipSections, maskSections, Broadcast, networkAddress, wildcard);
        cout << "Class Type: " << classType(maskSections) << endl;
        cout << "IP Type: " << ipType(ipSections) << endl;
        cout << "Aviable Hosts: " << totalHost << endl;
        break;

    case 4: //hosts number Requird
        cout << "Enter hosts number Requird: ";
        cin >> totalHost;

        for (int i = 0; i < sizeof(users); i++)
            if (totalHost >= users[i])
            {
                totalHost = 32 - i;
                subnet = 32 - totalHost;
                break;
            }
        binarySubnetStr = CIDR(subnet);
        for (int i = 0; i < 4; i++)
            *(subnetInt + i) = binaryToDecimal(binarySubnetStr, i * 8);

        for (int i = 0; i < 4; i++)
            cout << *(subnetInt + i) << ",";
        cout << endl;
        break;

    default:
        break;
    }
    system("pause");
    delete[] ipSections, maskSections, Broadcast, networkAddress, wildcard, subnetInt;
    return 0;
}

/*
decimalToBinary() is a function that takes an integers "octet of the IP in decimal" and convert it to 8bit string. this function is needed in two function : networkGenerater() and maskGenerater()
*/
string decimalToBinary(int sections)
{
    string binary, b;
    int number = sections, test = 128;
    //convert decimal number to binary by dividing the number by 2 and take the mod
    while (number != 0)
    {
        binary += (char)(number % 2) + 48;
        number /= 2;
    }
    //in case the number is less than 128 (11111111)binary so I had to add zeros to complete the octet
    while (sections < test)
    {
        binary += (char)0 + 48;
        test /= 2;
    }

    reverse(binary.begin(), binary.end());
    b += binary;
    binary.clear();

    return b;
}

/*
CIDR() is a function that takes integer number between 1 and 32 and to generate the mask in binary form.
for ex: if subnet =16 the retern value is (11111111111111110000000000000000)
*/
string CIDR(int subnet)
{
    string binarysubnet;
    int count = 0;
    for (count; count < subnet; count++)
    {
        binarysubnet += (char)1 + 48;
    }
    for (count; count < 32; count++)
    {
        binarysubnet += (char)0 + 48;
    }
    return binarysubnet;
}

/*
binaryToDecimal() is a function that takes string of binary number and the start postion of the string that we 
want it to convert for ex: if the given string is (11111111111111110000000000000000) and the position is 0
it will convert the bits from 0 to 7 it mean the first 8bit (11111111) and convert it to decimal so the value will be 255 
*/
int binaryToDecimal(string address, int place)
{
    int power = 0, count = 0;
    for (int i = place + 7; i >= (place); i--)
    {
        if (address[i] == '0')
        {
            power += (0 * pow(2, count));
        }
        else
            power += (1 * pow(2, count));
        count++;
    }
    return power;
}

/*
ip_mask_integers() is a function that take array of ascii numbers and convert each one of it to integers
*/
int *ip_mask_integers(string ip)
{
    int *ipsec = new int[4];
    int count = 0, place = 0;
    string section;

    for (int i = 0; i < 4; i++)
    {
        while (ip[count] != '.' && ip[count] != '\0')
        {
            section[place] = ip[count];
            place++;
            count++;
        }
        *(ipsec + i) = 0;
        *(ipsec + i) = stoi(section);
        place = 0;
        count++;
        section = "hhh";
    }
    return ipsec;
}

/*
brodcastGenerater() is a function that takes the IP and mask to generate the brodcast id.
the key is in the mask if the octet is 255 so the brodcast octet will be same as ip octet and if it 0 so the bordcast octet will be 255, otherwise we use the formula to find the octet. for ex, if the ip is 192.168.1.0 and the mask 255.255.255.0 so the bordcast id will be 192.168.1.255
*/
int *brodcastGenerater(int *ip, int *mask)
{
    static int broadcastAddress[4];
    for (int i = 0; i < 4; i++)
    {
        int multiplier, octet = 0;

        if (*(mask + i) == 255)
            broadcastAddress[i] = *(ip + i);
        else if (*(mask + i) == 0)
            broadcastAddress[i] = 255;
        else
        {
            multiplier = 256 - *(mask + i);
            while (*(ip + i) > octet || *(ip + i) == octet)
                octet += multiplier;
            octet -= 1;
            broadcastAddress[i] = octet;
        }
    }
    return broadcastAddress;
}

/*
networkGenerater() is a function that takes the IP and mask to generate the network id.
the key is in the mask if the octet is 255 so the network octet will be same as ip octet and if it 0 so the network octet will be 0, otherwise we use the formula to find the octet. for ex, if the ip is 192.168.1.0 and the mask 255.255.255.0 so the network id will be 192.168.1.0
*/
int *networkGenerater(int *ip, int *mask)
{
    static int networkAddress[4];
    string section, binarymask, binaryip;
    for (int i = 0; i < 4; i++)
    {
        int multiplier, octet = 0, place = 0;

        if (*(mask + i) == 255)
            networkAddress[i] = *(ip + i);
        else if (*(mask + i) == 0)
            networkAddress[i] = 0;
        else
        {
            binarymask.assign(decimalToBinary(*(mask + i)));
            binaryip.assign(decimalToBinary(*(ip + i)));
            for (int j = i * 8; j < (8 + (i * 8)); j++)
            {
                section += (char)(binarymask[place] & binaryip[place]);
                place++;
            }
            networkAddress[i] = binaryToDecimal(section, 0);
        }
    }
    return networkAddress;
}

/*
classType() is a function that takes the mask and find out the class type of the network 
*/
string classType(int *subnetmask)
{
    if (*subnetmask == 255 && *(subnetmask + 1) == 255 && *(subnetmask + 2) == 255)
    {
        return "Class C";
    }
    else if (*subnetmask == 255 && *(subnetmask + 1) == 255)
    {
        return "Class B";
    }
    else if (*subnetmask == 255)
    {
        return "Class A";
    }
    return "unknown";
}

/*
maskGenerater() is a function that takes the IP and the subnet that is requaird and find out the first subnet of the network
*/
int *maskGenerater(int *ip, int subRequaird)
{
    int bits = 0;

    int flag, count, place = 0;
    static int mask[4];
    string binarymask;
    while (pow(2, bits) <= subRequaird)
        bits++;

    int subnetsize;
    for (int i = 0; i < 4; i++)
    {
        if (*(ip + i) != 0)
        {
            mask[i] = 255;
            flag = 0;
        }
        else
        {
            mask[i] = 0;
            if (flag == 0)
                flag = i;
        }
    }
    count = flag;
    binarymask.assign(decimalToBinary(mask[count++]));
    while (count < 4)
        binarymask.append(decimalToBinary(mask[count++]));
    for (int i = 0; i < binarymask.length(); i++)
    {
        binarymask[i] = '1';
        if (i + 1 == bits)
            break;
    }

    mask[flag++] = binaryToDecimal(binarymask, place++);
    while (flag < 4)
        mask[flag++] = binaryToDecimal(binarymask, place++ * 8);

    return mask;
}

/*
print() is a function that prints the ipSections, maskSections, Broadcast, networkAddress and Wildcard
*/
void print(int *ipSections, int *maskSections, int *Broadcast, int *networkAddress, int *Wildcard)
{
    cout << "IP: ";
    for (int i = 0; i < 4; i++)
    {
        cout << *(ipSections + i) << ",";
    }
    cout << endl;

    cout << "Mask: ";
    for (int i = 0; i < 4; i++)
    {
        cout << *(maskSections + i) << ",";
    }
    cout << endl;

    cout << "Broadcast: ";
    for (int i = 0; i < 4; i++)
    {
        cout << *(Broadcast + i) << ",";
    }
    cout << endl;

    cout << "Network Address: ";
    for (int i = 0; i < 4; i++)
    {
        cout << *(networkAddress + i) << ",";
    }
    cout << endl;
    cout << "Wildcard Mask: ";
    for (int i = 0; i < 4; i++)
    {
        cout << *(Wildcard + i) << ",";
    }
    cout << endl;
}

/*
wildcardMask() is a function that takes the mask and find out the wild card mask of the network by subtracting 255 from every octet 
*/
int *wildcardMask(int *mask)
{
    static int wildcard[] = {255, 255, 255, 255};
    for (int i = 0; i < 4; i++)
    {
        wildcard[i] = wildcard[i] - *(mask + i);
    }
    return wildcard;
}

/*
ipType() is a function that takes the IP and find out the IP type of the network wither it is private or public 
*/
string ipType(int *ip)
{
    if (*ip == 10)
    {
        return "Private";
    }
    else if (*ip == 172)
    {
        if (*(ip + 1) >= 16 || *(ip + 1) <= 31)
            return "Private";
    }
    else if (*ip == 192)
    {
        if (*(ip + 1) == 168)
            return "Private";
    }
    return "Public";
}