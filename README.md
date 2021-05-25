<a href="#">
    <div align="center">
        <img alt="Ape Lang Logo" height="150" src="https://i.pinimg.com/originals/94/86/16/9486166e44e3023bf4fdbd40848f7255.png"/>
    </div>
    <div align="center">
        <h1>Ape Lang Compiler</h1>
    </div>
    <div align="center">
        <img alt="GitHub" src="https://img.shields.io/github/license/DanilAndreev/ape-lang-compiler"/>
        <img alt="GitHub issues" src="https://img.shields.io/github/issues-raw/DanilAndreev/ape-lang-compiler">
        <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/DanilAndreev/ape-lang-compiler">
        <img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/DanilAndreev/ape-lang-compiler">
    </div>   
</a>

### Ape Lang

### Language grammar
```
ProgBody = ’{’ StatementList ’}’
IfStatement = if ‘(‘ BoolExpr ‘)’ DoBlock1 [else DoBlock2]
DoBlock1 = DoBlock
DoBlock2 = DoBlock
ForStatement = for ‘(’ IndExpr ‘;’ BoolExpr ‘;’ IndExpr ‘)’ DoBlock
WhileStatement = while ‘(’ BoolExpr ‘)’ DoBlock
WhileStatement = do DoBlock while ‘(’ BoolExpr ‘)’
IndExpr = Type Ident ’=’ BoolExpression
ArithmExpression1 = BoolExpr
ArithmExpression2 = BoolExpr
DoBlock = ’{’ StatementList ’}’
DoSection = ’{’ StatementList ’}’
StatementList = Statement { Statement }
Statement = (Assign | Inp | Out | Initialization) ‘;’ | ForStatement 
| IfStatemant
Assign = Ident ’=’ Expression
Inp = read ’(’ Ident ’)’
Out = print ’(’ Ident ’)’
Initialization = Type IdentDecl
IdentDecl = Ident [‘=’ Expression]
Type = int | float | bool
BoolExpr = BoolTerm | BoolExpr ‘||’ BoolTerm
BoolTerm = BoolFactor | BoolTerm ‘&&’ BoolFactor
BoolFactor = BoolRelation | ‘(‘ BoolTerm ‘)’
BoolRelation = Expression [RelOp Expression]
Expression = Term | Expression AddOp Term
Term = Factor | Term MultOp Factor
Factor = FirstExpr | Factor PowOp FirstExpr
FirstExpr = ‘(’ Expression ‘)’ | Ident | Const
Ident = Letter {Letter | Digit}
Const = IntConst | RealConst | BoolConst
IntConst = UnsignedInt
RealConst = UnsignedReal
Sign = ‘+’ | ’-’
UnsignedInt = Digit {Digit}
UnsignedReal = UnsignedInt ’.’ UnsignedInt
BoolConst = true | false
SpecSymbols = ArithOp | BoolOp | RelOp | BracketsOp 
   			| AssignOp | Semicolon
ArithOp = AddOp | MultOp | PowOp
AddOp = ’+’ | ’-’
MultOp = ’*’ | ’/’
PowOp = ‘^’
BoolOp = ‘&&’ || ‘||’
RelOp = ’==’ | ’<= ’ | ’<’ | ’>’ | ’>=’ | ’!=’
BracketsOp = ’(’ | ’)’
BracesOp = ‘{’ | ‘}’
AssignOp = ’=’
Semicolon = ’;’
KeyWords = 
| int | float| bool 
| read | print 
| for | while
| if | else
Letter = ’a’ | ’b’ | ’c’ | ’d’ | ’e’ | ’f’ | ’g’ | ’h’ | ’i’ | ’j’
| ’k’ | ’l’ | ’m’ | ’n’ | ’o’ | ’p’ | ’q’ | ’r’ | ’s’ | ’t’ 
| ’u’ | ’v’ | ’w’ | ’x’ | ’y’ | ’z’
Digit = ’0’ | ’1’ | ’2’ | ’3’ | ’4’ | ’5’ | ’6’ | ’7’ | ’8’ | ’9’ 
SpecSsign = ’.’ | ’,’ | ’:’ | ’;’ | ’(’ | ’)’
| ’=’ | ’+’ | ’-’ | ’*’ | ’/’ | ’<’ | ’>’ | ‘!’ | ‘^’ 
| WhiteSpace | EndOfLine
WhiteSpace = ’ ’ | ’\t’
EndOfLine = ’\n’ | ’\r’ | ’\r\n’ | ’\n\r’
EndOfFile = \0
```
