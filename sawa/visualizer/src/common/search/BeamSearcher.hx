package search;
import eval.Evaluter;
import game.PunterId;
import game.Game;
import game.command.MoveStruct;

class BeamSearcher implements Searcher
{
    private var timeout:Float;
    private var evaluter:Evaluter;
    
    public function new(timeout:Float, evaluter:Evaluter) 
    {
        this.evaluter = evaluter;
        this.timeout = timeout;
    }
    
    public function getMove(game:Game, punter:PunterId):MoveStruct
    {
        var livingRivers = game.getLivingRivers();
        var moves:Array<MoveAndScore> = [];
        for (river in livingRivers)
        {
            moves.push(
                new MoveAndScore(
                    {
                        claim: {
                            punter: punter,
                            source: river.a,
                            target: river.b,
                        }
                    }
                )
            );
        }
        
        moves.push(new MoveAndScore({ pass: { punter:punter } }));
        for (move in moves)
        {
            game.addMove(move.move);
            move.score = evaluter.eval(game);
            game.undoMove();
        }
        
        moves.sort(MoveAndScore.compare.bind(punter));
        trace("score max:" +  moves[0].score.getFor(punter));
        return moves[0].move;
    }
}
