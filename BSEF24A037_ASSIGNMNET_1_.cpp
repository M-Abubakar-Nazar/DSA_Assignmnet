#include<iostream>
#include<string>
#include<stack>
#include<vector>
#include<map>
#include<cctype>
#include<cstdlib>
using namespace std;

class Token
{
public:
	string val;
	int type;

};

int getPrecedence(string op)
{
	if (op == "*" || op == "/")
	{
		return 2;
	}

	if (op == "+" || op == "-")
	{
		return 1;
	}

	return 0;
}

bool isOpenBracket(string c)
{
	return c == "(" || c == "[" || c == "{";
}

bool isOpenClosed(string c)
{
	return c == ")" || c == "]" || c == "}";
}

bool bracketMatch(string open, string close)
{
	if (open == "(" && close == ")")
	{
		return true;
	}

	if (open == "[" && close == "]")
	{
		return true;
	}

	if (open == "{" && close == "}")
	{
		return true;
	}

	return false;

}

vector<Token> tokenize(const string& expr)
{
	vector<Token> tokens;
	int i = 0;
	while (i < (int)expr.size())
	{
		if (isspace(expr[i]))
		{
			i++;
			continue;
		}

		if (isdigit(expr[i]))
		{
			string num = "";
			while (i < (int)expr.size() && isdigit(expr[i]))
			{
				num = num + expr[i];
				i++;
			}

			tokens.push_back({ num,0 });

		}
		else if (isalpha(expr[i]))
		{
			string var = "";
			while (i < (int)expr.size() && (isalnum(expr[i]) || expr[i] == '_'))
			{
				var = var + expr[i];
				i++;
			}
			tokens.push_back({ var,1 });

		}
		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')
		{
			tokens.push_back({ string(1, expr[i]), 2 });
			i++;
		}
		else if (expr[i] == '(' || expr[i] == '[' || expr[i] == '{')
		{
			tokens.push_back({ string(1, expr[i]), 3 });
			i++;
		}
		else if (expr[i] == ')' || expr[i] == ']' || expr[i] == '}')
		{
			tokens.push_back({ string(1, expr[i]), 4 });
			i++;
		}
		else
		{
			cerr << "Syntax Error : unexpected";
			exit(1);
		}


	}

	return tokens;
}

vector<Token> convertToPostfix(vector <Token>& tokens)
{
	vector<Token> output;
	stack<Token> ops;

	if (tokens.empty())
	{
		cerr << "Syntax error : Empty Expression" << endl;
		exit(1);

	}


	for (int i = 0;i < (int)tokens.size();i++)
	{
		Token t = tokens[i];
		if (t.type == 0 || t.type == 1)
		{
			output.push_back(t);

			if (i + 1 < (int)tokens.size() && (tokens[i + 1].type == 0 || tokens[i + 1].type == 1))
			{
				cerr << "Synatax Error Two operands in the row  " << endl;
				exit(1);
			}
		}
		else if (t.type == 2)
		{
			if (i == 0)
			{
				cerr << "Syntax Error Expression start With an operator " << endl;
				exit(1);
			}
			if (i == (int)tokens.size() - 1)
			{
				cerr << "Syntax Error Expression ends With an operator " << endl;
				exit(1);
			}
			if (i + 1 < (int)tokens.size() && tokens[i + 1].type == 2)
			{
				cerr << "Syntax Error Two operators start ina row " << endl;
				exit(1);
			}

			while (!ops.empty() && ops.top().type == 2 && getPrecedence(ops.top().val) >= getPrecedence(t.val))
			{
				output.push_back(ops.top());
				ops.pop();

			}
			ops.push(t);
		}
		else if (t.type == 3)
		{
			ops.push(t);
		}
		else if (t.type == 4)
		{
			bool found = false;
			while (!ops.empty())
			{
				if (ops.top().type == 3)
				{
					if (!bracketMatch(ops.top().val, t.val))
					{
						cerr << "Syntax Error : mismatch types " << endl;
						exit(1);

					}
					ops.pop();
					found = true;
					break;

				}
				output.push_back(ops.top());
				ops.pop();

			}

			if (!found)
			{
				cerr << "Syntax Error " << endl;
				exit(1);

			}
		}
		
	}

	while (!ops.empty())
	{
		if (ops.top().type == 3)
		{
			cerr << "Stntax Error Unmatched Opening brackets " << ops.top().val << " " << endl;
			exit(1);
		}
		output.push_back(ops.top());
		ops.pop();

	}

	return output;
}


int main()
{
	string expr;
	getline(cin, expr);
	vector<Token>tokens = tokenize(expr);
	vector<Token>postFix = convertToPostfix(tokens);

	map<string, double>varMap;
	vector<string>varList;

	for (auto& t : tokens)
	{
		if (t.type == 1 && varMap.find(t.val) == varMap.end())
		{
			varMap[t.val] = 0;
			varList.push_back(t.val);

		}

	}
		for (int i = 0;i < (int)postFix.size();i++)
		{
			if (i != 0)
			{
				cout << " ";

			}
			cout << postFix[i].val;
		}
		cout << "\n";
		
		for (auto& var : varList)
		{
			double val;
			cerr << "Enter value for " << var << " : ";
			cin >> val;
			varMap[var] = val;

		}

       stack<double> evalStack;

		for (auto& t : postFix)
		{
			if (t.type == 0)
			{
				evalStack.push(stod(t.val));
			}
			else if (t.type == 1)
			{
				evalStack.push(varMap[t.val]);

			}
			else if (t.type == 2)
			{
				if (evalStack.size() < 2)
				{
					cerr << "Runtime Error : Not enough operands for operator  " << t.val;
					exit(2);

				}
				double b = evalStack.top();
				evalStack.pop();
				double a = evalStack.top();
				evalStack.pop();

				if (t.val == "+")
				{
					evalStack.push(a + b);
				}
				else if (t.val == "-")
				{
					evalStack.push(a - b);
				}
				else if (t.val == "*")
				{
					evalStack.push(a * b);
				}
				else if (t.val == "/")
				{
					if (b == 0)
					{
						cerr << "Rutime Error : Division by zero" << endl;
						exit(2);
					}
					evalStack.push(a / b);
				}
			}
		}
	
	if(evalStack.size()!=1)
    {
		cerr << "Logical Error : Expression cannot be fully evaluated " << endl;
		exit(3);

	}

	double result = evalStack.top();

	if (result == (long long)result)
	{
		cout << (long long)result << "\n";
	}
	else
	{
		cout << result << "\n";

	}


	return 0;
}
