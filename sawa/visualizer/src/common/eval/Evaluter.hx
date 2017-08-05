package eval;
import game.Game;
import game.Punter;
import game.PunterId;
import game.Site;
import game.SiteId;

class Evaluter 
{
    public var table:Array<Float>;
    public static var MAX_DISTANCE = 1;
    public static var MS_TABLE = [for (i in 0...MAX_DISTANCE) Math.pow(0.2, i)];

    public function new(table:Array<Float>)
    {
        this.table = table;
    }
    
    public function eval(game:Game):EvalutionResult
    {
        var values = [
            for (punter in game.punters)
            {
                evalFor(game, punter.id);
            }
        ];
        
        return new EvalutionResult(values);
    }
    
    public function evalFor(game:Game, punterId:PunterId):Float
    {   
        var value = 0.0;
        for (mine in game.mines)
        {
            var currentSites = [mine];
            
            var distances = new Map<SiteId, Int>();
            distances[mine.id] = 0;
            
            for (i in 0...MAX_DISTANCE)
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
                value += table[distance] * game.scoreDictionary[mine.id][siteId];
            }
        }
        
        return value;
    }
    
    public function searchRivers(game:Game, site:Site, punterId:PunterId, i:Int, distances:Map<SiteId, Int>, nextSites:Array<Site>):Void
    {
        for (river in site.rivers)
        {
            while (true)
            {
                var another = river.getAnother(site.id); 
                if (!distances.exists(another))
                {
                    distances[another] = i;
                    nextSites.push(game.sites[another]);
                    
                    if (river.owner == punterId)
                    {
                        site = game.sites[another];
                        continue;
                    }
                }
                
                break;
            }
        }
    }
}
