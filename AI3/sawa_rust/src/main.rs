extern crate serde;
extern crate serde_json;

#[macro_use]
extern crate serde_derive;

use std::io;
use std::io::Read;
use std::io::Write;
use std::ops::Add;

use serde::ser::Serialize;
use serde::de::Deserialize;
use serde_json::{Value, Error};

fn main() 
{
    let mut stdin = io::stdin();
    let mut stdout = io::stdout();

    write_json_message(&mut stdout, serde_json::to_string(&mut Me{me: "shohei909".to_owned()}).unwrap());
    read_json_message(&mut stdin);
}

fn read_json_message(input: &mut Read) -> Value
{
    let mut number_text = String::new();
    
    loop {
        let mut buffer = [0u8];
        input.read(&mut buffer);

        let text = buffer[0] as char;
        if text == ':' {
            break;
        } else {
            number_text.push(text);
        }
    }

    let number:u64 = number_text.parse().unwrap();
    let mut message = String::new();
    input.take(number).read_to_string(&mut message);

    return serde_json::from_str(&message).unwrap();
}

fn write_json_message(output: &mut Write, string:String) {
    write!(
        output, 
        "{}:{}", 
        string.as_bytes().len(), 
        string
    );

    output.flush();
}

struct PunterId(i32);
struct SiteId(i32);
struct EdgeId(i32);

struct Edge {}

// MESSAGE
#[derive(Serialize, Deserialize)]
struct Me { me : String }
#[derive(Serialize, Deserialize)]
struct You { you : String }
