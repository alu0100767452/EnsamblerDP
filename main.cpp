
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <vector>
#include <queue>
#include <tuple>
#include <bitset>



using namespace std;

int nInst=0;
vector<tuple<string, string>> etiqueta;

string DecToBin2(int number){
    std::string binary = std::bitset<2>(number).to_string();
    return binary;
}

string DecToBin4(int number){
    std::string binary = std::bitset<4>(number).to_string();
    return binary;
}



string DecToBin8bits(int number){
    string binary = std::bitset<8>(number).to_string();
    return binary;
}

string DecToBin10bits(int number){
    string binary = std::bitset<10>(number).to_string();
    return binary;
}

string InstToBinary(string reg){
    int inst = atoi(reg.c_str());
    return DecToBin10bits(inst);
}

string InmToBinary(string reg){
    int inmediato = atoi(reg.c_str());
    return DecToBin8bits(inmediato);
}

string SrelToBinary(string srel){
    string inm;
    char signo = srel[0];

    if(signo == '+'){
        signo = '0';
    }
    else if(signo == '-'){
        signo = '1';
    }

    for(int i=1; i<srel.length();i++){
        inm += srel[i];
    }
    inm = InstToBinary(inm);
    inm[0] = signo;


    return inm;
}

string PortToBinary(string port){
    string puerto;
    for(int i=4; i<port.length(); i++){
        puerto += port[i];
    }

    int p = atoi(puerto.c_str());
    return DecToBin2(p);
}

std::string RegToBinary(std::string reg){
    std::string registro;
    if(reg[0] == '$'){
        for(int i=1; i<reg.length(); i++){
            registro += reg[i];
        }
    }
    int r = atoi(registro.c_str());
    registro = DecToBin4(r);
    return registro;
}


class Parser{
public:
    std::vector<std::tuple<int,std::string>> inst;
    std::vector<std::tuple<int,std::string>> port;

public:
    Parser(){
        inst.push_back(std::tuple<int,std::string>{0,"MOVE"});   //0
        inst.push_back(std::tuple<int,std::string>{1,"ADD"});   //1
        inst.push_back(std::tuple<int,std::string>{2,"SUB"});   //2
        inst.push_back(std::tuple<int,std::string>{3,"AND"});   //3
        inst.push_back(std::tuple<int,std::string>{4,"OR"});    //4
        inst.push_back(std::tuple<int,std::string>{5,"NOTA"});  //5
        inst.push_back(std::tuple<int,std::string>{6,"NOTB"});  //6
        inst.push_back(std::tuple<int,std::string>{7,"COMPL"}); //7
        inst.push_back(std::tuple<int,std::string>{8,"LI"});    //8
        inst.push_back(std::tuple<int,std::string>{9,"J"});     //9
        inst.push_back(std::tuple<int,std::string>{10,"BIZ"});   //10
        inst.push_back(std::tuple<int,std::string>{11,"BNIZ"});  //11
        inst.push_back(std::tuple<int,std::string>{12,"OUT_IN"});//12
        inst.push_back(std::tuple<int,std::string>{13,"SREL"});  //13
        inst.push_back(std::tuple<int,std::string>{14,"JL"});    //14
        inst.push_back(std::tuple<int,std::string>{15,"RA"});    //15
        inst.push_back(std::tuple<int,std::string>{16,"OUT"});   //16
        inst.push_back(std::tuple<int,std::string>{17,"IN"});    //17
        inst.push_back(std::tuple<int,std::string>{18,"NOP"});   //18
        port.push_back(std::tuple<int,std::string>{1,"PORT1"});
        port.push_back(std::tuple<int,std::string>{2,"PORT2"});
        port.push_back(std::tuple<int,std::string>{3,"PORT3"});
        port.push_back(std::tuple<int,std::string>{4,"PORT4"});

    }

    ~Parser(){
        while(!inst.empty()){
            inst.pop_back();
        }
    }
};


void etiquetas(char* fich){
    std::fstream fs;
    string instruccion;
    queue<string> pInst;
    Parser inst;

    fs.open(fich, std::ios::in);

    fs >> instruccion;
    while(!fs.eof()){
        pInst.push(instruccion);
        if(fs.peek() == '\n'){

            string etq(pInst.front());
            pInst.pop();

            bool isEtiqueta = true;

            for(int i=0; i<inst.inst.size(); i++){
                if(etq == std::get<1>(inst.inst[i]))
                    isEtiqueta = false;
            }

            if(isEtiqueta){
                etq.erase(etq.length()-1);
                etiqueta.push_back(tuple<string,string>{to_string(nInst),etq});
            }


            while(!pInst.empty()){
                pInst.pop();
            }
            nInst++;
        }

        fs >> instruccion;
    }
}



void leerfichero(char* fich){
    std::fstream fs;
    std::queue<std::string> pInst;

    fs.open(fich, std::ios::in);
    std::fstream fs_out("progfile.dat", std::fstream::in | std::fstream::out);

    std::filebuf fb;
    fb.open ("progfile.dat",std::ios::out);
    std::ostream os(&fb);




    std::string instruccion;
    std::string reg;
    std::string inm;
    Parser inst;

    fs >> instruccion;
    while(!fs.eof()){

         pInst.push(instruccion);
         if(fs.peek() == '\n'){
             std::string instruction(pInst.front());
             std::string codificacion;



             //ELIMINAR ETIQUETAS SI PROCEDE
             bool isInst = true;
             string et = instruction;
             et = et.erase(et.length()-1);
             for(int j=0; j<etiqueta.size(); j++){
                 if(et == get<1>(etiqueta[j])){
                     isInst = false;
                     pInst.pop();
                     instruction = pInst.front();
                     break;
                 }
             }

             pInst.pop();
             int i = 0;
             int count = 0;
             //IDENTIFICADOR DE LA INSTRUCCION
             while(count < inst.inst.size()){
                 if(instruction == std::get<1>(inst.inst[count])){
                     i = std::get<0>(inst.inst[count]);
                     break;
                 }
                 count++;
             }

             switch(i){
                 case 0:
                 {
                    codificacion = "_0000";
                    std::string reg(pInst.front());
                    std::string regbinary1(RegToBinary(reg));
                    pInst.pop();
                    reg = pInst.front();
                    string regbinary2(RegToBinary(reg));
                    codificacion = "_0000_" + regbinary2 + codificacion;
                    pInst.pop();
                    codificacion = regbinary1 + codificacion;


                 }
                 break;
                 case 1:
                 {
                     codificacion = "_0010";

                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary2(RegToBinary(reg));
                     codificacion = "_" + regbinary2 + codificacion;
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary3(RegToBinary(reg));
                     codificacion = "_" + regbinary3 + codificacion;
                     codificacion = regbinary1 + codificacion;
                 }


                 break;
                 case 2:
                 {
                     codificacion = "_0011";

                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary2(RegToBinary(reg));
                     codificacion = "_" + regbinary2 + codificacion;
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary3(RegToBinary(reg));
                     codificacion = "_" + regbinary3 + codificacion;
                     codificacion = regbinary1 + codificacion;
                 }

                 break;
                 case 3:
                 {
                     codificacion = "_0100";

                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary2(RegToBinary(reg));
                     codificacion = "_" + regbinary2 + codificacion;
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary3(RegToBinary(reg));
                     codificacion = "_" + regbinary3 + codificacion;
                     codificacion = regbinary1 + codificacion;

                 }
                 break;
                 case 4:
                 {
                     codificacion = "_0101";
                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary2(RegToBinary(reg));
                     codificacion = "_" + regbinary2 + codificacion;
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary3(RegToBinary(reg));
                     codificacion = "_" + regbinary3 + codificacion;
                     codificacion = regbinary1 + codificacion;
                 }
                 break;
                 case 5:
                 {
                     codificacion = "_0110";
                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary3(RegToBinary(reg));
                     pInst.pop();
                     codificacion = "_0000_" + regbinary3 + codificacion;
                     codificacion = regbinary1 + codificacion;

                 }
                 break;
                 case 6:
                 {
                    codificacion = "_0111";
                    codificacion = "_0110";
                    std::string reg(pInst.front());
                    std::string regbinary1(RegToBinary(reg));
                    pInst.pop();
                    reg = pInst.front();
                    string regbinary3(RegToBinary(reg));
                    pInst.pop();
                    codificacion =  regbinary3 + "_0000_"  + codificacion;
                    codificacion = regbinary1 + codificacion;
                 }
                 break;
                 case 7:
                 {
                     codificacion = "_0001";
                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     reg = pInst.front();
                     string regbinary3(RegToBinary(reg));
                     pInst.pop();
                     codificacion = "_0000_" + regbinary3 + codificacion;
                     codificacion = regbinary1 + codificacion;
                 }
                 break;
                 case 8:
                 {
                     codificacion = "_1000";
                     std::string reg(pInst.front());
                     std::string regbinary1(RegToBinary(reg));
                     pInst.pop();
                     string inm = pInst.front();
                     inm = InmToBinary(inm);
                     inm.insert(4,"_");
                     pInst.pop();
                     codificacion = regbinary1 + "_" + inm + codificacion;
                 }
                 break;
                 case 9:
                 {
                    codificacion = "00_1001";
                    string inst = pInst.front();
                    inst = InstToBinary(inst.c_str());
                    inst.insert(4,"_");
                    inst.insert(9,"_");
                    pInst.pop();
                    codificacion = inst + codificacion;
                 }
                 break;
                 case 10:
                 {
                    codificacion = "x0_1010";
                    string inst = pInst.front();
                    inst = InstToBinary(inst.c_str());
                    inst.insert(4,"_");
                    inst.insert(9,"_");
                    pInst.pop();
                    codificacion = inst + codificacion;

                 }
                 break;
                 case 11:
                 {
                    codificacion = "00_1011";
                    string inst = pInst.front();
                    inst = InstToBinary(inst.c_str());
                    inst.insert(4,"_");
                    inst.insert(9,"_");
                    pInst.pop();
                    codificacion = inst + codificacion;
                 }
                 break;
                 case 12:
                 {
                    codificacion = "00_1100";
                    string inm = pInst.front();
                    inm = InmToBinary(inm.c_str());
                    inm.insert(4,"_");
                    pInst.pop();
                    string port = pInst.front();
                    port = PortToBinary(port);
                    pInst.pop();
                    codificacion = inm + "_" + port + codificacion;

                 }
                 break;
                 case 13:
                 {
                    codificacion = "11_1010";
                    string srel = pInst.front();
                    srel = SrelToBinary(srel);
                    srel.insert(4,"_");
                    srel.insert(9,"_");
                    codificacion = srel + codificacion;

                 }
                 break;
                 case 14:
                 {
                    codificacion = "11_1011";
                    string inst = pInst.front();
                    for(int i=0; i<etiqueta.size(); i++){
                        if(inst == get<1>(etiqueta[i])){
                            inst = get<0>(etiqueta[i]);
                        }
                    }
                    inst = InstToBinary(inst.c_str());
                    inst.insert(4,"_");
                    inst.insert(9,"_");
                    pInst.pop();
                    codificacion = inst + codificacion;

                 }
                 break;
                 case 15:
                    codificacion = "0000_0000_0011_1101";
                 break;
                 case 16:
                 {
                    codificacion = "11_1110";
                    std::string reg(pInst.front());
                    std::string regbinary1(RegToBinary(reg));
                    pInst.pop();
                    string port = pInst.front();
                    port = PortToBinary(port);
                    pInst.pop();
                    codificacion = "0000_" + regbinary1 + "_" + port + codificacion;

                 }
                 break;
                 case 17:
                 {
                    codificacion = "11_1111";
                    std::string reg(pInst.front());
                    std::string regbinary1(RegToBinary(reg));
                    pInst.pop();
                    string port = pInst.front();
                    port = PortToBinary(port);
                    pInst.pop();
                    codificacion = regbinary1 + "_0000_" + port + codificacion;
                 }
                 break;
                 case 18:
                 {
                    codificacion = "0000_0000_0000_0000";
                 }
                 break;
                 default:
                    //pInst.pop();
                 break;

             }

             os << codificacion << endl;


             while(!pInst.empty()){
                 pInst.pop();
             }
         }
         fs >> instruccion;
    }

    fb.close();





}

int main(int argc, char *argv[])
{

    char* fich = argv[1];
    etiquetas(fich);
    leerfichero(fich);

    while(!etiqueta.empty()){
        etiqueta.pop_back();
    }

    return 0;
}
