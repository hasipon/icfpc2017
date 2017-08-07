%{
  open Json
%}

%token COLON
%token LBRACE
%token RBRACE
%token LBRACKET
%token RBRACKET
%token COMMA
%token <int> INTEGER
%token <bool> BOOL
%token <string> LITERALSTRING
%start chunk
%type <Json.t> chunk
%%

chunk:
  | LBRACE RBRACE { Hsh [] }
  | LBRACE hashs RBRACE { Hsh $2 }
  | LBRACKET RBRACKET { Arr [] }
  | LBRACKET chunks RBRACKET { Arr $2 }
  | LITERALSTRING { Str $1 }
  | INTEGER { Int $1 }
  | BOOL { Boo $1 }
  ;

hash:
  | LITERALSTRING COLON chunk { $1, $3 }
  ;

hashs:
  | hash COMMA hashs { $1 :: $3 }
  | hash { [$1] }
  ;

chunks:
  | chunk COMMA chunks { $1 :: $3 }
  | chunk { [$1] }
  ;
