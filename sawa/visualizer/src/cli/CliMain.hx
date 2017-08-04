package;

import cs.Lib;
import game.GameClient;
import haxe.io.Input;
import haxe.io.Output;
import sys.net.Host;
import sys.net.Socket;

class CliMain 
{
	public static function main():Void
    {   
        var args = Sys.args();
		var mode = if (args.length > 0 && args[0] == "on")
        {
            var host = "punter.inf.ed.ac.uk";
            var port = 9008;
            if (args.length > 1)
            {
                port = Std.parseInt(args[1]);
            }
            
            CliMode.On(host, port);
        }
        else
        {
            CliMode.Off;
        }
        
        var io = getIo(mode);
        new GameClient(io.input, io.output);
        
        Sys.println("finised");
        Sys.sleep(5.0);
	}	
    
    public static function getIo(mode:CliMode):{input:Input, output:Output}
    {
        return switch (mode)
        {
            case CliMode.Off:
                {
                    input: Sys.stdin(),
                    output: Sys.stdout(),
                }
                
            case CliMode.On(host, port):
                var socket = new Socket();
                socket.connect(new Host(host), port);
                GameClient.writeData(socket.output, {"me": "shohei909"});
                GameClient.waitData(socket.input);
                
                {
                    input: socket.input,
                    output: socket.output,
                }
        }
    }
}
