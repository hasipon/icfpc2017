package game;
import game.command.MoveOrStopStruct;
import game.command.MoveStruct;
import game.command.SetupStruct;
import haxe.Json;
import haxe.io.Bytes;
import haxe.io.BytesOutput;
import haxe.io.Input;
import haxe.io.Output;
import search.BeamSearcher;
import sys.io.File;

class GameClient 
{
    public var stderr:Output;
    public var input:Input;
    public var output:Output;
    public var game:Game;
    public var searcher:BeamSearcher;
    public var punter:PunterId;
    public static var result:String = "";
    
    public function new(input:Input, output:Output, searcher:BeamSearcher) 
    {   
        this.searcher = searcher;
        this.stderr = Sys.stderr();
        this.input = input;
        this.output = output;
        this.game = new Game();
        this.punter = PunterId.NotFound;
        waitSetup();
        
        while (waitMove()) {}
    }
    
    private function waitSetup():Void
    {
        var setupData:SetupStruct = waitData(input);
        
        game.setup(setupData);
        punter = setupData.punter;
        writeData(output, { ready: setupData.punter });
    }
    
    private function waitMove():Bool
    {
        var moveOrStopData:MoveOrStopStruct = waitData(input);
        
        return if (moveOrStopData.stop != null)
        {
            game.addMoves(moveOrStopData.stop.moves);
            false;
        }
        else
        {
            game.addMoves(moveOrStopData.move.moves);
            writeData(output, searcher.getMove(game, punter));
            true;
        }
    }
    
    public static function debug(string:String):Void
    {
        Sys.stderr().writeString(string + "\n");
    }
    
    private static var ereg = ~/(\r|\n)/g;
    public static function waitData(input:Input):Dynamic
    {
        var content = waitContent(input);
        result += ereg.replace(content, "") + "\n";
        return Json.parse(content);
    }
    
    public static function waitContent(input:Input):String
    {
        var numberOutput = new BytesOutput();
        while (true)
        {
            var byte = input.readByte();
            if (byte == 58)
            {
                break;
            }
            else
            {
                numberOutput.writeByte(byte);
            }
        }
        
        var length = Std.parseInt(numberOutput.getBytes().toString());
        var jsonOutput = new BytesOutput();
        for (i in 0...length)
        {
            var byte = input.readByte();
            jsonOutput.writeByte(byte);
        }
        
        var content = jsonOutput.getBytes().toString();
        debug("input: " + content);
        return content;
    }
    
    public static function writeData(output:Output, data:Dynamic):Void
    {
        var content = Json.stringify(data);
        debug("output: " + content);
        var bytes = Bytes.ofString(content);
        
        output.writeString(Std.string(bytes.length));
        output.writeString(":");
        output.writeString(content);
    }
}
