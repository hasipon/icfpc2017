package game;
import game.River;
import game.command.MapStruct;


class Game 
{
    public var sites:Map<SiteId, Site>;
    public var mines:Map<SiteId, Site>;
    public var rivers:Map<RiverId, River>;
    public var punters:Map<PunterId, Punter>;
    public var scoreDictionary:Map<SiteId, Map<SiteId, Int>>;
    public var mineCount:Int;
    public var siteCount:Int;
    public var riverCount:Int;
    
    public function new() 
    {
        sites = new Map();
        mines = new Map();
        rivers = new Map();
        punters = new Map();
        scoreDictionary = new Map();
        
        mineCount  = 0;
        siteCount  = 0;
        riverCount = 0;
    }
    
    public function setup(map:MapStruct) 
    {
        for (data in map.sites)
        {
            var site = new Site(
                this,
                data.id,
                if (data.x == null) Math.random() else data.x,
                if (data.y == null) Math.random() else data.y
            );
            
            sites[site.id] = site;
        }
        
        for (siteId in map.mines)
        {
            var site = sites[siteId];
            site.isMine = true;
            mines[siteId] = site;
        }
        
        var i = 0;
        for (data in map.rivers)
        {
            var id = new RiverId(i);
            var river = new River(this, id, data.source, data.target);
            rivers[id] = river;
            i++;
        }
        
        mineCount = map.mines.length;
        siteCount = map.sites.length;
        riverCount = map.rivers.length;
    }
}
