/*
 * Ellen Mackey
 * Theory of Computing Fall 2020
 * Finite State Transducer
 * 2.1b
 */

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <stdint.h>

using namespace std;

#define MASK_FIRST_PERSON   0x01
#define MASK_SECOND_PERSON  0x02
#define MASK_THIRD_PERSON   0x04
#define MASK_SINGULAR       0x08
#define MASK_PLURAL         0x10
#define MASK_ER             0x20
#define MASK_IR             0x40
#define MASK_RE             0x80
#define MASK_FEM            0x100
#define MASK_MASC           0x200
#define MASK_NEUT           0x400

uint16_t jeMask = MASK_FIRST_PERSON | MASK_SINGULAR;
uint16_t tuMask = MASK_SECOND_PERSON | MASK_SINGULAR;
uint16_t ilMask = MASK_THIRD_PERSON | MASK_SINGULAR | MASK_MASC;
uint16_t elleMask = MASK_THIRD_PERSON | MASK_SINGULAR | MASK_FEM;
uint16_t onMask = MASK_THIRD_PERSON | MASK_SINGULAR | MASK_NEUT;
uint16_t nousMask = MASK_FIRST_PERSON | MASK_PLURAL;
uint16_t vousMask = MASK_SECOND_PERSON | MASK_PLURAL;
uint16_t ilsMask = MASK_THIRD_PERSON | MASK_PLURAL | MASK_MASC;
uint16_t ellesMask = MASK_THIRD_PERSON | MASK_PLURAL | MASK_FEM;
uint16_t onsMask = MASK_THIRD_PERSON | MASK_PLURAL | MASK_NEUT;


/*
 * Function to make dictionary
 * loads a file to get verbs
 * @param[out] map<string, string> of english-french verbs
 */

map<string, string> makeDictionary(){

    //initialize translation libraries
    //regular verbs
    //map structure
    map<string, string> regVerbMap;
    
    //get file
    ifstream regVerbFile("regularVerbs.txt");
  
    if (regVerbFile.fail()){
        
        cerr << "File open failed." << endl;
        exit(1);
    }
    
    string line;
    
    //tokenize each line of the file to key and value pairs
    while(getline(regVerbFile, line)){
        
        stringstream linestream(line);
        string key, val;
        
        linestream >> key >> val;
        
        regVerbMap.insert({key, val});
        
    }
    
    regVerbFile.close();
    
    return regVerbMap;
}

/*
 ****************************************************************************
 * Function to set and validate user pronoun
 * @param[out] string pronoun
 ****************************************************************************
 */

string getPronoun(){
    
    string pronoun = "";
    
    //user enter pronoun, validate
   while (pronoun != "I" && pronoun != "i"&& pronoun != "you" && pronoun != "he"&& pronoun != "she"&& pronoun != "it"&& pronoun != "we"&& pronoun != "they"){
        cout << "Please enter a pronoun for your verb (eg. I, you, she): ";
        getline(cin, pronoun);
    } 
        
    cout << "You entered " << pronoun << '\n';
    
    return pronoun;
}

/*****************************************************************************
 * Function to get the bitmask for pronoun values
 * @param[in] string pronoun
 * @param[out] uint8_t bitmask
 * ***************************************************************************
 */

uint16_t getMask(string pronoun){
    
    uint16_t maskNum;
    string singPlural;
    string gender;
    
    if (pronoun == "you"){
        cout << "Is this you singular (enter s) or plural (enter p)?: ";
        getline(cin, singPlural);
        pronoun.append(singPlural);
    }
    
    if (pronoun == "they"){
        cout << "Would you like your 'they' to be masculine, feminine, or neutral? f for feminine, m for masculine, n for neutral: ";
        getline(cin, gender);
        pronoun.append(gender);
    }
    
    //set mask
    if (pronoun == "I" || pronoun == "i"){
        maskNum = jeMask; 
    } else if (pronoun == "yous"){ 
        maskNum = tuMask;
    } else if (pronoun == "he"){
        maskNum = ilMask;
    } else if (pronoun == "she"){
        maskNum = elleMask;
    } else if (pronoun == "it"){
        maskNum = onMask;
    } else if (pronoun == "we"){
        maskNum = nousMask;
    } else if (pronoun == "youp"){
        maskNum = vousMask;
    } else if (pronoun == "theyf"){
        maskNum = ellesMask;
    } else if (pronoun == "theym"){
        maskNum = ilsMask;
    } else if (pronoun == "theyn"){
        maskNum = onsMask;
    }
    
    return maskNum;
}

/****************************************************************************
 * Function toe translate english pronoun to french
 * @param[in] uint16_t bitMask
 * @param[out] string frenchPronoun
 ****************************************************************************
 */

string getFrenchPronoun(uint16_t bitMask){
    
    string frenchPronoun;
    
    if (bitMask == jeMask){
        
        frenchPronoun = "je";
        
    } else if (bitMask == tuMask){
        
        frenchPronoun = "tu";
        
    } else if (bitMask == ilMask){
        
        frenchPronoun = "il";
        
    } else if (bitMask == elleMask){
        
       frenchPronoun = "elle";
       
    } else if (bitMask == onMask){
        
        frenchPronoun = "on";
        
    } else if (bitMask == nousMask){
        
        frenchPronoun = "nous";
        
    } else if (bitMask == vousMask){
        
        frenchPronoun = "vous";
        
    } else if (bitMask == ilsMask){
        
        frenchPronoun = "ils";
    
    } else if (bitMask == ellesMask){
        
        frenchPronoun = "elles";
        
    } else if (bitMask == onsMask){
        
        frenchPronoun = "ons";
        
    }
    
    cout << "french pronoun is " << frenchPronoun << '\n';
    return frenchPronoun;
    
}

/********************************************************************************
 * Fundtion to get the verb for the user
 * @param[out] string userVerb
 ********************************************************************************
 */

string getVerb(){
    
    string userVerb;
    
    //enter the verb
    cout << "Please enter a verb in the present tense (eg. melt, play, act):  ";
    getline(cin, userVerb);
    cout << "You entered " << userVerb << '\n';
    
    return userVerb;
}

/********************************************************************************
 * Function to translate the verb
 * param[in] userVerb (key to find)
 * param[in] regVerbMap (map structure)
 * param[out] french translation (frenchVerb)
 *********************************************************************************/

string translateVerb(string userVerb, map<string, string> verbMap){
    
       
    //open file to write/append verbs not in the dictionary
    ofstream verbsToAdd("verbsToAdd.txt", ios_base::app);
    string frenchVerb;
   
    //check if user verb is in the dictionary
    //create the iterator
    map<string, string>::iterator it; 
                   
    it = verbMap.find(userVerb);
    
    if (it == verbMap.end()){
            
        cout << userVerb << " is not in our dictionary, but we will add it soon!\n";
        
        if (verbsToAdd.is_open()){
            
            verbsToAdd << userVerb << '\n';
            
        } else {
            cout << "can't write verb to file\n";
        }
        
    } else {
            
        frenchVerb = it->second;    

    }

    verbsToAdd.close();
    
    return frenchVerb;
}

/********************************************************************************
 *  Finite State Transducer Functions
 *  checks the french verb to find if ending is ir, er, re
 *  sets flag in bit mask for verb catagory
 *  strips last two chars and replaces with appropriate ending
 ********************************************************************************
 */

/*
 * function to replace the verb ending (conjugate)
 * param[in] string frenchVerb
 * param[in] uint16_t bitMask
 * param[out] string - conjugated verb
 */
string conjugateVerb(string frenchVerb, uint16_t bitMask){
    
    int leng = frenchVerb.length()-2;
    string ending = "";
    
    //check verb ending bit
    
    if (bitMask & (1 << 5)){                     //er bit
                                                //check singular
        if (bitMask & (1 << 3)){
            
            if (bitMask & (1 << 0)){
                ending = "e";                   //je bit
            } else if (bitMask & (1 << 1)){
                ending = "es";                  //tu bit
            } else if (bitMask & (1 << 2)){
                ending = "e";                   //il-el-on bit
            }
        } else if (bitMask & (1 << 4)){          //plural bit
            if (bitMask & (1 << 0)){
                ending = "ons";                 //nous bit
            } else if (bitMask & (1 << 1)){
                ending = "ez";                  //vous bit
            } else if (bitMask & (1 << 2)){
                ending = "ent";                 //ils-elles-ons bit
            }
        }
    } else if (bitMask & (1 << 6)){
        
        if (bitMask & (1 << 3)){                 //singular bit
            
            if (bitMask & (1 << 0)){
                ending = "is";                   //je bit
            } else if (bitMask & (1 << 1)){
                ending = "is";                   //tu bit
            } else if (bitMask & (1 << 2)){
                ending = "it";                   //il-el-on bit
            }
        } else if (bitMask & (1 << 4)){           //plural bit
            if (bitMask & (1 << 0)){
                ending = "issons";               //nous bit
            } else if (bitMask & (1 << 1)){
                ending = "issez";                //vous bit
            } else if (bitMask & (1 << 2)){
                ending = "issent";               //ils-elles-ons bit
            }
        }
    } else if (bitMask & ( 1 << 7)){
       
        if (bitMask & ( 1 << 3)){                 //singular bit
            
            if (bitMask & (1 << 0)){
                ending = "s";                   //je bit
            } else if (bitMask & (1 << 1)){
                ending = "s";                   //tu bit
            } else if (bitMask & (1 << 2)){
                ending = "";                   //il-el-on bit
            }
        } else if (bitMask & (1 << 4)){           //plural bit
            if (bitMask & (1 << 0)){
                ending = "ons";               //nous bit
            } else if (bitMask & (1 << 1)){
                ending = "ez";                //vous bit
            } else if (bitMask & (1 << 2)){
                ending = "ent";               //ils-elles-ons bit
            }
        } 
    }
    
    //strip ending
    
    string conjFrenchVerb = frenchVerb.substr(0, leng);
    
    //append ending
    
    conjFrenchVerb.append(ending);
    
    return conjFrenchVerb;
}

/*
 * function that sets the bitmask to add the verb ending bit
 * param[in] string frenchVerb
 * param[in] uint16_t bitMask
 * param[out] string = conjugated verb (from helper function)
 * calls- conjugateVerb(string, uint16_t)
 */
string acceptVerb(string frenchVerb, uint16_t bitMask){
  
    string temp;
    int len = frenchVerb.length();
     
    //accept er, ir, re verbs
    if (frenchVerb.substr(len-2) == "er"){
           
        bitMask = bitMask | MASK_ER;
        
    } else if (frenchVerb.substr(len-2) == "ir"){            
            
        bitMask = bitMask | MASK_IR;
            
    } else if (frenchVerb.substr(len-2) == "re"){
            
        bitMask = bitMask | MASK_RE;
            
    }
    
    //add the ending
    string conjFrenchVerb = conjugateVerb(frenchVerb, bitMask);
    
    return conjFrenchVerb;

}

/********************************************************************************
 *                                  main function
 *******************************************************************************/
    
int main(){
    
    //verbMap of regular verbs
    map<string, string> regVerbMap = makeDictionary();
    
    //irregular verbs- not yet supported ***PLACEHOLDER***    
     
    string quitSig;
    
    //program loop
    do {

        //get user Pronoun
        string userPronoun = getPronoun();
         
        //initialize mask    
        uint16_t maskNum = getMask(userPronoun);
           
        //translate pronoun to french
        string frenchPronoun = getFrenchPronoun(maskNum);
        
        
        //test print the mask
        //cout << "mask is " << bitset<16>(maskNum) << '\n';               
       
        cout << '\n';
        
        //get user verb
        string userVerb = getVerb();
        
        //translate user verb to french
        string frenchVerb = translateVerb(userVerb, regVerbMap);
        
        if (frenchVerb != ""){
            
            string newVerb = acceptVerb(frenchVerb, maskNum);
            
            //test print
            cout << userVerb << " is " << frenchVerb << "\n\n";
            cout << frenchPronoun << " " << newVerb << '\n';
            cout << endl;
        }
        cout << "any key to go again, q to quit: ";
        getline(cin, quitSig);
        if (quitSig == "q"){
            
            cout << "Thanks, have a great day!\n"; 
            
        } 
         
        //clear mask
        maskNum = maskNum^maskNum;
                  
        } while (quitSig != "q");
    
    
    return 0;
    
}
