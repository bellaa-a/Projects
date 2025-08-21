//Zi Yun Mao zmao16
//Bella Lu xlu62
//Ava Venuti avenuti1

#include <string>
#include <sstream>
#include <type_traits>
#include <vector>
#include <memory>
#include <deque>
#include <cmath>
#include <cassert>
#include "expr.h"
#include "expr_parser.h"
#include "exception.h"


using std::string;
using std::stringstream;
using std::cout;
using std::endl;

//Default constructor
ExprParser::ExprParser() {
 root = nullptr;
 curIndx = 0; 
}

//Destructor
ExprParser::~ExprParser() {
   clear(root);
   delete root;
}

//Recersive function that deletes all nodes under the parameter without deleting the parameter itself
//Helper function for the destructor
Expr * ExprParser::clear(Expr * cur) {
   if (cur->isX || cur->isNum || cur->isPi || cur->isInvalid){ return cur; }
   for (int i = 0; i < (int)cur->exprNodes.size(); i++){
       Expr *temp = cur->exprNodes[i];
       delete clear(temp);
   }
   return cur;
}

//Converts the current node to a string encoding 
void ExprParser::tostring(Expr * cur){
   if(cur->isSin)
       cout << "sin " << endl;
   else if (cur->isCos)
       cout << "cos "<< endl;
   else if(cur->isPlus)
       cout << "+ "<< endl;
   else if(cur->isMinus)
       cout << "- "<< endl;
   else if(cur->isMul)
       cout << "* "<< endl;
   else if(cur->isDiv)
       cout << "// "<< endl;
   else if(cur->isX)
       cout << "x "  << endl;
   else if (cur->isPi)
       cout << "pi "<< endl;
   else
       cout << cur->num << endl;
}

//Helper function used in debugging
//Prints the entire tree 
void ExprParser::print(Expr * cur){
   if(cur == root){
       cout << "root: ";
       tostring(root);
   }
   if (cur == nullptr){
        return;
   }
   for (int i = 0; i < (int)cur->exprNodes.size(); i++){
       tostring(cur->exprNodes[i]);
       Expr *temp = cur->exprNodes[i];
       print(temp);
   }
}

//Calculate the y value given an Expr and its corresponding x value
double ExprParser::calculate(Expr * cur, double x){
   double sum = 0.00;

   //this is used to prevent minusing, dividing, or multiplying 0 by itself 
   bool firstMin =true;
   bool firstDiv =true;
   bool firstMul =true;

    //Base case: nodes which are x, pi, a literal numeric value, and invalid expression
   if(cur->isX){
       return x;
   }
   else if (cur->isPi){
       return M_PI;
   }
   else if (cur->isNum){
       return cur->num;
   }
   else if (cur->isInvalid){
        throw PlotException("invalid token from expression");
   }
    //handling the case where * is the only node 
   if(cur == root && cur->exprNodes.size() == 0 && cur->isMul){
        return 1; 
   }

    //Traverses the exprNodes and performs the corresponding calculations on the recursive call
   for (int i = 0; i < (int)cur->exprNodes.size(); i++)
   {
       if (cur->isSin)
       {
           Expr *temp = cur->exprNodes[i];
           sum = sin(calculate(temp, x));
       }
       else if (cur->isCos){
           Expr *temp = cur->exprNodes[i];
           sum = cos(calculate(temp, x));
       }
       else if(cur->isPlus){
           Expr *temp = cur->exprNodes[i];
           sum += (calculate(temp, x));
       }
       else if(cur->isMinus){
           Expr *temp = cur->exprNodes[i];
            //If first time minusing, should assign sum to value of recursive call instead of sum -=
           if (firstMin){
               sum = (calculate(temp, x));
               //Sets firstMin to false because now we have seen a minus sign
               firstMin=false;
           }
           else{
                //normal operation ofter the first minus 
               sum -= (calculate(temp, x));
           }
       }
       else if(cur->isMul){
           Expr *temp = cur->exprNodes[i];
           if(firstMul){
                sum = (calculate(temp, x));
                firstMul = false; 
           }
           else{
                sum *= (calculate(temp, x));
           }
       }

       else if(cur->isDiv){
           Expr *temp = cur->exprNodes[i];
           if(firstDiv){
                sum = (calculate(temp, x));
                firstDiv = false;    
           }
           else {
                double result = (calculate(temp, x));
                //Cannot divide by zero
                if(result == 0){
                    throw PlotException("attempt to divide by 0"); 
                }
                else sum /= result;
            }
       }
   }
   return sum;
}

//checking whether the input is a valid double  
bool isDouble(string myString) {
   std::istringstream iss(myString);
   double f;
   iss >> f;
   return !iss.fail();
}

//read the file and generat a string vector with expressions 
void ExprParser::storeStrVec(std::istream &in){
    string expressions; 
    getline(in, expressions); 
    stringstream expressions2(expressions); 
    string expr; 
    while(expressions2 >> expr){
        strVec.push_back(expr); 
    }
}

//prints out the string vector for debugging purposes  
void ExprParser::printStrVec(){
    for(int i = 0; i < (int)strVec.size(); i++){
        cout << strVec[i] << endl; 
    }
}


//Building a tree of expression recursively 
Expr * ExprParser::parse(){
   string expression;
   //traversing through all the expressions stored in a vector  
   expression = strVec[curIndx];
   //incurmenting the current index which is stored inside the class 
   curIndx++; 
    if(expression == "("){
        //this is dealing with the case with extra layars of brackets: ((x)) 
        if(strVec[curIndx] == "("){
            Expr * temp = parse(); 
            if(strVec[curIndx] != ")"){
                throw PlotException("open and close brakets not matched");
            }
            curIndx ++; 
            return temp; 
        }
        //if not dealing with this special case, just return the child nodes  
        else return parse();
    }

    //terminates the loop from the previous node 
    else if(expression == ")"){
        return nullptr;
    }

    //this case applies for +, -, *, /, sin, cos 
    else if(expression == "+"){ 
        //creading a new node and setup its property 
        Expr *cur = new Expr();
        cur->isPlus = true;
        //if this is the first node, assign it as root 
        if(root == nullptr){
            root = cur;
            //handle the case when there's only one expression in the vector: fn1 + or fn1 (+)
            if(curIndx == (int)strVec.size()){
                return cur; 
            }
        }
        //parse through the function until it reaches a closing braket or invalid expression 
        Expr *temp = parse();
        while (temp != nullptr && !temp->isInvalid){
            //storing the nodes into a vector 
            cur->exprNodes.push_back(temp);
            temp = parse();
        }
        return cur;
    }

    else if(expression == "-") {
        Expr * cur = new Expr();
        cur->isMinus = true;
        if(root == nullptr){
            root = cur;
            if(curIndx == (int)strVec.size()){
                return cur; 
            }
        }
        Expr * temp = parse();
        while (temp != nullptr && !temp->isInvalid){
            cur->exprNodes.push_back(temp);
            temp = parse();
        }
        return cur;
    }

    else if(expression == "*") { 
        Expr * cur = new Expr();
        cur->isMul = true;
        if(root == nullptr){
            root = cur;
            if(curIndx == (int)strVec.size()){
                return cur; 
            }
        }
        Expr *temp = parse();
        while (temp != nullptr && !temp->isInvalid){
            cur->exprNodes.push_back(temp);
            temp = parse();
        }
        return cur;
    }
    else if(expression == "/"){ 
        Expr * cur = new Expr();
        cur->isDiv = true;
        if(root == nullptr){
            root = cur;
            if(curIndx == (int)strVec.size()){
                return cur; 
            }
        }
        Expr *temp = parse();
        while (temp != nullptr && !temp->isInvalid){
            cur->exprNodes.push_back(temp);
            temp = parse();
        }
        return cur;
    }

    else if(expression == "sin"){ 
        Expr * cur = new Expr();
        cur->isSin = true;
        if(root == nullptr){
            root = cur;
            if(curIndx == (int)strVec.size()){
                return cur; 
            }
        }
        Expr *temp = parse();
        while (temp != nullptr && !temp->isInvalid){
            cur->exprNodes.push_back(temp);
            temp = parse();
        }
        return cur;
    }
    
    else if(expression == "cos"){
        Expr * cur = new Expr();
        cur->isCos = true;
        if(root == nullptr){
            root = cur;
            if(curIndx == (int)strVec.size()){
                return cur; 
            }
        }
        Expr *temp = parse();
        while (temp != nullptr && !temp->isInvalid){
            cur->exprNodes.push_back(temp);
            temp = parse();
        }
        return cur;
    }

     //this case applies for pi, x, num, invalid 
    else if(expression == "pi"){
        Expr * cur = new Expr();
        cur->isPi = true;
        if(root == nullptr){
            //handling the case where there's only one node 
            // fn1 pi or fn1 (pi)
            if(curIndx == (int)strVec.size() || strVec[curIndx] == ")"){
                root = cur; 
            }
            else {
                root = cur; 
                throw PlotException("invalid expression"); 
            }
        }
        //for pi, x, num, we just return to the previous node because they cannot have children nodes 
        return cur;
    }

    else if(expression == "x"){
        Expr * cur = new Expr();
        cur->isX = true;
        if(root == nullptr){
            if(curIndx == (int)strVec.size() || strVec[curIndx] == ")"){
                root = cur; 
            }
            else {
                root = cur; 
                throw PlotException("invalid expression"); 
            }
        }
        return cur;
    }

    else{
        //check whether the expression is a valid number  
        if(isDouble(expression)){
            Expr * cur = new Expr();
            cur->isNum = true;
            cur->num = std::stod(expression);
            if(root == nullptr){
                if(curIndx == (int)strVec.size() || strVec[curIndx] == ")"){
                    root = cur; 
                }
                else {
                    root = cur; 
                    throw PlotException("invalid expression"); 
                }
            }
            return cur;
        }
        else {
            //if non of the expression case is fulfilled, then it would be an invalid expression 
            Expr * cur = new Expr();
            cur->isInvalid = true; 
            if(root == nullptr){
                if(curIndx == (int)strVec.size() || strVec[curIndx] == ")"){
                    root = cur; 
                }
                else {
                    root = cur; 
                    throw PlotException("invalid expression"); 
                }
            }
            return cur; 
        }
    }
    return root;
}

//recursively traversing the tree and check whether the 
//number of children nodes for different types of expressions fullfill the requirement 
void ExprParser::checkError(Expr * cur){
    if (cur == nullptr){
        return;
    }

   for (int i = 0; i < (int)cur->exprNodes.size(); i++){
        //checking the size of the exprArray 
        if((cur->isMinus || cur->isDiv) && (int)cur->exprNodes.size() != 2){
            throw PlotException("incorrect number of elements for minus or divide"); 
        }
        if((cur->isCos || cur->isSin) && (int)cur->exprNodes.size() != 1){
            throw PlotException("incorrect number of elements for sin or cos"); 
        }
       Expr *temp = cur->exprNodes[i];
       checkError(temp);
   }
}

//traversing through the strVec and see if the number of opened and closed brackets matched 
void ExprParser::checkBracket(){
    int numOpen = 0; 
    int numClosed = 0; 
    for(int i = 0; i < (int)strVec.size(); i++){
        if(strVec[i] == "("){
            numOpen++; 
        }
        if(strVec[i] == ")"){
            numClosed++; 
        }
    }
    if(numOpen != numClosed){
         throw PlotException("open and close brakets not matched");
    }
}