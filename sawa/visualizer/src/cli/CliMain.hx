package;

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
        var io = getIo(mode);
        var client = new GameClient(
            Sys.stdin(), 
            Sys.stdout(), 
            new BeamSearcher(0.9, new Evaluter(Evaluter.MS_TABLE))
        );
        File.saveContent("../../dashboard/static/logs/sawa@" + Math.ffloor(Date.now().getTime()) + ".log", GameClient.result);
        Sys.stderr().writeString("finised\n");
    }
}
