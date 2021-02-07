/*
	Patrick Ellway
    Project 6 for Nand2Tetris Course - Assembler
	Date Commenced - 18/01/2020
    Compiled With: g++ -o Assembler -std=c++11 Assembler.cpp
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

std::string convertToBinary(int number)
{
    int binary[16];
    std::string stringNumber;
    binary[15] = 0;
    for (int i = 0; i < 15; i++)
    {
        int k = number >> i;
        if (k & 1) binary[i] = 1;
        else binary[i] = 0;
    }
    for (int i = 15; i >= 0; i--)
    {
        stringNumber.push_back(binary[i] + '0');
    }
    return stringNumber;
}
                

int main(int argc, char *argv[])
{
    // If too many args, close program
    if (argc != 2)
    {
        std::cout << "Incorrect number of arguments!" << std::endl;
        return 0;
    }

    std::string filename = argv[1];

    // Parses file contents into an array line by line
    std::vector <std::string> asmFile;
    std::string line;
    std::fstream readFile(filename);
    while (getline (readFile, line))
    {
        // Deletes all whitespace in each line
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        if (line[0] == '/') continue; // Deletes lines that are only comments
        if (line.size() == 1) continue; // Deletes lines that are empty

        // Deletes comments in each line
        std::size_t pos = line.find('/'); // position of '/' in string
        line = line.substr(0, pos);

        asmFile.push_back(line);
    }

    // Create symbol table
    std::map<std::string, int> symbolTable;
    symbolTable.insert({"R0",  0});
    symbolTable.insert({"R1",  1});
    symbolTable.insert({"R2",  2});
    symbolTable.insert({"R3",  3});
    symbolTable.insert({"R4",  4});
    symbolTable.insert({"R5",  5});
    symbolTable.insert({"R6",  6});
    symbolTable.insert({"R7",  7});
    symbolTable.insert({"R8",  8});
    symbolTable.insert({"R9",  9});
    symbolTable.insert({"R10", 10});
    symbolTable.insert({"R11", 11});
    symbolTable.insert({"R12", 12});
    symbolTable.insert({"R13", 13});
    symbolTable.insert({"R14", 14});
    symbolTable.insert({"R15", 15});
    symbolTable.insert({"SP",  0});
    symbolTable.insert({"LCL", 1});
    symbolTable.insert({"ARG", 2});
    symbolTable.insert({"THIS", 3});
    symbolTable.insert({"THAT", 4});
    symbolTable.insert({"KBD",  24576});
    symbolTable.insert({"SCREEN", 16384});

    int count = 0;
    for (unsigned int i = 0; i < asmFile.size(); i++)
    {
        std::string strValue = asmFile[i];
        if (strValue[0] == '(') // Label Symbol - First Pass
        {
            strValue = asmFile[i].substr(1, asmFile[i].size()-3); // Removes the brackets
            if (symbolTable.count(strValue) == 0) symbolTable.insert({strValue, i-count});
            count++;
        }
    }

    int availableMemory = 16;
    for (unsigned int i = 0; i < asmFile.size(); i++)
    {
        std::string strValue = asmFile[i];
        if (strValue[0] == '@')
        {
            strValue = asmFile[i].substr(1, asmFile[i].size()-2); // Removes the @
            if (strValue.find_first_not_of("0123456789") == std::string::npos) continue;
            else // Variable Symbol - Second Pass
            {
                if (symbolTable.count(strValue) == 0)
                {
                    symbolTable.insert({strValue, availableMemory});
                    availableMemory++;
                }
            }
        }
    }
    
    // std::map<std::string, int>::iterator itr; 
    // std::cout << "\nThe map symbolTable is : \n"; 
    // std::cout << "\tKEY\tELEMENT\n"; 
    // for (itr = symbolTable.begin(); itr != symbolTable.end(); ++itr) { 
    //     std::cout << '\t' << itr->first 
    //          << '\t' << itr->second << '\n'; 
    // } 
    // std::cout << std::endl; 
    
    // Assembly translation begins here
    std::vector<std::string> binFile;
    for (unsigned int i = 0; i < asmFile.size(); i++)
    {
        // A-Instruction
        if (asmFile[i][0] == '@') 
        {
            std::string aInstruct;
            std::string strValue = asmFile[i].substr(1, asmFile[i].size()-2); // Removes the @
            
            if (strValue.find_first_not_of("0123456789") == std::string::npos)
            {
                // IF NUMBER
                int value = std::stoi(strValue);
                aInstruct = convertToBinary(value);
                binFile.push_back(aInstruct);
            }
            else
            {
                // IF SYMBOL
                int value = symbolTable[strValue];
                binFile.push_back(convertToBinary(value));
            }
        }
        else if (asmFile[i][0] == '(') continue; // Skip the labels

        // C-Instruction
        else
        {
            std::string comp;
            std::string dest;
            std::string jump;

            static const std::size_t npos = -1;
            std::size_t destTrue = asmFile[i].find('='); // Is dest present?
            std::size_t jumpTrue = asmFile[i].find(';'); // Is jump present?

            if (destTrue != npos)   // If occurence of dest found
            {
                std::string destToken = asmFile[i].substr(0, destTrue);
                if (destToken == "M")        dest = "001";
                else if (destToken == "D")   dest = "010";
                else if (destToken == "A")   dest = "100";
                else if (destToken == "MD")  dest = "011";
                else if (destToken == "AM")  dest = "101";
                else if (destToken == "AD")  dest = "110";
                else if (destToken == "AMD") dest = "111";
            }
            else dest = "000";      // No occurence of dest found

            if (jumpTrue != npos)   // If occurence of jump found
            {
                std::string jumpToken = asmFile[i].substr(jumpTrue+1, 3);

                if (jumpToken == "JGT")      jump = "001";
                else if (jumpToken == "JEQ") jump = "010";
                else if (jumpToken == "JLT") jump = "100";
                else if (jumpToken == "JGE") jump = "011";
                else if (jumpToken == "JNE") jump = "101";
                else if (jumpToken == "JLE") jump = "110";
                else if (jumpToken == "JMP") jump = "111";
            }
            else jump = "000";      // No occurence of jump found

            // Comp translation
            std::string compToken;

            // If dest and jump are present
            if (destTrue != npos && jumpTrue != npos)
                compToken = asmFile[i].substr(destTrue+1, jumpTrue-destTrue-1);
            // If only dest is present
            else if (destTrue != npos && jumpTrue == npos)
                compToken = asmFile[i].substr(destTrue+1, asmFile[i].size()-destTrue-2);
            // If only jump is present
            else if (destTrue == npos && jumpTrue != npos)
                compToken = asmFile[i].substr(0, jumpTrue);
                
            // Inefficient comp translation, a-bit is hard-coded
            if (compToken == "0")        comp = "0101010";
            else if (compToken == "1")   comp = "0111111";
            else if (compToken == "D")   comp = "0001100";
            else if (compToken == "A")   comp = "0110000";
            else if (compToken == "M")   comp = "1110000";
            else if (compToken == "-1")  comp = "0111010";
            else if (compToken == "!D")  comp = "0001101";
            else if (compToken == "!A")  comp = "0110001";
            else if (compToken == "-D")  comp = "0001111";
            else if (compToken == "-A")  comp = "0110011";
            else if (compToken == "!M")  comp = "1110001";
            else if (compToken == "-M")  comp = "1110011";
            else if (compToken == "D+1") comp = "0011111";
            else if (compToken == "A+1") comp = "0110111";
            else if (compToken == "D-1") comp = "0001110";
            else if (compToken == "A-1") comp = "0110010";
            else if (compToken == "D+A") comp = "0000010";
            else if (compToken == "D-A") comp = "0010011";
            else if (compToken == "A-D") comp = "0000111";
            else if (compToken == "D&A") comp = "0000000";
            else if (compToken == "D|A") comp = "0010101";
            else if (compToken == "M+1") comp = "1110111";
            else if (compToken == "M-1") comp = "1110010";
            else if (compToken == "D+M") comp = "1000010";
            else if (compToken == "D-M") comp = "1010011";
            else if (compToken == "M-D") comp = "1000111";
            else if (compToken == "D&M") comp = "1000000";
            else if (compToken == "D|M") comp = "1010101";

            std::string cInstruct = "111";
            cInstruct.append(comp).append(dest).append(jump);
            binFile.push_back(cInstruct);
        }
    }

    for (unsigned int i = 0; i < binFile.size(); i++)
    {
        std::cout << binFile[i] << std::endl;
    }
    for (unsigned int i = 0; i < asmFile.size(); i++)
    {
        std::cout << asmFile[i] << std::endl;
    }

    std::size_t ext = filename.find_last_of("."); 
    std::string file = filename.substr(0, ext);

    std::ofstream output;
    output.open (file + "-pat" + ".hack");
    for (unsigned int i = 0; i < binFile.size(); i++)
    {
        output << binFile[i] << "\n";
        //std::cout << binFile[i] << std::endl;
    }
    output.close();

    return 0;
}