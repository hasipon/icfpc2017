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
    moves: Vec<MoveMessage>,
    state: GameState,
}
#[derive(Serialize, Deserialize)]
struct MoveMessage {
    #[serde(skip_serializing)]
    claim : Option<ClaimMessage>,

    #[serde(skip_serializing)]
    pass : Option<PassMessage>, 
    
    #[serde(skip_serializing)]
    splurge : Option<SplugeMessage>,

    #[serde(skip_serializing)]
    state : Option<GameState>,
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
    Claim(SiteId, SiteId),
    Pass,
}

impl Move {
    fn to_message(self, id:PunterId, state:GameState) -> MoveMessage {
        match self {
            Move::Claim(a, b) => MoveMessage{
                claim : Option::Some(ClaimMessage {
                    punter: id,
                    source: a,
                    target: b,
                }),
                pass : Option::None, 
                splurge : Option::None,
                state : Option::Some(state),
            },
            Move::Pass => MoveMessage{
                claim : Option::None, 
                pass : Option::Some(PassMessage {
                    punter: id,
                }), 
                splurge : Option::None,
                state : Option::Some(state),
            },
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
fn think(message:MovesMessage)->MoveMessage
{
    return Move::Pass.to_message(message.state.punter, message.state);
}
