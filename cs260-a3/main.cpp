/*
	Author:		Cody Potter
	Title:		Infix -> Postfix Evalutator
	Purpose:	CS260 Assignment 3
	Desc:		This program converts given infix expressions to 
				postfix expressions, and evaluates them, printing
				as it goes.
				I really enjoyed this assignment.
	Instructor: Jen Miller
*/

#include<climits>	// only used this for INT_MAX as error indicator
#include<iostream>
#include<stack>
#include<string>
#include<vector>	// not necessary, but it made things a little easier

using namespace std;

/*			Function Prototypes			*/
/*  Most of these are self explanatory	*/
bool isOperand(char testChar);
bool isOperator(char testChar);
bool isLeftParenthesis(char testChar);
bool isRightParenthesis(char testChar);
bool isExponent(char testChar);
bool hasHigherPrecedence(char operator1, char operator2);
bool hasOnlyValidCharacters(string testString);
string removeWhiteSpace(string stringToFormat);
void convertInfixToPostfixAndEvaluate(std::vector<std::string> &infixExpressions);
string convertInfixToPostfix(string expression);
int getOperatorPriority(char operatorChar);
int evaluatePostfix(string expression);
int performOperation(char theoperator, int operand1, int operand2);
int convertCharToInt(char theChar);

/*
	Function:	Main
	Desc:		Entry point, output author name, define a vector
				of infix expressions, call a method to do the hard work
	Parameters:	void
	Return:		0 when complete
*/
int main(void) {
	cout << "Author: Cody Potter" << endl << endl;

	vector<string> infixExpressions = {	"5 # 2",
									"5 * - 6 8",
									"5*2  +   3",
									"3 + 5 * (7-2)",
									"3 + ( (8-5) * (4+9) )",
									"3 ^ 6"
								};

	convertInfixToPostfixAndEvaluate(infixExpressions);
	return 0;
}

/*
	Function:	Convert Infix To Postfix And Evaluate
	Desc:		Iterates over vector, converting and evaluating as it goes
	Parameters:	Vector of infix strings
	Return:		void
*/
void convertInfixToPostfixAndEvaluate(std::vector<std::string> &infixExpressions) {
	for (unsigned int i = 0; i < infixExpressions.size(); i++) {
		cout << "Infix: " << infixExpressions.at(i) << endl;
		string postfixExpression = "";

		postfixExpression = convertInfixToPostfix(infixExpressions.at(i));
		if (!hasOnlyValidCharacters(postfixExpression)) {
			cout << "ERROR: Not a valid infix expression. Please try again." << endl << endl;
			continue;
		}
		cout << "Postfix: " << postfixExpression << endl;

		int value = evaluatePostfix(postfixExpression);
		if (value == INT_MAX) {
			cout << "ERROR: Not a valid infix expression. Please try again." << endl << endl;
			continue;
		}
		cout << "Result: " << value << endl << endl;
	}
}

/*
	Function:	Remove White Space
	Desc:		Iterates over given string, deleting any space or newline characters
	Parameters:	String to format
	Return:		formatted string
*/
string removeWhiteSpace(string stringToFormat) {
	string formattedString = "";
	for (unsigned int i = 0; i < stringToFormat.length(); i++) {
		if (stringToFormat[i] != ' ' && stringToFormat[i] != '\n') {
			formattedString += stringToFormat[i];
		}
	}
	return formattedString;
}

/*
	Function:	Convert Infix To Postfix
	Desc:		Converts infix paramater into a postfix
				expression using a stack to store operators
	Parameters:	String infix expression
	Return:		String postfix expression
*/
string convertInfixToPostfix(string expression) {
	stack<char> operators;
	string postfix = "";
	expression = removeWhiteSpace(expression);

	for (unsigned int i = 0; i < expression.length(); i++) {
		if (isOperator(expression[i])) {
			// operator case
			while (!operators.empty() && !isLeftParenthesis(operators.top()) && hasHigherPrecedence(operators.top(), expression[i])) {
				postfix += operators.top();
				operators.pop();
			}
			operators.push(expression[i]);
		} else if (isOperand(expression[i])) {
			// operand case
			postfix += expression[i];
		} else if (isLeftParenthesis(expression[i])) {
			// left parenthesis case
			operators.push(expression[i]);
		} else if (isRightParenthesis(expression[i])) {
			// right parenthesis case
			while (!operators.empty() && !isLeftParenthesis(operators.top())) {
				postfix += operators.top();
				operators.pop();
			}
			operators.pop();
		} else {
			return "error";
		}
	}

	while (!operators.empty()) {
		postfix += operators.top();
		operators.pop();
	}

	return postfix;
}

/*
	Function:	Has Higher Precedence
	Desc:		Determines which of two operators has
				higher precedence
	Parameters:	2 Character operators
	Return:		Boolean if operator 1 has precedence over 
				operator 2
*/
bool hasHigherPrecedence(char operator1, char operator2) {
	int operator1Weight = getOperatorPriority(operator1);
	int operator2Weight = getOperatorPriority(operator2);

	if (operator1Weight == operator2Weight) {
		return !isExponent(operator1); 
	}
	return operator1Weight > operator2Weight;
}

/*
	Function:	Get Operator Priority
	Desc:		Assigns an integer 'priority' based on 
				operator precedence
	Parameters:	Character operator
	Return:		Integer priority number
*/
int getOperatorPriority(char operatorChar) {
	int priority = 0;
	switch (operatorChar) {
	case '+':
	case '-':
		priority = 1;
		break;
	case '*':
	case '/':
		priority = 2;
		break;
	case '^':
		priority = 3;
		break;
	}
	return priority;
}

/*
	Function:	Is Operand
	Desc:		Tests if parameter is an operand
	Parameters:	Character test character
	Return:		Boolean if test character is an operand
*/
bool isOperand(char testChar) {
	return (testChar >= '0' && testChar <= '9');
}

/*
	Function:	Is Operator
	Desc:		Tests if parameter is an operator
	Parameters:	Character test character
	Return:		Boolean if test character is an operator
*/
bool isOperator(char testChar) {
	return (testChar == '+' || testChar == '-' || testChar == '*' || testChar == '/' || testChar == '^');
}

/*
	Function:	Is Left Parenthesis
	Desc:		Tests if parameter is a left parenthesis
	Parameters:	Character test character
	Return:		Boolean if test character is a left parenthesis
*/
bool isLeftParenthesis(char testChar) {
	return testChar == '(';
}

/*
	Function:	Is Right Parenthesis
	Desc:		Tests if parameter is a right parenthesis
	Parameters:	Character test character
	Return:		Boolean if test character is a right parenthesis
*/
bool isRightParenthesis(char testChar) {
	return testChar == ')';
}

/*
	Function:	Is Exponent
	Desc:		Tests if parameter is an exponent operator
	Parameters:	Character test character
	Return:		Boolean if test character is an exponent operator
*/
bool isExponent(char testChar) {
	return testChar == '^';
}

/*
	Function:	Evaluate Postfix
	Desc:		Evaluates a postfix expression, returning the result
	Parameters:	String postfix expression
	Return:		Integer result from postfix expression
*/
int evaluatePostfix(string expression) {
	stack<int> operands;

	for (unsigned int i = 0; i < expression.length(); i++) {
		if (isOperand(expression[i])) {
			operands.push(convertCharToInt(expression[i]));

		} else if (isOperator(expression[i]) && !operands.empty()) {
			int operand2 = operands.top();	// store each operand from the stack, and pop off
			operands.pop();
			if (operands.empty()) { return INT_MAX; } // make sure there are enough operands to pop
			int operand1 = operands.top();
			operands.pop();

			int result = performOperation(expression[i], operand1, operand2);
			if (result == INT_MAX) { return result; }
			operands.push(result);
		}
	}
	return operands.top();
}

/*
	Function:	Perform Operation
	Desc:		Performs a passed in operation
	Parameters:	Character operator, 2 Integer operands
	Return:		Integer result from operation
*/
int performOperation(char theOperator, int operand1, int operand2) {
	int result = 1;
	switch (theOperator) {
	case '+': return operand1 + operand2;
	case '-': return operand1 - operand2;
	case '*': return operand1 * operand2;
	case '/': return operand1 / operand2;
	case '^':
		for (int i = 0; i < operand2; i++)
			result *= operand1;
		return result;
	default: return INT_MAX;
	}
}

/*
	Function:	Convert Char To Int
	Desc:		Converts a character to an integer that 
				visually represents the chracter
	Parameters:	Character to convert
	Return:		Integer result from conversion
*/
int convertCharToInt(char theChar) {
	int operand = theChar - '0';
	return operand;
}

/*
	Function:	Has Only Valid Characters
	Desc:		Tests given string for errors and
				checks that all characters are listed
				operators or operands
	Parameters:	String to test
	Return:		Boolean if only has valid characters
*/
bool hasOnlyValidCharacters(string testString) {
	if (testString == "error") { return false; }
	bool test = false;
	for (unsigned int i = 0; i < testString.length(); i++) {
		if (isOperand(testString[i]) || isOperator(testString[i])) {
			test = true;
		} else {
			return false;
		}
	}
	return test;
}