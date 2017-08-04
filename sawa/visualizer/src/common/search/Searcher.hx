package search;
import game.PunterId;
import game.Game;
import game.command.MoveStruct;

class Searcher 
{
    private var timeout:Float;
    
    public function new(timeout:Float) 
    {
        this.timeout = timeout;
    }
    
    public function getMove(game:Game, punter:PunterId):MoveStruct
    {
        var livingRivers = game.getLivingRivers();
        var moves:Array<MoveStruct> = [];
        for (river in livingRivers)
        {
            moves.push(
                {
                    claim: {
                        punter: punter,
                        source: river.a,
                        target: river.b,
                    }
                }
            );
        }
        
        moves.push({ pass: { punter:punter } });
        
        return moves[0];
    }
}
