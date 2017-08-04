package eval;
import game.PunterId;

class EvalutionResult 
{
    public var values:Array<Int>;
   
    public function new(values:Array<Int>):Void
    {
        this.values = values;
    }
    
    public function getFor(id:PunterId):Void
    {
        var result = 0;
        for (i in 0...values.length)
        {
            var current = new PunterId(i);
            value = values[i];
            if (current == id)
            {
                result += value;
            }
            else
            {
                result -= values / (values.length - 1);
            }
        }
    }
}
