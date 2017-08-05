package;

import cs.Lib;
import eval.Evaluter;
import game.GameClient;
import haxe.io.Input;
import haxe.io.Output;
import search.BeamSearcher;
import sys.io.File;
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
            var port = 9086;
            if (args.length > 1)
            {
                port = Std.parseInt(args[1]);
            }
            if (args.length > 2)
            {
                host = args[2];
            }
            CliMode.On(host, port);
        }
        else
        {
            CliMode.Off;
        }
        
        var io = getIo(mode);
        var client = new GameClient(io.input, io.output, new BeamSearcher(0.9, new Evaluter(Evaluter.MS_TABLE)));
        File.saveContent("../../dashboard/static/logs/sawa@" + Math.ffloor(Date.now().getTime()) + ".log", GameClient.result);
        Sys.stderr().writeString("finised\n");
    }	
    
    public static function getIo(mode:CliMode):{input:Input, output:Output}
    {
        return switch (mode)
        {
            case CliMode.Off:
                GameClient.result += '{you:"shohei909"}\n';
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
