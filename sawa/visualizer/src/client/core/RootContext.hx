package core;
import game.Game;
import game.command.MapStruct;
import haxe.Json;
import haxe.Resource;

class RootContext 
{
    public var game:Game;
    public var selectedIndex:Int;
    public var mapNames:Array<String>;
    public var currentGame:Game;
    
    public var updateUi:Void->Void;
    public var updatePixi:Void->Void;
    
    public function new()
    {
        mapNames = Resource.listNames();
        selectedIndex = 0;
        game = new Game();
    }
    
    public function select(index:Int):Void
    {
        selectedIndex = index;
        updateUi();
    }
    
    public function exec():Void
    {
        game = new Game();
        var name = mapNames[selectedIndex];
        var content = Resource.getString(name);
        var map:MapStruct = Json.parse(content);
        
        game.setup(map);
        
        updatePixi();
        updateUi();
    }
}
