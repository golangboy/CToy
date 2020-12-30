#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stack>



using namespace std;

namespace Lex {
	int cur = 0;
	const string srcCode =
            "func int  main()"
            "{"
            "int *abc;"
            "int cc;"
            "abc = Alloc(4);"
            "*abc=123;"
            "Free(abc);"
            "}";
	enum TokenSign {
		Add,
		Sub,
		Div,
		Mul,
		And,
		LeftParen,
		RightParen,
		LeftBra,
		RightBra,
		FEof,
		Comma,
		Inc,
		Dec,
		Semi,
		LeftMid,
		RightMid,
		Assign,
		Num,
		ID,
		Int,
		Equal,
		If,
		Else,
		ElseIf,
		While,
		Break,
		Print,
		Ret,
		Func,
		Str,
		Alloc,
		Free,
	};

	pair<int, string> GetToken() {
		char ch = srcCode[cur];
		int k = cur;
		if (cur == srcCode.size())
			return make_pair(TokenSign::FEof, "");
		if (ch == ' ') {
			while (cur < srcCode.size() && srcCode[cur] == ' ') {
				cur++;
			}
			if (cur < srcCode.size()) {
				ch = srcCode[cur];
			}
			else {
				return make_pair(TokenSign::FEof, "");
			}
		}
		k = cur;
		if (ch >= '0' && ch <= '9') {
			while (srcCode[k] >= '0' && srcCode[k] <= '9') {
				k++;
			}
			return make_pair(TokenSign::Num, srcCode.substr(cur, k - cur));
		}
		else if (ch == '\"') {
			string outStr;
			for (int j = cur; srcCode[j]; j++) {
				outStr += srcCode[j];
				if (j != cur && srcCode[j] == '\"')
					break;
			}
			return make_pair(TokenSign::Str, outStr);
		}
		else if (cur + 4 - 1 <= srcCode.size() - 1 && srcCode.substr(cur, 4) == "func") {
			return make_pair(TokenSign::Func, "func");
		}
		else if (cur + 6 - 1 <= srcCode.size() - 1 && srcCode.substr(cur, 6) == "return") {
			return make_pair(TokenSign::Ret, "return");
		}
		else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
			if (k + 3 <= srcCode.size()) {
				if (srcCode.substr(k, 3) == "int")
					return make_pair(TokenSign::Int, "int");
			}
			while ((srcCode[k] >= 'a' && srcCode[k] <= 'z') || (srcCode[k] >= 'A' && srcCode[k] <= 'Z') ||
				(srcCode[k] >= '0' && srcCode[k] <= '9')) {
				k++;
			}
			string tokenName = srcCode.substr(cur, k - cur);
			if(tokenName == "Alloc"){
			    return make_pair(TokenSign::Alloc,"Alloc");
			}
			if(tokenName == "Free"){
			    return make_pair(TokenSign::Free,"Free");
			}
			if (tokenName == "if")
				return make_pair(TokenSign::If, "if");
			if (tokenName == "else") {
                //else if
                string tmp;
                int tmpK=k++;

                while ((srcCode[k] >= 'a' && srcCode[k] <= 'z')){
                    tmp+=srcCode[k];
                    k++;
                }
                if(tmp == "if"){
                    return make_pair(TokenSign::ElseIf,"else if");
                }
                k = tmpK;
                return make_pair(TokenSign::Else, "else");
            }
			if (tokenName == "while") {
				return make_pair(TokenSign::While, "while");
			}
			if (tokenName == "break") {
				return make_pair(TokenSign::Break, "break");
			}
			if (tokenName == "print") {
				return make_pair(TokenSign::Print, "print");
			}
			return make_pair(ID, srcCode.substr(cur, k - cur));
		}
		else if (ch == '(') {
			return make_pair(TokenSign::LeftParen, "(");
		}
		else if (ch == ')') {
			return make_pair(TokenSign::RightParen, ")");
		}
		else if(ch=='['){
		    return make_pair(TokenSign::LeftBra,"[");
		}
        else if(ch==']'){
            return make_pair(TokenSign::RightBra,"]");
        }
		else if (ch == '+') {
			if (srcCode[cur + 1] == '+') {
				return make_pair(TokenSign::Inc, "++");
			}
			return make_pair(TokenSign::Add, "+");
		}
		else if (ch == '*') {
			return make_pair(TokenSign::Mul, "*");
		}
		else if (ch == '/') {
			return make_pair(TokenSign::Div, "/");
		}
		else if (ch == '-') {
			return make_pair(TokenSign::Sub, "-");
		}
		else if (ch == '&') {
			return make_pair(TokenSign::And, "&");
		}
		else if (ch == ',') {
			return make_pair(TokenSign::Comma, ",");
		}
		else if (ch == ';') {
			return make_pair(TokenSign::Semi, ";");
		}
		else if (ch == '{') {
			return make_pair(TokenSign::LeftMid, "{");
		}
		else if (ch == '}') {
			return make_pair(TokenSign::RightMid, "}");
		}
		else if (ch == '=') {
			if (cur + 1 <= srcCode.size() - 1 && srcCode[cur + 1] == '=') {
				return make_pair(TokenSign::Equal, "==");
			}
			return make_pair(TokenSign::Assign, "=");
		}
	}

	void SkipToken(string t) {
	    cout<<"Token:"<<t<<endl;
		cur += (t.size());
	}

	void Match(TokenSign tokenSign) {
		pair<int, string> c = GetToken();
		if (tokenSign != c.first) {
			cout << "Not Match " << c.second << endl;
			exit(0);
		}
		SkipToken(c.second);
	}

}

class VirtualMachine {
public:
	enum Ins {
		Ent, PushAx, Add, Sub, Mul, Div, Li, Si, Imm, Adj, Lev, Call, LoadParam, Exit, Push, Jz, Jmp, Print, Equ,Alloc,Free
	};
private:
	vector<int> text;
	vector<int> stack;

	//Register
	int AX, BP, SP;

public:
	VirtualMachine() {
		stack = vector<int>(3000, 0);
		BP = SP = 3000;
		AX = 0;
	}

	void Debug() {
		this->text.push_back(Ins::Exit);

		this->PushVal(1);
		this->PushVal(2);
		this->PushVal(3);
		this->PushVal(0);
	}

	void Debug2() {
		this->PushVal(1);
		this->PushVal(2);
		this->PushVal(3);
		text.push_back(Ins::Call);
		text.push_back(3);
		text.push_back(Ins::Exit);
		//CALL 2
		//EXIT
	}

private:
	void PushVal(int n) {
		this->stack[--SP] = n;
	}

	int PopVal() {
		return stack[SP++];
	}

public:
	void StartFunction(int start) {
		int cur = this->GetTextSize();
		text.push_back(Ins::Push);
		text.push_back(0);
		text.push_back(Ins::Call);
		text.push_back(start);
		text.push_back(Ins::Exit);
		cout << "Run Start in " << start << endl;
		PrintASM();
		Run(cur);
	}

	void AtText(int i, int val) {
		text[i] = val;
	}

	void PrintASM() {
		int i = 0;
		for (; i < text.size(); i++) {
			cout << "PC:" << i << " ";
			if (text[i] == Ins::Ent) {
				cout << "Ent " << text[i + 1] << endl;
				i++;
			}
			else if (text[i] == Ins::PushAx) {
				cout << "PushAx" << endl;
			}
			else if(text[i] == Ins::Mul){
			    cout<<"Mul"<<endl;
			}
			else if (text[i] == Ins::Add) {
				cout << "Add" << endl;
			}
			else if (text[i] == Ins::Sub) {
				cout << "Sub" << endl;
			}
			else if (text[i] == Ins::Li) {
				cout << "Li" << endl;
			}
			else if (text[i] == Ins::Imm) {
				cout << "Imm " << text[i + 1] << endl;
				i++;
			}
			else if (text[i] == Ins::Adj) {
				cout << "Adj " << text[i + 1] << endl;
				i++;
			}
			else if (text[i] == Ins::Lev) {
				cout << "Lev" << endl;
			}
			else if (text[i] == Ins::Call) {
				cout << "Call " << text[i + 1] << endl;
				i++;
			}
			else if(text[i] == Ins::Alloc){
			    cout<<"Alloc"<<endl;
			}else if(text[i] == Ins::Free){
                cout<<"Free"<<endl;
			}
			else if (text[i] == Ins::LoadParam) {
				cout << "LoadParam " << text[i + 1] << endl;
				i++;
			}
			else if (text[i] == Ins::Exit) {
				cout << "Exit" << endl;
			}
			else if (text[i] == Ins::Push) {
				cout << "Push" << endl;
			}
			else if (text[i] == Ins::Jz) {
				cout << "Jz " << text[i + 1] << endl;
				i++;
			}
			else if (text[i] == Ins::Jmp) {
				cout << "Jmp " << text[i + 1] << endl;
				i++;
			}
			else if (text[i] == Ins::Si) {
				cout << "Si" << endl;
			}
			else if (text[i] == Ins::Print) {
				cout << "Print" << endl;
			}
			else if (text[i] == Ins::Equ) {
				cout << "Equal" << endl;
			}
			else {
				cout << "UnKnow OpCode:" << text[i] << endl;
			}
		}
	}

	void Run(int start) {
		int pc = start;
		while (true) {
			int opCode = text[pc++];
			if (opCode == Ins::Exit)
				exit(AX);
			else if (opCode == Ins::Imm) {
				this->AX = this->text[pc++];
			}
			else if (opCode == Ins::Add) {
				this->AX = this->stack[SP++] + this->AX;
			}
			else if (opCode == Ins::Div) {
				this->AX = this->stack[SP++] / this->AX;
			}
			else if (opCode == Ins::Mul) {
				this->AX = this->stack[SP++] * this->AX;
			}
			else if (opCode == Ins::Sub) {
				this->AX = this->stack[SP++] - this->AX;
			}
			else if (opCode == Ins::PushAx) {
				this->PushVal(AX);
			}
			else if(opCode == Ins::Alloc){
			    this->AX = (int )new char[AX];
			}else if(opCode == Ins::Free){
			    delete [](char*)this->AX;
			}
			else if (opCode == Ins::Li) {
				this->AX = *((int*)AX);
			}
			else if (opCode == Ins::Si) {
				*((int*)this->stack[SP++]) = AX;
			}
			else if (opCode == Ins::Ent) {
				//push bp
				this->PushVal(BP);
				//mov bp , sp
				BP = SP;
				//local params
				SP -= (text[pc++]);
			}
			else if (opCode == Ins::Lev) {
				//mov sp,bp;
				//pop bp;
				SP = BP;
				BP = PopVal();
				pc = PopVal();
			}
			else if (opCode == Ins::LoadParam) {
				AX = (int)&stack[BP - text[pc++]];
				//cout << *(int *) AX << " " << AX << endl;
			}
			else if (opCode == Ins::Push)
				PushVal(text[pc++]);
			else if (opCode == Ins::Call) {
				PushVal(pc + 1);
				pc = text[pc];
			}
			else if (opCode == Ins::Adj) {
				SP += (text[pc++]);
			}
			else if (opCode == Ins::Jz) {
				if (AX == 0) {
					pc = text[pc];
				}
				else {
					pc++;
				}
			}
			else if (opCode == Ins::Jmp) {
				pc = text[pc];
			}
			else if (opCode == Ins::Print) {
				int paramCnt = 0;
				paramCnt = PopVal();
				int paramArrays[100]{ 0 };
				for (int i = 0; i < paramCnt; i++) {
					int t;
					t = PopVal();
					paramArrays[i] = t;
				}
				/*
				for (int j = 0; j < paramCnt; j++) {
					__asm {
						push paramArrays[j];
					}
				}
				*/
				__asm {
					mov edx, paramCnt;

					mov ecx, 0;
					lea ebx, paramArrays;

				s:
					mov eax, [ebx + ecx * 4];
					push eax;
					add ecx, 1;
					cmp ecx, edx;
					jnz s;
				}

				__asm {
					call printf
					mov eax, paramCnt
					mov ebx, 4
					mul ebx
					add esp, eax
				}
				//cout << this->AX << endl;
			}
			else if (opCode == Ins::Equ) {
				int tk = stack[SP++];
				AX = (tk == AX);

			}
			else {
				exit(0);
			}
		}

	};

public:

	int GetTextSize() {
		return this->text.size();
	}

	bool IsCurRightValue() {
		return text[text.size() - 1] == VirtualMachine::Li;
	}

	void PopTopOp() {
		if (!this->text.empty()) {
			this->text.pop_back();
		}
	}

	void Append(int ins) {
		text.push_back(ins);
	}


	void LoadIntInAx(int n) {
		text.push_back(Ins::Imm);
		text.push_back(n);
	}

	//ent 3
	void EnterFunction(int argumentParamNum) {
		text.push_back(Ins::Ent);
		text.push_back(argumentParamNum);
	}

	//lev
	void LeaveFunction() {
		text.push_back(Ins::Lev);
	}

	//idx Start From Left to Right (Bytes)
	//-8 -12 -14
	void LoadTheFormalParamAddressInAx(int idx, int formalParamNum) {
		text.push_back(Ins::LoadParam);
		text.push_back(-2 - (formalParamNum - 1 - idx));
	}

	//0 4 8
	void LoadLocalVarAddressInAx(int idx) {
		text.push_back(Ins::LoadParam);
		text.push_back(idx + 1);
	}

	//call xx
	void CallFunction(string funcName, int address) {
		text.push_back(Ins::Call);
		//funcRePosition.push_back(make_pair(funcName, this->GetTextSize()));
		text.push_back(address);
	}

	//adj x
	void AdjStack(int n) {
		text.push_back(VirtualMachine::Adj);
		text.push_back(n);
	}

	//push param
	void PushValueInStack(int n) {
		text.push_back(Ins::Push);
		text.push_back(n);
	}

	//push Ax in to Stack
	void PushAxInStack() {
		text.push_back(Ins::PushAx);
	}

};

VirtualMachine virtualMachine;

namespace Gram {
    enum VarType{
        Int,
        IntPtr=10
    };
	vector<pair<string, int>> repair_var, repair_func;
	map<string, int> func_addr;
	map<string, int> global_var;
	map<string, int> local_var;
	map<string, int> local_param;
	map<string,int> local_paramVarType;
	map<string,int> local_varType;
	map<string,int> global_varType;
	vector<int> breakPos, returnPos;
	vector<char*> strPtr;

	int resultType=0;
	int String2Int(string s) {
		int ret;
		stringstream ss(s);
		ss >> ret;
		return ret;
	}

	int parseMulSign(){
	    int cnt=0;
	    if(Lex::GetToken().first == Lex::Mul){
	        cnt++;
	        Lex::Match(Lex::Mul);
	    }
	    return cnt;
	}

	char* AllocStr(string s) {
		int len = s.size();
		char* ret = new char[len + 1];
		memcpy(ret, s.data(), len);
		ret[len] = 0;
		strPtr.push_back(ret);
		return ret;
	}


	int GetPriority(string p) {
		if (p == "=") {
			return 0;
		}
		else if (p == "==") {
			return 1;
		}
		else if (p == "+") {
			return 2;
		}
		else if (p == "-") {
			return 3;
		}
		else if (p == "*") {
			return 4;
		}
		else if (p == "/") {
			return 5;
		}
		else if (p == "++") {
			return 6;
		}
		else if (p == "*2") {
			return 7;
		}
		else if (p == "&2") {
			return 8;
		}
		else if (p == "(") {
			return -100;
		}
		else if (p == ")") {
			return -101;
		}
		else if (p == ";") {
			return -999;
		}
		else if (p == "}") {
			return -999;
		}
		return -99999;
	}

	void GlobalVarDef() {
		if (Lex::GetToken().first == Lex::Int) {
		    int varType=VarType::Int;
			Lex::Match(Lex::Int);
			//Praise *****
			int ptrCNt = parseMulSign();
            varType+=(VarType::IntPtr)*ptrCNt;
			while (true) {
				string name = Lex::GetToken().second;
				if (Lex::GetToken().first == Lex::ID) {
					Lex::SkipToken(name);
				}
				if (global_var.find(name) != global_var.end()) {
					cout << name << "has definite" << endl;
					exit(0);
				}
				global_var[name] = 0;
				global_varType[name]=varType;
				if (Lex::GetToken().first == Lex::Comma) {
					Lex::Match(Lex::Comma);
				}
				if (Lex::GetToken().first == Lex::Semi) {
					Lex::Match(Lex::Semi);
					break;
				}
			}
			GlobalVarDef();
		}
	}

	void ParamDecl() {
        int cnt = 0;
		while (true) {
			if (Lex::GetToken().first == Lex::Int) {
                Lex::Match(Lex::Int);
                int varType=VarType::Int;
                //Praise *****
                int ptrCNt = parseMulSign();
                varType+=(VarType::IntPtr)*ptrCNt;
				string name = Lex::GetToken().second;
				local_param[name] = cnt++;
				local_paramVarType[name]=varType;
				Lex::SkipToken(name);
			}
			if (Lex::GetToken().first == Lex::Comma) {
				Lex::Match(Lex::Comma);
			}
			else {
				break;
			}
		}
	}
	void Expression(int priority) {
		auto curToken = Lex::GetToken();
		if (curToken.first == Lex::Num) {
			virtualMachine.LoadIntInAx(String2Int(curToken.second));
			Lex::SkipToken(curToken.second);
            resultType = VarType::Int;
		}
		else if(curToken.first == Lex::Alloc){
            Lex::SkipToken(curToken.second);
            Lex::Match(Lex::LeftParen);
            Expression(GetPriority("="));
            virtualMachine.Append(VirtualMachine::Ins::Alloc);
            Lex::Match(Lex::RightParen);
		}
		else if(curToken.first == Lex::Free){
            Lex::SkipToken(curToken.second);
            Lex::Match(Lex::LeftParen);
            Expression(GetPriority("="));
            virtualMachine.Append(VirtualMachine::Ins::Free);
            Lex::Match(Lex::RightParen);
		}
		else if (curToken.first == Lex::Str) {
			string s = curToken.second;
			char* ptr = AllocStr(s.substr(1, s.size() - 2));
			virtualMachine.LoadIntInAx((int)ptr);
			Lex::SkipToken(curToken.second);
            resultType=VarType::IntPtr;
		}else if(curToken.first == Lex::And){
            Lex::SkipToken(curToken.second);
            Expression(GetPriority("*2"));
            resultType+=VarType::IntPtr;
		    if(virtualMachine.IsCurRightValue()){
		        virtualMachine.PopTopOp();
		    }
		}else if(curToken.first == Lex::Mul){
            Lex::SkipToken(curToken.second);
            Expression(GetPriority("*2"));
            if(resultType < VarType::IntPtr){
                cout<<"can not detach reference";
                exit(0);
            }
            resultType-=VarType::IntPtr;
            virtualMachine.Append(VirtualMachine::Li);
		}
		else if (curToken.first == Lex::ID) {
			string name = curToken.second;
			Lex::SkipToken(name);
			//Function Call
			if (Lex::GetToken().first == Lex::LeftParen) {
				if (func_addr.find(name) == func_addr.end()) {
					cout << "could not find function:" << name << endl;
					exit(0);
				}
				int paramCnt = 0;
				Lex::Match(Lex::LeftParen);
				while (true) {
					Expression(GetPriority("="));
					virtualMachine.Append(VirtualMachine::PushAx);
					if (Lex::GetToken().first == Lex::Comma) {
						Lex::Match(Lex::Comma);
					}
					paramCnt++;
					if (Lex::GetToken().first == Lex::RightParen) {
						break;
					}
				}
				virtualMachine.CallFunction(name, func_addr[name]);
				virtualMachine.AdjStack(paramCnt);
				Lex::Match(Lex::RightParen);
			}
			else {
				if (local_var.find(name) != local_var.end()) {
				    resultType = local_varType[name];
					virtualMachine.LoadLocalVarAddressInAx(local_var[name]);
					virtualMachine.Append(VirtualMachine::Li);
				}
				else if (local_param.find(name) != local_param.end()) {
				    resultType = local_param[name];
					virtualMachine.LoadTheFormalParamAddressInAx(local_param[name], local_param.size());
					virtualMachine.Append(VirtualMachine::Li);
				}
				else if (global_var.find(name) != global_var.end()) {
				    resultType = global_varType[name];
					virtualMachine.Append(VirtualMachine::Imm);
					repair_var.emplace_back(name, virtualMachine.GetTextSize());
					virtualMachine.Append(0);
					virtualMachine.Append(VirtualMachine::Li);
				}
				else {
					cout << "can not find " << name << endl;
					exit(0);
				}
			}
		}
		curToken = Lex::GetToken();
		while (GetPriority(curToken.second) >= priority) {
			if (curToken.first == Lex::Add) {
				Lex::Match(Lex::Add);
				virtualMachine.PushAxInStack();
				int leftType = resultType;
				Expression(GetPriority("*"));
				int rightType = resultType;
				if(leftType>=VarType::IntPtr && rightType>=VarType::IntPtr){
				    cout<<"ptr can not add with ptr";
				    exit(0);
				}
				resultType = max(leftType,rightType);
				virtualMachine.Append(VirtualMachine::Ins::Add);
			}
			else if (curToken.first == Lex::Sub) {
				Lex::Match(Lex::Sub);
				virtualMachine.PushAxInStack();
                int leftType = resultType;
				Expression(GetPriority("*"));
                int rightType = resultType;
                if(leftType>=VarType::IntPtr && rightType>=VarType::IntPtr){
                    cout<<"ptr can not sub with ptr";
                    exit(0);
                }
                resultType = max(leftType,rightType);
				virtualMachine.Append(VirtualMachine::Ins::Sub);
			}
			else if (curToken.first == Lex::Mul) {
				Lex::Match(Lex::Mul);
				virtualMachine.PushAxInStack();
                int leftType = resultType;
				Expression(GetPriority("++"));
                int rightType = resultType;
                if(leftType>=VarType::IntPtr && rightType>=VarType::IntPtr){
                    cout<<"ptr can not mul with ptr";
                    exit(0);
                }
                resultType = max(leftType,rightType);
				virtualMachine.Append(VirtualMachine::Ins::Mul);
			}
			else if (curToken.first == Lex::Assign) {
				Lex::Match(Lex::Assign);
                int leftType = resultType;
				if (!virtualMachine.IsCurRightValue()) {
					cout << "can not assign left-value" << endl;
				}
				virtualMachine.PopTopOp();
				virtualMachine.PushAxInStack();
				Expression(GetPriority("="));
				resultType=leftType;
				virtualMachine.Append(VirtualMachine::Ins::Si);
			}
			else if (curToken.first == Lex::Equal) {
				Lex::Match(Lex::Equal);
				virtualMachine.Append(VirtualMachine::Ins::PushAx);
				Expression(GetPriority("=="));
				resultType = VarType::Int;
				virtualMachine.Append(VirtualMachine::Ins::Equ);
			}else{
			    cout<<"not support operator "<<curToken.second;
			    exit(0);
			}
			curToken = Lex::GetToken();
		}

	}


	void Statement() {
		if (Lex::GetToken().first == Lex::Semi) {
			Lex::Match(Lex::Semi);
			Statement();
		}
		else if (Lex::GetToken().first == Lex::If) {
			int ifTrueEndPos = 0, ifFalsePos = 0;
			Lex::Match(Lex::If);
			Lex::Match(Lex::LeftParen);
			Expression(GetPriority("="));
			virtualMachine.Append(VirtualMachine::Jz);
			ifFalsePos = virtualMachine.GetTextSize();
			virtualMachine.Append(0);
			Lex::Match(Lex::RightParen);
			Lex::Match(Lex::LeftMid);
			Statement();
			virtualMachine.Append(VirtualMachine::Jmp);
			ifTrueEndPos = virtualMachine.GetTextSize();
			virtualMachine.Append(0);
			Lex::Match(Lex::RightMid);

            virtualMachine.AtText(ifFalsePos,virtualMachine.GetTextSize());
			stack<int> elseIfFalse;
			stack<int> elseIfJmp;
			while(Lex::GetToken().first == Lex::ElseIf){
			    //if false
			    int cur = virtualMachine.GetTextSize();
			    if(elseIfFalse.empty()){
                    virtualMachine.AtText(ifFalsePos,cur);
			    }else{
			        virtualMachine.AtText( elseIfFalse.top(),cur );
			    }
                Lex::Match(Lex::ElseIf);
                Lex::Match(Lex::LeftParen);
                Expression(GetPriority("="));
                virtualMachine.Append(VirtualMachine::Jz);
                int elseIfFalsePos = virtualMachine.GetTextSize();
                virtualMachine.Append(0);
                elseIfFalse.push(elseIfFalsePos);
                Lex::Match(Lex::RightParen);
                Lex::Match(Lex::LeftMid);
                Statement();
                virtualMachine.Append(VirtualMachine::Jmp);
                elseIfJmp.push(virtualMachine.GetTextSize());
                virtualMachine.Append(0);
                Lex::Match(Lex::RightMid);
			}

			//else
			if (Lex::GetToken().first == Lex::Else) {
			    if(!elseIfFalse.empty()) {
                    virtualMachine.AtText(elseIfFalse.top(), virtualMachine.GetTextSize());
                }
				Lex::Match(Lex::Else);
				Lex::Match(Lex::LeftMid);
				Statement();
				Lex::Match(Lex::RightMid);
				virtualMachine.AtText(ifTrueEndPos, virtualMachine.GetTextSize());
			}
			else {
                if(!elseIfFalse.empty()) {
                    virtualMachine.AtText(elseIfFalse.top(), virtualMachine.GetTextSize());
                }
				virtualMachine.AtText(ifTrueEndPos, virtualMachine.GetTextSize());
			}
			while(!elseIfJmp.empty()){
			    virtualMachine.AtText(elseIfJmp.top(), virtualMachine.GetTextSize());
			    elseIfJmp.pop();
			}
			Statement();
		}
		else if (Lex::GetToken().first == Lex::LeftMid) {
			Lex::Match(Lex::LeftMid);
			Statement();
			Lex::Match(Lex::RightMid);
			Statement();
		}
		else if (Lex::GetToken().first == Lex::While) {
			int conditionFalsePos = 0;
			int whileStartPos = 0;
			Lex::Match(Lex::While);
			Lex::Match(Lex::LeftParen);
			whileStartPos = virtualMachine.GetTextSize();
			Expression(GetPriority("="));
			virtualMachine.Append(VirtualMachine::Jz);
			conditionFalsePos = virtualMachine.GetTextSize();
			virtualMachine.Append(0);
			Lex::Match(Lex::RightParen);
			Lex::Match(Lex::LeftMid);
			Statement();
			virtualMachine.Append(VirtualMachine::Jmp);
			virtualMachine.Append(whileStartPos);
			Lex::Match(Lex::RightMid);
			for (auto& s : breakPos) {
				virtualMachine.AtText(s, virtualMachine.GetTextSize());
			}
			virtualMachine.AtText(conditionFalsePos, virtualMachine.GetTextSize());
			breakPos.clear();
			Statement();
		}
		else if (Lex::GetToken().first == Lex::Break) {
			Lex::Match(Lex::Break);
			virtualMachine.Append(VirtualMachine::Jmp);
			breakPos.emplace_back(virtualMachine.GetTextSize());
			virtualMachine.Append(0);
			Statement();
		}
		else if ( Lex::GetToken().first == Lex::Free||Lex::GetToken().first == Lex::Alloc||  Lex::GetToken().first==Lex::Mul|| Lex::GetToken().first == Lex::ID || Lex::GetToken().first == Lex::Num ||
			Lex::GetToken().first == Lex::Str) {
			Expression(GetPriority("="));
			Lex::Match(Lex::Semi);
			Statement();
		}
		else if (Lex::GetToken().first == Lex::Print) {
			Lex::Match(Lex::Print);
			Lex::Match(Lex::LeftParen);
			int cnt = 0;
			while (true) {
				Expression(GetPriority("="));
				virtualMachine.PushAxInStack();
				if (Lex::GetToken().first == Lex::Comma) {
					Lex::Match(Lex::Comma);
				}
				cnt++;
				if (Lex::GetToken().first == Lex::RightParen) {
					break;
				}
			}
			virtualMachine.PushValueInStack(cnt);
			virtualMachine.Append(VirtualMachine::Print);

			Lex::Match(Lex::RightParen);
			Lex::Match(Lex::Semi);
			Statement();
		}
		else if (Lex::GetToken().first == Lex::Ret) {
			Lex::Match(Lex::Ret);
			Expression(GetPriority("="));
			Lex::Match(Lex::Semi);
			int retPos = 0;
			virtualMachine.Append(VirtualMachine::Jmp);
			retPos = virtualMachine.GetTextSize();
			virtualMachine.Append(0);
			returnPos.emplace_back(retPos);
		}
	}

	void LocalVarDef(int fromVarCnt) {
        int cnt = fromVarCnt;
		if (Lex::GetToken().first == Lex::Int) {
			Lex::Match(Lex::Int);
            int varType=VarType::Int;
            //Praise *****
            int ptrCNt = parseMulSign();
            varType+=(VarType::IntPtr)*ptrCNt;
			while (true) {
				string name = Lex::GetToken().second;
				if (Lex::GetToken().first == Lex::ID) {
					Lex::SkipToken(name);
				}
				if (local_var.find(name) != local_var.end()) {
					cout << name << "has definite" << endl;
					exit(0);
				}
				local_var[name] = cnt++;
				local_varType[name] = varType;
				if (Lex::GetToken().first == Lex::Comma) {
					Lex::Match(Lex::Comma);
				}
				if (Lex::GetToken().first == Lex::Semi) {
					Lex::Match(Lex::Semi);
					break;
				}
			}
			LocalVarDef(cnt);
		}
	}

	void GlobalFuncDef() {
		while (Lex::GetToken().first == Lex::Func) {
			Lex::Match(Lex::Func);
			if (Lex::GetToken().first == Lex::Int) {
				Lex::Match(Lex::Int);
				string name = Lex::GetToken().second;

				//记录函数的地址
				func_addr[name] = virtualMachine.GetTextSize();

				Lex::SkipToken(name);
				Lex::Match(Lex::LeftParen);
				ParamDecl();
				Lex::Match(Lex::RightParen);

				Lex::Match(Lex::LeftMid);

				LocalVarDef(0);
				virtualMachine.EnterFunction(local_var.size());

				//statement
				Statement();

				Lex::Match(Lex::RightMid);

				for (auto& s : returnPos) {
					virtualMachine.AtText(s, virtualMachine.GetTextSize());
				}
				virtualMachine.LeaveFunction();
			}
			local_var.clear();
			local_param.clear();
		}
	}

	void Program() {
		if (Lex::GetToken().first == Lex::Int) {
			GlobalVarDef();
		}
		if (Lex::GetToken().first == Lex::Func) {
			GlobalFuncDef();
		}
	}

	void ReLocation() {
		//修复全局变量地址定位
		map<string, int> glo;
		for (auto& s : repair_var) {
			if (glo.find(s.first) == glo.end()) {
				glo[s.first] = (int) new int[1];
			}
			virtualMachine.AtText(s.second, glo[s.first]);
		}

		//修复函数地址定位
		for (auto& s : repair_func) {
			if (func_addr.find(s.first) == func_addr.end()) {
				cout << "can not find function:" << s.second;
				exit(0);
			}
			virtualMachine.AtText(s.second, func_addr[s.first]);
		}

	}
}

int main() {
	Gram::Program();
	Gram::ReLocation();
	virtualMachine.StartFunction(Gram::func_addr["main"]);
	return 0;
}
