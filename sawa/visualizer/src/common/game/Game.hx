package game;
import game.PunterId;
import game.River;
import game.command.ClaimStruct;
import game.command.MapStruct;
import game.command.MoveStruct;
import game.command.SetupStruct;


class Game 
{
    public static var MAX_DISTANCE = 20;
    public static var MS_TABLE = [for (i in 0...MAX_DISTANCE) 2048 * Math.pow(0.5, i)];
    
    public var sites:Map<SiteId, Site>;
    public var mines:Map<SiteId, Site>;
    public var rivers:Map<RiverId, River>;
    public var punters:Map<PunterId, Punter>;
    public var scoreDictionary:Map<SiteId, Map<SiteId, Int>>;
    public var mineCount:Int;
    public var siteCount:Int;
    public var riverCount:Int;
    public var maxScore:Int;
    public var moves:Array<MoveStruct>;
    
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
        maxScore   = 0;
        
        moves = [];
    }
    
    public function setup(setupStruct:SetupStruct):Void
    {
        setupMap(setupStruct.map);
        setupPanters(setupStruct.punters);
    }
    
    private function setupPanters(punterIds:Int) 
    {
        for (i in 0...punterIds)
        {
            var id = new PunterId(i);
            punters[id] = new Punter(this, id);
        }
    }
    
    public function setupMap(map:MapStruct) 
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
            
            sites[river.a].rivers[river.getAnother(river.a)] = river;
            sites[river.b].rivers[river.getAnother(river.b)] = river;
            i++;
        }
        
        mineCount = map.mines.length;
        siteCount = map.sites.length;
        riverCount = map.rivers.length;
        maxScore = 0;
        
        // 
        for (mine in mines)
        {
            var localScores = new Map();
            scoreDictionary[mine.id] = localScores;
            
            var currentSites = [mine];
            var distances = new Map<SiteId, Int>();
            distances[mine.id] = 0;
            
            for (i in 0...siteCount)
            {
                var nextSites = [];
                for (site in currentSites)
                {
                    for (river in site.rivers)
                    {
                        var another = river.getAnother(site.id); 
                        if (!distances.exists(another))
                        {
                            distances[another] = i + 1;
                            nextSites.push(sites[another]);
                        }
                    }
                }
                currentSites = nextSites;
            }
            
            for (siteId in distances.keys())
            {
                var distance = distances[siteId];
                var score = distance * distance;
                localScores[siteId] = score;
                maxScore += score;
            }
        }
    }
    
    public function addMove(move:MoveStruct):Void
    {
        if (move.pass != null)
        {
            pass(move.pass.punter);
        }
        else
        {
            claim(move.claim);
        }
        
        moves.push(move);
    }
    
    public function claim(move:ClaimStruct):Void
    {
        var river = sites[move.source].rivers[move.target];
        river.owner = move.punter;
        punters[move.punter].score = -1;
        
    }
    
    public function pass(punter:PunterId):Void
    {
        // 終了
    }
    
    public function applyScore(punter:PunterId, score:Int):Void
    {
        punters[punter].score = score;
    }
}
