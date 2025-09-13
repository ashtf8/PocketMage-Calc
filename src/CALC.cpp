//   .oooooo.         .o.       ooooo          .oooooo.  //
//  d8P'  `Y8b       .888.      `888'         d8P'  `Y8b //
// 888              .8"888.      888         888         //
// 888             .8' `888.     888         888         //
// 888            .88ooo8888.    888         888         //
// `88b    ooo   .8'     `888.   888       o `88b    ooo //
//  `Y8bood8P'  o88o     o8888o o888ooooood8  `Y8bood8P' // 

// PocketMage V3.0
// @Ashtf, @Sagar 2025

#include <pocketmage.h>

static constexpr const char* TAG = "MAIN";
static String currentLine = "";
static String currentWord = "";
static volatile bool doFull = false;


// !! Commented for code-review
// to-do 
// depreciate ~C~ and ~R~ for flags included in LineView 
// - flags bitmask 0000 0000
//  LF_NONE= 0000 0000
//  LF_RIGHT= 0000 0001
//  LF_CENTER= 0000 00010
//  other flags to be determined 
// make convertToRPN && evaluateRPN able to handle standard notation inputs from other apps 
//  - (in tokenize, convert operators into standard notation)
// programming mode
// implement matrix multiplication
// implement for loops and conditionals
// strategy design pattern implementation for evaluateRPM?
//  - could save lines with repeated evalStack.top() evalStack.pop();
#pragma region helpers
///////////////////////////// HELPER FUNCTIONS
inline void calcClear() { calcLines.clear(); }
// COMPARE TO SET OF CONSTANTS
bool isConstantToken(const String& token) {
  for (size_t i=0;i<constantsCalcCount;++i) {
    if (token == constantsCalc[i]) return true;
  }
  return false;
}
// CONFRIM TOKEN IS A NUMBER
bool isNumberToken(const String& token) {
    if (token.isEmpty()) return false;
    size_t start = 0;
    // check value after unary negative operators
    if (token[0] == '-') {
        start = 1;
        if (token.length() == 1) return false;
    }
    bool hasDecimal = false;
    // check if float is valid (no trailing '.' + no alpha)
    for (size_t i = start; i < token.length(); i++) {
        if (token[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
            // check for back '.'
            if (i == token.length() - 1) {
                return false; // "." at end
            }
        }
        else if (token[i] != '.' && !isDigit(token[i])) {
            return false;
        }
    }
    return true;
}
// CONFIRM TOKEN IS A FUNCTION
bool isFunctionToken(const String& token) {
  for (size_t i=0;i<functionsCalcCount;++i) {
    if (token == functionsCalc[i]) return true;
  }
  return false;
}
// CONFIRM A TOKEN IS A VARIABLE INCLUDING CONSTANTS
bool isVariableToken(const String& token) {
    if (token.isEmpty()) return false;
    // exclude constants and functions
    if (isConstantToken(token)) return false;
    if (isFunctionToken(token)) return false;

    // check if alphanumeric starting with alpha 
    if (!isAlpha(token[0])) return false;
    for (size_t i = 1; i < token.length(); i++) {
        if (!isAlphaNumeric(token[i])) return false;
    }
    return true;
}
// COMPARE TO SET OF OPERATORS
bool isOperatorToken(const String& token) {
  for (size_t i=0;i<OPS_N;++i) if (token.equals(OPS[i].token)) return true;
  return false;
}
// CHECK OPERATOR PRECEDENCE
uint8_t precedenceOfToken(const String& token) {
  for (size_t i=0;i<OPS_N;++i) if (token.equals(OPS[i].token)) return OPS[i].prec;
  return 0;
}
// CHECK OPERATOR RIGHT ASSOCIATIVITY
bool isRightAssociative(const String& token) {
  for (size_t i=0;i<OPS_N;++i) if (token.equals(OPS[i].token)) return OPS[i].rightAssoc;
  return false;
}
// CONVERT TRIG INPUTS
double convertTrig(double input,int trigType,bool reverse = false){
  switch (trigType){
    // 0 = degree mode
    case (0):
      return reverse? ((input*180)/PI) : ((input*PI)/180);
    break;
    // 1 = radian mode
    case (1):
      return input;
    break;
    // 2 = grad mode
    case (2):
      return reverse? ((input*200)/PI) : ((input*PI)/200);
    break;
    default:
      return input;
    break;
  }
}
// CONVERT UNITA TO UNITB
double convert(double value,const Unit* from,const Unit* to) {
    // convert to 
    double inBase = (value + from->offset) * from->factor;
    return (inBase / to->factor) - to->offset;
}
// ADD LINE TO CALCLINES
void calcAppend(const String& s, bool right = false, bool center=false) {
  uint8_t f = right ? LF_RIGHT : center ? LF_CENTER : LF_NONE;
  calcLines.pushLine(s.c_str(), (uint16_t)s.length(), f);
}
// GET UNIT FROM CURRENT UNIT TYPES
const Unit* getUnit(const String& sym) {
  const UnitSet* s = CurrentUnitSet;
  for (size_t i = 0; i < s->size; ++i) {
    // compare case-insensitively to handle "m^2" vs "M^2"
    //Serial.println("Searching set index " + String(i));
    //Serial.println("syn =  " + sym + "  s->data[i].symbol = " + String(s->data[i].symbol));
    if (sym.equalsIgnoreCase(s->data[i].symbol)) {
      //Serial.println("Found match! " + String(i));
      return &s->data[i];
    }
  }
  return nullptr;
}
// UPDATE UNIT FROM FRAME CHOICE
void selectUnitType(int idx) {
  //Serial.println("Applying selected Unit at idk " + String(idx));
  // clamp to known catalog size
  if (idx < 0) idx = 0;
  if ((size_t)idx >= (UnitCatalogCount)) {
    idx = UnitCatalogCount - 1;
  }
  CurrentUnitSet = &UnitCatalog[idx];
  // pair the visual list with the same index; guard if not defined yet
  if ((size_t)idx < (AllUnitListsCount) && allUnitLists[idx]) {
    CurrentUnitListSrc = allUnitLists[idx];
  }
  // point the visible frames at the new list, reset scroll/choice
  conversionFrameA.source = CurrentUnitListSrc;
  conversionFrameB.source = CurrentUnitListSrc;
  conversionFrameA.scroll = conversionFrameB.scroll = 0;
  conversionFrameA.prevScroll = conversionFrameB.prevScroll = -1;
  conversionFrameA.choice = conversionFrameB.choice = 0;
}

// TRIM EXCESS ZEROS !!
String trimValue(double value){
    char buffer[32];
    // Format mantissa with 10 decimals initially
    snprintf(buffer, sizeof(buffer), "%.8f", value);
    String valueStr(buffer);
    int dotPos = valueStr.indexOf('.');
    if (dotPos != -1) {
        int lastNonZero = valueStr.length() - 1;
        while (lastNonZero > dotPos && valueStr[lastNonZero] == '0') {
            lastNonZero--;
        }
        if (lastNonZero == dotPos) {
            valueStr.remove(dotPos + 2);
        } else {
            valueStr.remove(lastNonZero + 1);
        }
    }
    return valueStr;
}
#pragma endregion





#pragma region calc eink functions
///////////////////////////// CALC EINK FUNCTIONS
// CLOSE CALC AND UPDATE
void closeCalc(AppState newAppState){
  frames.clear();
  frames.push_back(&calcScreen);
  if (CurrentCALCState == CALC3){
    CurrentCALCState = CALC0;
  }
  // essential to display next app correctly 
  EINK().getDisplay().setFullWindow();
  EINK().getDisplay().fillScreen(GxEPD_WHITE);
  u8g2.clearBuffer();
  dynamicScroll = 0;
  prev_dynamicScroll = -1;
  updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
  if (newAppState == TXT) {
    //TXT_INIT();
  }  else {
    //CurrentAppState = HOME;
    //CurrentHOMEState = HOME_HOME; // ensure we land on Home grid, not NOWLATER
    currentLine     = "";
    newState        = true;
    CurrentKBState  = NORMAL; 
    disableTimeout = false;
  }

  rebootToPocketMage();
}

// CALC FRAME
void drawCalc(){
  // SET FONT
  EINK().getDisplay().setFullWindow();
  EINK().setTXTFont(EINK().getCurrentFont());
  Serial.println("drawing calc!");
  EINK().getDisplay().firstPage();
  do {
    // print status bar
    if (CurrentCALCState == CALC4){
      EINK().drawStatusBar("<- -> scroll | enter -> EXIT");
    } else {
      EINK().drawStatusBar("\"/4\" -> info | \"/6\" -> EXIT");
    }
    // draw calc bitmap

    EINK().getDisplay().drawBitmap(0, 0, calcAllArray[0], 320, 218, GxEPD_BLACK);
    // print current calc mode
    EINK().getDisplay().setCursor(25, 20);
    switch (CurrentCALCState) {
      case CALC0:      
        //standard mode
        EINK().getDisplay().print("Calc: Standard");
        break;
      case CALC1:
        //programming mode
        EINK().getDisplay().print("Calc: Programming");
        break;
      case CALC2:
        //scientific mode
        EINK().getDisplay().print("Calc: Scientific");
        break;
      case CALC3:
        //conversions
        EINK().getDisplay().print("Calc: Conversions");
        break;
      case CALC4:
        //help mode
        EINK().getDisplay().print("Calc: Help");
        break;  
    }
    // print current trig mode
    EINK().getDisplay().setCursor(240, 20);
    if (!(CurrentCALCState == CALC4)){
    switch (trigType){
      // 0 = degree mode
      case (0):
        EINK().getDisplay().print("deg");
      break;
      // 1 = radian mode
      case (1):
        EINK().getDisplay().print("rad");
      break;
      // 2 = gradian mode
      case (2):
        EINK().getDisplay().print("gon");
      break;
    }
  }
  } while (EINK().getDisplay().nextPage());
  Serial.println("done drawing calc!");
}
#pragma endregion





#pragma region calc algorithms
///////////////////////////// ALGORITHMS
// FORMAT INTO RPN,EVALUATE,SAVE
int calculate(const String& cleanedInput,String &result,const Unit *convA,const Unit *convB){
  // convert to reverse polish notation for easy calculation
  std::deque<String> inputRPN = convertToRPN(cleanedInput);
  // calculate the RPN expression
  result = evaluateRPN(inputRPN,convA,convB);
  return 0;
}
// CONVERT DOUBLE TO SCIENTIFIC NOTATION !!
String formatScientific(double value) {
    // handle overflow and underflow edge cases
    if (String(value) == "inf" || String(value) == "-inf") {
      //Serial.println("overflow error");
      return String(value);
    }
    if (abs(value) < 1e-300) return "0e0";
    int exponent = 0;
    double mantissa = value;
    bool negative = mantissa < 0;
    String result;
    // normalize
    if (negative) mantissa = -mantissa;
    while (mantissa >= 10.0) {
        mantissa /= 10.0;
        exponent++;
    }
    while (mantissa < 1.0) {
        mantissa *= 10.0;
        exponent--;
    }
    if (negative) mantissa = -mantissa;
    result = trimValue(mantissa);
    result += "e" + String(exponent);
    return result;
}
// CONVERT NUMBER TO FLOAT STRING OR INT STRING !!
String formatNumber(double value) {
    //Serial.println("formating number " + String(value));
    // NaN/Inf guards
    if (isnan(value)) return "nan";
    if (!isfinite(value)) return value < 0 ? "-inf" : "inf";
    String result;
    char buffer[32];
    //Serial.println("formatting number!");
    if (CurrentCALCState == CALC2){
      return formatScientific(value);
    } 
    else {
      if ((value > INT_MAX || ((fabs(value - round(value)) <= 1e-9)&&(fabs(value - round(value)) > 1e-300)))) {
        //Serial.println("sending scientifc instead for conversion!");
        return formatScientific(value);
      }
      // handle integer test case
      if (fabs(value - round(value)) < 1e-9) {
        snprintf(buffer, sizeof(buffer), "%lld", static_cast<long long>(round(value)));
        return String(buffer);
      }
      result = trimValue(value);
    }
    //Serial.println("Returning standard number!");
    return result;
}
// STRING INPUT TO RPN
std::deque<String> convertToRPN(String expression) {
    std::deque<String> outputQueue;
    std::stack<String> operatorStack;
    std::vector<String> tokens = tokenize(expression);
    //Serial.println("Converting to RPN: " + expression);
    // Parenthesis validation
    /*
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
      Serial.println("RPN Token: " + *it);
    }
    */
    int paren_balance = 0;
    for (char c : expression) {
        if (c == '(') paren_balance++;
        else if (c == ')') paren_balance--;
    }
    if (paren_balance != 0) return {}; 
    
    // Shunting Yard Algorithm
    for (size_t i = 0; i < tokens.size(); ++i) {
        const String& token = tokens[i];
        if (isNumberToken(token) || isConstantToken(token)) {
            outputQueue.push_back(token);
        } 
        else if (isFunctionToken(token)) {
            operatorStack.push(token);
        }
        // add argument 1 to output queue for functions with multiple arguments
        else if (token == ",") {
          while (!operatorStack.empty() && operatorStack.top() != "(") {
              outputQueue.push_back(operatorStack.top());
              operatorStack.pop();
          }
        } 
        else if (isAlpha(token[0]) && token != "E") {
          if (i + 1 < tokens.size() && tokens[i+1] == ":") {
              // For assignment, push variable name marker to output
              //Serial.println("pushed ~var~ + variable!");
              outputQueue.push_back("~var~" + token);
          } else {
              //Serial.println("pushed variable!");
              outputQueue.push_back(token);
          }
        }
        else if (token == "(") {
            operatorStack.push(token);
        } 
        // add all items in operator stack to output until first "(", remove "(", and push function to output
        else if (token == ")") {
          while (!operatorStack.empty() && operatorStack.top() != "(") {
              outputQueue.push_back(operatorStack.top());
              operatorStack.pop();
          }
          if (!operatorStack.empty()) operatorStack.pop(); // pop "("
          if (!operatorStack.empty() && isFunctionToken(operatorStack.top())) {
              outputQueue.push_back(operatorStack.top());
              operatorStack.pop();
          }
        } 
        /*
        if ((token == "~neg~")) {
          if ((i+1) < tokens.size()){
            //Serial.println("pushed unary negation! RPN");
            operatorStack.push(String(-1*variables[tokens[i+1]]));
          }

          i++;
          continue;
        }
        */
        // sort operators by precedence, push all operators of greater precedence to the output queue and place new operator in operator stack
        else if (isOperatorToken(token)) {
          // assignment guard stays the same...
          if (token == ":") {
            if (outputQueue.empty()) return {};
            const String& prev = outputQueue.back();
            // after the refactor, the variable is emitted as "~var~name"
            if (!prev.startsWith("~var~")) return {};
          }

          const bool rightA = isRightAssociative(token);
          const uint8_t p1 = precedenceOfToken(token);

          while (!operatorStack.empty()
                && operatorStack.top() != "(")
          {
            const String& o2 = operatorStack.top();
            if (!isOperatorToken(o2)) break;

            const uint8_t p2 = precedenceOfToken(o2);
            const bool popIt =
              (!rightA && (p1 <= p2)) ||   // left-assoc: pop while prec <=
              ( rightA && (p1 <  p2));     // right-assoc: pop while prec <
            if (!popIt) break;

            outputQueue.push_back(o2);
            operatorStack.pop();
          }

          operatorStack.push(token);
        }
    }
    // empty rest of operator stack
    while (!operatorStack.empty()) {
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}
// SPLIT STRING INTO TOKENS
// can make tokenize add proper operators instead of replacement operators so that convertToRPN is able to handle standard notation inputs from other apps
// i.e. " : " -> " = ", " ' " -> " * ", " " " -> "^"
std::vector<String> tokenize(const String& expression) {
    std::vector<String> tokens;
    String currentToken = "";
    bool usedRepeatFunction = false;
    //Serial.println("Tokenizing expression: " + expression);
    for (int i = 0; i < expression.length(); ++i) {
      char c = expression[i];
      //Serial.println("handling character: " + String(c));   
      // Handle assignment '='
      if (c == ':') {
          // Single '=' for assignment
          tokens.push_back(":");  // Keep as : for assignment
          continue;
      }
      // handle - and unary negation
      bool prevIsOperatorOrStart =
          (i == 0) || 
          isOperatorToken(String(expression[i - 1])) || 
          expression[i - 1] == '(' || 
          expression[i - 1] == ',' || 
          expression[i - 1] == ':';

      if (c == '-' && prevIsOperatorOrStart) {
          if (i + 1 < expression.length() && isDigit(expression[i + 1])) {
            //Serial.println("pushed back negative number");
            currentToken += c;
            while (i + 1 < expression.length() &&
                  (isDigit(expression[i + 1]) || expression[i + 1] == '.')) {
                currentToken += expression[++i];
            }
            tokens.push_back(currentToken);
            currentToken = "";
            continue;
          } else {
            //Serial.println("pushed back unary minus operator");
            tokens.push_back("~neg~");
            continue;
          }
      }
      // handle !! macro
      if (!usedRepeatFunction && c == '!' && i + 1 < expression.length() && expression[i + 1] == '!') {
          if (prevTokens.empty()) {
            OLED().oledWord("E: no previous expression");
            delay(1000);
            return {};
          }

          usedRepeatFunction = true;
          // Add in previous expression tokens
          for (auto it = prevTokens.begin(); it != prevTokens.end(); ++it) {
              Serial.println("pushing token: " + *it + " from previous expression");
              tokens.push_back(*it);
          }

          i++;
          continue;
      }
            
      // Handle numbers
      if (isDigit(c) || (c == '.' && i + 1 < expression.length() && isDigit(expression[i + 1]))) {
          //Serial.println("handling" + currentToken);
          currentToken += c;
          while (i + 1 < expression.length()) {
              char peek = expression[i + 1];
              if (isDigit(peek) || peek == '.') {
                  currentToken += expression[++i];
              } else {
                  break;
              }
          }

          //Serial.println("Pushing number " + String(currentToken));
          tokens.push_back(currentToken);
          currentToken = "";
          continue;
      }

      // Handle alphabetic tokens
      if (isAlpha(c) && c != 'E') {
          currentToken += c;
          while (i + 1 < expression.length()) {
              if (isAlphaNumeric(expression[i + 1]) && expression[i + 1] != 'E') {
                  currentToken += expression[++i];
              } else {
                  break;  // Stop at anything like '('
              }
          }

          tokens.push_back(currentToken);
          currentToken = "";
          continue;
      }

      // Handle parentheses with implied multiplication
      if (c == '(') {
          if (!tokens.empty()) {
              const String& prev = tokens.back();
              bool insertMultiply = false;
              // If the previous token is a number, constant, or closing parentheses
              if (isNumberToken(prev) || prev == ")" || prev == "pi" || prev == "e" || prev == "ans") {
                  insertMultiply = true;
              }
              // If it's an alphanumeric and not a known function, assume variable * (
              if (isAlpha(prev[0]) && !isFunctionToken(prev) && prev != "E") {
                  insertMultiply = true;
              }
              if (insertMultiply) {
                  tokens.push_back("'");
              }
          }

          tokens.push_back("(");
          continue;
      }
      // Handle closing parens
      if (c == ')') {
          tokens.push_back(")");
          continue;
      }
    // Handle operators and commas
    if (isOperatorToken(String(c)) || c == ',') {
        tokens.push_back(String(c));
        continue;
    }

      // Unknown token,error
      OLED().oledWord("E: malformed expression");
      delay(1000);
      return {}; 
    }
    prevTokens = tokens;
    return tokens;
}
// EVALUATE RPN
String evaluateRPN(std::deque<String> rpnQueue, const Unit *convA, const Unit *convB) {
    std::stack<double> evalStack;
    std::stack<String> varStack;
    // print queue
    //Serial.println("Handling evaluating RPN");
    //for (auto it = rpnQueue.begin(); it != rpnQueue.end(); it++) {
    //  Serial.println("eval Token: " + *it);
    //}
    
    while (!rpnQueue.empty()) {
        String token = rpnQueue.front();
        rpnQueue.pop_front();

        //can declare these if else blocks as inline functions 
        // Handle assignment variable marker first
        if (token.startsWith("~var~")) {
          String varName = token.substring(5);
          varStack.push(varName);
          continue;
        }
        else if (isNumberToken(token)) {
            evalStack.push(token.toDouble());
        }
        // Handle previous answer
        else if (token == "ans") {
            evalStack.push(variables["ans"]);
        }
        // Handle constants
        else if (token == "pi") {
            evalStack.push(PI);
        }
        else if (token == "e") {
            evalStack.push(EULER);
        // Handle unary negation
        } else if (token == "~neg~") {
          if (evalStack.empty()) return "E: ~neg~";
          double a = evalStack.top(); evalStack.pop();
          evalStack.push(-a);
        } else if (isAlpha(token[0]) && !isFunctionToken(token) && token != "E") {
          if (variables.find(token) != variables.end()) {
              evalStack.push(variables[token]);
          } else {
              OLED().oledWord("undefined variable");
              delay(1000);
              return "E: undefined variable";
          }
        }

        // Handle binary operators
        else if (token == "+") {
            if (evalStack.size() < 2) return "E: +";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a + b);
        }
        else if (token == "-") {
            //Serial.println("subtracting!");
            if (evalStack.size() < 2) return "E: -";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a - b);
        }
        else if (token == "'") {
            if (evalStack.size() < 2) return "E: *";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a * b);
        }
        else if (token == "/") {
            if (evalStack.size() < 2) return "E: /";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (b == 0) return "Div by 0";
            evalStack.push(a / b);
        }
        else if (token == "\"") {
            if (evalStack.size() < 2) return "E: ^";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(pow(a, b));
        }
        else if (token == "%") {
            if (evalStack.size() < 2) return "E: %";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            // Handle modulo with floating-point numbers using fmod
            if (b == 0) return "E: Div by 0 %";
            evalStack.push(fmod(a, b));
        }
        else if (token == "E") {
            if (evalStack.size() < 2) return "E: E";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (abs(b) > 300) return "E: large exponent";
            String temp = String(a, 8) + "e" + String(b,8);
            
            if (round(b) == 0){
              evalStack.push(1.0);
            } else {
              evalStack.push(a * pow(10, b));
            }

        }
        else if (token == ":") {
          // Needs exactly 1 value and 1 variable
          if (evalStack.empty() || varStack.empty()) {
              return "E: assignment needs 1 value and 1 variable";
          }
          String varName = varStack.top(); varStack.pop();
          double value = evalStack.top(); evalStack.pop();
          variables[varName] = value;
          evalStack.push(value);
        }


        // Handle unary operators
        else if (token == "!") {
            if (evalStack.empty()) return "E: !";
            double a = evalStack.top(); evalStack.pop();
            if (a < 0) return "E: ! input < 0 ";
            evalStack.push(tgamma(a + 1));
        }
        
        // Handle trig functions
        // Trig functions too numerous and messy, need to do something about repeat code 
        else if (token == "sin") {
            if (evalStack.empty()) return "E: sin";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(sin(a));
        }
        else if (token == "asin") {
            if (evalStack.empty()) return "E: asin";
            double a = evalStack.top(); evalStack.pop();
            if (a < -1 || a > 1) return "E: domain of asin";
            a = convertTrig(asin(a),trigType,true);
            evalStack.push(a);
        }
        else if (token == "sinh") {
            if (evalStack.empty()) return "E: sinh";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(sinh(a));
        }
        else if (token == "csc") {
            if (evalStack.empty()) return "E: csc";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (sin(a) == 0) return "E: Div by 0 csc";
            evalStack.push(1/sin(a));
        }
        else if (token == "acsc") {
            if (evalStack.empty()) return "E: acsc";
            double a = evalStack.top(); evalStack.pop();
            if (a==0) return "E: Div by 0 acsc";
            double inv = 1/a;
            if (inv < -1 || inv > 1) return "E: domain of acsc";
            a = convertTrig(asin(inv),trigType,true);
            evalStack.push(a);
        }
        else if (token == "csch") {
            if (evalStack.empty()) return "E: csch";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (sinh(a) == 0) return "E: Div by 0 csch";
            evalStack.push(1/sinh(a));
        }
        else if (token == "cos") {
            if (evalStack.empty()) return "E: cos";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(cos(a));
        }
        else if (token == "acos") {
            if (evalStack.empty()) return "E: acos";
            double a = evalStack.top(); evalStack.pop();
            if (a < -1 || a > 1) return "E: domain of acos";
            if (a == 0) a = PI/2;
            a = convertTrig(acos(a),trigType,true);
            evalStack.push(a);
        }
        else if (token == "cosh") {
            if (evalStack.empty()) return "E: cosh";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(cosh(a));
        }
        else if (token == "sec") {
            if (evalStack.empty()) return "E: sec";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (cos(a) == 0) return "E: Div by 0 sec";
            evalStack.push(1/cos(a));
        }
        // could add logic to handle a = +inf == PI/2
        else if (token == "asec") {
            if (evalStack.empty()) return "E: asec";
            double a = evalStack.top(); evalStack.pop();
            if (a==0) return "E: Div by 0 acsc";
            double inv = 1/a;
            if (inv < -1 || inv > 1) return "E: domain of acsc";
            a = convertTrig(acos(inv),trigType,true);
            evalStack.push(a);
        }
        else if (token == "sech") {
            if (evalStack.empty()) return "E: sech";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (cosh(a) == 0) return "E: Div by 0 sech";
            evalStack.push(1/cosh(a));
        }
        else if (token == "tan") {
            if (evalStack.empty()) return "E: tan";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(tan(a));
        }
        else if (token == "atan") {
            if (evalStack.empty()) return "E: atan";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(atan(a),trigType,true);
            evalStack.push(a);
        }
        else if (token == "tanh") {
            if (evalStack.empty()) return "E: tanh";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(tanh(a));
        }
        else if (token == "cot") {
            if (evalStack.empty()) return "E: cot";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (tan(a) == 0) return "E: Div by 0 cot";
            evalStack.push(1/tan(a));
        }
        else if (token == "acot") {
            if (evalStack.empty()) return "E: acot";
            double a = evalStack.top(); evalStack.pop();
            double result;
            if (a == 0) {
              // acot(0) = π/2
              result = PI / 2; 
            } else {
              // returns value in (−π/2, π/2)
              result = atan(1/a);
              // shift to principal range (0, π) 
              if (a<0) result += PI; 
            }
            evalStack.push(convertTrig(a,trigType,true));
        }
        else if (token == "coth") {
            if (evalStack.empty()) return "E: coth";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (tanh(a) == 0) return "E: Div by 0 coth";
            evalStack.push(1/tanh(a));
        }

        // handle single input functions
        else if (token == "sqrt") {
            if (evalStack.empty()) return "E: sqrt";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(sqrt(a));
        }
        else if (token == "cbrt") {
            if (evalStack.empty()) return "E: cbrt";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(cbrt(a));
        }
        else if (token == "exp") {
            if (evalStack.empty()) return "E: exp";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(exp(a));
        }
        else if (token == "round") {
            if (evalStack.empty()) return "E: round";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(round(a));
        }
        else if (token == "ln") {
            if (evalStack.empty()) return "E: ln";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(log(a));
        }
        else if (token == "floor") {
            if (evalStack.empty()) return "E: floor";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(floor(a));
        }
        else if (token == "ceil") {
            if (evalStack.empty()) return "E: ceil";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(ceil(a));
        }
        else if (token == "abs") {
            if (evalStack.empty()) return "E: abs";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(fabsf(a));
        }

        // handle multiple input functions
        // log(a,b) = log(a)/log(b) base b log of a
        else if (token == "log") {
            if (evalStack.size() < 2) return "E: log";
            double a = evalStack.top(); evalStack.pop();
            // base of log, order changed for a more natural input
            double b = evalStack.top(); evalStack.pop();
            evalStack.push(log(a)/log(b));
        }
        else if (token == "max") {
            if (evalStack.size() < 2) return "E: max";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(max(a, b));
        }
        else if (token == "min") {
            if (evalStack.size() < 2) return "E: min";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(min(a, b));
        }
        else if (token == "pow") {
            if (evalStack.size() < 2) return "E: pow";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(pow(a, b));
        }
        else if (token == "rand") {
            if (evalStack.size() < 2) return "E: rand";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(random(a, b));
        }
        else if (token == "dice") {
            if (evalStack.size() < 2) return "E: dice";
            int b = int(evalStack.top()); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (b < 2) return "E: dice sides";
            int roll = 0;

            for (int i = 0; i < a; i++){           
              roll+= (esp_random() % b) + 1;
              //Serial.println("current roll: " + String(roll));
            }
            evalStack.push(roll);
        }
        else if (token == "pick"){
            if (evalStack.size() < 1) return "E: pick no n arg";
            int a = static_cast<int>(evalStack.top()); evalStack.pop();
            int choices = a;
            if (evalStack.size() < choices) return "E: pick invalid choices";
            int pickedValue = random(1, choices+1);
            double valueToPush = 0;

            for (int i = 0; i < choices; i++){
              double poppedValue = evalStack.top(); evalStack.pop();
              if (i == (choices - pickedValue)) {
                valueToPush = poppedValue;
              }
            }
            evalStack.push(valueToPush);
        }
        else {
            return "E: Unknown token: " + token;
        }
    }

    if (evalStack.size() != 1) {
      return "E: malformed exp";
    }
    if (varStack.size() != 0){
      String varName = varStack.top(); varStack.pop();
      double value = evalStack.top();
      variables[varName] = value;
    }
    //Serial.println("exited evaluation RPN function");
    variables["ans"] = evalStack.top();
    // NOTE: for bit conversion, clamp input to int
    if ((convA != nullptr) && strcmp(convA->name, convB->name) != 0){
      //Serial.println("converting input!");
      cleanExpression = cleanExpression + convA->symbol;
      return formatNumber(convert(evalStack.top(), convA, convB));
    }
    return formatNumber(evalStack.top());
}
#pragma endregion






#pragma region I/O
///////////////////////////// INPUT/OUTPUT FUNCTIONS
// CALC DISPLAY ANSWER !!
void printAnswer(String cleanExpression,const Unit *convA,const Unit *convB) {
  int16_t x1, y1;
  uint16_t exprWidth, resultWidth, charHeight;
  String resultOutput = "";
  int maxWidth = EINK().getDisplay().width();
  int result = calculate(cleanExpression, resultOutput,convA,convB);

  // Set font before measuring text
  EINK().setTXTFont(EINK().getCurrentFont());
  if (CurrentCALCState == CALC3 && convA && convB){
    // Non-breaking space keeps value and unit together during wrapping
    const char* SP = " ";
    cleanExpression += String(SP) + convA->symbol;
    resultOutput    += String(SP) + convB->symbol;
  }
  calcAppend(cleanExpression);
  calcAppend("~R~" + resultOutput);
  newLineAdded = true;
}
// ENTER (CR) INPUT
void calcCRInput(){

  // trim spaces
  currentLine.replace(" ", ""); 
  // parse commands
  if (currentLine != ""){
    if (currentLine == "/0"){
        // standard mode
        CurrentCALCState = CALC0;
        calcSwitchedStates = 1;
    }  
    else if (currentLine == "/1"){
        // programming mode
        CurrentCALCState = CALC0;
        OLED().oledWord("Programming Mode not implemented"); 
        delay(1000);
        calcSwitchedStates = 1;
    }
    else if (currentLine == "/2"){
        // scientific mode
        CurrentCALCState = CALC2;
        calcSwitchedStates = 1;
    }
    else if (currentLine == "/3"){
        // conversion
        CurrentCALCState = CALC3;
        calcSwitchedStates = 1;
        newLineAdded = true;
    }
    else if (currentLine == "/4"){
        // help mode
        CurrentCALCState = CALC4;
        Serial.println("helpScreen scroll = " + String(helpScreen.scroll));
        calcSwitchedStates = true;

    }
    else if (currentLine == "/5") {
        // write current file to text
        OLED().oledWord("Exporting Data to TXT!");
        delay(1000);

        // CHANGED: copy from source into a vector
        allLines = sourceToVector(CurrentFrameState->source);

        // remove '~R~' or '~C~' formatting (not used by txt app)
        for (int i = 0; i < allLines.size(); i++) {
          if (!(i % 2 == 0)) {
            String temp = allLines[i - 1] + " = " + allLines[i].substring(3);
            allLines[i - 1] = temp;
            allLines[i] = "";
          }
        }
        delay(200);
        closeCalc(TXT);
    }
    else if (currentLine == "/6"){
      closeCalc(HOME);
    }
    else if (currentLine == "/gon"){
      trigType = 2;
      drawCalc();
    }
    else if (currentLine == "/rad"){
      trigType = 1;
      drawCalc();
    }
    else if (currentLine == "/deg"){
      trigType = 0;
      drawCalc();      
    }
    else {
      // no command, calculate answer
      dynamicScroll = 0;
      updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);
      // handle only !!
      currentLine == "!!" ? prevLine : currentLine;
      //Serial.println("PrevLine updated to : " + currentLine);
      if (CurrentCALCState == CALC3){
        conversionFrameA.bottom = conversionFrameA.origBottom;
        conversionFrameB.bottom = conversionFrameA.origBottom;
        CurrentFrameState = &conversionScreen;
        String conversionA = frameChoiceString(conversionFrameA);
        String conversionB = frameChoiceString(conversionFrameB);

        const Unit* convA = getUnit(conversionA);
        const Unit* convB = getUnit(conversionB);

        conversionDirection.choice == 0 ? printAnswer(currentLine,convA,convB):printAnswer(currentLine,convB,convA);


        
      } else {
        printAnswer(currentLine, &emptyUnit, &emptyUnit);
      }
      prevLine = currentLine;
    }
  }
  calculatedResult = "";
  currentLine = "";
  newLineAdded = true;
}
#pragma endregion

///////////////////////////// MAIN FUNCTIONS
// CALC INITIALIZETION
void CALC_INIT() {
  // open calc
  Serial.println("Initializing CALC!");
  CurrentKBState = FUNC;
  CurrentCALCState = CALC0;
  CurrentFrameState = &calcScreen;
  frames.clear();
  frames.push_back(&calcScreen);

  // not clean, relying on magic number 11 since the frame hasn't been drawn with EinkFrameDynamic
  helpScreen.scroll = helpSrc.size() - 11;
  conversionFrameA.extendBottom = FRAME_BOTTOM + 8;
  conversionFrameA.overlap = 1;
  conversionFrameB.extendBottom = FRAME_BOTTOM + 8;
  conversionFrameB.overlap = 1;
  conversionUnit.extendBottom = FRAME_BOTTOM + 40;
  conversionUnit.overlap = 1;
  conversionDirection.choice = 0;
  conversionFrameA.choice = 0;
  conversionFrameB.choice = 0;
  conversionUnit.choice = 0;
  currentFrameChoice = conversionUnit.choice;

  dynamicScroll = 0;
  prev_dynamicScroll = -1;
  lastTouch = -1;
  newState = true;
  doFull = true;
  disableTimeout = true;
  EINK().setTXTFont(&FreeMonoBold9pt7b);
  currentLine = "";
}
// KB HANDLER
void processKB_CALC() {

  if (OLEDPowerSave) {
    u8g2.setPowerSave(0);
    OLEDPowerSave = false;
  }
  disableTimeout = false;
  int currentMillis = millis();
  if (currentMillis - KBBounceMillis >= KB_COOLDOWN) {  
    char inchar = KB().updateKeypress();

    EINK().setTXTFont(EINK().getCurrentFont());
    // update scroll (calc specific function,could be abstracted to processSB_CALC())
    updateScrollFromTouch_Frame();
    if (frameSelection){
      if (CurrentFrameState->choice != -1){
        if (CurrentFrameState == &conversionUnit) {
          //Serial.println("changed conversionUnit type!");
          selectUnitType(conversionUnit.choice);
        } else {
          String sym = frameChoiceString(*CurrentFrameState);
          //Serial.println("Setting Unit to " + sym);
          CurrentFrameState->unit = (Unit*)getUnit(sym);
        }
      }
      frameSelection = 0;
    }
    switch (CurrentCALCState) {

      // CONVERSIONS MODE 
      case CALC3:
        // HANDLE INPUTS
        // no char recieved
        if (inchar == 0);  
        else if (inchar == 8) {
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }
        }
        else if (inchar == '.') {
          currentLine += ".";
          CurrentKBState = FUNC;
        }
        else if (inchar == ','){
          if (currentLine.endsWith(",")){
            //Serial.println("Performed character replacement for ',,' ");
            currentLine[currentLine.length()-1] = '.';
          } else {
            currentLine += ",";
          }
        }
        // TAB Recieved (switch unit types aka length to area)
        else if (inchar == 9 || inchar == 14) {    
          conversionDirection.choice == 1 ? conversionDirection.choice = 0 :conversionDirection.choice= 1;
          newLineAdded = true;
          //("adjusting convDir to : " + String(conversionDirection.choice));
        }                                      
        // SHIFT Recieved
        else if (inchar == 17) {                                  
          if (CurrentKBState == SHIFT) CurrentKBState = NORMAL;
          else CurrentKBState = SHIFT;
        }
        // FN Recieved
        else if (inchar == 18) {                                  
          if (CurrentKBState == FUNC) CurrentKBState = NORMAL;
          else CurrentKBState = FUNC;
        }
        // Space Recieved
        else if (inchar == 32) {                                  
          if (CurrentFrameState != &conversionScreen){
            CurrentFrameState->bottom = CurrentFrameState->origBottom;
            CurrentFrameState = &conversionScreen;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;
            newLineAdded = true;
          } 
        }
        // CR Recieved
        else if (inchar == 13) {    
          //Serial.println(" current direction is " + String(conversionDirection.choice));
          calcCRInput(); // existing behavior                      
        }
        // ESC / CLEAR Recieved
        else if (inchar == 20) {   
          if (CurrentFrameState == &conversionScreen){                               
            calcClear();
            currentLine = "";
            OLED().oledWord("Clearing...");
            delay(500);
            drawCalc();
            //Serial.println("In CALC3 Mode calling frames");
            einkFramesDynamic(frames,false);
            delay(300);
          }
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 12) {
          Serial.println("Pressed LEFT in CALC3!");
          if (CurrentFrameState != &conversionFrameA){
            Serial.println("Switching to frame A!");
            CurrentFrameState->bottom = CurrentFrameState->origBottom;
            CurrentFrameState = &conversionFrameA;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;

            CurrentFrameState->bottom = CurrentFrameState->extendBottom;

            newLineAdded = true;
          }           
        }
        // RIGHT (switch to conversion a tab or scroll down in tab)
        else if (inchar == 21 || inchar == 6) { 
          if (CurrentFrameState != &conversionFrameB){
            CurrentFrameState->bottom = CurrentFrameState->origBottom;
            CurrentFrameState = &conversionFrameB;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;

            CurrentFrameState->bottom = CurrentFrameState->extendBottom;
            newLineAdded = true;
          } 
        }
        //BKSP Recieved
        else if (inchar == 8) {    
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }                       
        }
        //SAVE Recieved (no plan to save calculations)
        else if (inchar == 6) {}
        //LOAD Recieved (no plan to load calculations)
        else if (inchar == 12) {}
        //FILE Recieved (switch to unit types tab)
        else if (inchar == 7) {
          if (CurrentFrameState != &conversionUnit){
            CurrentFrameState->bottom = CurrentFrameState->origBottom;
            CurrentFrameState = &conversionUnit;
            CurrentFrameState->bottom = CurrentFrameState->extendBottom;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;
            newLineAdded = true;
          } 
        }
        // Font Switcher (regular tab also starts the font switcher)
        else if (inchar == 14) {                                  
        }
        // add non-special characters to line
        else {
          currentLine += inchar;
          // No to return FN for ease of input
          // SHIFT will return to FUNC
          if (CurrentKBState == SHIFT) CurrentKBState = FUNC;
        }
        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          // ONLY SHOW OLEDLINE WHEN NOT IN SCROLL MODE
          if (lastTouch == -1) {
            OLED().oledLine(currentLine);
            if (prev_dynamicScroll != dynamicScroll) prev_dynamicScroll = dynamicScroll;
            updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          }
          else oledScrollFrame();
        }
        break;
      // HELP MODE (no need inputs)
      case CALC4:
          // CR Recieved
          if (inchar == 13) {
            currentLine = "/0";    
            calcCRInput();   
            break;
          }   
      // PROGRAMMING MODE (not implemented)
      case CALC1:

      // SCIENTIFIC MODE 
      case CALC2:


      // standard mode
      case CALC0:
        // HANDLE INPUTS
        // no char recieved
        if (inchar == 0);  
        else if (inchar == 8) {
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }
        }
        else if (inchar == '.') {
          currentLine += ".";
          CurrentKBState = FUNC;
        }
        else if (inchar == ','){
          if (currentLine.endsWith(",")){
            //Serial.println("Performed character replacement for ',,' ");
            currentLine[currentLine.length()-1] = '.';
          } else {
            currentLine += ",";
          }
        }
        // TAB Recieved (This starts the font switcher now)
        else if (inchar == 9) {    
          CurrentCALCState = CALCFONT;
          CurrentKBState = FUNC;
          newState = true;                              
        }                                      
        // SHIFT Recieved
        else if (inchar == 17) {                                  
          if (CurrentKBState == SHIFT) CurrentKBState = NORMAL;
          else CurrentKBState = SHIFT;
        }
        // FN Recieved
        else if (inchar == 18) {                                  
          if (CurrentKBState == FUNC) CurrentKBState = NORMAL;
          else CurrentKBState = FUNC;
        }
        // Space Recieved
        else if (inchar == 32) {                                  
          currentLine += " ";
        }
        // CR Recieved
        else if (inchar == 13) {    
          calcCRInput();                      
        }
        // ESC / CLEAR Recieved
        else if (inchar == 20) {                                  
          calcClear();
          currentLine = "";
          OLED().oledWord("Clearing...");
          delay(500);
          drawCalc();
          einkFramesDynamic(frames,false);
          delay(300);
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 12) {
          if (dynamicScroll < CurrentFrameState->source->size() - (9 + SCROLL_MAX)){
             dynamicScroll += SCROLL_MAX;
          } else if (dynamicScroll < CurrentFrameState->source->size() - (9 + SCROLL_MED)){
             dynamicScroll += SCROLL_MED;
          } else if (dynamicScroll < CurrentFrameState->source->size() - (9 + SCROLL_SML)){
             dynamicScroll += SCROLL_SML;
          } else if (dynamicScroll < CurrentFrameState->source->size() - 10) {
            dynamicScroll++;
          }
          newLineAdded = true; 
          updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);                            
        }
        // RIGHT (scroll down)
        else if (inchar == 21 || inchar == 6) { 

          if (dynamicScroll > (SCROLL_MAX +1)){
            dynamicScroll -= SCROLL_MAX;
          }else if (dynamicScroll > (SCROLL_MED +1)){
            dynamicScroll -= SCROLL_MED;
          } else if (dynamicScroll > (SCROLL_SML +1)){
            dynamicScroll -= SCROLL_SML;
          } else if (dynamicScroll > 1){
            dynamicScroll--;
          } 
          updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          newLineAdded = true;
        }
        //BKSP Recieved
        else if (inchar == 8) {    
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }                       
        }
        //SAVE Recieved (no plan to save calculations)
        else if (inchar == 6) {}
        //LOAD Recieved (no plan to load calculations)
        else if (inchar == 13) {}
        //FILE Recieved 
        else if (inchar == 7) {
          calcClear();
          currentLine = "";
          OLED().oledWord("Clearing...");
          delay(500);
          drawCalc();
          einkFramesDynamic(frames,false);
          delay(300);
        }
        // Font Switcher (regular tab also starts the font switcher)
        else if (inchar == 14) {                                  
          CurrentCALCState = CALCFONT;
          CurrentKBState = FUNC;
          newState = true;         
        }
        // add non-special characters to line
        else {
          currentLine += inchar;
          // No to return FN for ease of input
          // SHIFT will return to FUNC
          if (CurrentKBState == SHIFT) CurrentKBState = FUNC;
        }
        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          // ONLY SHOW OLEDLINE WHEN NOT IN SCROLL MODE
          if (lastTouch == -1) {
            OLED().oledLine(currentLine);
            if (prev_dynamicScroll != dynamicScroll) prev_dynamicScroll = dynamicScroll;
            updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          }
          else oledScrollFrame();
        }
        break;

      // FONT SWITCHER
      case CALCFONT:
        //No char recieved
        if (inchar == 0);
        //BKSP Recieved
        else if (inchar == 8) {                  
          CurrentCALCState = CALC0;
          CurrentKBState = NORMAL;
          newLineAdded = false;
          editingFile = "";
          currentLine = "";
          drawCalc();
          einkFramesDynamic(frames,true);
        }
        else if (inchar >= '0' && inchar <= '9') {
          int fontIndex = (inchar == '0') ? 10 : (inchar - '0');
          switch (fontIndex) {
            case 1:
              EINK().setCurrentFont(&FreeMonoBold9pt7b);
              break;
            case 2:
              EINK().setCurrentFont(&FreeSans9pt7b);
              break;
            case 3:
              EINK().setCurrentFont(&FreeSerif9pt7b);
              break;
            case 4:
              EINK().setCurrentFont(&FreeSerifBold9pt7b);
              break;
            case 5:
              EINK().setCurrentFont(&FreeMono12pt7b);
              break;
            case 6:
              EINK().setCurrentFont(&FreeSans12pt7b);
              break;
            case 7:
              EINK().setCurrentFont(&FreeSerif12pt7b);
              break;
            default:
              EINK().setCurrentFont(&FreeMonoBold9pt7b);
              break;
          }
          // SET THE FONT
          EINK().setTXTFont(EINK().getCurrentFont());

          // UPDATE THE ARRAY TO MATCH NEW FONT SIZE
          String fullTextStr = vectorToString();
          stringToVector(fullTextStr);

          CurrentCALCState = CALC0;
          CurrentKBState = NORMAL;
          newLineAdded = true;
          currentWord = "";
          currentLine = "";
      
          calcClear();

          // REFRESH SCREEN

          EINK().getDisplay().refresh();
          drawCalc();
          einkFramesDynamic(frames,true);

        }
        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          OLED().oledLine(currentWord, false);

        }

        break;
    }
    KBBounceMillis = currentMillis;
  }
}
// Eink handler
void einkHandler_CALC() {
  if (newLineAdded || newState) {
    // reset eink screen if returning from a new mode
    if (calcSwitchedStates == 1){
      calcSwitchedStates = 0;
      doFull = 1;
      frames.clear();
      if (CurrentCALCState != CALC3 && CurrentCALCState != CALC4)  {   
        // to-do: convert to helper function to reset all frame tabs
        conversionFrameA.bottom = conversionFrameA.origBottom;
        conversionFrameB.bottom = conversionFrameB.origBottom;
        conversionUnit.bottom = conversionUnit.origBottom;
        frames.push_back(&calcScreen);
        CurrentFrameState = &calcScreen;
      } else {
        doFull = 1;
        frames.clear();
        if (CurrentCALCState == CALC1){
          //frames.push_back(&programmingScreen);
          //frames.push_back(&numberSizeFrame);
          //frames.push_back(&hexadecimalFrame);
          //frames.push_back(&decimalFrame);
          //frames.push_back(&octalFrame);
          //frames.push_back(&binaryFrame);
          CurrentFrameState = &calcScreen; // placeholder
          CurrentFrameState->scroll = 0;
          CurrentFrameState->prevScroll = -1;
        } else if (CurrentCALCState == CALC3){
          frames.push_back(&conversionScreen);
          frames.push_back(&conversionFrameA);
          frames.push_back(&conversionFrameB);
          frames.push_back(&conversionDirection);
          frames.push_back(&conversionUnit);
          CurrentFrameState = &conversionScreen;
          CurrentFrameState->scroll = 0;
          CurrentFrameState->prevScroll = -1;
        }else {

          EINK().setCurrentFont(&FreeMonoBold9pt7b);
          EINK().setTXTFont(EINK().getCurrentFont());
          frames.push_back(&helpScreen);
          CurrentFrameState = &helpScreen;
          dynamicScroll = helpScreen.scroll;
          updateScroll(CurrentFrameState, prev_dynamicScroll, dynamicScroll);
          prev_dynamicScroll = -1;
        }
      }
      drawCalc();
      newLineAdded = true;
      einkFramesDynamic(frames,false);
      
    } else {

      switch (CurrentCALCState) {
        case CALC0:
          //standard mode
          if (newState && doFull) { 
            EINK().setFastFullRefresh(false);
            drawCalc();
            einkFramesDynamic(frames,false);
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              EINK().getDisplay().setFullWindow();
              drawCalc(); 
              EINK().setFastFullRefresh(true);
              einkFramesDynamic(frames,true);
              refresh_count = 0;
            } else {
              einkFramesDynamic(frames,false);
            }
            EINK().setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC1:
          //programming mode
          break;
        case CALC2:
          //scientific mode
          if (newState && doFull) { 
            drawCalc();
            einkFramesDynamic(frames,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              EINK().setFastFullRefresh(false);
              einkFramesDynamic(frames,true);
              refresh_count = 0;
            } else {
              einkFramesDynamic(frames,false);
            }
            EINK().setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC3:
          //conversions 
          if (newState && doFull) { 
            drawCalc();
            //Serial.println("In CALC3 Mode calling frames");
            einkFramesDynamic(frames,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              EINK().setFastFullRefresh(false);
              //Serial.println("In CALC3 Mode calling frames");
              einkFramesDynamic(frames,true);
              refresh_count = 0;
            } else {
              //Serial.println("In CALC3 Mode calling frames");
              einkFramesDynamic(frames,false);
            }
            EINK().setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC4:
          // help mode
          if (newState && doFull) { 
            drawCalc();
            einkFramesDynamic(frames,true);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              EINK().setFastFullRefresh(false);
              einkFramesDynamic(frames,true);
              refresh_count = 0;
            } else {
              einkFramesDynamic(frames,true);
            }
            EINK().setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALCFONT:
          EINK().getDisplay().firstPage();
          do {
            // false avoids full refresh
            EINK().einkTextDynamic(true, false);      
            EINK().getDisplay().setPartialWindow(60, 0, 200, 218);
            EINK().drawStatusBar("Select a Font (0-9)");
            EINK().getDisplay().fillRect(60, 0, 200, 218, GxEPD_WHITE);
            EINK().getDisplay().drawBitmap(60, 0, fontfont0, 200, 218, GxEPD_BLACK);
            for (int i = 0; i < 7; i++) {
              EINK().getDisplay().setCursor(88, 54 + (17 * i));
              switch (i) {
                case 0: EINK().setTXTFont(&FreeMonoBold9pt7b); break;
                case 1: EINK().setTXTFont(&FreeSans9pt7b); break;
                case 2: EINK().setTXTFont(&FreeSerif9pt7b); break;
                case 3: EINK().setTXTFont(&FreeSerifBold9pt7b); break;
                case 4: EINK().setTXTFont(&FreeMono12pt7b); break;
                case 5: EINK().setTXTFont(&FreeSans12pt7b); break;
                case 6: EINK().setTXTFont(&FreeSerif12pt7b); break;
              }
              EINK().getDisplay().print("Font Number " + String(i + 1));
            }
          } while (EINK().getDisplay().nextPage());
          CurrentKBState = FUNC;
          newState = false;
          newLineAdded = false;
          break;
        }
    }
    newState = false;
    newLineAdded = false;
  }
}



/////////////////////////////////////////////////////////////
//  ooo        ooooo       .o.       ooooo ooooo      ooo  //
//  `88.       .888'      .888.      `888' `888b.     `8'  //
//   888b     d'888      .8"888.      888   8 `88b.    8   //
//   8 Y88. .P  888     .8' `888.     888   8   `88b.  8   //
//   8  `888'   888    .88ooo8888.    888   8     `88b.8   //
//   8    Y     888   .8'     `888.   888   8       `888   //
//  o8o        o888o o88o     o8888o o888o o8o        `8   //
/////////////////////////////////////////////////////////////

void processKB() {
  processKB_CALC();
}

void applicationEinkHandler() {
  einkHandler_CALC();
}

// SETUP
void setup() {
  PocketMage_INIT();
  CALC_INIT();
}

void loop() {
  // Check battery
  pocketmage::power::updateBattState();
  
  // Run KB loop
  processKB();

  // Yield to watchdog
  vTaskDelay(50 / portTICK_PERIOD_MS);
  yield();
}

// migrated from einkFunc.cpp
void einkHandler(void* parameter) {
  vTaskDelay(pdMS_TO_TICKS(250)); 
  for (;;) {
    applicationEinkHandler();

    vTaskDelay(pdMS_TO_TICKS(50));
    yield();
  }
}