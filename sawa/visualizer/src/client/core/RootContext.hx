package core;
import game.Game;
import game.command.MapStruct;
import game.command.MoveStruct;
import game.command.ScoreStruct;
import game.command.SetupStruct;
import game.command.StopStruct;
import haxe.Http;
import haxe.Json;
import haxe.Resource;
import haxe.ds.Option;
import js.Browser;

class RootContext 
{
    private var hash:String;
    public var game:Game;
    public var selectedIndex:Int;
    public var mapNames:Array<String>;
    public var currentGame:Game;
    public var log:String;
    
    public var updateUi:Void->Void;
    public var updatePixi:Void->Void;
    public var warning:String;
    public var playingState:Option<PlayingState>;
    public var speed:Float;
    
    public function new()
    {
        hash = "";
        mapNames = Resource.listNames();
        selectedIndex = 0;
        game = new Game();
        playingState = Option.None;
        log = 
    '{"you":"shiota_ai@1501872522"}
{"punter":2,"punters":3,"map":{"sites":[{"id":18,"x":0.75,"y":0.0},{"id":25,"x":0.9375,"y":-0.10825317547305482},{"id":4,"x":0.75,"y":-0.4330127018922193},{"id":30,"x":0.5,"y":-0.4330127018922193},{"id":14,"x":0.3125,"y":-0.10825317547305482},{"id":15,"x":0.25,"y":-0.21650635094610965},{"id":31,"x":0.625,"y":-0.649519052838329},{"id":12,"x":0.375,"y":0.0},{"id":8,"x":0.125,"y":-0.21650635094610965},{"id":1,"x":1.0,"y":0.0},{"id":23,"x":0.5625,"y":-0.10825317547305482},{"id":3,"x":0.5,"y":0.0},{"id":16,"x":0.3125,"y":-0.3247595264191645},{"id":24,"x":0.875,"y":0.0},{"id":21,"x":0.625,"y":0.0},{"id":36,"x":0.625,"y":-0.4330127018922193},{"id":26,"x":0.8125,"y":-0.10825317547305482},{"id":11,"x":0.0625,"y":-0.10825317547305482},{"id":9,"x":0.125,"y":0.0},{"id":13,"x":0.4375,"y":-0.10825317547305482},{"id":19,"x":0.875,"y":-0.21650635094610965},{"id":32,"x":0.375,"y":-0.649519052838329},{"id":17,"x":0.1875,"y":-0.3247595264191645},{"id":40,"x":0.5625,"y":-0.7577722283113838},{"id":6,"x":0.25,"y":0.0},{"id":27,"x":0.75,"y":-0.21650635094610965},{"id":38,"x":0.5625,"y":-0.5412658773652741},{"id":34,"x":0.4375,"y":-0.5412658773652741},{"id":22,"x":0.6875,"y":-0.10825317547305482},{"id":28,"x":0.8125,"y":-0.3247595264191645},{"id":5,"x":0.25,"y":-0.4330127018922193},{"id":33,"x":0.375,"y":-0.4330127018922193},{"id":37,"x":0.6875,"y":-0.5412658773652741},{"id":41,"x":0.4375,"y":-0.7577722283113838},{"id":10,"x":0.1875,"y":-0.10825317547305482},{"id":0,"x":0.0,"y":0.0},{"id":39,"x":0.5,"y":-0.649519052838329},{"id":7,"x":0.375,"y":-0.21650635094610965},{"id":35,"x":0.3125,"y":-0.5412658773652741},{"id":29,"x":0.6875,"y":-0.3247595264191645},{"id":2,"x":0.5,"y":-0.8660254037844386},{"id":20,"x":0.625,"y":-0.21650635094610965}],"rivers":[{"source":24,"target":25},{"source":19,"target":25},{"source":1,"target":25},{"source":13,"target":14},{"source":12,"target":14},{"source":7,"target":14},{"source":6,"target":14},{"source":8,"target":15},{"source":7,"target":15},{"source":6,"target":12},{"source":3,"target":12},{"source":22,"target":23},{"source":21,"target":23},{"source":20,"target":23},{"source":3,"target":23},{"source":15,"target":16},{"source":7,"target":16},{"source":5,"target":16},{"source":18,"target":24},{"source":1,"target":24},{"source":18,"target":21},{"source":3,"target":21},{"source":30,"target":36},{"source":4,"target":36},{"source":25,"target":26},{"source":24,"target":26},{"source":19,"target":26},{"source":18,"target":26},{"source":10,"target":11},{"source":9,"target":11},{"source":8,"target":11},{"source":0,"target":11},{"source":6,"target":9},{"source":0,"target":9},{"source":12,"target":13},{"source":7,"target":13},{"source":3,"target":13},{"source":16,"target":17},{"source":15,"target":17},{"source":8,"target":17},{"source":5,"target":17},{"source":39,"target":40},{"source":31,"target":40},{"source":2,"target":40},{"source":20,"target":27},{"source":19,"target":27},{"source":37,"target":38},{"source":36,"target":38},{"source":31,"target":38},{"source":30,"target":38},{"source":33,"target":34},{"source":32,"target":34},{"source":30,"target":34},{"source":21,"target":22},{"source":20,"target":22},{"source":18,"target":22},{"source":27,"target":28},{"source":19,"target":28},{"source":4,"target":28},{"source":30,"target":33},{"source":5,"target":33},{"source":36,"target":37},{"source":31,"target":37},{"source":4,"target":37},{"source":40,"target":41},{"source":39,"target":41},{"source":32,"target":41},{"source":2,"target":41},{"source":9,"target":10},{"source":8,"target":10},{"source":6,"target":10},{"source":32,"target":39},{"source":31,"target":39},{"source":34,"target":35},{"source":33,"target":35},{"source":32,"target":35},{"source":5,"target":35},{"source":28,"target":29},{"source":27,"target":29},{"source":20,"target":29},{"source":4,"target":29}],"mines":[3,4,5]}}
{"move":{"moves":[{"claim":{"punter":0,"source":34,"target":35}},{"claim":{"punter":1,"source":30,"target":33}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":3,"target":23}},{"claim":{"punter":1,"source":28,"target":29}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":33,"target":35}},{"claim":{"punter":1,"source":32,"target":41}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":6,"target":12}},{"claim":{"punter":1,"source":7,"target":13}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":12,"target":14}},{"claim":{"punter":1,"source":32,"target":34}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":15,"target":16}},{"claim":{"punter":1,"source":4,"target":37}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":15,"target":17}},{"claim":{"punter":1,"source":5,"target":17}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":0,"target":11}},{"claim":{"punter":1,"source":40,"target":41}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":31,"target":37}},{"claim":{"punter":1,"source":6,"target":14}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":24,"target":25}},{"claim":{"punter":1,"source":24,"target":26}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":9,"target":10}},{"claim":{"punter":1,"source":22,"target":23}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":5,"target":35}},{"claim":{"punter":1,"source":31,"target":38}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":21,"target":23}},{"claim":{"punter":1,"source":9,"target":11}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":8,"target":11}},{"claim":{"punter":1,"source":37,"target":38}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":36,"target":38}},{"claim":{"punter":1,"source":18,"target":24}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":30,"target":36}},{"claim":{"punter":1,"source":8,"target":10}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":27,"target":29}},{"claim":{"punter":1,"source":4,"target":29}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":20,"target":27}},{"claim":{"punter":1,"source":31,"target":40}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":16,"target":17}},{"claim":{"punter":1,"source":20,"target":29}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":31,"target":39}},{"claim":{"punter":1,"source":6,"target":9}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":2,"target":40}},{"claim":{"punter":1,"source":4,"target":28}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":1,"target":24}},{"claim":{"punter":1,"source":19,"target":26}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":10,"target":11}},{"claim":{"punter":1,"source":32,"target":35}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":2,"target":41}},{"claim":{"punter":1,"source":20,"target":22}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":7,"target":16}},{"claim":{"punter":1,"source":18,"target":22}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":33,"target":34}},{"claim":{"punter":1,"source":4,"target":36}},{"pass":{"punter":2}}]}}
{"move":{"moves":[{"claim":{"punter":0,"source":7,"target":15}},{"claim":{"punter":1,"source":25,"target":26}},{"pass":{"punter":2}}]}}
{"stop":{"moves":[{"pass":{"punter":0}}, {"pass":{"punter":1}}, {"pass":{"punter":2}}], "scores":[{"punter":0, "score":8}, {"punter":1, "score":155}, {"punter":2, "score":0}]}}
';
    }
    
    public function select(index:Int):Void
    {
        selectedIndex = index;
        updateUi();
    }
    
    public function exec():Void
    {
        game = new Game();
        playingState = Option.None;
        var name = mapNames[selectedIndex];
        var content = Resource.getString(name);
        var map:MapStruct = Json.parse(content);
        
        game.setupMap(map);
        
        updatePixi();
        updateUi();
    }
    
    public function changeLog(text:String):Void
    {
        log = text;
        updateUi();
    }
    
    public function execLog():Void
    {
        game = new Game();
        playingState = Option.None;
        warning = null;
        try 
        {
            var allMoves:Array<MoveStruct> = [];
            
            var data = ~/(\r\n|\n|\r)/g.split(log);
            Browser.console.log(data);
            var handshake = Json.parse(data[0]);
            var setupData:SetupStruct = Json.parse(data[1]);
            
            game.setup(setupData);
            var you = setupData.punter;
            var first = true;
            var scores:Array<ScoreStruct> = null;
            for (content in data.slice(2, data.length))
            {
                trace(content);
                var data = Json.parse(content);
                inline function addMoves(moves:Array<MoveStruct>)
                {
                    var start = if (first) (moves.length - you) else 0;
                    for (count in start...moves.length)
                    {
                        var index = (count + (you:Int)) % moves.length;
                        var move = moves[index];
                        allMoves.push(move);
                    }
                    
                    first = false;
                }
                
                if (data.stop != null)
                {
                    addMoves(data.stop.moves);
                    scores = data.stop.scores;
                    break;
                }
                else
                {
                    addMoves(data.move.moves);
                }
            }
            
            if (scores == null)
            {
                throw "stopがありません";
            }
            
            playingState = Option.Some(new PlayingState(this, you, allMoves, scores));
        }
        catch (e:Dynamic)
        {
            warning = Std.string(e); 
        }
        
        updatePixi();
        updateUi();
    }
    
    public function onFrame(time:Float):Void
    {
        switch (playingState)
        {
            case Option.None:
                
            case Option.Some(_playingState):
                _playingState.update();
        }
        
        var hash = Browser.location.hash.substr(1);
        trace(hash);
        if (this.hash != hash)
        {
            updateHash(hash);
        }
    }
    
    private function updateHash(hash:String) 
    {
        this.hash = hash;
        var http = new Http(hash);
        trace(hash);
        http.onData = function (data)
        {
            trace(data);
            changeLog(data);
            execLog();
        }
        http.request();
    }
}
