{
  open Lexing
  open Json
  open Parser
}

let white = [' ' '\t']+
let newline = '\r' | '\n' | '\r' '\n'

rule read = parse
  | white { read lexbuf }
  | newline { read lexbuf }
  | ':' { COLON }
  | '{' { LBRACE }
  | '}' { RBRACE }
  | '[' { LBRACKET }
  | ']' { RBRACKET }
  | ',' { COMMA }
  | ['0'-'9']+
      { Parser.INTEGER (int_of_string (lexeme lexbuf)) }
  | '"'
      { Parser.LITERALSTRING (literal_string lexbuf) }
  | "true"
      { Parser.BOOL true }
  | "false"
      { Parser.BOOL false }

and literal_string = parse
  | '\\' '"' { "\"" ^ literal_string lexbuf }
  | _ as ch
      { if ch = '"'
        then ""
        else Char.escaped ch ^ literal_string lexbuf }
