package search;
import eval.EvalutionResult;
import game.PunterId;
import game.command.MoveStruct;

class MoveAndScore 
{
    public var move:MoveStruct;
    public var score:EvalutionResult;
    public var scores:Map<PunterId, Int>;
    
    public function new(move:MoveStruct) 
    {
        this.move = move;
        this.score = null;
        this.scores = new Map();
    }
    
    public static function compare(id:PunterId, a:MoveAndScore, b:MoveAndScore) 
    {
        return Reflect.compare(b.score.getFor(id), a.score.getFor(id));
        
    }
}