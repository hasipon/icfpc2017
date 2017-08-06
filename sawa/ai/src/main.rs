use std::io;
use std::io::Read;
use std::ops::Add;
extern crate serde_json;
use serde_json::{Value, Error};

fn main() 
{
    let stdout = io::stdout();
    println!("{:?}", readJsonMessage());
}

fn read_json_message() -> Value
{
    let mut number_text = String::new();
 
    let stdin =  io::stdin();   
    for current in stdin.bytes() {
        let text = current.unwrap().to_string();
        if text == ":" {
            break;
        } else {
            number_text += &text;
        }
    }

    let number:u64 = number_text.parse().unwrap();
    let mut message = String::new();
    io::stdin().take(number).read_to_string(&mut message);
    return serde_json::from_str(&message).unwrap();
}
