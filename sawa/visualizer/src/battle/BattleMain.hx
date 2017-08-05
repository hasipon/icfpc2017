package;
import eval.Evaluter;
import game.Game;
import game.PunterId;
import haxe.Json;
import search.BeamSearcher;
import sys.FileSystem;
import sys.io.File;

class BattleMain {
    public static var PUNTERS = 4;
    public static var MAPS = "../../maps";
    
    public static function main():Void
    {
        var maps = [for (map in FileSystem.readDirectory(MAPS)) if (StringTools.endsWith(map, ".json")) MAPS + "/" + map];
        for (i in 0...200)
        {
            new BattleMain(maps[Std.int(maps.length * Math.random())]);
            trace("GAME終了:" + i);
        }
        
        Sys.getChar(false);
    }
    
    public function new(mapPath:String)
    {
        trace(mapPath);
        var game = new Game();
        var map = Json.parse(File.getContent(mapPath));
        var evaluter = new Evaluter(Evaluter.MS_TABLE);
        var sercher = new BeamSearcher(0.1, evaluter);
        game.setupMap(map);
        game.setupPunters(PUNTERS);
        
        for (i in 0...game.riverCount)
        {
            var punter = new PunterId(i % PUNTERS);
            var move = sercher.getMove(game, punter);
            game.addMove(move);
            trace("move" + i);
        }
    }
}
