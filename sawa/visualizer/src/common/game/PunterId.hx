package game;

abstract PunterId(Int) to Int
{
    public static var NotFound = new PunterId( -1);
    
    public function new(value:Int)
    {
        this = value;
    }
}
