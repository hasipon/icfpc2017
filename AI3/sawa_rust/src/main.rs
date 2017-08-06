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
use std::collections::HashSet;
use std::collections::HashMap;

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
            let state:GameState = serde_json::from_value(object.remove("state").unwrap()).unwrap();
            let mov = think(message, state);
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
    punters: i32,
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
struct MovesMessage {
    moves: Vec<Value>,
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
#[derive(Serialize, Deserialize, Copy, Clone, Hash, Eq, PartialEq)]
struct PunterId(i32);
#[derive(Serialize, Deserialize, Copy, Clone, Hash, Eq, PartialEq)]
struct SiteId(i32);
#[derive(Serialize, Deserialize, Copy, Clone, Hash, Eq, PartialEq)]
struct RiverId(i32);
#[derive(Serialize, Deserialize, Copy, Clone, Hash, Eq, PartialEq)]
struct GroupId(i32);

enum Move {
    Claim(PunterId, SiteId, SiteId),
    Pass(PunterId),
    Splurge(PunterId, Vec<SiteId>),
}

#[derive(Serialize, Deserialize, Clone)]
struct GameState {
    punter_id: PunterId,
    punters: Vec<Punter>,
    sites: HashMap<SiteId, Site>,
    rivers: HashMap<RiverId, River>,
    mines: Vec<SiteId>,
    scores: HashMap<SiteId, HashMap<SiteId, i32>>,
}

#[derive(Serialize, Deserialize, Clone)]
struct Site {
    id: SiteId,
    is_mine: bool,
    rivers: Vec<RiverId>,
    group_ids: HashMap<PunterId, GroupId>,
}

#[derive(Serialize, Deserialize, Clone)]
struct River {
    owner: Option<PunterId>,
    id: RiverId,
    a:SiteId, 
    b:SiteId,
    distance_from_mine: HashMap<SiteId, i32>,
}

#[derive(Serialize, Deserialize, Clone)]
struct Punter {
    id: PunterId,
    groups: HashMap<GroupId, Group>,
}

#[derive(Serialize, Deserialize, Clone)]
struct Group {
    id: GroupId,
    has_mine: bool,
    sites: HashSet<SiteId>,
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

impl River {
    fn get_another(&self, site:SiteId) -> SiteId {
        return if self.a == site {
            self.b
        } else {
            assert!(self.b == site, "riverに含まれない");
            self.a
        }
    }
}

impl GameState {
    fn do_move(&mut self, mov:&Move) {
        //
        
    }
}

// AI
fn setup(message:SetupMessage)->GameState
{
    let mut punters = Vec::new();
    for i in 0..message.punters
    {
        punters.push(
            Punter {
                id: PunterId(i),
                groups: HashMap::new(),
            }
        );
    };

    let mut sites = HashMap::new();
    for site in &message.map.sites {
        let id = site.id;
        let mut group_ids = HashMap::new();
        let SiteId(id_num) = id;
        let group_id = GroupId(id_num);

        for punter in &mut punters 
        {
            let mut sites = HashSet::new();
            sites.insert(id);
            punter.groups.insert(
                group_id, 
                Group{
                    id: group_id,
                    has_mine: false,
                    sites: sites, 
                }
            );
            group_ids.insert(punter.id, group_id);
        }

        sites.insert(
            id, 
            Site{
                id: id,
                is_mine: false,
                rivers: Vec::new(),
                group_ids: group_ids,
            }
        );
    }

    let mut rivers = HashMap::new();
    let mut index = 0;
    for river in &message.map.rivers {
        let id = RiverId(index);
        rivers.insert(
            RiverId(index),
            River {
                owner: Option::None,
                id: id,
                a: river.target,
                b: river.source,
                distance_from_mine: HashMap::new(),
            }
        );

        sites.get_mut(&river.target).unwrap().rivers.push(id);
        sites.get_mut(&river.source).unwrap().rivers.push(id);
        
        index += 1;
    }

    for mine in &message.map.mines {
        let site = sites.get_mut(&mine).unwrap();
        site.is_mine = true;
        for punter in &mut punters 
        {
            let group_id = site.group_ids.get(&punter.id).unwrap();
            let group = punter.groups.get_mut(&group_id).unwrap();
            group.has_mine = true;
        }
    }

    let mut scores:HashMap<SiteId, HashMap<SiteId, i32>> = HashMap::new();
    for mine in &message.map.mines {
        let mut current_sites:Vec<SiteId> = Vec::new();
        let mut local_scores = HashMap::new();
        
        local_scores.insert(mine.clone(), 0);
        current_sites.push(mine.clone());
        
        for i in 0..sites.len() {
            let mut next_sites:Vec<SiteId> = Vec::new();
            for site_id in &current_sites {
                let site = sites.get(&site_id).unwrap();
                for river_id in &site.rivers {
                    let river = rivers.get(&river_id).unwrap();
                    let another_id = river.get_another(site.id);
                    if !local_scores.contains_key(&another_id) {
                        let d = i as i32;
                        local_scores.insert(another_id, d * d);
                        next_sites.push(another_id);
                    }
                }
            }
            current_sites = next_sites;
            if current_sites.len() <= 0 {
                break;
            }
        }

        scores.insert(mine.clone(), local_scores);
    }

    return GameState{
        punter_id: message.punter,
        punters: punters,
        sites: sites,
        rivers: rivers,
        mines: message.map.mines,
        scores: scores,
    }
}

fn think(message:MovesMessage, state:GameState)->Value
{
    let mut distances:HashMap<PunterId, HashMap<GroupId, HashMap<GroupId, i32>>> = HashMap::new();
    for punter in &state.punters {
        let mut groups = &punter.groups;
        let mut punter_distances = HashMap::new();

        for mine in &state.mines {
            let mut current_groups:Vec<GroupId> = Vec::new();
            let mut mine_distances = HashMap::new();
            let mine_group_id = state.sites.get(&mine).unwrap().group_ids.get(&punter.id).unwrap();

            mine_distances.insert(mine_group_id, 0);
            current_groups.push(mine_group_id.clone());
            
            for i in 0..state.sites.len() {
                let mut next_groups:Vec<GroupId> = Vec::new();

                for group_id in &current_groups {
                    let group = groups.get(&group_id).unwrap();
                    for site_id in &group.sites {
                        let site = state.sites.get(&site_id).unwrap();

                        for river_id in &site.rivers {
                            let river = state.rivers.get(&river_id).unwrap();
                            let another_id = river.get_another(site.id);
                            let another = state.sites.get(&another_id).unwrap();                            
                            let another_group_id = another.group_ids.get(&punter.id).unwrap();

                            if !mine_distances.contains_key(another_group_id) {
                                let d = i as i32;
                                mine_distances.insert(another_group_id, d);
                                next_groups.push(another_group_id.clone());
                            }
                        }
                    }
                }
                current_groups = current_groups;
                if current_groups.len() <= 0 {
                    break;
                }
            }

            punter_distances.insert(mine.clone(), mine_distances);
        }
    }

    return Move::Pass(state.punter_id).to_message(state);
}

fn think_random() {

}
