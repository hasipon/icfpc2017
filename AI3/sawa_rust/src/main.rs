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
            let mut state:GameState = serde_json::from_value(object.remove("state").unwrap()).unwrap();
            for mov in message.moves {
                state.do_move(&Move::from_value(mov));
            }
            let mov = think(state.clone()).to_message(state);
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
    Claim(ClaimMessage),
    Pass(PassMessage),
    Splurge(SplugeMessage),
    Option(ClaimMessage),
}

#[derive(Serialize, Deserialize, Clone)]
struct GameState {
    punter_id: PunterId,
    punters: HashMap<PunterId, Punter>,
    sites: HashMap<SiteId, Site>,
    rivers: HashMap<RiverId, River>,
    mines: Vec<SiteId>,
    scores: HashMap<SiteId, HashMap<SiteId, i32>>,
}

#[derive(Serialize, Deserialize, Clone)]
struct Site {
    id: SiteId,
    is_mine: bool,
    rivers: HashMap<SiteId, RiverId>,
    group_ids: HashMap<PunterId, GroupId>,
}

#[derive(Serialize, Deserialize, Clone)]
struct River {
    owner: Option<PunterId>,
    option_owner: Option<PunterId>,
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

enum MoveMode {
    Claim,
    Option,
    Auto,
}

impl Group {
    fn merge(mut self, smaller:Group, punter:&Punter, sites:&mut HashMap<SiteId, Site>)->Group {
        if self.sites.len() < smaller.sites.len() {
            return smaller.merge(self, punter, sites);
        }

        self.has_mine = self.has_mine || smaller.has_mine;
        for site_id in smaller.sites {
            let mut site = sites.get_mut(&site_id).unwrap();
            self.sites.insert(site_id);
            site.group_ids.insert(punter.id, self.id);
        }

        return self;
    }
}
impl Move {
    fn from_value(mut value:Value) -> Move
    {
        let mut object = value.as_object_mut().unwrap();
        if let Option::Some(mut claim) = object.remove("claim") {
            Move::Claim(serde_json::from_value(claim).unwrap())
        } else if let Option::Some(mut pass) = object.remove("pass") {
            Move::Pass(serde_json::from_value(pass).unwrap())
        } else if let Option::Some(mut splurge) = object.remove("splurge") {
            Move::Splurge(serde_json::from_value(splurge).unwrap())
        } else if let Option::Some(mut option) = object.remove("option") {
            Move::Splurge(serde_json::from_value(option).unwrap())
        } else {
            panic!("unsuppoted move value");
        }
    }

    fn to_message(self, state:GameState) -> Value {
        return match self {
            Move::Claim(data) => Value::Object(
                map! {
                    "claim" => data,
                    "state" => state
                }
            ),
            Move::Pass(data) => Value::Object(
                map!{
                    "pass" => data,
                    "state" => state
                }
            ),
            Move::Splurge(data) => Value::Object(
                map!{
                    "spluge" => data,
                    "state" => state
                }
            ),
            Move::Option(data) => Value::Object(
                map!{
                    "pass" => data,
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
        match mov {
            &Move::Pass(_) => (),
            &Move::Claim(ref data) => 
                self.own_river(data.punter, data.target, data.source),

            &Move::Option(ref data) => 
                self.own_river(data.punter, data.target, data.source),

            &Move::Splurge(ref data) => 
                for i in 1..data.route.len() {
                    let previous = data.route[i - 1];
                    let current = data.route[i];
                    self.own_river(data.punter, current, previous);
                }
        }
    }

    fn own_river(&mut self, punter_id:PunterId, a:SiteId, b:SiteId) {
        let (group_a_id, group_b_id) = {
            let site_a = self.sites.get(&a).unwrap();
            let site_b = self.sites.get(&b).unwrap();
            let river_id = site_a.rivers.get(&b).unwrap();
            let river = self.rivers.get_mut(&river_id).unwrap();
            if river.owner.is_none() {
                river.owner = Option::Some(punter_id)
            } else {
                river.option_owner = Option::Some(punter_id)
            }

            (
                site_a.group_ids.get(&punter_id).unwrap().clone(),
                site_b.group_ids.get(&punter_id).unwrap().clone()
            )
        };
        
        if group_a_id != group_b_id {
            let mut punter = self.punters.get_mut(&punter_id).unwrap();
            let mut group_a = punter.groups.remove(&group_a_id).unwrap();
            let mut group_b = punter.groups.remove(&group_b_id).unwrap();
            let new_group = group_a.merge(group_b, &punter, &mut self.sites);
            punter.groups.insert(new_group.id, new_group);
        }
    }
}

// AI
fn setup(message:SetupMessage)->GameState
{
    let mut punters = HashMap::new();
    for i in 0..message.punters
    {
        let id = PunterId(i);
        punters.insert(
            id,
            Punter {
                id: id,
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

        for (_, punter) in &mut punters 
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
                rivers: HashMap::new(),
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
                option_owner: Option::None,
                id: id,
                a: river.target,
                b: river.source,
                distance_from_mine: HashMap::new(),
            }
        );

        sites.get_mut(&river.target).unwrap().rivers.insert(river.source, id);
        sites.get_mut(&river.source).unwrap().rivers.insert(river.target, id);
        
        index += 1;
    }

    for mine in &message.map.mines {
        let site = sites.get_mut(&mine).unwrap();
        site.is_mine = true;
        for (_, punter) in &mut punters 
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
                for (another_id, _) in &site.rivers {
                    if !local_scores.contains_key(&another_id) {
                        let d = i as i32;
                        local_scores.insert(another_id.clone(), d * d);
                        next_sites.push(another_id.clone());
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

fn think(state:GameState)->Move
{
    let mut distances:HashMap<PunterId, HashMap<GroupId, HashMap<GroupId, i32>>> = HashMap::new();
    for (_, punter) in &state.punters {
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

                        for (another_id, _) in &site.rivers {
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

    return Move::Pass(PassMessage{punter:state.punter_id});
}

fn think_random() {

}
