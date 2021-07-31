#include<iostream>
#include<iomanip>
#include<fstream>
#include<stdlib.h>
#include <conio.h>
#include<windows.h>
#include<bits/stdc++.h>
#include<vector>
#include <functional>
#include<map>
#include<set>
#include<iterator>
#include <ctime>
using namespace std;
using std::right;
using std::left;
#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 30
#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER 13
#define BACKSPACE 8
#define HASHTAG 35
//30 line in display is the viewable height
//120 character wide in the viewable display
set<string>::iterator uItr;
map<string,string>::iterator cItr;
string currUsr;
class credentials{
  protected:
    string userName;
    string hashedPassword;
    friend class variables;
    friend class program;
  public :
    void setCredentials(string a,string b){
      userName = a;
      hashedPassword = b;
    }  
    string stringHashing(string str)
    {
        hash<string> mystdhash;
        str=mystdhash(str);
        return str;
    }
};
struct readingFile{
    string date;
    string sender;
    string data;
};
typedef struct readingFile readingFile;
readingFile message;
credentials newUsr;
credentials obj;
class variables:public credentials{
  protected:
    friend class program;
    map<string,string> credslist;
    set<string> uNames;
    variables(){
      ifstream file;  
      file.open("Creds.txt", ios::in |ios::binary );
      if(!file){
        file.close();
        ofstream temp;
        temp.open("Creds.txt", ios::out |ios::binary);
        temp.close();
        file.open("Creds.txt", ios::in|ios::binary);
      }
      file.read((char*)&obj, sizeof(obj));
      while (!file.eof()) {
        uNames.insert(obj.userName);
        credslist.insert({obj.userName, obj.hashedPassword});
        file.read((char*)&obj, sizeof(obj));
      }
      file.close();

    }
};


//this custom encoding and decoding will be 
//use in writing the message in file and to avoid spaces as delimiter in reading from the file
//because of this direct readline will work, at the saem time message is safe too
class displayDriver{
  protected:
    friend class program;
    string customEncodingofString(string str){
      int len=str.length();
      for(int i=0;i<len;i++){
        str[i]+=100;
      }
      return str;
    }
    string customDecodingofString(string str){
      int len=str.length();
      for(int i=0;i<len;i++){
        str[i]-=100;
      }
      return str;
    }
    void hidecursor()
    {
      HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_CURSOR_INFO info;
      info.dwSize = 100;
      info.bVisible = FALSE;
      SetConsoleCursorInfo(consoleHandle, &info);
    }
    void EmptyLine(int count){
      while(count!=0){
        cout<<endl;
        count--;
      }
    }
    void Space(int count,char ch=' '){
      while(count!=0){
        cout<<ch;
        count--;
      }
    }
    void DisplaySingleLineMsg(string msg,int colorCode){
        //system("Color <bgcolor><textcolor>"); A - green , 0 - black,7 - white text, 4- red
        system("CLS"); //clear screen
        EmptyLine((DISPLAY_HEIGHT-2)/2);
        cout<<msg;
        if(colorCode==1){ //success
            system("Color A0");
        }
        else{ 
            system("Color 40");
        } 
        getch();
        system("Color 07");
    }
    void ExactDisplay(string array[],int aLen,string parameters[],int type,int curr=1000){
        system ("CLS"); //clears screen
        EmptyLine((DISPLAY_HEIGHT-aLen-2)/2);
        for(int i=0;i<aLen;i++){
          cout<<"\t";
          cout<<array[i];
          cout<<setw(4)<<right;
          Space(8);
          if(type==1){
            if(i==curr){
              cout<<parameters[i];
            }
          }
          if(type==2||type==3){
            if(i<=curr){
              cout<<parameters[i];
              if(i==curr)
                cout<<"_";
            }
          }
          cout<<endl;
        }
        if(type==1||type==3){
              EmptyLine((DISPLAY_HEIGHT-aLen-1)/2);
              Space(55);
              if(type==1)
                cout<<"use up arrow, down arrow, and enter";
              if(type==3)
                cout<<"press # when you finished typing the message";
        }
    }
    int keyPressing(string array[],int aLen,string parameters[],int type){  //1 - menus,  2 - with input fields  
        int curr=0; //current position of arrow
        if(type==1){
          int button;
              while(button!=ENTER)
              {
                  button = 0;
                  ExactDisplay(array,aLen,parameters,type,curr);
                  switch((button=getch())) {
                  
                  case KEY_UP:
                                //cout << endl << "Up" << endl;
                                curr=curr+aLen-1;
                                break;
                  
                  case KEY_DOWN:
                                //cout << endl << "Down" << endl;   
                                curr=curr+1;
                                break;
                  }
                  curr=curr%aLen;

              }
              
        }
        else{

              while(curr!=aLen){
                  int button = 0;
                  stringstream ss;
                  string appendingString;
                  char tempHolder;
                  int len;
                  ExactDisplay(array,aLen,parameters,type,curr);
                  switch((button=getch())){
                  
                    case HASHTAG: if(type==3&&curr==1)return 2;
                    
                    case BACKSPACE:
                                  len=parameters[curr].length();
                                  if(len!=0)
                                    parameters[curr]=parameters[curr].substr(0,len-1);
                                  break;
                    case ENTER:
                                  //cout << endl << "Up" << endl;
                                  if(type!=3||curr!=1){
                                    curr=curr+1;
                                    ExactDisplay(array,aLen,parameters,type,curr);
                                    break;
                                  }
                                  //fall through in switch
                                  
                    
                    default:
                                  tempHolder=button;
                                  ss<<tempHolder;
                                  ss>>appendingString;
                                  if(type==3)
                                  {
                                    appendingString[0]=tempHolder;
                                    switch(button){
                                      case 13: appendingString="\n";break;
                                      case 9: appendingString="\t";break;
                                      case 32: appendingString=" ";break;
                                    }
                                    
                                    parameters[curr].append(appendingString);
                                    ss.clear();
                                    break;
                                  }
                                  else if(appendingString!=" "||appendingString!="\t")
                                    parameters[curr]=parameters[curr]+appendingString;
                                  ss.clear();
                                  break;
                  }
              }
        }
        return curr;
    }
};
class program:public displayDriver, public variables{
  protected:
    string menu[5];
    string para[5];
    vector<string> msgs;
    vector<string>::iterator vItr;
      void PrintingTheCredentialsFile(){
            fstream file; 
            file.open("Creds.txt",ios::out|ios::binary);
            for(auto mItr=credslist.begin();mItr!=credslist.end();mItr++){
              obj.setCredentials(mItr->first,mItr->second);
              file.write((char*)&obj, sizeof(obj));

            }
            file.close();
            // file.open("Creds.txt",ios::binary|ios::in);
            // file.read((char*)&obj, sizeof(obj));
            // while (!file.eof()) {
            //   cout<<obj.userName<<"     "<<obj.hashedPassword<<endl;
            //   uNames.insert(obj.userName);
            //   credslist.insert({obj.userName, obj.hashedPassword});
            //   file.read((char*)&obj, sizeof(obj));
            // }
            // getch();
            // getch();
            file.close();
      } 
    void printVector(){
      for(auto i=msgs.begin();i!=msgs.end();i++){
        cout<<(*i)<<endl;
      }
    }
    void resetParameters(int pageCode){ //1-menu,2-login,3-signup,4-aftersignup 5-sending message page
        string m1,m2,m3,m4,m5; //for menus
        string p1,p2,p3,p4,p5; //for parameters
        if(pageCode==1){
          m1="Login";m2="Sign Up";m3="Exit";
          p1="<-";
          menu[0]=m1;menu[1]=m2;menu[2]=m3;
          para[0]=p1;para[1]=p1;para[2]=p1;
        }
        else if(pageCode==2){
          m1="Username";m2="Password";
          p1="";
          menu[0]=m1;menu[1]=m2;
          para[0]=p1;para[1]=p1;
        }
        else if(pageCode==3){
          m1="Username";m2="Password";m3="Re Enter Password";
          p1="";
          menu[0]=m1;menu[1]=m2;menu[2]=m3;
          para[0]=p1;para[1]=p1;para[2]=p1;
        }
        else if(pageCode==4){
          m1="Send Message";m2="Read Recieved Messages";m3="Log Out";
          p1="<-";
          menu[0]=m1;menu[1]=m2;menu[2]=m3;
          para[0]=p1;para[1]=p1;para[2]=p1;
        }
        else if(pageCode==5){
          m1="Reciever's Username";m2="Message";
          p1="";
          menu[0]=m1;menu[1]=m2;
          para[0]=p1;para[1]=p1;
        }
    }
    void loadVector(string fname){
      //struct readingFile var;
      fstream file1;
      string s;
      file1.open(fname,ios::in|ios::binary);
      getline(file1,s,'#');
      while (!file1.eof()) {
        msgs.push_back(s);
        //cout<<"In load"<<var.data<<var.date<<var.sender<<endl;getch();getch();
         getline(file1,s,'#');
      } 
      file1.close();  
    }
    void unloadVector(string fname){
      //struct readingFile var;
      string s;
      fstream file2;
      file2.open(fname,ios::out|ios::binary);
      for(int i=0;i!=msgs.size();i++){
        // var.data=i->data;
        // var.date=i->date;
        // var.sender=i->sender;
        // cout<<(*i).date<<"    "<<(*i).data<<"      "<<(*i).sender<<endl;
        s=msgs[i]+"#";
        file2<<s;
      }
      file2.close();
      cout<<"\n\nWritten in file";getch();
    }
    void SendMessage(){
      int success;
      string receiver;
      time_t now = time(0);
      resetParameters(5);    
      success=keyPressing(menu,2,para,3);
      uItr=uNames.find(para[0]);
      if(uItr==uNames.end()){
        string str="User doesnt exist !";
        DisplaySingleLineMsg(str,0);
        return;
      }
      //cout<<para[0]<<"\n\n"<<para[1]<<endl;getch();getch();
      char* dt = ctime(&now);
      message.data=para[1];
      //message.data=customEncodingofString(para[1]);
      message.sender=currUsr;
      string s=currUsr+" : "+para[1]+"#";
      message.date="random";              //needs to be fixed later
      // cout<<endl;
      // cout<<message.data<<endl;
      // cout<<message.date<<endl;
      // cout<<message.sender<<endl;
      // getch();
      receiver=para[0]+".txt";
      //cout<<"Before Loading Vector : \n";
      //printVector();
      loadVector(receiver);
      msgs.push_back(s);
      //cout<<"After Pushing and Loading Vector : \n";
      //printVector();
      unloadVector(receiver);
      //cout<<"After unloading Vector : \n";
      //printVector();
      //getch();getch();
      msgs.clear();   
      string str="Message Successfully Sent !";
      DisplaySingleLineMsg(str,1);
    }
    void ReadMessage(){
      system("CLS");
      string temp=currUsr+".txt";
      loadVector(temp);
      int count=0;
      system("Color E0");
      for(int i=0;i!=msgs.size();i=i+2){
        
        // message.data=(*i).data;
        // message.date=(*i).date;
        // message.sender=(*i).sender;
        EmptyLine(2);
        temp=msgs[i];
        cout<<temp;
        // cout<<message.date<<endl;
        // cout<<" From "<<message.sender<<" ,"<<endl;
        // //message.data=customDecodingofString(message.data);
        // cout<<"\t"<<message.data;
        EmptyLine(2);
        Space(120,'_');
      }
      getch();
      system("Color 07");
      msgs.clear();
    }
    void MessagingPage(){
      int ret;
      resetParameters(4);
      ret=keyPressing(menu,3,para,1);  
      do{
        switch(ret){
          case 0: SendMessage();break;//call the function for login
          case 1: ReadMessage();break;//call the function for signup
          case 2: DisplaySingleLineMsg("Successfully Logged Out !",1);getch();return;//display thank you and exit
        }
        resetParameters(4);
        ret=keyPressing(menu,3,para,1);
      }while(ret!=2);

    }
    void Login(){
      int success;
      resetParameters(2);    
      try{
          success=keyPressing(menu,2,para,2);

          if(success!=2){
            throw("Bug in the code. Contact your administrator");
          }           
      }
      catch(string msg){
          cout<<msg<<endl;
          getch();
          exit(0);
      }
      uItr=uNames.find(para[0]);
      cItr=credslist.find(para[0]);
      para[1]=stringHashing(para[1]);
      if(uItr==uNames.end()){
        string str="Incorrect Credentials ! Check your username and password !";
        DisplaySingleLineMsg(str,0);
        return;
      }
      else if(cItr->second==para[1]){
        string str="Login Successfull !";
        DisplaySingleLineMsg(str,1);
        currUsr=para[0];
        MessagingPage();
        //successful login
        //call the read ad send function
        //remove the name from set
        //add it back after logout 
      }
      else{
        string str="Incorrect Credentials ! Check your username and password !";
        DisplaySingleLineMsg(str,0);
        return;
      }
    }
    void Signup(){
      int success;
      resetParameters(3);  
      try{
          success=keyPressing(menu,3,para,2);
          if(success!=3){
            throw("Bug in the code. Contact your administrator");
          }           
      }
      catch(string msg){
          cout<<msg<<endl;
          getch();
          exit(0);
      }
      uItr=uNames.find(para[0]);
      //cItr=credslist.find(para[0]); this will no exist
      if(para[1].length()<1||para[2].length()<1||para[0].length()<1){ 
        string str="Fill the details properly !";
        DisplaySingleLineMsg(str,0);
        return;
      }
      if(uItr!=uNames.end()){
        string str="Username already exists, try another username !";
        DisplaySingleLineMsg(str,0);
        return;
      }
      else if(para[1]!=para[2]){
        string str="The passwords doesnt match !";
        DisplaySingleLineMsg(str,0);
        return;
      }
      //cout<<para[0]<<para[1]<<para[2]<<endl;getch();
      ofstream file;//creating a file to store messages
      currUsr=para[0]+".txt";
      file.open(currUsr,ios::out|ios::binary);
      file.close();
      newUsr.setCredentials(para[0],stringHashing(para[1]));
      credslist.insert({para[0],stringHashing(para[1])});
      uNames.insert(para[0]);
      string str="Account Successfully Created !";
      DisplaySingleLineMsg(str,1);
      PrintingTheCredentialsFile();

    }
    public:
    void execution(){
      int ret;
      resetParameters(1);
      ret=keyPressing(menu,3,para,1);
      do{
        switch(ret){
          case 0: Login();break;//call the function for login
          case 1: Signup();break;//call the function for signup
          case 2: DisplaySingleLineMsg("Thanks for Using",1);getch();return;//display thank you and exit
        }
        getch();
        resetParameters(1);
        ret=keyPressing(menu,3,para,1);
      }while(ret!=2);
    }
    program(){
      hidecursor();
    }
};
int main(){
   
  program T;
  T.execution();
  return 0;
}