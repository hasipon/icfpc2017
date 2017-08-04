package eval;
import game.Game;
import game.Punter;
import game.PunterId;

class Evaluter 
{
    public static var MAX_DISTANCE = 20;
    public static var MS_TABLE = [for (i in 0...MAX_DISTANCE) Math.pow(0.5, i)];
    
    public static function eval(game:Game):EvalutionResult
    {
        var values = [
            for (punter in game.punters)
            {
                evalFor(game, punter.id);
            }
        ];
        
        return new EvalutionResult(values);
    }
    
    public static function evalFor(game:Game, punterId:PunterId):Int
    {
        
    }
}
