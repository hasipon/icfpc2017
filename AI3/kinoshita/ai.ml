exception Server_error of string

let server_fail msg = raise (Server_error msg)

let json_of_string str =
  Parser.chunk Lexer.read (Lexing.from_string str)

let read_message ?(cin=stdin) () : Json.t =
  let rec read_length acc =
    let c = input_char cin in
    if c = ':'
    then acc
    else
      let ci = int_of_char c - 48 in
      if 0 <= ci && ci <= 9
      then read_length (acc * 10 + ci)
      else read_length acc
  in
  let len = read_length 0 in
  let buf = Bytes.create len in
  ignore (input cin buf 0 len);
  json_of_string buf

let send_message ?(cout=stdout) (json : Json.t) =
  let json_str = Json.to_string json in
  let len = String.length json_str in
  let message = Printf.sprintf "%d:%s" len json_str in
  output_bytes cout message; flush cout

let handshake ?(cin=stdin) ?(cout=stdout) name =
  let message = Json.(Hsh [ ("me", Str name) ]) in
  send_message ~cout message;
  read_message ~cin ()
  
let dispatch setup_handler gameplay_handler message =
  let open Json in
  if has_field "map" message
  then
    setup_handler
      (get_field "punter" message |> get_int)
      (get_field "punters" message |> get_int)
      (get_field "map" message)
    |> send_message
  else if has_field "move" message
  then
    gameplay_handler
      (get_field "move" message |> get_field "moves")
      (get_field "state" message |> get_str |> json_of_string)
    |> send_message
  else if has_field "stop" message
  then ()
  else
    server_fail ("dispatch: invalid: " ^ Json.(to_string message))

(* module State = struct *)
(*   type t = *)
(*     { punter_id : int *)
(*     ; map : Json.t *)
(*     ; occupied : (int * int) list *)
(*     } *)

(*   let to_json state = *)
(*     let open Json in *)

(*     Hsh [ ("punter_id", Int state.punter_id) *)
(*         ; ("map", state.map) *)
(*         ; ("occupied", List.map (fun (x,y) -> state.occupied) *)
    
(* end *)

(* SETUP *)
let setup_handler punter punters map =
  let open Json in

  let state =
    Hsh [ ("punter_id", Int punter)
        ; ("map", map)
        ; ("occupied", Arr [])
        ]
    |> to_string
    |> String.escaped
  in

  Hsh [ ("ready", Int punter); ("state", Str state) ]

(* GAMEPLAY *)
let gameplay_hander moves state =
  let open Json in

  let punter_id = get_field "punter_id" state
  and map = get_field "map" state
  in
  let rivers = get_field "rivers" map |> get_arr in
  let claim = List.nth rivers (Random.int (List.length rivers)) in

  Hsh [ "claim", Hsh [ "punter", punter_id
                     ; "source", (get_field "source" claim)
                     ; "target", (get_field "target" claim)
                     ]
      ; "state", Str (state |> to_string |> String.escaped)
      ]

  (* let state = state |> json_of_string in *)
  (* let punter_id = get_field "punter_id" state *)
  (* and map = get_field "map" state *)
  (* and occupied = get_field "occupied" state *)
  (* in *)

  (* let occupied = *)
  (*   List.fold_left *)
  (*     (fun acc move -> *)
  (*       if has_field "claim" move *)
  (*       then *)
  (*         let claim = get_field "claim" move in *)
  (*         let source = get_field "source" claim |> get_int *)
  (*         and target = get_field "target" claim |> get_int *)
  (*         in *)
  (*         [source; target] :: acc *)
  (*       else *)
  (*         acc) *)
  (*     occupied *)
  (*     moves *)
  (* in *)

  (* Hsh [ "pass", Hsh [ "punter", Int punter_id ] *)
  (*     ; "state", (\* Str (string_of_int punter_id) *\) *)
(*     ] *)

let main ~name =
  handshake name
  |> ignore;

  read_message ()
  |> dispatch setup_handler gameplay_hander

let () = main ~name:"kinoshita"
