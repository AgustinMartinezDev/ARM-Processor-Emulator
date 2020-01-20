/* “On my honor, I have neither given nor received unauthorized aid on this assignment”.
    Agustin Martinez 
    CDA3101 
    Dr.Mishra 
*/

#include <iostream>
#include <fstream> 
#include <map> 
#include <string> 
#include <vector>
#include <cmath> 

using namespace std; 

map<string,string> setMap(){
    map<string,string> mp; 
    mp.emplace("10000", "CBZ"); 
    mp.emplace("10001", "CBNZ");
    mp.emplace("1000000","ORRI");
    mp.emplace("1000001","EORI");
    mp.emplace("1000010","ADDI");
    mp.emplace("1000011","SUBI");
    mp.emplace("1000100","ANDI");
    mp.emplace("10100000","EOR"); 
    mp.emplace("10100010","ADD");
    mp.emplace("10100011","SUB"); 
    mp.emplace("10100100","AND");
    mp.emplace("10100101","ORR");
    mp.emplace("10100110","LSR"); 
    mp.emplace("10100111","LSL");
    mp.emplace("10101010","LDUR");
    mp.emplace("10101011","STUR");      
    return mp; 
}

int findCategoryType(string binaryLine, bool dummy){
    string categoryCode = ""; 
    for(int i=0; i < 3; i++){
        categoryCode+=binaryLine[i];
    }
    if(categoryCode.compare("001") == 0 && !dummy){
        return 1; 
    }
    else if(categoryCode.compare("010") == 0 && !dummy){
        return 2; 
    }
    else if(categoryCode.compare("011") == 0 && !dummy){
        return 3; 
    }
    else if(categoryCode.compare("100") == 0 && !dummy){
        return 4; 
    }
    else if(categoryCode.compare("101") == 0 && !dummy){
        return 5; 
    } 
    return 0;
}

string binarytoDecimal(string binaryString){ 
    return to_string(stoi(binaryString,0,2)); 
}

string undoTwosComplement(string binaryString){
    string binary = "";
    bool flip = false;  
    for(int i=binaryString.length()-1; i>=0; i--){
        if(flip){
           if(binaryString.at(i) == '1'){
               binary.insert(0, "0");
           } 
           else{
               binary.insert(0, "1");
           } 
        }
        else if(binaryString.at(i) == '1' && !flip){
            binary.insert(0, "1");
            flip = true;
        }
        else if(binaryString.at(i) == '0' && !flip){
            binary.insert(0, "0");
        }
    }
    return "-"+binary; 
}

string parseCategoryZero(string binaryString){
    if(binaryString.at(0) == '1'){
        return binarytoDecimal(undoTwosComplement(binaryString)); 
    }
    return binarytoDecimal(binaryString); 
}

vector<string> parsingCategoryOne(string binaryString, map<string,string>& mp){
    vector<string> list; 
    list.push_back(to_string(1)); // category 
    list.push_back(mp.at(binaryString.substr(3,5))); // Opcode  
    if(binarytoDecimal(binaryString.substr(8,5)).compare("31") == 0){ // Source 1
        list.push_back("XZR"); 
    }
    else{
    list.push_back("X"+binarytoDecimal(binaryString.substr(8,5)));
    }
    if(binaryString.at(13) == '1'){ // Branch offset
        list.push_back("#"+binarytoDecimal(undoTwosComplement(binaryString.substr(13,19)))); 
    } 
    else{
        list.push_back("#"+binarytoDecimal(binaryString.substr(13,19)));
    }
    return list; 
}

vector<string> parsingCategoryTwo(string binaryString, map<string,string>& mp){
    vector<string> list; 
    list.push_back(to_string(2)); // type 
    list.push_back(mp.at(binaryString.substr(3,7))); // Opcode  
    list.push_back("X" + binarytoDecimal(binaryString.substr(10,5))); // Destination Register 
    if(binarytoDecimal(binaryString.substr(15,5)).compare("31") == 0){ // Source 1
        list.push_back("XZR"); 
    }
    else{
    list.push_back("X"+binarytoDecimal(binaryString.substr(15,5)));
    }
    if(binaryString.at(20) == '1' && (list.at(1).compare("ADDI") == 0 || list.at(1).compare("SUBI") == 0)){ // Immediate Value 
        list.push_back("#" + binarytoDecimal(undoTwosComplement(binaryString.substr(20,12))));
    }
    else{ list.push_back("#" + binarytoDecimal(binaryString.substr(20,12)));
    } 
    return list; 
}

vector<string> parsingCategoryThree(string binaryString, map<string,string>& mp){ // some can be unsigned and some can be signed 
    vector<string> list; 
    list.push_back(to_string(3)); // type 
    list.push_back(mp.at(binaryString.substr(3,8))); // Opcode  
    list.push_back("X" + binarytoDecimal(binaryString.substr(11,5))); // Destination 
    if(binarytoDecimal(binaryString.substr(16,5)).compare("31") == 0){ // Source 1
        list.push_back("XZR"); 
    }
    else{
    list.push_back("X"+binarytoDecimal(binaryString.substr(16,5)));
    }
    if(binarytoDecimal(binaryString.substr(21,5)).compare("31") == 0){ // Source 1
        list.push_back("XZR"); 
    }
    else{
    list.push_back("X"+binarytoDecimal(binaryString.substr(21,5)));
    } // Source 2 
    return list; 
}

vector<string> parsingCategoryFour(string binaryString, map<string,string>& mp){
    vector<string> list; 
    list.push_back(to_string(4)); // category
    list.push_back(mp.at(binaryString.substr(3,8))); // Opcode  
    list.push_back("X"+binarytoDecimal(binaryString.substr(11,5))); // Destination Register 
    if(binarytoDecimal(binaryString.substr(16,5)).compare("31") == 0){ // Source 1
        list.push_back("XZR"); 
    }
    else{
    list.push_back("X"+binarytoDecimal(binaryString.substr(16,5)));
    }
    if(binaryString.at(21) == '1'){ // Immediate Value
        list.push_back("#"+binarytoDecimal(undoTwosComplement(binaryString.substr(21,11)))); 
    } 
    else{
        list.push_back("#"+binarytoDecimal(binaryString.substr(21,11)));
    } 
    return list; 
}

void publishRegisters(vector<int> &registers, ofstream &file){
    for(int i =0; i<registers.size(); i++){
        if(i%8 == 0){
            file<<"X"; 
            if(i<10){
                file << "0"+to_string(i); 
                file << ":";
            }
            else{
                file << to_string(i); 
                file << ":";
            }
            file << '\t'; 
            file << registers.at(i);  
            file << '\t'; 
        }
        else{
            file << registers.at(i);  
            file << '\t'; 
            if((i+1)%8 == 0 && i > 0 ){
            file << '\n'; 
        }
        }
    }
}

void publishMemory(map<int,int> &memoryDataMap, ofstream &file){
    auto iter = memoryDataMap.begin(); 
    int begin = iter->first; 
    for(; iter != memoryDataMap.end(); iter++){
        if(iter->first == begin || (iter->first - begin) % 32 == 0){
            file<<iter->first; 
            file<<":"; 
            file<<'\t'; 
            file<<iter->second; 
            file<<'\t'; 
        }
        else if((iter->first - begin + 4) % 32 == 0){
            file<< iter->second; 
            file<< '\n';
        }
        else{
            file<< iter->second; 
            file<< '\t'; 
        }
    }
}

int categoryOneSimulation(vector<int> &registersVector, int source,int immmediate, int programcounter, string arminstruction){  
    if(arminstruction.compare("CBZ") == 0){
        if(registersVector.at(source) == 0){
            return ((immmediate*4)+programcounter); 
        }
    }
    else if(arminstruction.compare("CBNZ") == 0){
        if(registersVector.at(source) != 0){
            return ((immmediate*4)+programcounter); 
        }
    }
    return programcounter; 
}

void categoryTwoSimulation(int destination, int source, int immediate, vector<int> &registersVector, string armInstruction){
    if(armInstruction.compare("ADDI") == 0 ){
        registersVector.at(destination) = (registersVector.at(source) + immediate); 
    }
    else if (armInstruction.compare("SUBI") == 0){
        registersVector.at(destination) = (registersVector.at(source) - immediate);
    }
    else if (armInstruction.compare("ORRI") == 0){
        registersVector.at(destination) = (registersVector.at(source) | immediate);
    }
    else if (armInstruction.compare("EORI") == 0){
        registersVector.at(destination) = (registersVector.at(source) ^ immediate);
    }
    else if (armInstruction.compare("ANDI") == 0){
        registersVector.at(destination) = (registersVector.at(source) & immediate);
    }
}

void categoryThreeSimulation(int destination, int source, int source2, vector<int> &registersVector, string armInstruction){
    if(armInstruction.compare("ADD") == 0 ){
        registersVector.at(destination) = (registersVector.at(source) + registersVector.at(source2)); 
    }
    else if (armInstruction.compare("SUB") == 0){
        registersVector.at(destination) = (registersVector.at(source) - registersVector.at(source2));
    }
    else if (armInstruction.compare("ORR") == 0){
        registersVector.at(destination) = (registersVector.at(source) | registersVector.at(source2));
    }
    else if (armInstruction.compare("EOR") == 0){
        registersVector.at(destination) = (registersVector.at(source) ^ registersVector.at(source2));
    }
    else if (armInstruction.compare("AND") == 0){
        registersVector.at(destination) = (registersVector.at(source) & registersVector.at(source2));
    }
    else if (armInstruction.compare("LSR") == 0){
        registersVector.at(destination) = (registersVector.at(source) >> registersVector.at(source2));
    }
    else if (armInstruction.compare("LSL") == 0){
        registersVector.at(destination) = (registersVector.at(source) << registersVector.at(source2));
    }
}

void categoryFourSimulation(int destination, int source, int immediate, vector<int> &registersVector, string armInstruction, map<int, int> &memoryDataMap){
    if(armInstruction.compare("STUR") == 0){
        memoryDataMap[registersVector.at(source) + immediate] = registersVector.at(destination); 
    }
    else if(armInstruction.compare("LDUR") == 0){
        registersVector.at(destination) = memoryDataMap[registersVector.at(source) + immediate]; 
    }
}

int generateCycle(int cycleNumber, int programCounter, int category, string armInstruction, vector<int> &registersVector, map<int, int> &memoryDataMap, vector<string> interpreterVector, ofstream &file){
    file << "--------------------" << '\n'; 
    file << "Cycle " << to_string(cycleNumber) << ":" << '\t' << to_string(programCounter) << '\t' << armInstruction << '\n'; 
    file << '\n'; 
    file << "Registers" << '\n'; 
    int branch = programCounter; 
    if(category == 1){
        if(interpreterVector.at(2).compare("XZR") == 0){
            branch = categoryOneSimulation(registersVector, 31, stoi(interpreterVector.at(3).substr(1)), programCounter,interpreterVector.at(1));
        }
        else{
            branch = categoryOneSimulation(registersVector, stoi(interpreterVector.at(2).substr(1)), stoi(interpreterVector.at(3).substr(1)), programCounter,interpreterVector.at(1)); 
        }
    }
    else if(category == 2){
        if(interpreterVector.at(3).compare("XZR") == 0){
            categoryTwoSimulation(stoi(interpreterVector.at(2).substr(1)), 31, stoi(interpreterVector.at(4).substr(1)), registersVector, interpreterVector.at(1));
        }
        else{
            categoryTwoSimulation(stoi(interpreterVector.at(2).substr(1)), stoi(interpreterVector.at(3).substr(1)), stoi(interpreterVector.at(4).substr(1)), registersVector, interpreterVector.at(1));
        }
        
    }
    else if(category == 3){
        if(interpreterVector.at(3).compare("XZR") == 0){
            categoryThreeSimulation(stoi(interpreterVector.at(2).substr(1)), 31, stoi(interpreterVector.at(4).substr(1)), registersVector, interpreterVector.at(1));
        }
        else{
            categoryThreeSimulation(stoi(interpreterVector.at(2).substr(1)), stoi(interpreterVector.at(3).substr(1)), stoi(interpreterVector.at(4).substr(1)), registersVector, interpreterVector.at(1));
        }
    }
    else if(category == 4){
        if(interpreterVector.at(3).compare("XZR") == 0){
            categoryFourSimulation(stoi(interpreterVector.at(2).substr(1)), 31, stoi(interpreterVector.at(4).substr(1)), registersVector, interpreterVector.at(1), memoryDataMap);
        }
        else{
            categoryFourSimulation(stoi(interpreterVector.at(2).substr(1)), stoi(interpreterVector.at(3).substr(1)), stoi(interpreterVector.at(4).substr(1)), registersVector, interpreterVector.at(1),memoryDataMap);
        }
    }
    publishRegisters(registersVector, file); 
    file<< '\n'; 
    file<< "Data"; 
    file<<'\n'; 
    publishMemory(memoryDataMap, file); 
    file<<'\n'; 
    if(branch != programCounter) return branch;
    return programCounter; 
}

int main(int argc, char* argv[]){
    map<string,string> opcodeInstruction = setMap();
    vector<string> vectorOfInstructions;
    vector<vector<string>> interpreterVector;   
    vector<int> registers(32); 
    map<int, int> memoryDataMap;   
    string binaryFileName = argv[1];
    ifstream binaryFile(binaryFileName);
    string line;  
    int programCounter = 64; 
    int cycle = 1; 
    bool dummyHit = false; 
    ofstream firstPart; 
    firstPart.open("disassembly.txt"); 
    ofstream secondPart; 
    secondPart.open("simulation.txt"); 
    if(binaryFile.is_open()){
        while(getline(binaryFile,line)){
            if (findCategoryType(line, dummyHit) == 0){
                string armString = line + '\t' + to_string(programCounter) + '\t' + parseCategoryZero(line) +'\n'; 
                memoryDataMap.emplace(programCounter, stoi(parseCategoryZero(line))); 
                firstPart << armString; 
            }
            else if (findCategoryType(line, dummyHit) == 1){
                vector<string> armInstruction = parsingCategoryOne(line, opcodeInstruction);
                string armString = line + '\t' +  to_string(programCounter) + '\t' +  armInstruction.at(1) + " " +  armInstruction.at(2) + ", " +  armInstruction.at(3) + '\n'; 
                vectorOfInstructions.push_back(armInstruction.at(1) + " " +  armInstruction.at(2) + ", " +  armInstruction.at(3));
                interpreterVector.push_back(armInstruction); 
                firstPart << armString; 
            }
            else if (findCategoryType(line, dummyHit) == 2){
                vector<string> armInstruction = parsingCategoryTwo(line, opcodeInstruction); 
                string armString = line + '\t' + to_string(programCounter) + '\t' +  armInstruction.at(1) + " " +  armInstruction.at(2) + ", " + armInstruction.at(3) + ", " + armInstruction.at(4) + '\n'; 
                vectorOfInstructions.push_back(armInstruction.at(1) + " " +  armInstruction.at(2) + ", " + armInstruction.at(3) + ", " + armInstruction.at(4));
                interpreterVector.push_back(armInstruction); 
                firstPart<< armString; 
            }
            else if (findCategoryType(line, dummyHit) == 3){
                vector<string> armInstruction = parsingCategoryThree(line, opcodeInstruction);
                string armString = line + '\t' + to_string(programCounter) + '\t' + armInstruction.at(1) + " " + armInstruction.at(2) + ", " + armInstruction.at(3) + ", " + armInstruction.at(4) + '\n';
                vectorOfInstructions.push_back(armInstruction.at(1) + " " + armInstruction.at(2) + ", " + armInstruction.at(3) + ", " + armInstruction.at(4));
                interpreterVector.push_back(armInstruction); 
                firstPart << armString;  
            }
            else if (findCategoryType(line, dummyHit) == 4){
                vector<string> armInstruction = parsingCategoryFour(line, opcodeInstruction);
                string armString = line + '\t' + to_string(programCounter) + '\t' + armInstruction.at(1) + " " + armInstruction.at(2) + ", [" + armInstruction.at(3) + ", " + armInstruction.at(4) +"]" + '\n'; 
                vectorOfInstructions.push_back(armInstruction.at(1) + " " + armInstruction.at(2) + ", [" + armInstruction.at(3) + ", " + armInstruction.at(4) +"]");
                interpreterVector.push_back(armInstruction); 
                firstPart << armString; 
            }
            else{
                string armString = line + '\t' + to_string(programCounter) + '\t' + "DUMMY" + '\n'; 
                vectorOfInstructions.push_back("DUMMY");
                vector<string> vec; 
                vec.push_back(to_string(5)); 
                interpreterVector.push_back(vec); 
                firstPart << armString; 
                dummyHit = true;
            }
            programCounter += 4; 
        }
        binaryFile.close();
        firstPart.close();  
    }
    int pcounter = 64;
    for(int i = 1; i<= vectorOfInstructions.size(); i++){
        int branchOrDontBranch = generateCycle(cycle, pcounter,stoi(interpreterVector[i-1][0]),vectorOfInstructions.at(i-1), registers, memoryDataMap, interpreterVector.at(i-1), secondPart); 
        if(pcounter != branchOrDontBranch){
            i = ((branchOrDontBranch-64)/4); 
            pcounter = branchOrDontBranch; 
        }
        else{
            pcounter+=4;
        }
        cycle++; 
    }
    secondPart.close(); 
    return 0; 
}