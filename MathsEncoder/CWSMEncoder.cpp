#include "pch.h"
#include "CWSMEncoder.h"
#include <iostream>
#include <cstring>
#include <time.h>
#define _DLL_EXPORTS
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
using namespace std;
void clear(queue<Command>& q) {
	queue<Command> empty;
	swap(empty, q);
}

void char_to_wchar(const char* ch, std::wstring& w_str)
{
	wchar_t* wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, ch, strlen(ch), NULL, 0);
	wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, ch, strlen(ch), wchar, len);
	wchar[len] = '\0';
	w_str = wchar;
	delete[]wchar;
}
string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}
string LeftRotateString(string str, int n = 1) {
	if (n <= 0 || str.size() == 0)
		return str;
	int len = str.size();
	n = n % len;
	//str += str;
	return str.substr(n, len);
}
int ParseIntValue(map<string, Value>* Context, string ParseString)
{
	if (ParseString[0] == '$')
	{
		string Target = LeftRotateString(ParseString);
		Value TargetValue = Context->at(Target);
		if (TargetValue.v_Type == VT_INT)
		{
			return TargetValue.v_Value_Int;
		}
		if (TargetValue.v_Type == VT_DOB)
		{
			return TargetValue.v_Value_Dob;
		}
	}
	else
	{
		return atoi(ParseString.c_str());
	}
}
double ParseDobValue(map<string, Value>* Context, string ParseString)
{
	if (ParseString[0] == '$')
	{
		string Target = LeftRotateString(ParseString);
		Value TargetValue = Context->at(Target);
		if (TargetValue.v_Type == VT_DOB)
		{
			return TargetValue.v_Value_Dob;
		}
		if (TargetValue.v_Type == VT_INT)
		{
			return TargetValue.v_Value_Int;
		}
	}
	else
	{
		return atof(ParseString.c_str());
	}
}
string ParseStrValue(map<string, Value>* Context, string ParseString)
{
	if (ParseString[0] == '$')
	{
		string Target = LeftRotateString(ParseString);
		Value TargetValue = Context->at(Target);
		if (TargetValue.v_Type == VT_STR)
		{
			return TargetValue.v_Value_Str;
		}
	}
	else
	{
		return ParseString;
	}
}
char ParseCharValue(map<string, Value>* Context, string ParseString)
{
	if (ParseString[0] == '$')
	{
		string Target = LeftRotateString(ParseString);
		Value TargetValue = Context->at(Target);
		if (TargetValue.v_Type == VT_CHAR)
		{
			return TargetValue.v_Value_Char;
		}
	}
	else
	{
		return ParseString[0];
	}
}
pair<string, int> ParseSpaceString(string *s, int offset)
{
	int pos = s->find(" ", offset);
	string substr = s->substr(offset + 1, pos - offset - 1);
	if (substr.c_str()[0] == '\"')
	{
		int s_pos = s->find("\"", offset);
		string s_substr = s->substr(offset + 1, s_pos - offset - 1);
		pair<string, int> si(s_substr, s_pos);
		return si;
	}
	pair<string, int> si(substr, pos);
	return si;
}
map<string, Value>* lastMapper = NULL;
DWORD WINAPI Worker(LPVOID param)
{
	CWSMEncoder* Encoder = (CWSMEncoder*)param;
	int i = Encoder->EBP_Stack;
	queue<string> reset;
	map<string, Value> ValueMapper;
	if (lastMapper != NULL)
	{
		for (map<string, Value>::iterator p = lastMapper->begin(); p != lastMapper->end(); p++)
		{
			ValueMapper.insert(pair<string, Value>((*p).first, (*p).second));
		}
	}
	Value v_x, v_y, v_n_x, v_n_y;
	v_x.v_Name = "__FUNC__X";
	v_x.v_Type = VT_INT;
	v_x.v_Value_Int = Encoder->Stackx;
	v_y.v_Name = "__FUNC__Y";
	v_y.v_Type = VT_INT;
	v_y.v_Value_Int = Encoder->Stacky;
	v_n_x.v_Name = "__FUNC__N__X";
	v_n_x.v_Type = VT_INT;
	v_n_x.v_Value_Int = Encoder->StackNativeX;
	v_n_y.v_Name = "__FUNC__N__Y";
	v_n_y.v_Type = VT_INT;
	v_n_y.v_Value_Int = Encoder->StackNativeY;
	ValueMapper.insert_or_assign(v_x.v_Name, v_x);
	ValueMapper.insert_or_assign(v_y.v_Name, v_y);
	ValueMapper.insert_or_assign(v_n_x.v_Name, v_n_x);
	ValueMapper.insert_or_assign(v_n_y.v_Name, v_n_y);
	while (true)
	{
		string t_cmd;
		//char *s;
		t_cmd = Encoder->Cmd_List->at(i);
		//s = t_cmd.c_str();
		string s_cmd;
		int pos = t_cmd.find(" ");
		s_cmd = t_cmd.substr(0, pos);
		if (!strcmp(s_cmd.c_str(), "MSG"))
		{
			string text, cap;
			int x, y, time, type;
			string cx, cy;
			//scanf("%s%s%s%s%d%d", text, cap, cx, cy, &type, &time);
			string tmp;
			int last = pos;
			for (int i = 2; i < 8; i++)
			{
				int c_pos = t_cmd.find(" ", last + 1);
				tmp = t_cmd.substr(last + 1, c_pos - last - 1);
				switch (i)
				{
				case 2:
					text = ParseStrValue(&ValueMapper, tmp);
					break;
				case 3:
					cap = ParseStrValue(&ValueMapper, tmp);
					break;
				case 4:
					x = ParseIntValue(&ValueMapper, tmp);
					break;
				case 5:
					y = ParseIntValue(&ValueMapper, tmp);
					break;
				case 6:
					type = ParseIntValue(&ValueMapper, tmp);
					break;
				case 7:
					time = ParseIntValue(&ValueMapper, tmp);
					break;
				}
				last = c_pos;
			}
			wstring T, C;
			char_to_wchar(UTF8ToGB(text.c_str()).c_str(), T);
			char_to_wchar(UTF8ToGB(cap.c_str()).c_str(), C);
			Message Msg;
			Msg.caption = C;
			Msg.Msg = T;
			Msg.Timeout = time;
			Msg.Type = (MSG_Type)type;
			Msg.x = x;
			Msg.y = y;
			if (Msg.x<0 || Msg.y<0 || Msg.x>Encoder->Screen_Width || Msg.y>Encoder->Screen_Height)
			{
				Msg.x = rand() % (Encoder->Screen_Width - 400);
				Msg.y = rand() % (Encoder->Screen_Height - 100);
			}
			Command cmd;
			cmd.cType = MESG;
			cmd.Msg = Msg;
			Encoder->PushCmd(cmd);
		}
		else if (!strcmp(s_cmd.c_str(), "FUNC"))
		{
			double x_4;
			double x_3;
			double x_2;
			double x_1;
			int c;
			int RangeMin;
			int RangeMax;
			int Offset;
			int Timewait;
			//char text[200000], cap[200000];
			//int x, y, time, type;
			//scanf("%lf%lf%lf%lf%d%d%d%d%d", &x_4, &x_3, &x_2, &x_1, &c, &RangeMin, &RangeMax, &Offset, &Timewait);
			//wstring T, C;
			//char_to_wchar(text, T);
			//char_to_wchar(cap, C);
			string tmp;
			int last = pos;
			for (int i = 2; i < 6; i++)
			{
				int c_pos = t_cmd.find(" ", last + 1);
				tmp = t_cmd.substr(last + 1, c_pos - last - 1);
				switch (i)
				{
				case 2:
					x_4 = ParseDobValue(&ValueMapper, tmp);
					break;
				case 3:
					x_3 = ParseDobValue(&ValueMapper, tmp);
					break;
				case 4:
					x_2 = ParseDobValue(&ValueMapper, tmp);
					break;
				case 5:
					x_1 = ParseDobValue(&ValueMapper, tmp);
					break;
				}
				last = c_pos;
			}
			for (int i = 6; i < 10; i++)
			{
				int c_pos = t_cmd.find(" ", last + 1);
				tmp = t_cmd.substr(last + 1, c_pos - last - 1);
				switch (i)
				{
				case 6:
					c = ParseIntValue(&ValueMapper, tmp);
					break;
				case 7:
					RangeMin = ParseIntValue(&ValueMapper, tmp);
					break;
				case 8:
					RangeMax = ParseIntValue(&ValueMapper, tmp);
					break;
				case 9:
					Offset = ParseIntValue(&ValueMapper, tmp);
					break;
				}
				last = c_pos;
			}
			tmp = t_cmd.substr(last + 1, t_cmd.length());
			Timewait = ParseIntValue(&ValueMapper, tmp);
			if (RangeMin < 0 - (Encoder->Screen_Width / 2))
			{
				RangeMin = 0 - (Encoder->Screen_Width / 2);
			}
			if (RangeMin > (Encoder->Screen_Width / 2))
			{
				RangeMin = (Encoder->Screen_Width / 2);
			}
			if (RangeMax < 0 - (Encoder->Screen_Width / 2))
			{
				RangeMax = 0 - (Encoder->Screen_Width / 2);
			}
			if (RangeMax > (Encoder->Screen_Width / 2))
			{
				RangeMax = (Encoder->Screen_Width / 2);
			}
			if (c < 0 - (Encoder->Screen_Width / 2))
			{
				c = 0 - (Encoder->Screen_Width / 2);
			}
			if (c > (Encoder->Screen_Width / 2))
			{
				c = (Encoder->Screen_Width / 2);
			}
			for (int x = RangeMin; x < RangeMax; x += Offset)
			{
				int y = x_4 * x*x*x*x + x_3 * x*x*x + x_2 * x*x + x_1 * x + c;
				Message Msg;
				//Msg.caption = C;
				//Msg.Msg = T;
				//Msg.Timeout = time;
				//Msg.Type = (MSG_Type)type;
				Msg.x = x + Encoder->Screen_Width / 2;
				Msg.y = Encoder->Screen_Height / 2 - y;
				if (Msg.x<0 || Msg.y<0 || Msg.x>Encoder->Screen_Width || Msg.y>Encoder->Screen_Height)
				{
					Msg.x = rand() % Encoder->Screen_Width;
					Msg.y = rand() % Encoder->Screen_Height;
				}
				Encoder->Stackx = Msg.x;
				Encoder->Stacky = Msg.y;
				Encoder->EBP_Stack = i + 1;
				lastMapper = &ValueMapper;
				Worker(Encoder);
				Command cmd, cmd2;
				cmd.cType = MESG;
				cmd.Msg = Msg;
				//Encoder->PushCmd(cmd);
				cmd2.cType = SLEP;
				cmd2.Slp.MillionSec = Timewait;
				Encoder->PushCmd(cmd2);
			}
			for (int j = i; j < Encoder->Cmd_List->size(); j++)
			{
				if (Encoder->Cmd_List->at(j) == "END")
				{
					i = j;
					break;
				}
			}
		}
		else if (!strcmp(s_cmd.c_str(), "ADO"))
		{
			//string path;
			//path.resize(10000);
			//scanf("%s", &path[0]);
			string path;
			//gets_s(path);
			path = t_cmd.substr(pos + 1, t_cmd.length());
			path = ParseStrValue(&ValueMapper, path);
			wstring wpath;
			char_to_wchar(path.c_str(), wpath);
			Audio Ado;
			Ado.Path = wpath;
			Command cmd;
			cmd.cType = ADO;
			cmd.Ado = Ado;
			Encoder->PushCmd(cmd);
		}
		else if (!strcmp(s_cmd.c_str(), "SLEP"))
		{
			int Timewait;
			//scanf("%d", &Timewait);
			string path;
			path = t_cmd.substr(pos + 1, t_cmd.length());
			Timewait = ParseIntValue(&ValueMapper, path);;
			Command cmd2;
			cmd2.cType = SLEP;
			cmd2.Slp.MillionSec = Timewait;
			Encoder->PushCmd(cmd2);
		}
		else if (!strcmp(s_cmd.c_str(), "CMD"))
		{
			string path;
			//gets_s(path);
			path = t_cmd.substr(pos + 1, t_cmd.length());
			path = ParseStrValue(&ValueMapper, path);
			Cmd_Param cp;
			char_to_wchar(path.c_str(), cp.ExecusePath);
			//char_to_wchar(param, cp.Params);
			Command cmd;
			cmd.cType = CMD;
			cmd.cp = cp;
			Encoder->PushCmd(cmd);
		}
		else if (!strcmp(s_cmd.c_str(), "IMG"))
		{
			int x, y, t;
			//scanf("%d%d%d", &x, &y, &t);
			string tmp;
			int last = pos;
			for (int i = 2; i < 4; i++)
			{
				int c_pos = t_cmd.find(" ", last + 1);
				tmp = t_cmd.substr(last + 1, c_pos - last - 1);
				switch (i)
				{
				case 2:
					x = ParseIntValue(&ValueMapper, tmp);
					break;
				case 3:
					y = ParseIntValue(&ValueMapper, tmp);
					break;
				}
				last = c_pos;
			}
			int c_pos = t_cmd.find(" ", last + 1);
			tmp = t_cmd.substr(last + 1, c_pos - last - 1);
			t = ParseIntValue(&ValueMapper, tmp);
			last = c_pos;
			string str;
			str = t_cmd.substr(last + 1, t_cmd.length());
			str = ParseStrValue(&ValueMapper, str);
			//gets_s(str);
			ImageParam ip;
			ip.x = x;
			ip.y = y;
			ip.Timeout = t;
			char_to_wchar(str.c_str(), ip.Path);
			Command cmd;
			cmd.ip = ip;
			cmd.cType = IMG;
			Encoder->PushCmd(cmd);
		}
		else if (!strcmp(s_cmd.c_str(), "VDO"))
		{
			int x, y, t, cx, cy;
			/*scanf("%d%d%d%d%d", &x, &y, &cx, &cy, &t);
			char str[20000];
			gets_s(str);*/
			string tmp;
			int last = pos;
			for (int i = 2; i < 6; i++)
			{
				int c_pos = t_cmd.find(" ", last + 1);
				tmp = t_cmd.substr(last + 1, c_pos - last - 1);
				switch (i)
				{
				case 2:
					x = ParseIntValue(&ValueMapper, tmp);
					break;
				case 3:
					y = ParseIntValue(&ValueMapper, tmp);
					break;
				case 4:
					cx = ParseIntValue(&ValueMapper, tmp);
					break;
				case 5:
					cy = ParseIntValue(&ValueMapper, tmp);
					break;
				}
				last = c_pos;
			}
			int c_pos = t_cmd.find(" ", last + 1);
			tmp = t_cmd.substr(last + 1, c_pos - last - 1);
			t = ParseIntValue(&ValueMapper, tmp);
			last = c_pos;
			string str;
			str = t_cmd.substr(last + 1, t_cmd.length());
			str = ParseStrValue(&ValueMapper, str);
			VedioParam ip;
			ip.x = x;
			ip.y = y;
			ip.cx = cx;
			ip.cy = cy;
			ip.Timeout = t;
			char_to_wchar(str.c_str(), ip.Path);
			Command cmd;
			cmd.vp = ip;
			cmd.cType = MDI;
			Encoder->PushCmd(cmd);
		}
		else if (!strcmp(s_cmd.c_str(), "int"))
		{
			int c_pos = t_cmd.find(" ", pos + 1);
			string tmp = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int last = c_pos;
			string str;
			str = t_cmd.substr(last + 1, t_cmd.length());
			int v = atoi(str.c_str());
			Value vl;
			vl.v_Name = tmp;
			vl.v_Type = VT_INT;
			vl.v_Value_Int = v;
			ValueMapper.insert(pair<string, Value>(tmp, vl));
		}
		else if (!strcmp(s_cmd.c_str(), "string"))
		{
			int c_pos = t_cmd.find(" ", pos + 1);
			string tmp = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int last = c_pos;
			string str;
			str = t_cmd.substr(last + 1, t_cmd.length());
			Value vl;
			vl.v_Name = tmp;
			vl.v_Type = VT_STR;
			vl.v_Value_Str = str;
			ValueMapper.insert(pair<string, Value>(tmp, vl));
		}
		else if (!strcmp(s_cmd.c_str(), "double"))
		{
			int c_pos = t_cmd.find(" ", pos + 1);
			string tmp = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int last = c_pos;
			string str;
			str = t_cmd.substr(last + 1, t_cmd.length());
			double v = atof(str.c_str());
			Value vl;
			vl.v_Name = tmp;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = v;
			ValueMapper.insert(pair<string, Value>(tmp, vl));
		}
		else if (!strcmp(s_cmd.c_str(), "char"))
		{
			int c_pos = t_cmd.find(" ", pos + 1);
			string tmp = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int last = c_pos;
			string str;
			str = t_cmd.substr(last + 1, t_cmd.length());
			char v = str.c_str()[0];
			Value vl;
			vl.v_Name = tmp;
			vl.v_Type = VT_CHAR;
			vl.v_Value_Char = v;
			ValueMapper.insert(pair<string, Value>(tmp, vl));
		}
		else if (t_cmd.c_str()[0] == '$')
		{
			int c_pos = t_cmd.find('=');
			string targetValue = t_cmd.substr(1, c_pos - 1);
			if (t_cmd[c_pos + 1] == '$')
			{
				string sourceValue = t_cmd.substr(c_pos + 2, t_cmd.length());
				Value target, source;
				target = ValueMapper[targetValue];
				source = ValueMapper[sourceValue];
				if (target.v_Type == source.v_Type)
				{
					source.v_Name = target.v_Name;
					ValueMapper.insert_or_assign(source.v_Name, source);
				}
			}
			else
			{
				string sourceValue = t_cmd.substr(c_pos + 1, t_cmd.length());
				Value target;
				target = ValueMapper[targetValue];
				ValueType vt = target.v_Type;
				switch (vt)
				{
				case VT_INT:
					target.v_Value_Int = atoi(sourceValue.c_str());
					break;
				case VT_STR:
					target.v_Value_Str = sourceValue;
					break;
				case VT_DOB:
					target.v_Value_Dob = atof(sourceValue.c_str());
					break;
				case VT_CHAR:
					target.v_Value_Char = sourceValue.c_str()[0];
					break;
				default:
					break;
				}
				ValueMapper.insert_or_assign(target.v_Name, target);
			}
		}
		else if (!strcmp(s_cmd.c_str(), "ADD"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			string fartargetValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			if (targetValue.c_str()[0] == '$')
			{
				targetValue = LeftRotateString(targetValue);
				if (sourceValue[0] == '$')
				{
					sourceValue = LeftRotateString(sourceValue);
					Value tvt, svt, fvt;
					tvt = ValueMapper[targetValue];
					svt = ValueMapper[sourceValue];
					if (tvt.v_Type == svt.v_Type)
					{
						ValueType vt = tvt.v_Type;
						int i_end;
						string s_end;
						double d_end;
						char c_end;
						Value ret = tvt;
						ret.v_Name = LeftRotateString(fartargetValue);
						switch (vt)
						{
						case VT_INT:
							i_end = tvt.v_Value_Int + svt.v_Value_Int;
							ret.v_Value_Int = i_end;
							break;
						case VT_STR:
							s_end = tvt.v_Value_Str + tvt.v_Value_Str;
							ret.v_Value_Str = s_end;
							break;
						case VT_DOB:
							d_end = tvt.v_Value_Dob + svt.v_Value_Dob;
							ret.v_Value_Dob = d_end;
							break;
						case VT_CHAR:
							c_end = tvt.v_Value_Char + svt.v_Value_Char;
							ret.v_Value_Char = c_end;
							break;
						default:
							break;
						}
						ValueMapper.insert_or_assign(ret.v_Name, ret);
					}
				}
				else
				{
					Value TargetValue = ValueMapper[targetValue];
					ValueType vt = TargetValue.v_Type;
					Value RetValue = TargetValue;
					RetValue.v_Name = fartargetValue;
					switch (vt)
					{
					case VT_INT:
						RetValue.v_Value_Int += atoi(sourceValue.c_str());
						break;
					case VT_STR:
						RetValue.v_Value_Str += sourceValue;
						break;
					case VT_DOB:
						RetValue.v_Value_Dob += atof(sourceValue.c_str());
						break;
					case VT_CHAR:
						RetValue.v_Value_Char += sourceValue.c_str()[0];
						break;
					default:
						break;
					}
					ValueMapper.insert_or_assign(RetValue.v_Name, RetValue);
				}
			}
		}
		else if (!strcmp(s_cmd.c_str(), "SUB"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			string fartargetValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			if (targetValue[0] == '$')
			{
				targetValue = LeftRotateString(targetValue);
				if (sourceValue[0] == '$')
				{
					sourceValue = LeftRotateString(sourceValue);
					Value tvt, svt, fvt;
					tvt = ValueMapper[targetValue];
					svt = ValueMapper[sourceValue];
					if (tvt.v_Type == svt.v_Type)
					{
						ValueType vt = tvt.v_Type;
						int i_end;
						string s_end;
						double d_end;
						char c_end;
						Value ret = tvt;
						ret.v_Name = LeftRotateString(fartargetValue);
						switch (vt)
						{
						case VT_INT:
							i_end = tvt.v_Value_Int - svt.v_Value_Int;
							ret.v_Value_Int = i_end;
							break;
						case VT_STR:
							//s_end = tvt.v_Value_Str - tvt.v_Value_Str;
							//ret.v_Value_Str = s_end;
							break;
						case VT_DOB:
							d_end = tvt.v_Value_Dob - svt.v_Value_Dob;
							ret.v_Value_Dob = d_end;
							break;
						case VT_CHAR:
							c_end = tvt.v_Value_Char - svt.v_Value_Char;
							ret.v_Value_Char = c_end;
							break;
						default:
							break;
						}
						ValueMapper.insert_or_assign(ret.v_Name, ret);
					}
				}
				else
				{
					Value TargetValue = ValueMapper[targetValue];
					ValueType vt = TargetValue.v_Type;
					Value RetValue = TargetValue;
					RetValue.v_Name = fartargetValue;
					switch (vt)
					{
					case VT_INT:
						RetValue.v_Value_Int -= atoi(sourceValue.c_str());
						break;
					case VT_STR:
						//RetValue.v_Value_Str -= sourceValue;
						break;
					case VT_DOB:
						RetValue.v_Value_Dob -= atof(sourceValue.c_str());
						break;
					case VT_CHAR:
						RetValue.v_Value_Char -= sourceValue.c_str()[0];
						break;
					default:
						break;
					}
					ValueMapper.insert_or_assign(RetValue.v_Name, RetValue);
				}
			}
		}
		else if (!strcmp(s_cmd.c_str(), "MUL"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			string fartargetValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			if (targetValue[0] == '$')
			{
				targetValue = LeftRotateString(targetValue);
				if (sourceValue[0] == '$')
				{
					sourceValue = LeftRotateString(sourceValue);
					Value tvt, svt, fvt;
					tvt = ValueMapper[targetValue];
					svt = ValueMapper[sourceValue];
					if (tvt.v_Type == svt.v_Type)
					{
						ValueType vt = tvt.v_Type;
						int i_end;
						string s_end;
						double d_end;
						char c_end;
						Value ret = tvt;
						ret.v_Name = LeftRotateString(fartargetValue);
						switch (vt)
						{
						case VT_INT:
							i_end = tvt.v_Value_Int * svt.v_Value_Int;
							ret.v_Value_Int = i_end;
							break;
						case VT_STR:
							/*s_end = tvt.v_Value_Str * tvt.v_Value_Str;
							ret.v_Value_Str = s_end;*/
							break;
						case VT_DOB:
							d_end = tvt.v_Value_Dob * svt.v_Value_Dob;
							ret.v_Value_Dob = d_end;
							break;
						case VT_CHAR:
							c_end = tvt.v_Value_Char * svt.v_Value_Char;
							ret.v_Value_Char = c_end;
							break;
						default:
							break;
						}
						ValueMapper.insert_or_assign(ret.v_Name, ret);
					}
				}
				else
				{
					Value TargetValue = ValueMapper[targetValue];
					ValueType vt = TargetValue.v_Type;
					Value RetValue = TargetValue;
					RetValue.v_Name = fartargetValue;
					switch (vt)
					{
					case VT_INT:
						RetValue.v_Value_Int *= atoi(sourceValue.c_str());
						break;
					case VT_STR:
						//RetValue.v_Value_Str = sourceValue;
						break;
					case VT_DOB:
						RetValue.v_Value_Dob *= atof(sourceValue.c_str());
						break;
					case VT_CHAR:
						RetValue.v_Value_Char *= sourceValue.c_str()[0];
						break;
					default:
						break;
					}
					ValueMapper.insert_or_assign(RetValue.v_Name, RetValue);
				}
			}
		}
		else if (!strcmp(s_cmd.c_str(), "DIV"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			string fartargetValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			if (targetValue[0] == '$')
			{
				targetValue = LeftRotateString(targetValue);
				if (sourceValue[0] == '$')
				{
					sourceValue = LeftRotateString(sourceValue);
					Value tvt, svt, fvt;
					tvt = ValueMapper[targetValue];
					svt = ValueMapper[sourceValue];
					if (tvt.v_Type == svt.v_Type)
					{
						ValueType vt = tvt.v_Type;
						int i_end;
						string s_end;
						double d_end;
						char c_end;
						Value ret = tvt;
						ret.v_Name = LeftRotateString(fartargetValue);
						switch (vt)
						{
						case VT_INT:
							i_end = tvt.v_Value_Int / (svt.v_Value_Int == 0 ? 1 : svt.v_Value_Int);
							ret.v_Value_Int = i_end;
							break;
						case VT_STR:
							//s_end = tvt.v_Value_Str / tvt.v_Value_Str;
							//ret.v_Value_Str = s_end;
							break;
						case VT_DOB:
							d_end = tvt.v_Value_Dob / (svt.v_Value_Dob == 0 ? 1 : svt.v_Value_Dob);
							ret.v_Value_Dob = d_end;
							break;
						case VT_CHAR:
							c_end = tvt.v_Value_Char / (svt.v_Value_Char == 0 ? 1 : svt.v_Value_Char);
							ret.v_Value_Char = c_end;
							break;
						default:
							break;
						}
						ValueMapper.insert_or_assign(ret.v_Name, ret);
					}
				}
				else
				{
					Value TargetValue = ValueMapper[targetValue];
					ValueType vt = TargetValue.v_Type;
					Value RetValue = TargetValue;
					RetValue.v_Name = fartargetValue;
					switch (vt)
					{
					case VT_INT:
						RetValue.v_Value_Int /= atoi(sourceValue.c_str());
						break;
					case VT_STR:
						//RetValue.v_Value_Str /= sourceValue;
						break;
					case VT_DOB:
						RetValue.v_Value_Dob /= atof(sourceValue.c_str());
						break;
					case VT_CHAR:
						RetValue.v_Value_Char /= sourceValue.c_str()[0];
						break;
					default:
						break;
					}
					ValueMapper.insert_or_assign(RetValue.v_Name, RetValue);
				}
			}
		}
		else if (!strcmp(s_cmd.c_str(), "DIVI"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			string fartargetValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			if (targetValue[0] == '$')
			{
				targetValue = LeftRotateString(targetValue);
				if (sourceValue[0] == '$')
				{
					sourceValue = LeftRotateString(sourceValue);
					Value tvt, svt, fvt;
					tvt = ValueMapper[targetValue];
					svt = ValueMapper[sourceValue];
					if (tvt.v_Type == svt.v_Type)
					{
						ValueType vt = tvt.v_Type;
						int i_end;
						string s_end;
						double d_end;
						char c_end;
						Value ret = tvt;
						ret.v_Name = LeftRotateString(fartargetValue);
						switch (vt)
						{
						case VT_INT:
							i_end = tvt.v_Value_Int % (svt.v_Value_Int == 0 ? 1 : svt.v_Value_Int);
							ret.v_Value_Int = i_end;
							break;
						case VT_STR:
							//s_end = tvt.v_Value_Str + tvt.v_Value_Str;
							//ret.v_Value_Str = s_end;
							break;
						case VT_DOB:
							d_end = 0;
							ret.v_Value_Dob = d_end;
							break;
						case VT_CHAR:
							c_end = tvt.v_Value_Char % (svt.v_Value_Char == 0 ? 1 : svt.v_Value_Char);
							ret.v_Value_Char = c_end;
							break;
						default:
							break;
						}
						ValueMapper.insert_or_assign(ret.v_Name, ret);
					}
				}
				else
				{
					Value TargetValue = ValueMapper[targetValue];
					ValueType vt = TargetValue.v_Type;
					Value RetValue = TargetValue;
					RetValue.v_Name = fartargetValue;
					switch (vt)
					{
					case VT_INT:
						RetValue.v_Value_Int %= atoi(sourceValue.c_str());
						break;
					case VT_STR:
						//RetValue.v_Value_Str = sourceValue;
						break;
					case VT_DOB:
						RetValue.v_Value_Dob = 0;
						break;
					case VT_CHAR:
						RetValue.v_Value_Char %= sourceValue.c_str()[0];
						break;
					default:
						break;
					}
					ValueMapper.insert_or_assign(RetValue.v_Name, RetValue);
				}
			}
		}
		else if (!strcmp(s_cmd.c_str(), "CMP"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			bool end = false;
			if (targetValue[0] == '$')
			{
				targetValue = LeftRotateString(targetValue);
				if (sourceValue[0] == '$')
				{
					sourceValue = LeftRotateString(sourceValue);
					Value tvt, svt, fvt;
					tvt = ValueMapper[targetValue];
					svt = ValueMapper[sourceValue];
					if (tvt.v_Type == svt.v_Type)
					{
						ValueType vt = tvt.v_Type;
						int i_end;
						string s_end;
						double d_end;
						char c_end;
						switch (vt)
						{
						case VT_INT:
							end = tvt.v_Value_Int == svt.v_Value_Int;
							break;
						case VT_STR:
							end = tvt.v_Value_Str == tvt.v_Value_Str;
							//ret.v_Value_Str = s_end;
							break;
						case VT_DOB:
							//d_end = 0;
							//.v_Value_Dob = d_end;
							end = tvt.v_Value_Dob == svt.v_Value_Dob;
							break;
						case VT_CHAR:
							end = tvt.v_Value_Char == svt.v_Value_Char;
							break;
						default:
							break;
						}
						//ValueMapper.insert_or_assign(ret.v_Name, ret);
					}
				}
				else
				{
					Value TargetValue = ValueMapper[targetValue];
					ValueType vt = TargetValue.v_Type;
					Value RetValue = TargetValue;
					//RetValue.v_Name = fartargetValue;
					switch (vt)
					{
					case VT_INT:
						end = RetValue.v_Value_Int == atoi(sourceValue.c_str());
						break;
					case VT_STR:
						//RetValue.v_Value_Str = sourceValue;
						end = RetValue.v_Value_Str == sourceValue;
						break;
					case VT_DOB:
						end = RetValue.v_Value_Dob == atof(sourceValue.c_str());
						break;
					case VT_CHAR:
						end = RetValue.v_Value_Char == sourceValue.c_str()[0];
						break;
					default:
						break;
					}
					//ValueMapper.insert_or_assign(RetValue.v_Name, RetValue);
				}
			}
			if (!end)
			{
				i++;
			}
		}
		else if (!strcmp(s_cmd.c_str(), "COS"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			double param, target;
			param = ParseDobValue(&ValueMapper, targetValue);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			target = cos(param);
			Value vl;
			sourceValue = LeftRotateString(sourceValue);
			vl.v_Name = sourceValue;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = target;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "SIN"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			double param, target;
			param = ParseDobValue(&ValueMapper, targetValue);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			target = sin(param);
			Value vl;
			sourceValue = LeftRotateString(sourceValue);
			vl.v_Name = sourceValue;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = target;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "TAN"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			double param, target;
			param = ParseDobValue(&ValueMapper, targetValue);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			target = tan(param);
			Value vl;
			sourceValue = LeftRotateString(sourceValue);
			vl.v_Name = sourceValue;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = target;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "LOG"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			double param, target;
			param = ParseDobValue(&ValueMapper, targetValue);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			target = log(param);
			Value vl;
			sourceValue = LeftRotateString(sourceValue);
			vl.v_Name = sourceValue;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = target;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "POW"))
		{
			string targetValue, s_pow, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			s_pow = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			sourceValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			double param, target;
			param = ParseDobValue(&ValueMapper, targetValue);
			double d_pow = ParseDobValue(&ValueMapper, s_pow);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			target = pow(param, d_pow);
			Value vl;
			sourceValue = LeftRotateString(sourceValue);
			vl.v_Name = sourceValue;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = target;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "SQRT"))
		{
			string targetValue, sourceValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			double param, target;
			param = ParseDobValue(&ValueMapper, targetValue);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			target = sqrt(param);
			Value vl;
			sourceValue = LeftRotateString(sourceValue);
			vl.v_Name = sourceValue;
			vl.v_Type = VT_DOB;
			vl.v_Value_Dob = target;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "RAND"))
		{
			string targetValue, sourceValue, sendValue;
			int c_pos = t_cmd.find(' ', pos + 1);
			targetValue = t_cmd.substr(pos + 1, c_pos - pos - 1);
			int c1_pos = t_cmd.find(' ', c_pos + 1);
			sourceValue = t_cmd.substr(c_pos + 1, c1_pos - c_pos - 1);
			sendValue = t_cmd.substr(c1_pos + 1, t_cmd.length());
			int param, target, ans;
			param = ParseIntValue(&ValueMapper, targetValue);
			target = ParseIntValue(&ValueMapper, sourceValue);
			//target = ParseDobValue(&ValueMapper, sourceValue);
			srand((unsigned)time(NULL));
			ans = (rand() % (target - param + 1)) + param;
			Value vl;
			sendValue = LeftRotateString(sendValue);
			vl.v_Name = sendValue;
			vl.v_Type = VT_INT;
			vl.v_Value_Int = ans;
			ValueMapper.insert_or_assign(vl.v_Name, vl);
		}
		else if (!strcmp(s_cmd.c_str(), "JMP"))
		{
			string path;
			//gets_s(path);
			path = t_cmd.substr(pos + 1, t_cmd.length());
			int to = ParseIntValue(&ValueMapper, path) - 1;
			i += to;
		}
		else if (!strcmp(s_cmd.c_str(), "END"))
		{
			//Encoder->EBP_Stack->pop();
			return 0;
		}
		i++;
	}
	return 0;
}

BOOL CWSMEncoder::HasReady()
{
	return c_Status == STU_READY;
}
vector<string> vec;
void CWSMEncoder::LoadWSM(string Path)
{
	freopen(Path.c_str(), "r", stdin);
	char cmd[200000];
	Cmd_List = &vec;
	while (gets_s(cmd) != NULL)
	{
		string s_cmd = cmd;
		Cmd_List->push_back(s_cmd);
	}
}

void CWSMEncoder::Prepare()
{
	Screen_Width = GetSystemMetrics(SM_CXSCREEN);
	Screen_Height = GetSystemMetrics(SM_CYSCREEN);
	//stack<int> s;
	EBP_Stack = 0;
	//EBP_Stack->push(0);
	Worker(this);
	//EBP_Stack = NULL;
}

void CWSMEncoder::Run()
{
	c_Status = STU_RUNNING;
}

void CWSMEncoder::Pause()
{
	if (c_Status == STU_RUNNING)
	{
		c_Status = STU_PAUSED;
		clear(CMD_Paused);
		swap(CMD_Paused, CMD_Queue);
	}
}

void CWSMEncoder::Suspend()
{
	if (c_Status == STU_PAUSED)
	{
		c_Status = STU_RUNNING;
		swap(CMD_Paused, CMD_Queue);
		clear(CMD_Paused);
	}
}

void CWSMEncoder::Stop()
{
	if (c_Status != STU_RELEASED)
	{
		c_Status = STU_READY;
		clear(CMD_Paused);
		clear(CMD_Queue);
	}
}

void CWSMEncoder::Release()
{
	c_Status = STU_RELEASED;
	clear(CMD_Paused);
	clear(CMD_Queue);
	fclose(stdin);
}

Command CWSMEncoder::GetCurrentCmd()
{
	if (CMD_Queue.empty() || c_Status != STU_RUNNING)
	{
		Command Nop;
		Nop.cType = NOP;
		return Nop;
	}
	else
	{
		Command p_cmd;
		p_cmd = CMD_Queue.front();
		CMD_Queue.pop();
		return p_cmd;
	}
}

void CWSMEncoder::PushCmd(Command cmd)
{
	RMT_Queue->push(cmd);
}

void CWSMEncoder::SetStatus(EDR_Statu c_Statu)
{
	c_Status = c_Statu;
}

EDR_Statu CWSMEncoder::GetStatus()
{
	return c_Status;
}

void CWSMEncoder::SetCmdQueue(queue<Command>* p_Queue)
{
	RMT_Queue = p_Queue;
}
