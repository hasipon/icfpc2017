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

macro_rules! map(
    { $($key:expr => $value:expr),+ } => {
        {
            let mut m = ::serde_json::Map::new();
            $(
                m.insert($key.to_owned(), serde_json::to_value($value).unwrap());
            )+
            m
        }
     };
);

fn main() 
{
    let mut stdin = io::stdin();
    let mut stdout = io::stdout();

    write_json_message(&mut stdout, serde_json::to_string(&Me{me: "sawa_rust".to_owned()}).unwrap());
    read_json_message(&mut stdin);
    let value = read_json_message(&mut stdin);
    
    if let Value::Object(mut object) = value
    {
        if object.contains_key("punters") {
            let value = Value::Object(object);
            let message:SetupMessage = serde_json::from_value(value).unwrap();
            let punter = message.punter;
            let state = setup(message);
            write_json_message(&mut stdout, serde_json::to_string(&ReadyMessage{ready:punter, state:state}).unwrap());
        } else if let Option::Some(value) = object.remove("move") {
            let message:MovesMessage = serde_json::from_value(value).unwrap();
            let mov = think(message);
            write_json_message(&mut stdout, serde_json::to_string(&mov).unwrap());
        }
    }
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

// MESSAGE
#[derive(Serialize, Deserialize)]
struct Me { me : String }
#[derive(Serialize, Deserialize)]
struct You { you : String }
#[derive(Serialize, Deserialize)]
pub struct SetupMessage {
    punter: PunterId,
    punters: i64,
    map: MapMessage,
}
#[derive(Serialize, Deserialize)]
struct MapMessage {
    sites: Vec<SiteMessage>,
    rivers: Vec<RiverMessage>,
    mines: Vec<SiteId>,
}
#[derive(Serialize, Deserialize)]
struct SiteMessage {
    id: SiteId,
}
#[derive(Serialize, Deserialize)]
struct RiverMessage {
    source: SiteId,
    target: SiteId,
}
#[derive(Serialize, Deserialize)]
struct ReadyMessage {
    ready: PunterId,
    state: GameState,
}
#[derive(Serialize, Deserialize)]
struct GameState {
    punter: PunterId,
}
#[derive(Serialize, Deserialize)]
struct MovesMessage {
    moves: Vec<Value>,
    state: GameState,
}

#[derive(Serialize, Deserialize)]
struct PassMessage {
    punter: PunterId,
}
#[derive(Serialize, Deserialize)]
struct ClaimMessage {
    punter: PunterId,
    source: SiteId, 
    target: SiteId,
}
#[derive(Serialize, Deserialize)]
struct SplugeMessage {
    punter: PunterId,
    route: Vec<SiteId>, 
}
#[derive(Serialize, Deserialize, Copy, Clone)]
struct PunterId(i32);
#[derive(Serialize, Deserialize, Copy, Clone)]
struct SiteId(i32);
#[derive(Serialize, Deserialize, Copy, Clone)]
struct EdgeId(i32);

enum Move {
    Claim(PunterId, SiteId, SiteId),
    Pass(PunterId),
    Splurge(PunterId, Vec<SiteId>),
}

impl Move {
    fn from_value(mut value:Value) -> Move
    {
        let mut object = value.as_object_mut().unwrap();
        if let Option::Some(mut claim) = object.remove("claim") {
            let mut obj = claim.as_object_mut().unwrap();
            Move::Claim(
                serde_json::from_value(obj.remove("punter").unwrap()).unwrap(), 
                serde_json::from_value(obj.remove("source").unwrap()).unwrap(), 
                serde_json::from_value(obj.remove("target").unwrap()).unwrap(),
            )
        } else if let Option::Some(mut pass) = object.remove("pass") {
            let mut obj = pass.as_object_mut().unwrap();
            Move::Pass(
                serde_json::from_value(obj.remove("punter").unwrap()).unwrap(),
            )
        } else if let Option::Some(mut splurge) = object.remove("splurge") {
            let mut obj = splurge.as_object_mut().unwrap();
            Move::Splurge(
                serde_json::from_value(obj.remove("punter").unwrap()).unwrap(), 
                serde_json::from_value(obj.remove("route").unwrap()).unwrap(),
            )
        } else {
            panic!("unsuppoted move value");
        }
    }

    fn to_message(self, state:GameState) -> Value {
        return match self {
            Move::Claim(id, a, b) => Value::Object(
                map! {
                    "claim" => ClaimMessage {
                        punter: id,
                        source: a,
                        target: b,
                    },
                    "state" => state
                }
            ),
            Move::Pass(id) => Value::Object(
                map!{
                    "pass" => PassMessage {
                        punter: id,
                    },
                    "state" => state
                }
            ),
            Move::Splurge(id, route) => Value::Object(
                map!{
                    "pass" => SplugeMessage {
                        punter: id,
                        route: route,
                    },
                    "state" => state
                }
            ),
        }
    }
}

// AI
fn setup(message:SetupMessage)->GameState
{
    return GameState{
        punter: message.punter,
    }
}
fn think(message:MovesMessage)->Value
{
    return Move::Pass(message.state.punter).to_message(message.state);
}
