package game;
import game.command.SetupStruct;
import haxe.Json;
import haxe.io.Bytes;
import haxe.io.BytesOutput;
import haxe.io.Input;
import haxe.io.Output;

class GameClient 
{
    public var stderr:Output;
    public var input:Input;
    public var output:Output;
    public var game:Game;
    
    public function new(input:Input, output:Output) 
    {
        this.stderr = Sys.stderr();
        this.input = input;
        this.game = new Game();
        waitSetup();
    }
    
    private function waitSetup():Void
    {
        var setupData:SetupStruct = waitData(input);
        game.setup(setupData);
    }
    
    public static function debug(string:String):Void
    {
        Sys.stderr().writeString(string + "\n");
    }
    
    public static function waitData(input:Input):Dynamic
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
        return Json.parse(content);
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
