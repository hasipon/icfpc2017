package eval;
import game.Game;
import game.Punter;
import game.PunterId;
import game.Site;
import game.SiteId;

class Evaluter 
{
    public var table:Array<Float>;
    public static var MAX_DISTANCE = 5;
    public static var MS_TABLE = [for (i in 0...MAX_DISTANCE + 1) Math.pow(0.1, i)];

    public function new(table:Array<Float>)
    {
        this.table = table;
    }
    
    public function eval(game:Game):EvalutionResult
    {
        var values = [
            for (i in 0...game.punterCount)
            {
                evalFor(game, new PunterId(i));
            }
        ];
        return new EvalutionResult(values);
    }
    
    public function evalFor(game:Game, punterId:PunterId):Float
    {   
        var value = 0.0;
        for (mine in game.mines)
        {
            var currentSites = [];
            var distances = new Map<SiteId, Int>();
            
            searchRivers(game, mine, punterId, 0, distances, currentSites);
            
            for (i in 1...MAX_DISTANCE+1)
            {
                var nextSites = [];
                for (site in currentSites)
                {
                    searchRivers(game, site, punterId, i, distances, nextSites);
                }
                
                currentSites = nextSites;
            }
            
            for (siteId in distances.keys())
            {
                var distance = distances[siteId];
                var score = game.scoreDictionary[mine.id][siteId];
                var aditional = table[distance] * score;
                value += aditional;
            }
        }
        
        return value;
    }
    
    public function searchRivers(game:Game, site:Site, punterId:PunterId, i:Int, distances:Map<SiteId, Int>, nextSites:Array<Site>):Void
    {
        for (river in site.rivers)
        {
            var another = river.getAnother(site.id); 
            if (!distances.exists(another))
            {   
                if (river.owner == punterId)
                {
                    distances[another] = i;
                    searchRivers(game, game.sites[another], punterId, i, distances, nextSites);
                }
                else
                {
                    distances[another] = i + 1;
                    nextSites.push(game.sites[another]);
                }
            }
        }
    }
}
