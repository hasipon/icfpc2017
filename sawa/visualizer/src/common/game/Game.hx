package game;
import game.Game.SiteId;

class Game 
{
    public var sites:Map<SiteId, Site>;
    public var mines:Map<MineId, Site>;
    public var rivers:Map<RiverId, River>;
    public var punters:Map<PunterId, Punter>;
    public var scoreDictionary:Map<SiteId, Map<SiteId, Int>>;
    
    public function new() 
    {
        sites = new Map();
        mines = new Map();
        rivers = new Map();
        punters = new Map();
        scoreDictionary = new Map();
    }
}
