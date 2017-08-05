package eval;
import game.PunterId;

class EvalutionResult 
{
    public var values:Array<Float>;
   
    public function new(values:Array<Float>):Void
    {
        this.values = values;
    }
    
    public function getFor(id:PunterId):Float
    {
        var result = 0.0;
        for (i in 0...values.length)
        {
            var current = new PunterId(i);
            var value = values[i];
            if (current == id)
            {
                result += value;
            }
            else
            {
                result -= value / (values.length - 1);
            }
        }
        
        return result;
    }
}
