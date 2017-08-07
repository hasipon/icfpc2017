type t =
  | Arr of t list
  | Hsh of (string * t) list
  | Str of string
  | Int of int
  | Boo of bool

let rec to_string = function
  | Arr a -> "[" ^ (String.concat "," (List.map to_string a)) ^ "]"
  | Hsh h -> "{" ^ (String.concat "," (List.map (fun (k,v) -> Printf.sprintf "\"%s\":%s" k (to_string v)) h)) ^ "}"
  | Str s -> "\"" ^ s ^ "\""
  | Int x -> string_of_int x
  | Boo b -> string_of_bool b

let has_field name = function
  | Hsh h -> List.mem_assoc name h
  | other -> failwith ("has_filed " ^ name ^ ": " ^ to_string other)

let get_field name = function
  | Hsh h -> List.assoc name h
  | other -> failwith ("get_filed " ^ name ^ ": " ^ to_string other)

let get_int = function
  | Int x -> x
  | other -> failwith ("get_int: " ^ to_string other)

let get_arr = function
  | Arr x -> x
  | other -> failwith ("get_arr: " ^ to_string other)

let get_str = function
  | Str x -> x
  | other -> failwith ("get_str: " ^ to_string other)
